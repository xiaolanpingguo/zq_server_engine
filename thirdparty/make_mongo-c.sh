echo "=======begin build mongo-c=========="

cd mongo-c-driver-1.23.3
rm -rf linux-build/
rm -rf linux-install/
mkdir linux-build
cd linux-build
cmake .. -DCMAKE_INSTALL_PREFIX=../linux-install -DENABLE_AUTOMATIC_INIT_AND_CLEANUP=OFF -DCMAKE_BUILD_TYPE=Release
sudo cmake --build . --target install
make clean
cd ../..

echo "=======finish build mongo-c=========="