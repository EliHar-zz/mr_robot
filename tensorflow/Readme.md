### Run Tensorflow on Docker

```
cd $MR_ROBOT/tensorflow/
docker run --name mr_robot -it -v $PWD/shared:/tf_files  gcr.io/tensorflow/tensorflow:latest-devel
```

### Run from scratch:

#### In docker:
```
apt-get update
apt-get install wget
cd /tensorflow/tensorflow
git pull
cd examples/image_retraining
wget https://raw.githubusercontent.com/amirbawab/googliser/master/googliser.sh
chmod a+x googliser.sh
apt-get install ImageMagick
./googliser.sh -n 200 --phrase "red stop sign" --upper-size 100000 --lower-size 2000 --failures 0 -g
mv red_stop_sign/ /tf_files/stop_sign
# use googliser to donwload different categories, and move the created folder to /tf_files
```
