/////////////////////////////////////////////////////////////////////////////////////////
// This code contains NVIDIA Confidential Information and is disclosed
// under the Mutual Non-Disclosure Agreement.
//
// Notice
// ALL NVIDIA DESIGN SPECIFICATIONS AND CODE ("MATERIALS") ARE PROVIDED "AS IS" NVIDIA MAKES
// NO REPRESENTATIONS, WARRANTIES, EXPRESSED, IMPLIED, STATUTORY, OR OTHERWISE WITH RESPECT TO
// THE MATERIALS, AND EXPRESSLY DISCLAIMS ANY IMPLIED WARRANTIES OF NONINFRINGEMENT,
// MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
//
// NVIDIA Corporation assumes no responsibility for the consequences of use of such
// information or for any infringement of patents or other rights of third parties that may
// result from its use. No license is granted by implication or otherwise under any patent
// or patent rights of NVIDIA Corporation. No third party distribution is allowed unless
// expressly authorized by NVIDIA.  Details are subject to change without notice.
// This code supersedes and replaces all information previously supplied.
// NVIDIA Corporation products are not authorized for use as critical
// components in life support devices or systems without express written approval of
// NVIDIA Corporation.
//
// Copyright (c) 2015-2019 NVIDIA Corporation. All rights reserved.
//
// NVIDIA Corporation and its licensors retain all intellectual property and proprietary
// rights in and to this software and related documentation and any modifications thereto.
// Any use, reproduction, disclosure or distribution of this software and related
// documentation without an express license agreement from NVIDIA Corporation is
// strictly prohibited.
//
/////////////////////////////////////////////////////////////////////////////////////////

// Samples
#include <framework/DriveWorksSample.hpp>
#include <framework/SimpleStreamer.hpp>
#include <framework/SampleFramework.hpp>
#include <framework/SimpleCamera.hpp>
#include <framework/Checks.hpp>

// Core
#include <dw/core/Context.h>
#include <dw/core/VersionCurrent.h>
#include <dw/core/Logger.h>

// HAL
#include <dw/sensors/Sensors.h>

// SignNet
#include <dw/dnn/SignNet.h>
#include <dw/waitcondition/camera/TrafficLightSignClassifier.h>
#include <dw/waitcondition/camera/TrafficLightSignClassifier_processPipeline.h>

// DriveNet
#include <dw/dnn/DriveNet.h>
#include <dw/perception/object/camera/ObjectDetector.h>

// Renderer
#include <dwvisualization/core/RenderEngine.h>

// c++ header
// C++ Header
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>

#include <boost/filesystem.hpp>

using namespace dw_samples::common;

typedef std::vector<std::string> stringvec;
struct path_leaf_string
{
    std::string operator()(const boost::filesystem::directory_entry &entry) const
    {
        return entry.path().leaf().string();
    }
};

void read_directory(const std::string &name, stringvec &v)
{
    boost::filesystem::path p(name);
    boost::filesystem::directory_iterator start(p);
    boost::filesystem::directory_iterator end;
    std::transform(start, end, std::back_inserter(v), path_leaf_string());
}

class SignNetApp : public DriveWorksSample
{
private:
    // ------------------------------------------------
    // Driveworks Context and SAL
    // ------------------------------------------------
    dwContextHandle_t m_sdk = DW_NULL_HANDLE;
    dwVisualizationContextHandle_t m_viz = DW_NULL_HANDLE;
    dwSALHandle_t m_sal = DW_NULL_HANDLE;

    // ------------------------------------------------
    // DriveNet
    //-------------------------------------------------
    dwDriveNetHandle_t m_driveNet = DW_NULL_HANDLE;
    dwObjectDetectorHandle_t m_objectDetector = DW_NULL_HANDLE;
    const dwObjectClass *m_driveNetClasses = nullptr;
    uint32_t m_numDriveNetClasses = 0;
    const uint32_t m_maxClustersPerClass = 400U;
    dwObjectArray m_objectDetections;

