@echo "=======begin build all thirdparty=========="

start make_redis.bat
start make_mongo-c.bat
start make_protobuf.bat

pause

@echo "=======finish build all thirdparty=========="