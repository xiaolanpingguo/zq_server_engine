@echo begin generate proto....

cd s2s

..\protoc.exe --cpp_out=./ s2s_common.proto
..\protoc.exe --cpp_out=./ db_player.proto
..\protoc.exe --cpp_out=./ db_mongo_proxy.proto

cd..

@echo finish generate s2s proto..
pause