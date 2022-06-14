// Project Jester
// Implements a sensor encapsulating skeletal tracking and body-indexed gesture recognition
// Copyright 2018 Leif Walker-Grant, Inc. All Rights Reserved.

#include "BodySensor.h"

#include "Runtime/Core/Public/Misc/ScopeLock.h"
#include "Kinect.h"
#include "Kinect.VisualGestureBuilder.h"

// helper macro
#define WAIT_OBJECT(n) (WAIT_OBJECT_0 + (n))

// ms to wait for handles to timeout
#define WAIT_TIME 1

#pragma region ctor/dtor

BodySensor::BodySensor()
{
    IKinectSensor* sensor;
    GetDefaultKinectSensor(&sensor);
    Initialise(sensor);
}

BodySensor::BodySensor(IKinectSensor* _sensor)
{
    Initialise(_sensor);
}

BodySensor::~BodySensor()
{
    bodyReader->UnsubscribeFrameArrived(bodyWaitHandle);
    sensor->Close();

    for (int i = 0; i < numGesturesInLibrary; ++i) SAFE_RELEASE(gestureLibrary[i]);
    SAFE_RELEASE(bodyReader);
    SAFE_RELEASE(bodySource);

    for (Skeleton s : skeletons)
    {
        if (s.Gestures.Source != nullptr)
            s.Gestures.Source->UnsubscribeTrackingIdLost(s.Gestures.TrackingIdWaitHandle);

        if (s.Gestures.Reader != nullptr)
            s.Gestures.Reader->UnsubscribeFrameArrived(s.Gestures.GestureWaitHandle);

        SAFE_RELEASE(s.Gestures.Reader);
        SAFE_RELEASE(s.Gestures.Source);
    }
}

void BodySensor::Initialise(IKinectSensor* _sensor)
{
    HRESULT result;
    BOOLEAN sensorOpen;

    // i'm not so trusting as not to do this
    check(_sensor != nullptr);
    sensor = _sensor;

    // attempt to open the sensor
    sensor->get_IsOpen(&sensorOpen);
    check(!sensorOpen);
    if (!sensorOpen)
    {
        sensor->Open();
        sensor->get_IsOpen(&sensorOpen);
        check(sensorOpen);
    }

    // attempt to get the frame source
    sensor->get_BodyFrameSource(&bodySource);
    check(bodySource != nullptr);

    // attempt to get the frame reader
    bodySource->OpenReader(&bodyReader);
    check(bodyReader != nullptr);

    // and subscribe
    result = bodyReader->SubscribeFrameArrived(&bodyWaitHandle);
    check(result == S_OK);
}

#pragma endregion

#pragma region tick

// wait on all handles and then call the necessary function
bool BodySensor::Tick(float deltaTime)
{
    HANDLE handles[] = {
        reinterpret_cast<HANDLE> (bodyWaitHandle), /*
        reinterpret_cast<HANDLE> (skeletons[Body_0].Gestures.WaitHandle),
        reinterpret_cast<HANDLE> (skeletons[Body_1].Gestures.WaitHandle),
        reinterpret_cast<HANDLE> (skeletons[Body_2].Gestures.WaitHandle),
        reinterpret_cast<HANDLE> (skeletons[Body_3].Gestures.WaitHandle),
        reinterpret_cast<HANDLE> (skeletons[Body_4].Gestures.WaitHandle),
        reinterpret_cast<HANDLE> (skeletons[Body_5].Gestures.WaitHandle), */
    };
    
    static size_t count = sizeof(handles) / sizeof(*handles);
    switch (MsgWaitForMultipleObjects(count, handles, false, WAIT_TIME, QS_ALLINPUT))
    {
        case WAIT_OBJECT(0):
            check(count != 0)
            ProcessBodyFrame();
            break;

        case WAIT_OBJECT(1):
            ProcessVgbSkeletonLost(Body_0);
            break;

        case WAIT_OBJECT(2):
            ProcessVgbSkeletonLost(Body_1);
            break;

        case WAIT_OBJECT(3):
            ProcessVgbSkeletonLost(Body_2);
            break;

        case WAIT_OBJECT(4):
            ProcessVgbSkeletonLost(Body_3);
            break;

        case WAIT_OBJECT(5):
            ProcessVgbSkeletonLost(Body_4);
            break;

        case WAIT_OBJECT(6):
            ProcessVgbSkeletonLost(Body_5);
            break;

        case WAIT_TIMEOUT:
            break;
    }

    return true;
}

