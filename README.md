# Mr. Robot [![Build Status](https://travis-ci.org/EliHar/mr_robot.svg?branch=master)](https://travis-ci.org/EliHar/mr_robot)

### Table of Contents

  * [Install Virtual Machine](#install-virtual-machine)
  * [Start Virtual Machine](#start-virtual-machine)
  * [Import C++ into Eclipse](#)

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
