Required Compiler: clang (>= 10.0.0) or gcc (>= 10.3) or Apple-clang (>= 14)

How to Build:

WIN(for development):

Required: vs2022, cmake:https://cmake.org/download/
build step:
1: make thirdparty lib:
	enter zq_server_engine\thirdparty, click make_all.bat, this will make all thirdparty 
2: use cmake make project
	make dir: win-build,
	use cmake command line: 
	  cmake .. -G "Visual Studio 17 2022"
	  cmake --build . --target install
	or cmake gui for generate VisualStaudio Solution and build project by vs
	
	
Linux:
Required: use Ubuntu-22.04 or use any linux distribution above gcc-10 version

1: install required dependence:
	sudo apt update
	sudo apt upgrade
	sudo apt-get install cmake 
	sudo apt-get install libssl-dev 
	sudo apt-get install libsasl2-dev
2: build project:
	1: clone project
	2: run ./make.sh

