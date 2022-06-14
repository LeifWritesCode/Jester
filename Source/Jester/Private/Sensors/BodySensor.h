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

    // gets the x/y lean for the given body
    FVector2D GetLean(EBodyNumber skeletonId);

    // returns the vector of gestures from the given body
    std::vector<FGesture> GetGestures(EBodyNumber skeletonId);

    // returns the specified hand of the specified skeletons
    FHand GetHand(EBodyNumber skeletonId, EHand hand);

    // returns the specified joint of the specified skeleton
    FJoint GetJoint(EBodyNumber skeletonId, EJoint joint);

    // returns a pointer to the specified joint of the specified skeleton
    FJoint* GetJointPtr(EBodyNumber skeletonId, EJoint joint);

    // returns true and sets outResult if gesture is found, else false
    bool GetContinuousGestureResult(EBodyNumber skeletonId, FString gestureName, float& outResult);
 
    // returns true and sets outResult if gesture is found, else false
    bool GetDiscreteGestureResult(EBodyNumber skeletonId, FString gestureName, bool& outResult, float& outConfidence);

private:
    // initialisation function
    // kinect api is C style, so no way to overload constructors using delegation
    void Initialise(IKinectSensor* sensor);

    // process a single frame of data, if available
    void ProcessBodyFrame();

    // frees up source assets when tracking is lost
    void ProcessVgbSkeletonLost(EBodyNumber skeletonId);

    // process a frame from the visual gesture builder backend
    void ProcessVgbFrame(EBodyNumber skeletonId);

    // converts a Kinect IBody to a Jester Skeleton
    void ConvertRepresentationKinectToJester(IBody** body, EBodyNumber skeletonId);

    // creates the necessary assets for gesture recognition
    void UpdateGestureSource(EBodyNumber skeletonId, UINT64 newTrackingId);

    // local copy so we can close it later
    IKinectSensor* sensor;

    // thread safety
    FCriticalSection apiMutex;

    // handle on which we wait for frame events
    WAITABLE_HANDLE bodyWaitHandle;

    // source of body frames - not per skeleton
    IBodyFrameSource * bodySource;
    IBodyFrameReader * bodyReader;

    // the tracked skeletons
    Skeleton skeletons[BodyNumber_Count];
};