    // ------------------------------------------------
    // SignNet
    // ------------------------------------------------
    dwSignNetHandle_t m_signNet = DW_NULL_HANDLE;
    dwTrafficLightSignClassifierHandle_t m_signClassifier = DW_NULL_HANDLE;
    const dwVector4f m_signBoxColor = {1.0f, 0.0f, 1.0f, 1.0f}; // orange

    // Labels of each class
    std::vector<std::string> m_classLabels;
    // Vectors of boxes and class label ids
    std::vector<std::vector<dwRectf>> m_dnnBoxList;
    std::vector<std::vector<std::string>> m_dnnLabelList;
    std::vector<std::vector<const char *>> m_dnnLabelListPtr;

    // Number of outputs (raw classes) in a given SignNet DNN.
    uint32_t m_numOutputs = 0;

    // ------------------------------------------------
    // Renderer
    // ------------------------------------------------
    dwRenderEngineHandle_t m_renderEngine = DW_NULL_HANDLE;
    dwImageHandle_t m_imageRGBA;
    std::unique_ptr<SimpleImageStreamerGL<>> m_streamerCUDA2GL;
    const dwVector4f m_colorCyan{10.0f / 255.0f, 230.0f / 255.0f, 230.0f / 255.0f, 1.0f};
    const dwVector4f m_colorDarkYellow{180.0f / 255.0f, 180.0f / 255.0f, 10.0f / 255.0f, 1.0f};

    // ------------------------------------------------
    // Camera
    // ------------------------------------------------
    std::unique_ptr<SimpleCamera> m_camera;
    dwImageGL *m_imgGl = nullptr;
    dwImageCUDA *m_rcbImage = nullptr;

    // ------------------------------------------------
    // Misc
    // ------------------------------------------------
    cudaStream_t m_cudaStream = 0;

    // Image File List
    stringvec imageList;
    // streamer from GL to CUDA
    dwImageStreamerHandle_t cm_streamerGL2CUDA = DW_NULL_HANDLE;
    dwImageGL m_sampleImage;
    dwImageProperties glProp{};
    dwImageHandle_t m_ImageGl;
    dwImageGL *ImageGl = nullptr;

    dwStatus status;
    uint32_t count = 0;

public:
    /// -----------------------------
    /// Initialize application
    /// -----------------------------
    SignNetApp(const ProgramArguments &args)
        : DriveWorksSample(args) {}

