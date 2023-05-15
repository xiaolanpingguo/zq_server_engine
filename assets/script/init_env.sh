sudo apt update
sudo apt upgrade
sudo apt dist-upgrade

sudo add-apt-repository ppa:ubuntu-toolchain-r/test
sudo apt update
sudo apt-get install -y gcc-11
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-9 20
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-11 50
sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-9 20
sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-11 50