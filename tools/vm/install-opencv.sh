#######################################################
# This file is used by new-vm.sh and expects some
# files to be placed in specific directories on the VM
# 
# NODE: Don't execute this file manually
#######################################################

# Variables
SHELL_FILE_PATH=$(dirname $0)
OPENCV_DIR="opencv"
BUILD_DIR="release"
OPENCV_CONTRIB="contrib"

# Clean build directories if any
echo "Removing old build directory (if exists): $BUILD_DIR"
rm $SHELL_FILE_PATH/$OPENCV_DIR/$BUILD_DIR -rf 

# Create realease directory
echo "Creating $SHELL_FILE_PATH/$OPENCV_DIR/$BUILD_DIR ..."
mkdir "$SHELL_FILE_PATH/$OPENCV_DIR/$BUILD_DIR"

# CMAKE
echo "Starting cmake ..."
cd "$SHELL_FILE_PATH/$OPENCV_DIR/$BUILD_DIR"
cmake -D CMAKE_BUILD_TYPE=Release -D RUNTIME_OUTPUT_DIRECTORY=$BUILD_DIR -D CMAKE_INSTALL_PREFIX=/usr/local -D OPENCV_EXTRA_MODULES_PATH=../../$OPENCV_CONTRIB/modules ..

# Make
echo "Calling make (Takes few minutes to finish)"
make

# Install
echo "Installing libraries on the machines (Requires root permission)"
sudo make install