    /// -----------------------------
    /// Initialize modules
    /// -----------------------------
    bool onInitialize() override
    {
        // Initialize Cuda stream
        CHECK_CUDA_ERROR(cudaStreamCreateWithFlags(&m_cudaStream, cudaStreamNonBlocking));

        // -----------------------------------------
        // Initialize DriveWorks SDK context and SAL
        // -----------------------------------------
        {
            // initialize logger to print verbose message on console in color
            CHECK_DW_ERROR(dwLogger_initialize(getConsoleLoggerCallback(true)));
            CHECK_DW_ERROR(dwLogger_setLogLevel(DW_LOG_VERBOSE));

            // initialize SDK context, using data folder
            dwContextParameters sdkParams = {};
            sdkParams.dataPath = DataPath::get_cstr();
            sdkParams.enableCudaTaskGraph = getArgument("useCudaGraph").compare("1") == 0;

#ifdef VIBRANTE
            sdkParams.eglDisplay = getEGLDisplay();
#endif

            CHECK_DW_ERROR(dwInitialize(&m_sdk, DW_VERSION, &sdkParams));
            CHECK_DW_ERROR(dwSAL_initialize(&m_sal, m_sdk));
        }

        //------------------------------------------------------------------------------
        // initialize Renderer
        //------------------------------------------------------------------------------
        {
            CHECK_DW_ERROR(dwVisualizationInitialize(&m_viz, m_sdk));

            // Setup render engine
            dwRenderEngineParams params{};
            CHECK_DW_ERROR(dwRenderEngine_initDefaultParams(&params, getWindowWidth(), getWindowHeight()));
            params.defaultTile.lineWidth = 0.2f;
            params.defaultTile.font = DW_RENDER_ENGINE_FONT_VERDANA_20;
            CHECK_DW_ERROR(dwRenderEngine_initialize(&m_renderEngine, &params, m_viz));
        }

        //------------------------------------------------------------------------------
        // initialize Sensors
        //------------------------------------------------------------------------------
        {
            dwSensorParams params{};
            {
#ifdef VIBRANTE
                if (getArgument("input-type").compare("camera") == 0)
                {
                    std::string parameterString = "camera-type=" + getArgument("camera-type");
                    parameterString += ",camera-group=" + getArgument("camera-group");
                    parameterString += ",slave=" + getArgument("slave");
                    parameterString += ",serialize=false,output-format=raw,camera-count=4";
                    std::string cameraMask[4] = {"0001", "0010", "0100", "1000"};
                    uint32_t cameraIdx = std::stoi(getArgument("camera-index"));
                    if (cameraIdx < 0 || cameraIdx > 3)
                    {
                        std::cerr << "Error: camera index must be 0, 1, 2 or 3" << std::endl;
                        return false;
                    }
                    parameterString += ",camera-mask=" + cameraMask[cameraIdx];

                    params.parameters = parameterString.c_str();
                    params.protocol = "camera.gmsl";

                    m_camera.reset(new RawSimpleCamera(params, m_sal, m_sdk, m_cudaStream, DW_CAMERA_OUTPUT_NATIVE_PROCESSED));
                }
                else
#endif
                {
                    if ((getArgument("input-type").compare("imagefile") == 0) || (getArgument("input-type").compare("imagedir") == 0))
                    {

                        glProp.type = DW_IMAGE_GL;
                        glProp.width = getWindowWidth();
                        glProp.height = getWindowHeight();
                        glProp.format = DW_IMAGE_FORMAT_RGBA_UINT8;
                        glProp.memoryLayout = DW_IMAGE_MEMORY_TYPE_DEFAULT;

                        status = dwImageStreamerGL_initialize(&cm_streamerGL2CUDA, &glProp, DW_IMAGE_CUDA, m_sdk);

                        if (status != DW_SUCCESS)
                        {
                            logError("Cannot init image streamer m_streamerGL2CUDA: %s\n",
                                     dwGetStatusName(status));
                            return false;
                        }
                    }

                    else
                    {
                        std::string parameterString = getArgs().parameterString();
                        params.parameters = parameterString.c_str();
                        params.protocol = "camera.virtual";

                        std::string videoFormat = getArgument("video");
                        std::size_t found = videoFormat.find_last_of(".");

                        if (videoFormat.substr(found + 1).compare("h264") == 0 || videoFormat.substr(found + 1).compare("mp4") == 0)
                        {
                            m_camera.reset(new SimpleCamera(params, m_sal, m_sdk));
                            dwImageProperties outputProperties = m_camera->getOutputProperties();
                            outputProperties.type = DW_IMAGE_CUDA;
                            outputProperties.format = DW_IMAGE_FORMAT_RGB_FLOAT16_PLANAR;
                            m_camera->setOutputProperties(outputProperties);
                        }
                        else
                        {
                            m_camera.reset(new RawSimpleCamera(params, m_sal, m_sdk, m_cudaStream,
                                                               DW_CAMERA_OUTPUT_NATIVE_PROCESSED));
                        }
                    }
                }
            }
            if (getArgument("input-type").compare("imagedir") == 0)
            {
                read_directory(getArgument("path"), imageList);
            }
            else if (getArgument("input-type").compare("imagefile") == 0)
            {
                std::cout << "Image file name' " << getArgument("path") << std::endl;
            }
            else
            {
                if (m_camera == nullptr)
                {
                    logError("Camera could not be created\n");
                    return false;
                }
            }

#ifdef VIBRANTE
            if (getArgument("input-type").compare("camera") == 0)
            {
                dwCameraRawFormat rawFormat = m_camera->getCameraProperties().rawFormat;
                if (rawFormat != DW_CAMERA_RAW_FORMAT_RCCB &&
                    rawFormat != DW_CAMERA_RAW_FORMAT_BCCR &&
                    rawFormat != DW_CAMERA_RAW_FORMAT_CRBC &&
                    rawFormat != DW_CAMERA_RAW_FORMAT_CBRC)
                {
                    logError("Camera not supported, see documentation\n");
                    return false;
                }
            }
#endif

            if ((getArgument("input-type").compare("imagefile") == 0) || (getArgument("input-type").compare("imagedir") == 0))
            {
                std::cout << "Image mode: " << getArgument("input-type") << " Image path " << getArgument("path") << "" << std::endl;
            }
            else
            {

                std::cout << "Camera image with " << m_camera->getCameraProperties().resolution.x << "x"
                          << m_camera->getCameraProperties().resolution.y << " at "
                          << m_camera->getCameraProperties().framerate << " FPS" << std::endl;

                dwImageProperties displayProperties = m_camera->getOutputProperties();
                displayProperties.format = DW_IMAGE_FORMAT_RGBA_UINT8;

                CHECK_DW_ERROR(dwImage_create(&m_imageRGBA, displayProperties, m_sdk));

                m_streamerCUDA2GL.reset(new SimpleImageStreamerGL<>(displayProperties, 1000, m_sdk));
            }
        }

        dwPrecision precison = DW_PRECISION_FP32;
        if (getArgument("precision").compare("fp32") == 0)
        {
            precison = DW_PRECISION_FP32;
        }
        else if (getArgument("precision").compare("fp16") == 0)
        {
            precison = DW_PRECISION_FP16;
        }
        else
        {
            std::cerr << "Unknown SignNet model precision." << std::endl;
            return false;
        }

        //------------------------------------------------------------------------------
        // initialize DriveNet detector
        //------------------------------------------------------------------------------
        {
            // Initialize DriveNet detector
            dwDriveNetParams driveNetParams{};
            CHECK_DW_ERROR(dwDriveNet_initDefaultParams(&driveNetParams));
            driveNetParams.maxProposalsPerClass = 3000U;
            driveNetParams.networkPrecision = precison;
            CHECK_DW_ERROR(dwDriveNet_initialize(&m_driveNet, &driveNetParams, m_sdk));
            CHECK_DW_ERROR(dwDriveNet_getAvailableClasses(&m_driveNetClasses, &m_numDriveNetClasses, m_driveNet));
            // Initialize ObjectDetector from DriveNet
            dwObjectDetectorParams detectorParams{};
            dwObjectDetector_initDefaultParams(&detectorParams);
            CHECK_DW_ERROR(dwObjectDetector_initializeFromDriveNet(&m_objectDetector, &detectorParams, m_driveNet, m_sdk));
            CHECK_DW_ERROR(dwObjectDetector_setCUDAStream(m_cudaStream, m_objectDetector));

            // Allocate memory for the detections array
            CHECK_DW_ERROR(dwObjectArray_create(&m_objectDetections, m_maxClustersPerClass, DW_OBJECT_TYPE_CAMERA));
        }

        //------------------------------------------------------------------------------
        // initialize SignNet detector.
        //------------------------------------------------------------------------------
        {
            // Initialize SignNet network.
            dwSignNetParams signNetParams{};
            // Default to FP32, DW_SIGNNET_MODEL
            CHECK_DW_ERROR(dwSignNet_initDefaultParams(&signNetParams));

            std::string model = getArgument("model");
            if (model == "US_V1")
            {
                signNetParams.networkModel = DW_SIGNNET_MODEL_US_V1;
            }
            else if (model == "US_V2")
            {
                signNetParams.networkModel = DW_SIGNNET_MODEL_US_V2;
            }
            else if (model == "EU_V1")
            {
                signNetParams.networkModel = DW_SIGNNET_MODEL_EU_V1;
            }
            else if (model == "EU_V2")
            {
                signNetParams.networkModel = DW_SIGNNET_MODEL_EU_V2;
            }
            else
            {
                throw std::runtime_error("Unsupported model type.");
            }
            signNetParams.networkPrecision = precison;

            CHECK_DW_ERROR(dwSignNet_initialize(&m_signNet, &signNetParams, m_sdk));

            // Initialize TrafficLightSign Classifier.

            CHECK_DW_ERROR(dwTrafficLightSignClassifier_initializeFromSignNet(&m_signClassifier, m_signNet, m_sdk));
            CHECK_DW_ERROR(dwTrafficLightSignClassifier_setCUDAStream(m_cudaStream, m_signClassifier));

            // Get number of outputs in the DNN.

            dwSignNet_getNumberOutputs(&m_numOutputs, m_signNet);

            // Initialize box list.
            m_dnnBoxList.resize(m_numOutputs);
            m_dnnLabelList.resize(m_numOutputs);
            m_dnnLabelListPtr.resize(m_numOutputs);

            // Get mapping from classId to text label for each sign.
            m_classLabels.resize(m_numOutputs);
            for (uint32_t classIdx = 0U; classIdx < m_numOutputs; ++classIdx)
            {
                const char *classLabel;
                CHECK_DW_ERROR(dwSignNet_getClassLabel(&classLabel, classIdx, m_signNet));
                m_classLabels[classIdx] = classLabel;

                // Reserve label and box lists.
                m_dnnBoxList[classIdx].reserve(m_maxClustersPerClass);
                m_dnnLabelList[classIdx].reserve(m_maxClustersPerClass);
                m_dnnLabelListPtr[classIdx].reserve(m_maxClustersPerClass);
            }
        }

        return true;
    }

