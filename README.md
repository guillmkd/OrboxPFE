The project is hosted at [https://github.com/age2pierre/OrboxPFE](https://github.com/age2pierre/OrboxPFE)

## Synopsis
The Orbox is a projet initiated at Polytech' Tours in 2017. It aims at providing help to those suffering visual deficiency by providing audio message about the objects put on its surface.

At its heart there is a RaspberryPi using OpenCV, a camera and some LEDs to do some machine vision.

## Motivation
The first physical prototype is nearly up and running. As of today (february 2017) only the remote control is working. But it'll be only a matter of weeks before that feature is fully implemented and tested.

## Hardware ressources
In order to make your own Orbox, you can be creative and make your own case and PCBs... or you can use the followings schematics :

* [OnShape document - mechanical CAD and drawings](https://cad.onshape.com/documents/5f9e1a768941a34f06bbb396/w/f230842e43067cb46ecea699/e/ded45623d71b4f65f55b64b7)
* [Upverter document](https://upverter.com/OrboxTeam/cfa40d292bac9e7b/Orbox_PFE/)

## Installation

Follow these instructions to start running the code.

First clone this reposiory using git, extract it and place yourself at the root of the folder.

You'll need to have OpenCV3.2.0 installed. Install the latest release by running the following script :
```
$ sh ./installOpencv.sh
```

You'll also need NodeJS with EC6 support and pico2wave.
Install those by running the following script :
```
$ sh ./installNode.sh
```

If you are running on RasperryPi you'll need WiringPi.
If it's not packaged by default with your distribution, run :
```
$ git clone git://git.drogon.net/wiringPi
$ cd ~/wiringPi
$ ./build
```

You'll need two configure two file :
* ./WebServer/config.js
* ./ImgProcML/build/config.json
In each put the absolute path for ./data and ./ImgProcML/build

You are now ready to compile and run the server.
This can be done by running the following script :
```
$ sh ./install.sh
```

### IMPORTANT NOTE :
For those who want to run the program on their computer instead of their Pi.
You still can, but because you won't have WiringPi and the ability to turn on and off the led, the programm won't compile as it is.
You 'll need to comment a line in _./ImgProcML/CMakeLists.txt_ :
```
cmake_minimum_required(VERSION 3.2)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")
add_subdirectory(Common)
add_subdirectory(StatUtil)
add_subdirectory(PerfTestUtil)
add_subdirectory(PredictUtil)
# add_subdirectory(ScreenshotUtil)
```

## End users instructions

We distinguished two type types of user :
* Those using the physical Orbox.
* And those managing the training database through the web app.

For the former, using the Orbox is childish :
1. Turn on the Orbox with the switch located on the side.
2. Wait a few seconds for the system to boot (~15s and blinking led).
3. Put the object to analyse on the glass and press the button on top of the box.
4. Wait for the audio response

For the later :
1. Open a browser and navigate to [http://localhost:3000](http://localhost:3000) or [pi.ip.ad.rs:3000](#)
2. Navigate on the different tabs
3. If you don't know understand certain actions return on the homepage and read the instructions.

## For the developer
Developers maybe interested into two aspects :
* Adding and testing new image descriptors. For those, you need to look at the following files :
    * _./ImgProcML/Common/Rois.h_ (and also _Rois.cpp_) this is where the functions calculating the image descriptors are defined.
    * _./ImgProcML/PerfTest/main.cpp_ (and also _ClassificationTest.h/cpp_) to see how the previous functions are called.
* Adding new methods of classification than those tested here. For those, you should look here :
    * _./ImgProcML/PerfTest/ClassificationTest.h/cpp_ there the are two type of method : _testXXX()_ is where you test your algorithm by k-fold cross validation and _setXXX()_ is where you serialize the result of training over the full dataset for later use by the predictUtil.

## License

This file is part of Orbox.

Orbox is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

Orbox is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with Orbox.  If not, see <http://www.gnu.org/licenses/>.