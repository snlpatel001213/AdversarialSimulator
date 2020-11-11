# Adversarial Simulator

Adversarial Simulator allows one to simulate adversarial attacks in the virtual environment within the Unreal Engine and inference with Nvidia DRIVE SDK. This repository contains the code, assets, example datasets, models, test results, and a video demo for the paper :

### A Simulated Environment Approach to Investigate the effect of Adversarial Perturbations on Traffic Signs

| Sunil Patel | Pallab Maji |
| :---: | :---: |
| supatel@nvidia.com | pmaji@nvidia.com |
| Sunil Patel | Pallab Maji |

**Abstract** **-To study the effect of adversarial attack environment must be controlled. Autonomous driving includes mainly 5 phases sense, perceive, map, plan, and Drive. Autonomous Vehicle ‘sense’ the surrounding with the help of Cameras and Lidars. Deep Learning techniques are considered Blackbox and found to be vulnerable to adversarial attacks. In this research, we study the effect of the various known adversarial attack in the Unreal Engine-based high-fidelity real-time raytraced simulated environment. This experiment seeks an answer to questions such as if adversarial attacks work in moving vehicle scenarios and can an unknown network be targeted. We found that the existing Blackbox and Whitebox affects different traffic signs differently. Attack found to be affecting classification in static scenes are not similarly affecting in the moving vehicle scenarios. However, some attacks with barely noticeable perturbations were found to completely block the identification of certain traffic signs. By simulating the interaction of light on Traffic sign have also observed that daylight condition has a significant impact on the performance of the model. Our findings are found to be closely simulating scenarios as found in the real world.**

