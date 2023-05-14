@echo "=======begin build mongo-c=========="

cd mongo-c-driver-1.23.3
rmdir  /s /q .\win-build\
rmdir /s /q  .\win-install\
mkdir win-build
cd win-build
cmake .. -G "Visual Studio 17 2022" -DCMAKE_INSTALL_PREFIX=..\win-install -DENABLE_AUTOMATIC_INIT_AND_CLEANUP=OFF -DENABLE_EXTRA_ALIGNMENT=0
cmake --build . --config RelWithDebInfo --target install

pause

@echo "=======finish build mongo-c=========="