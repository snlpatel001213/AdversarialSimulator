# Copyright (c) 2018-2019, NVIDIA CORPORATION.  All rights reserved.

@page dwx_sign_classification_sample Sign Classification Sample (SignNet)
@tableofcontents

@section dwx_sign_classification_description Description

This traffic sign classification example demonstrates how to use the NVIDIA<sup>&reg;</sup>
proprietary SignNet deep neural network (DNN) to perform traffic sign classification. 
It outputs the class of the traffic signs from images captured by the ego car.

SignNet currently supports RCB images. RGBA images are not supported. SignNet models currently cover two geographical regions. For one of such regions, the United States (US), two models are provided: `US_V1` and `US_V2`. The other supported region is the European Union (EU). For EU, there are also two available models: `EU_V1` and `EU_V2`. Thus, each of the supported regions has a version 1 and version 2 models. The main difference between different model versions for the same region is in the number of supported classes. Models version 2 for each region have significantly more traffic sign classes covered than version 1 models.

The default model for the sample app is `US_V2` for the US. In order to use the European traffic sign models, or to use the US model version 1, one would need to explicitly select it with a corresponding command line parameter when using the sample app.

The classification sample shows a simple implementation of traffic sign classification built around the NVIDIA SignNet DNN. The classification is done by first detecting the traffic signs with the help of NVidia DriveNet DNN and then classification of found image-crops with the help of SignNet DNN. There is no tracking of traffic signs applied, so one may notice some flickering of detections. 
For more information on the SignNet DNN and how to customize it for your applications, consult your NVIDIA sales or business representative.

@subsection dwx_sign_classification_sensor_details Sensor Details

The image datasets used to train SignNet have been captured by a View Sekonix Camera Module (SF3325) with
AR0231 RCCB sensor. The camera is mounted on a rig on top of the vehicle. Demo videos are
captured at 2.3 MP and downsampled to 960 x 604. Eight cameras were used to collect the data for training the provided SignNet models.
The following list shows the setup position and field of view (FOV) of each such camera:
- Center Front 60&deg; FOV
- Center Front 120&deg; FOV
- Center Front 30&deg; FOV
- Center Right 120&deg; FOV
- Center Left 120&deg; FOV
- Rear Left 120&deg; FOV
- Rear Center 120&deg; FOV
- Rear Center 60&deg; FOV

To achieve the best traffic sign detection performance, NVIDIA recommends to adopt a camera setup similar to one or more cameras from the list above and align the video center vertically with the horizon before recording new videos.

@subsection dwx_sign_classification_sample_limitations Limitations

@warning Currently, the SignNet DNN has limitations that could affect its performance:
- It was trained mostly for bright day-light, overcast, twilight, non-rain visibility conditions. 
  Training for artificial light, night-light conditions, and rainy-weather visibility was limited and, thus, the performance of the classifier may suffer in rain or in constrained illumination.
- The classification performance of SignNet depends on the size of the traffic signs detected in an image frame. 
  Good classification performance is observed when the height of the traffic signs is 20 pixels or more. Predictions for very small signs may be unreliable.
- The provided SignNet models were trained on data collected in the United States and countries comprising European Union.
  As a result, SignNet models may not be suitable for other geographical regions. However, the use of the EU model may be appropriate for other countries which adopted the Vienna convention for traffic signs. But the specific sign classes available in those countries should be reviewed case by cases basis against the ones available in the provided model.

Even though the SignNet DNN was trained with data from cameras setup pointing in various direction of the sensor rig (see the list above), it is recommeded to use it for the following directional and FOV setup::
* Center-front camera location with a 60&deg; FOV.
* Center-front camera location with a 120&deg; FOV.

@section dwx_sign_classification_running Running the Sample

    ./sample_sign_classifier  --input-type=[video|camera]
                              --model=[US_V1|US_V2|EU_V1|EU_V2]
                              --video=[path/to/video]
                              --camera-type=[camera]
                              --camera-group=[a|b|c|d]
                              --slave=[0|1]
                              --camera-index=[0|1|2|3]
                              --precision=[fp16|fp32]
                              --useCudaGraph=[0|1]
