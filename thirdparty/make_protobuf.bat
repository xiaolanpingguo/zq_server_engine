@echo "=======begin build protobuf=========="

cd protobuf-22.1
rmdir  /s /q .\win-build\
rmdir /s /q  .\win-install\
mkdir win-build
cd win-build
cmake .. -G "Visual Studio 17 2022" -DCMAKE_INSTALL_PREFIX=..\win-install -Dprotobuf_MSVC_STATIC_RUNTIME=off
cmake --build . --target install
cd ../..


@echo "=======finish build protobuf=========="