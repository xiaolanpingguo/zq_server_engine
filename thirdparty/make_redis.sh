echo "=======begin build redis=========="

cd hiredis-1.1.0
rm -rf linux-build/
rm -rf linux-install/
mkdir linux-build
cd linux-build
cmake ..  -DCMAKE_INSTALL_PREFIX=../linux-install -DCMAKE_BUILD_TYPE=Release
cmake --build . --target install
make clean
cd ../..


echo "=======finish build redis=========="