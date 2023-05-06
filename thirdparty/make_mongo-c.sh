@echo "=======begin build mongo-c=========="

cd mongo-c-driver-1.23.3
rm -rf cmake-build
mkdir cmake-build
cd cmake-build
cmake .. -DCMAKE_INSTALL_PREFIX=..\install -DENABLE_AUTOMATIC_INIT_AND_CLEANUP=OFF
cmake --build . --config Release --target install

@echo "=======finish build mongo-c=========="