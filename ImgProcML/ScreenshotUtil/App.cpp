#include "App.h"

using namespace cv;
using namespace std;

//void App::initLight() {
    // init wiring pi with mapping from virtual pin numbers 0 through 16
//    wiringPiSetup();
    // set pin 1 in pwm mode
//    pinMode(1, PWM_OUTPUT);
//}


void App::initCamera() {
    if (!videoCapture.open(deviceId))
        cerr << "ERROR : Failed to open device id " + to_string(deviceId) << endl;
    else
        cerr << "Camera " + to_string(deviceId) + " opened !" << endl;
    if (!videoCapture.set(CAP_PROP_AUTOFOCUS, 0))
        cerr << "ERROR : Failed to set autofocus OFF" << endl;
    if (!videoCapture.set(CAP_PROP_AUTO_EXPOSURE, 0))
        cerr << "ERROR : Failed to set auto exposure OFF"  << endl;
    if (!videoCapture.set(CAP_PROP_FRAME_WIDTH, 1280))  //640
        cerr << "ERROR : Failed to set frame width to 1920" << endl;
    if (!videoCapture.set(CAP_PROP_FRAME_HEIGHT, 960)) //480
        cerr << "ERROR : Failed to set frame height to 1080" << endl;
}

Mat App::takeCroppedPicture(int x, int y, int width, int height) {
    cerr << "INFO : start takeCroppedPicture" << endl;

    if (videoCapture.isOpened()) {
        Mat frame, rslt;
        std::chrono::steady_clock::time_point current = std::chrono::steady_clock::now();
        std::chrono::steady_clock::time_point previous;
        do {
            videoCapture.grab();
            previous = current;
            current = std::chrono::steady_clock::now();
        } while (std::chrono::duration_cast<std::chrono::milliseconds>(current - previous).count() < 15);
        // grab x frames to flush the buffer if grabbing takes less than 15 ms it comes from the buffer
        videoCapture >> frame; 
	//imwrite("/home/pi/OrboxPFE/image.png", frame);

    // Check if image is loaded fine
    if( frame.empty()) {
        cerr << " ERROR : frame is empty" << endl;
    }

    cerr << "INFO : copyMakeBorder" << endl;
    try 
    {
	//Extraction d'une partie de la photo
	copyMakeBorder(frame(Rect(x, y, width, height)), rslt,
                       0, (width > height) ? width - height : 0,
                       0, (width > height) ? 0 : width - height,
                       cv::BORDER_CONSTANT, Scalar(0));
        cerr << "INFO : end takeCroppedPicture" << endl;
        return rslt;
    }
    catch (Exception e)
    {
        cerr << "Erreur MakeBorder : " << e.msg << endl;
        return frame;
    }
    }
    // ERROR : App.videoCapture is closed
    return Mat();
}

void App::calibrateCamera() {
    Mat cameraMatrix, distortionCoefficients, map1, map2, newCamMat;
    int croppingWidth, croppingHeight, squareSide, croppingX, croppingY;

    FileStorage fs("../CalibrationConfig.xml", FileStorage::READ);
    if (!fs.isOpened())
        return;
    if (fs["map_1"].empty() || fs["map_2"].empty()) {
        //Computing map_1 and map_2 from camera matrix and distortion coefficients
        fs["cropping_width"] >> croppingWidth;
        fs["cropping_height"] >> croppingHeight;
        fs["cropping_x"] >> croppingX;
        fs["cropping_y"] >> croppingY;
        squareSide = max(croppingHeight, croppingWidth);

        fs["camera_matrix"] >> cameraMatrix;
        fs["distortion_coefficients"] >> distortionCoefficients;

        fs.release();

        cv::fisheye::estimateNewCameraMatrixForUndistortRectify(cameraMatrix,
                                                                distortionCoefficients,
                                                                cvSize(squareSide, squareSide),
                                                                Matx33d::eye(),
                                                                newCamMat,
                                                                1);
        cv::fisheye::initUndistortRectifyMap(cameraMatrix,
                                             distortionCoefficients,
                                             Matx33d::eye(),
                                             newCamMat,
                                             cvSize(squareSide, squareSide),
                                             CV_16SC2,
                                             map1,
                                             map2);
        fs.open("../CalibrationConfig.xml", FileStorage::APPEND);
        fs << "map_1" << map1;
        fs << "map_2" << map2;
        camMap1 = map1;
        camMap2 = map2;
    }
    else {
        //Using map_1 and map_2 from config file
        fs["map_1"] >> camMap1;
        fs["map_2"] >> camMap2;
    }
    fs.release();
}

void App::undistord(Mat &src, Mat &dst) {
    dst = src.clone();
    remap(src, dst, camMap1, camMap2, INTER_LANCZOS4);
}
