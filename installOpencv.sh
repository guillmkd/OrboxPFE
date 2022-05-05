#!/usr/bin/env bash
sudo apt update
sudo apt install -y build-essential git cmake libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev
git clone https://github.com/opencv/opencv.git
cd ./opencv
mkdir build
cd build
cmake -DBUILD_SHARED_LIBS=OFF -DBUILD_EXAMPLES=OFF -DBUILD_TESTS=OFF -DBUILD_PERF_TESTS=OFF -DWITH_IPP=OFF ..
make -j4
sudo make install