where

    --input-type=[video|camera]
            Defines if the input is from live camera or from a recorded video.
            Live camera is supported only on NVIDIA DRIVE(tm) platforms.
            It is not supported on Linux (x86 architecture) host systems.
            Default value: video

    --video=[path/to/video]
            Specifies the absolute or relative path of a raw, lraw or h264 recording.
            Only applicable if --input-type=video
            Default value: samples/waitcondition/traffic_signs.h264
    
    --model=[US_V1|US_V2|EU_V1|EU_V2]
            Specifies which regional signnet model and its version to use for classifying the signs. 
            Default value: US_V2 (USA model, version 2).

    --camera-type=[camera]
            Specifies a supported AR0231 `RCCB` sensor.
            Only applicable if --input-type=camera.
            Default value: ar0231-rccb-bae-sf3324

    --camera-group=[a|b|c|d]
            Is the group where the camera is connected to.
            Only applicable if --input-type=camera.
            Default value: a

    --slave=[0|1]
            Setting this parameter to 1 when running the sample on Xavier B accesses the camera
            on Xavier A.
            Applicable only when --input-type=camera.
            Default value: 0

    --camera-index=[0|1|2|3]
            Specifies the camera index within the camera group.
            Default value: 0

    --precision=[fp16|fp32]
            Specifies the precision to be used for the SignNet DNN model.
            Default value: fp32

    --useCudaGraph=[0|1]
            Setting this parameter to 1 runs SignNet DNN inference by CUDAGraph if the hardware supports.
            Default value: 0

### To run the sample on Linux

    ./sample_sign_classifier --video=<video file.raw>
or
    ./sample_sign_classifier --video=<video file.lraw>
or
    ./sample_sign_classifier --video=<video file.h264>
   
### To run the sample on video with european signs classifier version 2:

    ./sample_sign_classifier --video=<video file.h264> --signnet-model EU_V2    

### To run the sample on a camera on NVIDIA DRIVE platforms

    ./sample_sign_classifier --input-type=camera --camera-type=<rccb camera type> --camera-group=<camera-group> --camera-index=<camera idx on camera group> --model <model_name>

where `<rccb camera type>` is one of the following:

- `ar0231-rccb-bae-sf3324`
- `ar0231-rccb-bae-sf3325`


@subsection dwx_sign_classification_output Output

The sample creates a window, displays a video, and overlays bounding boxes for detected traffic signs.
The class of the sign is displayed with the text label above the bounding box.

The following table describes the models provided as part of the package. Follow the hyperlinks to the the full list of classes
supported by each model.

|    MODEL NAME     |    MODEL IDENTIFIER    |     SUPPORTED REGION      | # OF MODEL'S OUTPUTS | # OF DISTINCT CLASS LABELS | DESCRIPTION    |
|-------------------|------------------------|---------------------------|----------------------|----------------------------|----------------|
| @ref signnet_US_v1_0 | US_V1  | United States of America |     30      |       24       | Minimal USA sign model with focus on speed limit signs.|
| @ref signnet_US_v2_0 | US_V2  | United States of America |     312     |       272      | Advanced USA model with expanded sign coverage.        |
| @ref signnet_EU_v1_0 | EU_V1  | European Union           |     31      |       28       | Minimal EU sign model with focus on speed limit signs. |
| @ref signnet_EU_v2_0 | EU_V2  | European Union           |     176     |       159      | Advanced EU model with expanded sign coverage.         |

Note, the EU SignNet models may be appropriate to classify road signs from other non-EU countries that follow Vienna convention.

![Wait Conditions Classification Sample](sample_trafficsign_classification.png)

@section dwx_sign_classification_more Additional Information

For more information, see @ref signnet_mainsection.