    ///------------------------------------------------------------------------------
    /// Main processing of the sample
    ///     - collect sensor frame
    ///     - run detection and classification
    ///------------------------------------------------------------------------------
    void onProcess() override
    {
        // read from camera
        m_rcbImage = nullptr;

        if ((getArgument("input-type").compare("imagefile") == 0))
        {
            if (count == 0)
            {
                getNextFrame(&m_rcbImage, &m_imgGl);
            }
            else
            {

                if (getArgument("viz").compare("false") == 0)
                {
                    std::exit(0);
                }
                else
                {
                    // char quit;
                    // std::cout << "Press any key to exit " << std::endl;
                    // std::cin >> quit;
                    std::exit(0);
                    // onRelease();
                }
            }
        }
        else
        {
            getNextFrame(&m_rcbImage, &m_imgGl);
            std::this_thread::yield();
            while (m_rcbImage == nullptr)
            {
                onReset();
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
                getNextFrame(&m_rcbImage, &m_imgGl);
            }
        }

        // Clear vectors
        for (uint32_t classIdx = 0U; classIdx < m_classLabels.size(); ++classIdx)
        {
            m_dnnLabelListPtr[classIdx].clear();
            m_dnnLabelList[classIdx].clear();
            m_dnnBoxList[classIdx].clear();
        }

        // Run DriveNet object detector on the image.
        CHECK_DW_ERROR(dwObjectDetector_detectObjects(&m_objectDetections, m_rcbImage, m_objectDetector));

        // At this point m_objectDetections contains detections of all classes (cars, pedestrians, ...)
        // Filter all but DW_OBJECT_CLASS_TRAFFIC_SIGN detections from it.
        dwObjectCamera trafficSigns[m_maxClustersPerClass];
        uint32_t numSigns = 0;
        for (uint32_t i = 0; i < m_objectDetections.count; ++i)
        {
            dwObjectCamera *array = static_cast<dwObjectCamera *>(m_objectDetections.objects);
            if (array[i].obstacle.objectClass == DW_OBJECT_CLASS_TRAFFIC_SIGN)
            {
                trafficSigns[numSigns++] = array[i];
            }
        }

        if (numSigns > 0)
        {
            // Infer traffic sign classifier.
            CHECK_DW_ERROR(dwTrafficLightSignClassifier_inferDeviceAsync(&m_rcbImage, trafficSigns, numSigns, DW_OBJECT_CLASS_TRAFFIC_SIGN, m_signClassifier));
            // Interpret host.
            CHECK_DW_ERROR(dwTrafficLightSignClassifier_interpretHost(numSigns, DW_OBJECT_CLASS_TRAFFIC_SIGN, m_signClassifier));
            // Get Classification Results.
            CHECK_DW_ERROR(dwTrafficLightSignClassifier_getClassifiedObjects(trafficSigns, numSigns, m_signClassifier));

            for (uint32_t id = 0; id < numSigns; ++id)
            {
                m_dnnBoxList[trafficSigns[id].objectSubClass].push_back(trafficSigns[id].box2D);
                std::string box_annotation = m_classLabels[trafficSigns[id].objectSubClass];
                // This operation is safe because m_dnnLabelList is allocated using `reserve` at initialization
                // and it is not going to reallocate.
                m_dnnLabelList[trafficSigns[id].objectSubClass].push_back(box_annotation);
                m_dnnLabelListPtr[trafficSigns[id].objectSubClass].push_back(m_dnnLabelList[trafficSigns[id].objectSubClass].back().c_str());
            }
        }
    }

