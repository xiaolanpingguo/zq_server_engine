echo '-----begin make protobuf------------'

cd protobuf-22.1
rm -rf linux-build/
rm -rf linux-install/
mkdir linux-build
cd linux-build
cmake .. -DCMAKE_INSTALL_PREFIX=../linux-install -DCMAKE_BUILD_TYPE=Release
cmake --build . --target install
make clean
cd ../..

echo '-----finish make protobuf------------'