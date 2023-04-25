@echo begin generate proto....

cd s2s

..\protoc.exe --cpp_out=./ s2s_common.proto

cd..

@echo finish generate proto..
pause