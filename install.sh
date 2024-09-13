#!/bin/bash

if [ -z "$1" ]; then
  echo "Usage: $0 [all|zlib|sockets]"
  exit 1
fi

# Install zlib if 'all' or 'zlib' is passed
if [ "$1" = "all" ] || [ "$1" = "zlib" ]; then
  git clone --recurse-submodules https://github.com/madler/zlib
  cd zlib
  ./configure
  make install
  cd ../
  echo "zlib installed"
fi

# Install uSockets if 'all' or 'sockets' is passed
if [ "$1" = "all" ] || [ "$1" = "sockets" ]; then
  git clone https://github.com/uNetworking/uSockets
  cd uSockets
  make
  cp uSockets.a ../lib
  cd ../

  git clone https://github.com/uNetworking/uWebSockets
  cd uWebSockets
  mkdir -p ../include/uWebSockets
  rm ../include/uWebSockets/*
  cp -r src/* ../include/uWebSockets
  cd ..

  echo "uWebSockets installed"
fi

# Cleanup 
rm -rf zlib uSockets uWebSockets
