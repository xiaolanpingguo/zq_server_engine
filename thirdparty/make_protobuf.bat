@echo "=======begin build protobuf=========="

cd protobuf-22.1
rmdir  /s /q .\build\
rmdir /s /q  .\install\
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022" -DCMAKE_INSTALL_PREFIX=..\install -Dprotobuf_MSVC_STATIC_RUNTIME=off
cmake --build . --target install

pause

@echo "=======finish build protobuf=========="