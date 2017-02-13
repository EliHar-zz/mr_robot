Run Tensorflow on Docker

```
cd $MR_ROBOT/tensorflow/
docker run -it -v $PWD/shared:/tf_files  gcr.io/tensorflow/tensorflow:latest-devel
```
