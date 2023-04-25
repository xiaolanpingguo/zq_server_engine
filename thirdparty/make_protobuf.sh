echo '-----begin make protobuf------------'

cd protobuf-22.1
rm build -force -recurse
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022" -DCMAKE_INSTALL_PREFIX=..\install
cmake --build . --config Release --target install

echo '-----finish make protobuf------------'