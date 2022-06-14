// Project Jester
// Implements a sensor encapsulating skeletal tracking and body-indexed gesture recognition
// Copyright 2018 Leif Walker-Grant, Inc. All Rights Reserved.
#pragma once

#include <string>
#include "Kinect.h"
#include "JesterTypes.h"
#include "JesterTypes.internal.h"

class BodySensor
{
public:
    BodySensor();
    BodySensor(IKinectSensor* sensor);
    ~BodySensor();

    // process any new data
    bool Tick(float deltaTime);

    // loads the database and adds it to the known gestures
    bool SetGestureDatabase(std::string filename);

    // returns true if the body is actively associated with the kinect device
    bool IsBodyTracked(EBodyNumber skeletonId);

    // returns the specified joint of the specified skeleton
    FJoint GetJoint(EBodyNumber skeletonId, EJoint joint);

    // returns a pointer to the specified joint of the specified skeleton
    FJoint* GetJointPtr(EBodyNumber skeletonId, EJoint joint);

    // returns orientation of the specified joint of the specified skeleton
    FJointOrientation GetJointOrientation(EBodyNumber skeletonId, EJoint joint);

    // returns orientation of the specified joint of the specified skeleton
    FJointOrientation* GetJointOrientationPtr(EBodyNumber skeletonId, EJoint joint);

    // returns true and sets outResult if gesture is found, else false
    bool GetContinuousGestureResult(EBodyNumber skeletonId, std::string gestureName, float* outResult);
 
    // returns true and sets outResult if gesture is found, else false
    bool GetDiscreteGestureResult(EBodyNumber skeletonId, std::string gestureName, bool* outResult);

private:
    // initialisation function
    // kinect api is C style, so no way to overload constructors using delegation
    void Initialise(IKinectSensor* sensor);

    // handle on which we wait for frame events
    WAITABLE_HANDLE bodyWaitHandle;

    // source of body frames
    IBodyFrameSource * bodySource;
    IBodyFrameReader * bodyReader;

    // gestures loaded
    IGesture * gestureLibrary;

    // the tracked skeletons
    Skeleton skeletons[BodyNumber_Count];
};
