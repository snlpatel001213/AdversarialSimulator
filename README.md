# Adversarial Simulator

## Adversarial Simulator

Adversarial Simulator allows one to simulate adversarial attacks in the virtual environment within the Unreal Engine and inference with Nvidia DRIVE SDK. This repository contains the code, assets, eample datasets, models, test results and a video demo for the paper :

## A Simulated Environment Approach to Investigate Effect of Adversarial Perturbations on Traffic Signs

| Sunil Patel | Pallab Maji |
| :---: | :---: |
| supatel@nvidia.com | pmaji@nvidia.com |
| Sunil Patel | Pallab Maji |

_**Abstract**_ **-To study effect of adversarial attack environment must be the same. Autonomous driving includes mainly 5 phases sense, perceive, map, plan and Drive. Autonomous Vehicle 'sense' the surrounding with the help of Cameras and Lidars. Deep Learning techniques are considered Blackbox and found to be vulnerable to adversarial attach. In this research we study the effect of various known adversarial attack in the Unreal Engine based high-fidelity real-time Ray-Traced simulated environment. This experiment seeks answer to questions like if adversarial attacks works in moving vehicle scenario and can an unknown network be targeted. We found that existing Blackbox and Whitebox attacks do not affect all traffic signs uniformly. Attack found to be affecting classification in static scenes are not similarly affecting in the moving vehicle scenarios. However, some attack found to completely block the identification of certain traffic signs.**

## This documentation covers vital  aspect of the project including ;

### 1. Installation procedures : 

Adversarial simulator requires two components to be installed including an Unreal Engine  and Nvidia Drive SDK. An Unreal Engine required for to generate a real-like scenario.

### 2.  Setting up the Environament

Post   installtion we require to  setup the environamnt so that different scenarios can be generated. In unreal engine such scenarios are organised in Maps. adversarial Simulator provides two maps. 

#### MAP - 1 

It has basic components for the rcontrolled testing , tracks are organized in parallel for testing plurality of scenarios in parallel, keeping the overall environamnt the same.  

#### MAP - 2 

It is consisting of vast open land with various objects like buildings, water , over bridges, traffic lights, traffic signals, roadside objects, various kind of animals etc. This map can be used for variety of testing.

### 3. Compiling Drivenet samples

Drive net sample comes up with video inferecne apabilities cby default. additionally if you want to predict on Images then you need to modify samples. This section covers all the basic procedure for compilation and modifying the samples to r

### 4. Add ons 

This section suggests various objects packs that can be added to the provided maps to create more complex scenarios.