#pragma endregion

#pragma region public api

bool BodySensor::SetGestureDatabase(std::string filename)
{
    FScopeLock Lock(&apiMutex);
    return false;
}

bool BodySensor::IsBodyTracked(EBodyNumber skeletonId)
{
    FScopeLock Lock(&apiMutex);
    return skeletons[skeletonId].Tracked;
}

FVector2D BodySensor::GetLean(EBodyNumber skeletonId)
{
    FScopeLock Lock(&apiMutex);
    return skeletons[skeletonId].Lean;
}

FJoint BodySensor::GetJoint(EBodyNumber skeletonId, EJoint joint)
{
    FScopeLock Lock(&apiMutex);
    return skeletons[skeletonId].Joints[joint];
}

FJoint * BodySensor::GetJointPtr(EBodyNumber skeletonId, EJoint joint)
{
    FScopeLock Lock(&apiMutex);
    return &(skeletons[skeletonId].Joints[joint]);
}

bool BodySensor::GetContinuousGestureResult(EBodyNumber skeletonId, std::string gestureName, float* outResult)
{
    // TODO: GetContinuousGestureResult
    FScopeLock Lock(&apiMutex);
    return false;
}

bool BodySensor::GetDiscreteGestureResult(EBodyNumber skeletonId, std::string gestureName, bool* outResult)
{
    // TODO: GetDiscreteGestureResult
    FScopeLock Lock(&apiMutex);
    return false;
}

#pragma endregion

#pragma region private api

void BodySensor::ProcessBodyFrame()
{
    FScopeLock Lock(&apiMutex);

    IBodyFrameArrivedEventArgs *args = nullptr;
    IBodyFrameReference *ref = nullptr;
    IBodyFrame *frame = nullptr;
    IBody *bodies[BodyNumber_Count] = { 0 };
    BOOLEAN tracked = FALSE;
    HRESULT result = S_OK;

    // extract all the data
    result = bodyReader->GetFrameArrivedEventData(bodyWaitHandle, &args);

    if (result == S_OK)
        result = args->get_FrameReference(&ref);

    if (result == S_OK)
        result = ref->AcquireFrame(&frame);

    if (result == S_OK)
        result = frame->GetAndRefreshBodyData(BodyNumber_Count, bodies);

    if (result == S_OK)
    {
        // update all the things
        for (unsigned int idx = Body_0; idx < BodyNumber_Count; ++idx)
        {
            // update tracking status
            skeletons[idx].BodyID = (EBodyNumber)idx;
            bodies[idx]->get_IsTracked(&tracked);
            skeletons[idx].Tracked = tracked;

            // if applicable, regenerate the gesture sources (if the body was lost, for example)
            UINT64 trackingId;
            bodies[idx]->get_TrackingId(&trackingId);
            /*
            if (skeletons[idx].TrackingID != trackingId || skeletons[idx].Gestures.Source == nullptr)
                RegenerateGestureSource((EBodyNumber)idx, trackingId);
            */
            skeletons[idx].TrackingID = trackingId;
            
            IBody* body = bodies[idx];
            EBodyNumber num = (EBodyNumber)idx;
            ConvertRepresentationKinectToJester(&body, num);
        }
    }

    for (IBody* body : bodies) SAFE_RELEASE(body);
    SAFE_RELEASE(frame);
    SAFE_RELEASE(ref);
    SAFE_RELEASE(args);
}

void BodySensor::ProcessVgbSkeletonLost(EBodyNumber skeletonId)
{
    FScopeLock Lock(&apiMutex);
    Skeleton* s = &skeletons[skeletonId];

    if (s->Gestures.Reader != nullptr) s->Gestures.Reader->UnsubscribeFrameArrived(s->Gestures.GestureWaitHandle);
    if (s->Gestures.Source != nullptr) s->Gestures.Source->UnsubscribeTrackingIdLost(s->Gestures.TrackingIdWaitHandle);
    SAFE_RELEASE(s->Gestures.Reader);
    SAFE_RELEASE(s->Gestures.Source);
}

