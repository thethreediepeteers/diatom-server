git clone --recurse-submodules https://github.com/madler/zlib
cd zlib

./configure
make install

cd ../
rm -rf zlib
