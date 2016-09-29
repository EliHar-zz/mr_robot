# Mr. Robot [![Build Status](https://travis-ci.org/EliHar/mr_robot.svg?branch=master)](https://travis-ci.org/EliHar/mr_robot)

### Table of Contents

  * [Install Virtual Machine](#install-virtual-machine)
  * [Start Virtual Machine](#start-virtual-machine)
  * [Import C++ into Eclipse](#import-c-into-eclipse)
  * [Architecture](#architecture)

### Install Virtual Machine

  * Install VirtualBox
    * Download VirtualBox https://www.virtualbox.org/wiki/Downloads
    * Compatible version https://www.vagrantup.com/docs/virtualbox/
  * Install Vagrant
    * Download Vagrant https://www.vagrantup.com/downloads.html

### Start Virtual Machine

  * Go to the repository directory and run `vagrant up`
  * After the VM starts, run the following:  
```
cd tools/vm/
./new-vm.sh
```

### Import C++ into Eclipse
From the VM, open eclipse and import project at directory: `cv`

### Architecture

#### Overall architecture:

<img src="https://raw.githubusercontent.com/EliHar/mr_robot/master/tools/docs/architecture.png"/>

#### Image processor

<img src="https://raw.githubusercontent.com/EliHar/mr_robot/master/tools/docs/IP-SD.png"/>
