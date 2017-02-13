#!/usr/bin/env bash

cd /ImgProcML/build
cmake ../..
make

cd ../../WebServer
npm install
npm start