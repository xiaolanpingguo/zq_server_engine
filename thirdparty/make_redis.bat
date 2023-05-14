@echo "=======begin build redis=========="

cd hiredis-1.1.0
rmdir  /s /q .\win-build\
rmdir /s /q  .\win-install\
mkdir win-build
cd win-build
cmake .. -G "Visual Studio 17 2022" -DCMAKE_INSTALL_PREFIX=..\win-install
cmake --build . --target install

pause

@echo "=======finish build redis=========="