    ///------------------------------------------------------------------------------
    /// Render sample output on screen
    ///     - render video
    ///     - render sign state
    ///------------------------------------------------------------------------------
    void onRender() override
    {
        if (getArgument("viz").compare("false") == 0)
        {
            for (size_t classIdx = 0; classIdx < m_numOutputs; classIdx++)
            {
                auto dwrects = m_dnnBoxList[classIdx];
                for (unsigned int i = 0; i < dwrects.size(); i++)
                {
                    std::string box_annotation = m_classLabels[classIdx];
                    std::cout << "ClassId: " << classIdx <<  " ClassName: " << box_annotation<<std::endl;
                    std::cout << "index " << i << " x: " << dwrects[i].x << ", y: " << dwrects[i].y << ", width: " << dwrects[i].width << ", height: " << dwrects[i].height << std::endl;
                    std::cout << "-----------------------------------------------------------------------------" << std::endl;
                }
            }
            return;
        }

        CHECK_DW_ERROR(dwRenderEngine_setTile(0, m_renderEngine));
        CHECK_DW_ERROR(dwRenderEngine_resetTile(m_renderEngine));

        dwVector2f range{};
        range.x = m_imgGl->prop.width;
        range.y = m_imgGl->prop.height;
        CHECK_DW_ERROR(dwRenderEngine_setCoordinateRange2D(range, m_renderEngine));
        CHECK_DW_ERROR(dwRenderEngine_renderImage2D(m_imgGl, {0.0f, 0.0f, range.x, range.y}, m_renderEngine));
        std::cout << " %%% [";
        for (size_t classIdx = 0; classIdx < m_numOutputs; classIdx++)
        {
            CHECK_DW_ERROR(dwRenderEngine_setColor(m_signBoxColor, m_renderEngine));
            CHECK_DW_ERROR(dwRenderEngine_renderWithLabels(DW_RENDER_ENGINE_PRIMITIVE_TYPE_BOXES_2D,
                                                           &m_dnnBoxList[classIdx][0],
                                                           sizeof(dwRectf),
                                                           0,
                                                           &m_dnnLabelListPtr[classIdx][0],
                                                           m_dnnBoxList[classIdx].size(),
                                                           m_renderEngine));

            auto dwrects = m_dnnBoxList[classIdx];
            
            for (unsigned int i = 0; i < dwrects.size(); i++)
            {
                std::string box_annotation = m_classLabels[classIdx];
                std::cout << " { 'totalIdx' :"<< m_numOutputs <<", 'classIdx':" << classIdx <<  ", 'box_annotation':'" << box_annotation << "', 'index' :" << classIdx << ", 'x':" << dwrects[i].x << ", 'y':" << dwrects[i].y << ", 'width':" << dwrects[i].width << ", 'height':" << dwrects[i].height << "},";
                
            }
            
        }

        renderutils::renderFPS(m_renderEngine, getCurrentFPS());
        std::cout << "] %%% " ;
    }

