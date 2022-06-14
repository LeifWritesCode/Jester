// Project Jester
// Implements a sensor encapsulating skeletal tracking and body-indexed gesture recognition
// Copyright 2018 Leif Walker-Grant, Inc. All Rights Reserved.

#include "BodySensor.h"

#pragma region private api

#pragma endregion

#pragma region ctor/dtor

BodySensor::BodySensor()
{
    IKinectSensor* sensor;
    GetDefaultKinectSensor(&sensor);
    Initialise(sensor);
}

BodySensor::BodySensor(IKinectSensor* sensor)
{
    Initialise(sensor);
}

BodySensor::~BodySensor()
{
    // todo: destructions
}

void BodySensor::Initialise(IKinectSensor* sensor)
{
    // todo: initial setup
}

#pragma endregion

#pragma region tick

bool BodySensor::Tick(float deltaTime)
{
    return true;
}

#pragma endregion

#pragma region public api

bool BodySensor::SetGestureDatabase(std::string filename)
{
    return false;
}

bool BodySensor::IsBodyTracked(EBodyNumber skeletonId)
{
    return false;
}

FJoint BodySensor::GetJoint(EBodyNumber skeletonId, EJoint joint)
{
    return skeletons[skeletonId].Joints[joint];
}

FJoint * BodySensor::GetJointPtr(EBodyNumber skeletonId, EJoint joint)
{
    return &(skeletons[skeletonId].Joints[joint]);
}

FJointOrientation BodySensor::GetJointOrientation(EBodyNumber skeletonId, EJoint joint)
{
    return skeletons[skeletonId].Orientations[joint];
}

FJointOrientation * BodySensor::GetJointOrientationPtr(EBodyNumber skeletonId, EJoint joint)
{
    return &(skeletons[skeletonId].Orientations[joint]);
}

bool BodySensor::GetContinuousGestureResult(EBodyNumber skeletonId, std::string gestureName, float* outResult)
{
    return false;
}

bool BodySensor::GetDiscreteGestureResult(EBodyNumber skeletonId, std::string gestureName, bool* outResult)
{
    return false;
}

#pragma endregion
