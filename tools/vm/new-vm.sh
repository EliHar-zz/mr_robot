#!/bin/bash

#########################################
# Install tools on a new VM
# Example of usage:
#   ./new-vm.sh
#########################################

# Variables
VM_USER="vagrant"
REPO_PATH="/home/$VM_USER/mr_robot"

# Confirmation function
confirm () {
    # call with a prompt string or use a default
    read -r -p "${1} (y/n) " response
    case $response in
        [yY][eE][sS]|[yY]) 
            true
            ;;
        *)
            false
            ;;
    esac
}

confirm  "Install curl ?" && 
sudo apt-get install curl -y &&
echo 

confirm "Install Git ?" &&
sudo apt-get install git -y &&
echo

confirm "Install Eclipse ?" &&
cd "/home/$VM_USER/" &&
wget "http://www.eclipse.org/downloads/download.php?file=/technology/epp/downloads/release/helios/SR2/eclipse-cpp-helios-SR2-linux-gtk-x86_64.tar.gz" -O "eclipse.tar.gz" &&
sudo tar -xvf eclipse.tar.gz &&
sudo rm *.tar.gz &&
echo

confirm "Install Java JDK 7 ?" &&
sudo apt-get install openjdk-7-jdk -y &&
echo

confirm "Install Java JDK 8 ?" &&
sudo add-apt-repository ppa:openjdk-r/ppa -y &&
sudo apt-get update &&
sudo apt-get install openjdk-8-jdk &&
echo

confirm "Install g++ 4.9 and gcc 4.9 ?" &&
sudo apt-get install build-essential -y &&
sudo add-apt-repository ppa:ubuntu-toolchain-r/test -y &&
sudo apt-get update &&
sudo apt-get install gcc-4.9 g++-4.9 cpp-4.9 -y &&
cd /usr/bin &&
sudo rm gcc g++ cpp &&
sudo ln -s gcc-4.9 gcc &&
sudo ln -s g++-4.9 g++ &&
sudo ln -s cpp-4.9 cpp &&
echo

confirm "Install cmake ?" &&
sudo apt-get install cmake -y &&
echo 

confirm "Install OpenCV ?" &&
cd /tmp &&
#sudo apt-get install cmake git libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev -y &&
#sudo apt-get install python-dev python-numpy libtbb2 libtbb-dev libjpeg-dev libpng-dev libtiff-dev libjasper-dev libdc1394-22-dev -y &&
#wget "https://github.com/opencv/opencv_contrib/archive/3.1.0.zip" -O opencv_contrib.zip &&
#unzip opencv_contrib.zip &&
#rm opencv_contrib.zip &&
#mv opencv_contrib* contrib &&
#wget "https://github.com/Itseez/opencv/archive/3.1.0.zip" -O opencv.zip &&
#unzip opencv.zip &&
#rm opencv.zip &&
#mv opencv* opencv &&
cp "$REPO_PATH/tools/vm/install-opencv.sh" . &&
./install-opencv.sh &&
rm -rf /tmp/opencv* &&
echo