    ///------------------------------------------------------------------------------
    /// Free up used memory here
    ///------------------------------------------------------------------------------
    void onRelease() override
    {
        if (m_imageRGBA)
        {
            CHECK_DW_ERROR(dwImage_destroy(m_imageRGBA));
        }
        // Release Object Detector
        CHECK_DW_ERROR(dwObjectDetector_release(m_objectDetector));

        // Release Sign Detector
        CHECK_DW_ERROR(dwTrafficLightSignClassifier_release(m_signClassifier));

        // Release DriveNet
        CHECK_DW_ERROR(dwDriveNet_release(m_driveNet));

        // Release array of object detections
        CHECK_DW_ERROR(dwObjectArray_destroy(&m_objectDetections));

        // Release SignNet
        CHECK_DW_ERROR(dwSignNet_release(m_signNet));

        // Release render engine
        CHECK_DW_ERROR(dwRenderEngine_release(m_renderEngine));
        // Release camera
        if (m_camera)
        {
            m_camera.reset();
        }

        if (m_ImageGl)
        {
            CHECK_DW_ERROR(dwImage_destroy(m_ImageGl));
        }

        if (imageList.size() > 0)
        {
            imageList.clear();
        }

        // Release SDK
        CHECK_DW_ERROR(dwSAL_release(m_sal));
        CHECK_DW_ERROR(dwVisualizationRelease(m_viz));
        CHECK_DW_ERROR(dwRelease(m_sdk));
        CHECK_CUDA_ERROR(cudaStreamDestroy(m_cudaStream));
    }

