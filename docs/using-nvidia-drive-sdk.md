# Using Nvidia DRIVE SDK



NVIDIA DRIVE™ Perception enables the robust perception of obstacles, paths, and wait conditions \(such as stop signs and traffic lights\) right out of the box with an extensive set of pre-processing, post-processing, and fusion processing modules. Together with NVIDIA DRIVE™ Networks, these form an end-to-end perception pipeline for autonomous driving that uses data from multiple sensor types \(e.g. camera, radar, LIDAR\). DRIVE Perception makes it possible for developers to create new perception, sensor fusion, mapping, and/or planning/control/actuation autonomous vehicle \(AV\) functionalities without having to first develop and validate the underlying perception building blocks.  
  


**DRIVE Perception** is designed for a variety of objectives:

* Developing perception algorithms for obstacles, path, and wait conditions
* Detecting and classifying objects, drivable space, lanes and road markings, and traffic lights and signs
* Tracking detected objects \(such as other vehicles, pedestrians, road markings\) from across frames
* Estimating distances to detected objects
* Fusing inputs from sensors of different modalities

Compiling **Nvidia** Drive Net

Post-installation of Drive SDK, samples are found to be installed at the following location

```text
/usr/local/driveworks-2.2/samples/build/src/
```

Copy the sample code provided at `scripts` directory to current installation directory 

```text
cp -r scripts/*.* /usr/local/driveworks-2.2/samples/build/src/waitconditions/
```

Post-installation of Drive SDK, samples are found to be installed at the following location

Re-compilation:

```text
cmake -DCUDA_TOOLKIT_ROOT_DIR=/usr/local/cuda-9.2  \
  -DSDK_NO_GITVERSION_BUILD=true    -DCMAKE_BUILD_TYPE=Release  \
  -DSDK_BUILD_EXPERIMENTAL=false    -DSDK_USE_CCACHE=true \
  -DSDK_BUILD_{DEMOS,BENCHMARKS,PACKAGE}=false  \
  -DSDK_BUILD_DOCUMENTATION=false    -DSDK_BUILD_BENCHMARKS=false  \
  -DSDK_BUILD_SAMPLES=true    -DCMAKE_C_COMPILER=gcc-6  \
  -DCMAKE_CXX_COMPILER=g++-6    -DSDK_USE_CCACHE:BOOL=ON ..

cmake --build .
```

To run the sample on PNG image, one must change the SignNet file to accept PNG files instead of the video feeds, The example file is provided at : 

```lua
./sample_sign_classifier --model=EU_V2 --offscreen=2 --path=/media/sunil/Shots/directional_light_20_lux/null_real_test.0220.png  --input-type=imagefile 
```

To run the sample on png image with Visualization

```lua
 ./sample_sign_classifier --path=/path/image.png --input-type=imagefile --viz true
```

Files are organized as given below. The repository has the following pre-compiled assets :

In addition to this, the repository has all the codes require to infer on the images as well as video.

Following is the output of the command `tree -d -L 2.` It shows the directory structure up to level 2.