void BodySensor::ProcessVgbFrame(EBodyNumber skeletonId)
{
    HRESULT result = S_OK;
    Skeleton* s = &skeletons[skeletonId];
    IVisualGestureBuilderFrameArrivedEventArgs* args = nullptr;
    IVisualGestureBuilderFrameReference* ref = nullptr;
    IVisualGestureBuilderFrame* frame = nullptr;
    IGesture* g = nullptr;
    GestureType t;
    IDiscreteGestureResult* d = nullptr;
    IContinuousGestureResult* c = nullptr;

    result = s->Gestures.Reader->GetFrameArrivedEventData(s->Gestures.GestureWaitHandle, &args);
    if (result == S_OK)
        result = args->get_FrameReference(&ref);

    if (result == S_OK)
        result = ref->AcquireFrame(&frame);

    if (result == S_OK)
    {
        s->Gestures.ContinuousGestureResults.clear();
        s->Gestures.DiscreteGestureResults.clear();

        for (int i = 0; i < numGesturesInLibrary; ++i)
        {
            g = gestureLibrary[i];
            g->get_GestureType(&t);
            switch (t)
            {
            case GestureType_Continuous:
                result = frame->get_ContinuousGestureResult(g, &c);
                if (result == S_OK)
                {
                    s->Gestures.ContinuousGestureResults.push_back(std::shared_ptr<IContinuousGestureResult>(c));
                }
                break;

            case GestureType_Discrete:
                result = frame->get_DiscreteGestureResult(g, &d);
                if (result == S_OK)
                {
                    s->Gestures.DiscreteGestureResults.push_back(std::shared_ptr<IDiscreteGestureResult>(d));
                }
                break;
            }
        }
    }
}

void BodySensor::ConvertRepresentationKinectToJester(IBody** body, EBodyNumber skeletonId)
{
    Joint joints[Joint_Count], j;
    JointOrientation orientations[Joint_Count], o;

    if ((*body)->GetJoints(Joint_Count, joints) == S_OK &&
        (*body)->GetJointOrientations(Joint_Count, orientations) == S_OK)
    {
        for (int i = 0; i < Joint_Count; i++)
        {
            j = joints[i];
            o = orientations[i];

            skeletons[skeletonId].Joints[i].Type = TEnumAsByte<EJoint>(j.JointType);
            skeletons[skeletonId].Joints[i].TrackingState = TEnumAsByte<ETrackedState>(j.TrackingState);
            skeletons[skeletonId].Joints[i].Position = FVector(
                j.Position.X,
                j.Position.Y,
                j.Position.Z
            );
            skeletons[skeletonId].Joints[i].Orientation = FVector4(
                o.Orientation.x,
                o.Orientation.y,
                o.Orientation.z,
                o.Orientation.w
            );
        }
    }
}

void BodySensor::RegenerateGestureSource(EBodyNumber skeletonId, UINT64 newTrackingId)
{
    HRESULT result;
    Skeleton* s = &skeletons[skeletonId];
 
    // safe release all the sources, just in case
    if (s->Gestures.Reader != nullptr) s->Gestures.Reader->UnsubscribeFrameArrived(s->Gestures.GestureWaitHandle);
    if (s->Gestures.Source != nullptr) s->Gestures.Source->UnsubscribeTrackingIdLost(s->Gestures.TrackingIdWaitHandle);
    SAFE_RELEASE(s->Gestures.Reader);
    SAFE_RELEASE(s->Gestures.Source);

    // rebuild!
    result = CreateVisualGestureBuilderFrameSource(sensor, newTrackingId, &s->Gestures.Source);
    if (result == S_OK)
        result = s->Gestures.Source->SubscribeTrackingIdLost(&s->Gestures.TrackingIdWaitHandle);

    if (result == S_OK)
        result = s->Gestures.Source->OpenReader(&s->Gestures.Reader);

    if (result == S_OK)
        result = s->Gestures.Reader->SubscribeFrameArrived(&s->Gestures.GestureWaitHandle);

    // TODO: Additional checks on gesture read readiness
}

#pragma endregion
