#!/bin/bash

git clone https://github.com/uNetworking/uSockets
cd uSockets
make
mkdir -p ../lib
cp uSockets.a ../lib
cd ../

git clone https://github.com/uNetworking/uWebSockets
cd uWebSockets
mkdir -p /usr/include/uWebSockets
cp -r src/* /usr/include/uWebSockets
cd ..
wget https://raw.githubusercontent.com/uNetworking/uSockets/master/src/libusockets.h -P /usr/include/ -O /usr/include/libusockets.h

echo "uWebSockets installed"
rm -rf uSockets uWebSockets
