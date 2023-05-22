@echo begin generate proto....

cd c2s

..\protoc.exe --cpp_out=./ c2s_common.proto
..\protoc.exe --cpp_out=./ c2s_login.proto

cd..

@echo finish generate c2s proto..
pause