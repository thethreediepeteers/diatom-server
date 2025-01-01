ld -v > LD_TEST
cc --help > CLANG_TEST
g++ --help > G++_TEST

if ! grep -q GNU "LD_TEST" ; then
  echo "Binutils is required to build uSockets! Install binutils-llvm to proceed"

  exit
fi

if ! grep -q clang "CLANG_TEST" ; then
  echo "Clang compiler is required to build uSockets! Install llvm and clang to proceed"

  exit
fi

if ! grep -q clang "G++_TEST" ; then
  echo "Clang compiler is required to build uSockets! Install llvm and clang to proceed"

  exit
fi

git clone https://github.com/uNetworking/uSockets
cd uSockets
make
mkdir -p ../lib
cp uSockets.a ../lib
cd ../

git clone https://github.com/uNetworking/uWebSockets
cd uWebSockets
mkdir -p ../include/uWebSockets
cp -r src/* ../include/uWebSockets
cd ..

wget https://raw.githubusercontent.com/uNetworking/uSockets/master/src/libusockets.h -P include/ -O include/libusockets.h

echo "uWebSockets installed"

rm -rf uSockets uWebSockets
rm LD_TEST CLANG_TEST G++_TEST
