# Copyright (c) 2018, NVIDIA CORPORATION.  All rights reserved.

project(sample_sign_classifier C CXX)

#-------------------------------------------------------------------------------
# Project files
#-------------------------------------------------------------------------------
set(PUBLIC_DOCS
    README.md
)
find_package(Boost REQUIRED COMPONENTS filesystem)

set(SOURCES
    main.cpp
)

set(LIBRARIES
    ${Driveworks_LIBRARIES}
    ${Boost_LIBRARIES}
    samples_framework
    
)

#-------------------------------------------------------------------------------
# Final target
#-------------------------------------------------------------------------------

add_executable(${PROJECT_NAME} ${SOURCES})
#target_link_libraries(${PROJECT_NAME} PUBLIC )
target_include_directories(${PROJECT_NAME} PUBLIC ${Boost_INCLUDE_DIRS}) 
target_link_libraries(${PROJECT_NAME} ${LIBRARIES})
set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER "Samples")

# ------------------------------------------------------------------------------
# Install target
# ------------------------------------------------------------------------------
sdk_add_sample(${PROJECT_NAME})