For details related to setting up environments,  Building Nvidia Drive SDK, Recording sessions, and Addons refer to the documentation:  [https://sunil.gitbook.io/adversarial-simulator/](https://sunil.gitbook.io/adversarial-simulator/) 

## This documentation covers a vital  aspect of the project including ;

### 1. [Installation procedures](https://sunil.gitbook.io/adversarial-simulator/docs/installation-procedures)

An Adversarial simulator requires two components to be installed including an Unreal Engine and Nvidia Drive SDK. An Unreal Engine required to generate a real-like scenario.

### 2.  [Setting up the Environment](https://sunil.gitbook.io/adversarial-simulator/docs/setting-up-environment)

Post-installation we require to set up the environment so that different scenarios can be generated. In Unreal Engine, such scenarios are organized in Maps. adversarial Simulator provides two maps. 

#### 2.1 [MAP - 1 ](https://sunil.gitbook.io/adversarial-simulator/docs/setting-up-environment/environment-1)

It has basic components for controlled testing, tracks are organized in parallel for a testing plurality of scenarios in parallel, keeping the overall environment the same.  

#### 2.2 [MAP - 2 ](https://sunil.gitbook.io/adversarial-simulator/docs/setting-up-environment/environment-2)

It is consisting of vast open land with various objects like buildings, water, over bridges, traffic lights, traffic signals, roadside objects, various kinds of animals, etc. This map can be used for a variety of testing.

### 3. [Compiling Drivenet samples](https://sunil.gitbook.io/adversarial-simulator/docs/using-nvidia-drive-sdk)

Drive net sample comes up with video inference capabilities by default. additionally, if you want to predict Images then you need to modify samples. This section covers all the basic procedure for compilation and modifying the samples to r

### 4. [Add ons ](https://sunil.gitbook.io/adversarial-simulator/docs/add-ons)

This section suggests various objects packs that can be added to the provided maps to create more complex scenarios.

## Project Organization

1. Animals
2. Cars and mopeds 
3. Buildings
4. Road builder
5. Rocks 
6. Trees 
7. Shrubs and various kind of Grass
8. car material packs
9. Water simulation
10. Weather simulation assets
11. Dashboard for experimentation management

```lua
├── Config # Unreal configuration file
├── Content
│   ├── AnimalVarietyPack # Animals
│   ├── AutomotiveMats424 # Automotive materials like glass, metals
│   ├── cinematics # Camera and recording related dashboards
│   ├── City # Buildings and Homes
│   ├── City_Pack # Buildings and Homes
│   ├── EssentialTreeKit # Variety of trees
│   ├── Experiment # Roadsigns, patchs, postrs, 
│   ├── Foliage # Flower shrub
│   ├── Geometry # Landscape
│   ├── GT_Free2020 # car esseentials
│   ├── Maps # MAP-1 and MAP-2 are located here
│   ├── MegascansMeadowPack # Gradss, shrubs etc 
│   ├── MRT # modular road tool for constructing custom road
│   ├── rocks # rock and boulders
│   ├── Vehicles # bikes and other vehicles
│   ├── VehicleVarietyPack # various 4 wheeler vehicles
│   ├── WaterPlane # water simulation assets
│   └── Weather #  wather effect such as Wind, leaf fall and lightning
├── docs # Documentation
├── scripts # C++ scripts for video and frame based inference
└── tools # Swiss army knife toolbox for windows file management
```

## Cloning the Code

The experimentation included many high-resolution assets and some-time single file span above 50 MB. 50 Mb is the size limitation imposed by Github for any single fine. Hence to manage larger files this project uses`DVC (Data Version Control)`an Open-source Version Control System for Machine Learning Projects. [`DVC`](https://dvc.org/) allow us to share the large files with git easily

#### Git clone base project

```text
git clone 
```

identify large texture files

```text
dvc pull
```

If you're interested in contributing  to this project, refer the following guidelines:

Identify a larger file than 50MB, these files will be sent to DVC. Following is the procedure to identify larger files recursively in windows and Linux:

{% tabs %}
{% tab title="Windows" %}
Use the Swiss army knife tool provided with the repository:

```text
.\tools\sfk197.exe list -big -mbytes \path\to\content\folder
```

If you have not downloaded then download and setup [DVC for windows](https://github-production-release-asset-2e65be.s3.amazonaws.com/83878269/a0e69c80-186a-11eb-8551-3fba89b66ead?X-Amz-Algorithm=AWS4-HMAC-SHA256&X-Amz-Credential=AKIAIWNJYAX4CSVEH53A%2F20201107%2Fus-east-1%2Fs3%2Faws4_request&X-Amz-Date=20201107T053708Z&X-Amz-Expires=300&X-Amz-Signature=4996ba67919c7313285fbef8c4dad539c216216ef57ec74fcefdcd7a619442c2&X-Amz-SignedHeaders=host&actor_id=6880272&key_id=0&repo_id=83878269&response-content-disposition=attachment%3B%20filename%3Ddvc-1.9.1.exe&response-content-type=application%2Foctet-stream)

Commit larger files to DVC,

```text
dvc add \path\to\the\large\file 
```

Exclude files to git which are committed to DVC. This command will be provided by DVC prompt post committing files to DVC

Push files to DVC

```text
dvc commit -m "the most imp file added"
dvc push 
```

Push remaining files to Github
{% endtab %}

{% tab title="Linux" %}
Use bash command to find larger files

```text
sudo find ./path/to/AdversarialSiumator/dir -type f -printf "%s\t%p\n" | sort -n | tail -1
```

Commit larger files to DVC

```
dvc add \path\to\the\large\file 
```

Exclude files to git which are committed to DVC. This command will be provided by DVC prompt post committing files to DVC

Push files to DVC

```text
dvc commit -m "the most imp file added"
dvc push 
```

Push remaining files to Github
{% endtab %}
{% endtabs %}

## Compiling Nvidia SDK

Post-installation of Drive SDK, samples can be found at `/usr/local/driveworks-2.2/samples/build/src/`

```lua
cmake -DCUDA_TOOLKIT_ROOT_DIR=/usr/local/cuda-9.2  \
  -DSDK_NO_GITVERSION_BUILD=true    -DCMAKE_BUILD_TYPE=Release  \
  -DSDK_BUILD_EXPERIMENTAL=false    -DSDK_USE_CCACHE=true \
  -DSDK_BUILD_{DEMOS,BENCHMARKS,PACKAGE}=false  \
  -DSDK_BUILD_DOCUMENTATION=false    -DSDK_BUILD_BENCHMARKS=false  \
  -DSDK_BUILD_SAMPLES=true    -DCMAKE_C_COMPILER=gcc-6  \
  -DCMAKE_CXX_COMPILER=g++-6    -DSDK_USE_CCACHE:BOOL=ON ..

cmake --build .
```

For more details on the installation procedure: [https://sunil.gitbook.io/adversarial-simulator/docs/installation-procedures](https://sunil.gitbook.io/adversarial-simulator/docs/installation-procedures) 

## Processing Image Frames

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

## Other Related Assets   

This project provides all the necessary assets for adversarial simulation. That includes the followings:

* **Modular Roads**
  1. Road signs with different effects like patches, perturbation.
  2. Any road sign can be modified easily
* Animal pack
  1. It has animals like Fox, Pig, Crow etc..
* 3D people pack
  1. It has people with limited motion to check algorithm with people identification
* Vehicle Variety Pack
  1. Various kinds of vehicles including cars, Mopeds, Trucks etc..
* City - object pack
  * has various objects like house, dustbin, street light, roadsigns etc

## Work In Progress

* [ ] Standalone HTML API based dashboard
* [ ] More vehicles 
* [ ] Vehicle Spawning
* [ ] Central Weather control

