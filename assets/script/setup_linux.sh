# install wsl on windows
wsl --install -d Ubuntu-22.04

# linux setup
# backup sources list
sudo cp /etc/apt/sources.list /etc/apt/sources.list.bak
sudo  vi /etc/apt/sources.list
# delete or comments old content and copy follows to /apt/sources.list
deb http://mirrors.aliyun.com/ubuntu/ jammy main restricted universe multiverse
deb http://mirrors.aliyun.com/ubuntu/ jammy-security main restricted universe multiverse
deb http://mirrors.aliyun.com/ubuntu/ jammy-updates main restricted universe multiverse
deb http://mirrors.aliyun.com/ubuntu/ jammy-backports main restricted universe multiverse

sudo apt update && sudo apt upgrade && sudo apt-get install net-tools && sudo apt-get install -y gcc && sudo apt-get install -y g++ && sudo apt-get install -y cmake && sudo apt-get install -y libssl-dev && sudo apt-get install -y libsasl2-dev

# fd limits
# sudo echo "ulimit -n 10240" >> /home/"your user"/ .bash_profile 
# ulimit -n

# setup redis   you can use redis-windows for development: https://github.com/tporadowski/redis/releases 
curl -fsSL https://packages.redis.io/gpg | sudo gpg --dearmor -o /usr/share/keyrings/redis-archive-keyring.gpg
echo "deb [signed-by=/usr/share/keyrings/redis-archive-keyring.gpg] https://packages.redis.io/deb $(lsb_release -cs) main" | sudo tee /etc/apt/sources.list.d/redis.list
sudo apt-get update
sudo apt-get install redis
# start stop restart
#sudo /usr/bin/redis-server redis.conf
# or sudo service redis-server start
#or 
#sudo /usr/bin/redis-server /mnt/d/program/my_osp/zq_server_engine/assets/redis_cfg/redis.conf
# use redis GUI client: https://github.com/qishibo/AnotherRedisDesktopManager/releases
# setup redis cluster
#redis-cli -h 192.168.0.109 -p 7001
#redis-cli --cluster create 192.168.0.109:7001 192.168.0.109:7002 192.168.0.109:7003


# git
#git config --global user.name "your user name"
#git config --global user.email "your user email"
# ssh
# ssh-keygen -t rsa -C "your git user name"
# go github-> Settings -> SSH and GPG keys ->New SSH key, copy your ssh id_rsa.pub text content just genrated to this
# check if ok: ssh -T git@github.com
# then go Settings->develop setting->personal access token->generate new token, then remember save the token after the token generated!!!
# now you can use git push/pull, you will promote to input username, and password,  we use the token instead of password