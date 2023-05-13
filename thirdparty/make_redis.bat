@echo "=======begin build redis=========="

cd hiredis-1.1.0
rmdir  /s /q .\build\
rmdir /s /q  .\install\
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022" -DCMAKE_INSTALL_PREFIX=..\install
cmake --build . --target install

pause

@echo "=======finish build redis=========="