    ///------------------------------------------------------------------------------
    /// Reset tracker and detector
    ///------------------------------------------------------------------------------
    void onReset() override
    {
        CHECK_DW_ERROR(dwSignNet_reset(m_signNet));
        CHECK_DW_ERROR(dwDriveNet_reset(m_driveNet));
        CHECK_DW_ERROR(dwTrafficLightSignClassifier_reset(m_signClassifier));
        CHECK_DW_ERROR(dwObjectDetector_reset(m_objectDetector));
        count = 0;
    }

    ///------------------------------------------------------------------------------
    /// Change renderer properties when main rendering window is resized
    ///------------------------------------------------------------------------------
    void onResizeWindow(int width, int height) override
    {
        {
            CHECK_DW_ERROR(dwRenderEngine_reset(m_renderEngine));
            dwRectf rect;
            rect.width = width;
            rect.height = height;
            rect.x = 0;
            rect.y = 0;
            CHECK_DW_ERROR(dwRenderEngine_setBounds(rect, m_renderEngine));
        }
    }

private:
    //------------------------------------------------------------------------------
    void getNextFrame(dwImageCUDA **nextFrameCUDA, dwImageGL **nextFrameGL)
    {
        if (getArgument("input-type").compare("imagefile") == 0)
        {
            getNextFrameImage(nextFrameCUDA, nextFrameGL);
        }
        else if ((getArgument("input-type").compare("imagedir") == 0))
        {
            getNextFrameImageDir(nextFrameCUDA, nextFrameGL);
        }

        else
        {
            dwImageHandle_t nextFrame = m_camera->readFrame();
            if (nextFrame == nullptr)
            {
                m_camera->resetCamera();
            }
            else
            {
                CHECK_DW_ERROR(dwImage_getCUDA(nextFrameCUDA, nextFrame));
                CHECK_DW_ERROR(dwImage_copyConvert(m_imageRGBA, nextFrame, m_sdk));
                dwImageHandle_t frameGL = m_streamerCUDA2GL->post(m_imageRGBA);
                CHECK_DW_ERROR(dwImage_getGL(nextFrameGL, frameGL));
            }
        }
    }
    void getNextFrameImage(dwImageCUDA **nextFrameCUDA, dwImageGL **nextFrameGL)
    {

        dwRenderEngine_createImageGLFromFile(&m_sampleImage, getArgument("path").c_str(), m_renderEngine);
        ImageGl = &m_sampleImage;
        CHECK_DW_ERROR(dwImage_createAndBindTexture(&m_ImageGl, glProp, ImageGl->tex, ImageGl->target));
        CHECK_DW_ERROR(dwImageStreamerGL_producerSend(m_ImageGl, cm_streamerGL2CUDA));
        status = dwImageStreamerGL_consumerReceive(&m_imageRGBA, 1000, cm_streamerGL2CUDA);
        if (status != DW_SUCCESS)
        {
            std::stringstream err;
            err << "Cannot return cuda image to cm_streamerGL2CUDA: " << dwGetStatusName(status);
            throw std::runtime_error(err.str().c_str());
        }
        CHECK_DW_ERROR(dwImage_getCUDA(nextFrameCUDA, m_imageRGBA));
        CHECK_DW_ERROR(dwImage_getGL(nextFrameGL, m_ImageGl));
        count++;
    }

    void getNextFrameImageDir(dwImageCUDA **nextFrameCUDA, dwImageGL **nextFrameGL)
    {

        if (count < imageList.size())
        {
            std::cout << "reading file " << getArgument("path") + "/" + imageList[count] << std::endl;
            dwRenderEngine_createImageGLFromFile(&m_sampleImage, (getArgument("path") + "/" + imageList[count]).c_str(), m_renderEngine);
            ImageGl = &m_sampleImage;
            CHECK_DW_ERROR(dwImage_createAndBindTexture(&m_ImageGl, glProp, ImageGl->tex, ImageGl->target));
            CHECK_DW_ERROR(dwImageStreamerGL_producerSend(m_ImageGl, cm_streamerGL2CUDA));
            status = dwImageStreamerGL_consumerReceive(&m_imageRGBA, 1000, cm_streamerGL2CUDA);
            if (status != DW_SUCCESS)
            {
                std::stringstream err;
                err << "Cannot return cuda image to cm_streamerGL2CUDA: " << dwGetStatusName(status);
                throw std::runtime_error(err.str().c_str());
            }
            CHECK_DW_ERROR(dwImage_getCUDA(nextFrameCUDA, m_imageRGBA));
            CHECK_DW_ERROR(dwImage_getGL(nextFrameGL, m_ImageGl));

            count++;
        }
    }
};

int main(int argc, const char **argv)
{
    // -------------------
    // define all arguments used by the application

    ProgramArguments args(argc, argv,
                          {
#ifdef VIBRANTE
                              ProgramArguments::Option_t("camera-type", "ar0231-rccb-bae-sf3324", "camera gmsl type (see sample_sensors_info for all available camera types on this platform)"),
                              ProgramArguments::Option_t("camera-group", "a", "input port"),
                              ProgramArguments::Option_t("camera-index", "0", "camera index within the camera group: 0-3"),
                              ProgramArguments::Option_t("slave", "0", "activate slave mode for Tegra B"),
                              ProgramArguments::Option_t("input-type", "video", "input type either video or camera or image"),
#endif
                              ProgramArguments::Option_t("viz", "false", "true to visualize and default false to avoid visualization of output"),
                              ProgramArguments::Option_t("path", (DataPath::get() + "/image").c_str(), "path to image"),
                              ProgramArguments::Option_t("input-type", "video", "input type either video or camera or imagefile or imagedir"),
                              ProgramArguments::Option_t("video", (DataPath::get() + "/samples/waitcondition/traffic_signs.h264").c_str(), "path to video"),
                              ProgramArguments::Option_t("model", "US_V2", "Name of a SignNet model to use for classification of signs. Choices: US_V1, US_V2, EU_V1, EU_V2. Default is US_v2"),
                              ProgramArguments::Option_t("precision", "fp32", "network precision. Possible options are \"fp16\" and \"fp32\"."),
                              ProgramArguments::Option_t("useCudaGraph", "0", "switch to use Cuda Graph for infer")},
                          "SignNet sample which classifies traffic sign status.");

    SignNetApp app(args);
    app.initializeWindow("SignNet Sample", 1280, 800, args.enabled("offscreen"));
    if (!args.enabled("offscreen"))
        app.setProcessRate(30);
    return app.run();
}