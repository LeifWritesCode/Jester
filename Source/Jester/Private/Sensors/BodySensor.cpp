// Project Jester
// Implements a sensor encapsulating skeletal tracking and body-indexed gesture recognition
// Copyright 2018 Leif Walker-Grant, Inc. All Rights Reserved.

#include "BodySensor.h"

#include <algorithm>
#include "Runtime/Core/Public/Misc/ScopeLock.h"
#include "Kinect.h"
#include "Kinect.VisualGestureBuilder.h"

// helper macro
#define WAIT_OBJECT(n) (WAIT_OBJECT_0 + n)

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

// man what am i to do with all these checks
void BodySensor::Initialise(IKinectSensor* _sensor)
{
    HRESULT result;

    // i'm not so trusting as not to do this
    check(_sensor != nullptr);
    sensor = _sensor;

    // attempt to get the frame source
    result = sensor->get_BodyFrameSource(&bodySource);
    check(result == S_OK);

    // attempt to get the frame reader
    bodySource->OpenReader(&bodyReader);
    check(result == S_OK);

    // and subscribe
    result = bodyReader->SubscribeFrameArrived(&bodyWaitHandle);
    check(result == S_OK);

    // prepare the gesture sources
    for (int i = 0; i < BodyNumber_Count; ++i)
    {
        Skeleton* s = &skeletons[i];
        result = CreateVisualGestureBuilderFrameSource(sensor, 0, &s->Gestures.Source);
        check(result == S_OK);

        result = s->Gestures.Source->OpenReader(&s->Gestures.Reader);
        check(result == S_OK);

        result = CreateVisualGestureBuilderDatabaseInstanceFromFile(L"C:\\Seated.gbd", &s->Gestures.Database);
        check(result == S_OK);

        uint32 numGestures;
        IGesture** gestures;

        result = s->Gestures.Database->get_AvailableGesturesCount(&numGestures);
        check(result == S_OK);

        gestures = new IGesture*[numGestures];
        result = s->Gestures.Database->get_AvailableGestures(numGestures, gestures);
        check(result == S_OK);

        result = s->Gestures.Source->AddGestures(numGestures, gestures);
        check(result == S_OK);

        result = s->Gestures.Source->SubscribeTrackingIdLost(&s->Gestures.TrackingIdWaitHandle);
        check(result == S_OK);

        result = s->Gestures.Reader->SubscribeFrameArrived(&s->Gestures.GestureWaitHandle);
        check(result == S_OK);

        delete[] gestures;
    }

    // attempt to open the sensor
    result = sensor->Open();
    check(result == S_OK);
}

#pragma endregion

#pragma region tick

// wait on all handles and then call the necessary function
bool BodySensor::Tick(float deltaTime)
{
    // TODO: if multi-single wait experiment works, need to incorporate return result
    bool result = true;

    DWORD signal;
    HANDLE h;

    // process body data, if present
    h = reinterpret_cast<HANDLE>(bodyWaitHandle);
    signal = WaitForSingleObject(h, 1);
    if (signal == WAIT_OBJECT_0) ProcessBodyFrame();

    // process each skellyboye, if present
    for (int i = 0; i < BodyNumber_Count; ++i)
    {
        h = reinterpret_cast<HANDLE>(skeletons[i].Gestures.GestureWaitHandle);
        signal = WaitForSingleObject(h, 1);
        if (signal == WAIT_OBJECT_0) ProcessVgbFrame((EBodyNumber)i);
    }

    return result;
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
    if (skeletonId == BodyNumber_Count) return false;
    FScopeLock Lock(&apiMutex);
    return skeletons[skeletonId].Tracked;
}

FVector2D BodySensor::GetLean(EBodyNumber skeletonId)
{
    if (skeletonId == BodyNumber_Count) return FVector2D();
    FScopeLock Lock(&apiMutex);
    return skeletons[skeletonId].Lean;
}

std::vector<FGesture> BodySensor::GetGestures(EBodyNumber skeletonId)
{
    if (skeletonId == BodyNumber_Count) return std::vector<FGesture>(0);
    FScopeLock Lock(&apiMutex);
    return skeletons[skeletonId].Gestures.Detected;
}

FHand BodySensor::GetHand(EBodyNumber skeletonId, EHand hand)
{
    if (skeletonId == BodyNumber_Count || hand == Hand_None) return FHand();

    FScopeLock Lock(&apiMutex);
    return skeletons[skeletonId].Hands[hand].Hand;
}

FJoint BodySensor::GetJoint(EBodyNumber skeletonId, EJoint joint)
{
    if (skeletonId == BodyNumber_Count || joint == Joint_Count) return FJoint();
    FScopeLock Lock(&apiMutex);
    return skeletons[skeletonId].Joints[joint];
}

FJoint * BodySensor::GetJointPtr(EBodyNumber skeletonId, EJoint joint)
{
    if (skeletonId == BodyNumber_Count || joint == Joint_Count) return nullptr;
    FScopeLock Lock(&apiMutex);
    return &(skeletons[skeletonId].Joints[joint]);
}

bool BodySensor::GetContinuousGestureResult(EBodyNumber skeletonId, FString gestureName, float& outResult)
{
    if (skeletonId == BodyNumber_Count) return false;
    FScopeLock Lock(&apiMutex);
    bool result = false;

    Skeleton& s = skeletons[skeletonId];
    std::vector<FGesture>::iterator it = std::find_if(s.Gestures.Detected.begin(), s.Gestures.Detected.end(), [gestureName](const FGesture& arg) { return arg.Name == gestureName; });
    if (it != s.Gestures.Detected.end() && it->Type == Gesture_Continuous)
    {
        outResult = it->Progress;
        result = true;
    }

    return result;
}

bool BodySensor::GetDiscreteGestureResult(EBodyNumber skeletonId, FString gestureName, bool& outResult, float& outConfidence)
{
    if (skeletonId == BodyNumber_Count) return false;
    FScopeLock Lock(&apiMutex);
    
    FGesture f;
    Skeleton& s = skeletons[skeletonId];
    bool result = false;

    /* should really be using the vector iterator with std::find_if,
    however for a size 1 vector begin = 1 = end so that won't work for single gesture files. */
    for (int i = 0; i < s.Gestures.Detected.size(); ++i)
    {
        f = s.Gestures.Detected[i];
        if (f.Name == gestureName)
        {
            outResult = f.Detected;
            outConfidence = f.Confidence;
            result = true;
            break;
        }
    }

    return result;
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
            if (skeletons[idx].TrackingID != trackingId)
            {
                skeletons[idx].TrackingID = trackingId;
                UpdateGestureSource((EBodyNumber)idx, trackingId);
            }
            ConvertRepresentationKinectToJester(&bodies[idx], (EBodyNumber)idx);
        }
    }

    for (IBody* body : bodies) SAFE_RELEASE(body);
    SAFE_RELEASE(frame);
    SAFE_RELEASE(ref);
    SAFE_RELEASE(args);
}

void BodySensor::ProcessVgbSkeletonLost(EBodyNumber skeletonId)
{
    if (skeletonId == BodyNumber_Count) return;
    FScopeLock Lock(&apiMutex);
}

void BodySensor::ProcessVgbFrame(EBodyNumber skeletonId)
{
    if (skeletonId == BodyNumber_Count) return;
    FScopeLock Lock(&apiMutex);

    HRESULT result = S_OK;
    Skeleton* s = &skeletons[skeletonId];
    IVisualGestureBuilderFrameArrivedEventArgs* args = nullptr;
    IVisualGestureBuilderFrameReference* ref = nullptr;
    IVisualGestureBuilderFrame* frame = nullptr;
    GestureType type;
    BOOLEAN detected;
    float confidence;
    float progress;
    wchar_t gestureName[260];

    // clear out the existing list
    s->Gestures.Detected.clear();

    // pull the new frame
    result = s->Gestures.Reader->GetFrameArrivedEventData(s->Gestures.GestureWaitHandle, &args);
    if (result == S_OK)
        result = args->get_FrameReference(&ref);

    if (result == S_OK)
        result = ref->AcquireFrame(&frame);

    if (result == S_OK)
    {
        // get the stored gestures
        UINT numGestures = 0;
        IGesture** gestures;
        s->Gestures.Source->get_GestureCount(&numGestures);

        gestures = new IGesture*[numGestures];
        s->Gestures.Source->get_Gestures(numGestures, gestures);

        // iterate over list and start to extract information
        for (UINT id = 0; id < numGestures; ++id)
        {
            result = gestures[id]->get_GestureType(&type);

            // TODO: Understand why get_Name is throwing E_INVALIDARG
            // apparently, 260 is a genuinely magic number
            result = gestures[id]->get_Name(260, &gestureName[0]);
            check(result == S_OK);

            // for each gesture, check to see if it has been detected
            if (type == GestureType_Discrete)
            {
                IDiscreteGestureResult* discrete;
                result = frame->get_DiscreteGestureResult(gestures[id], &discrete);
                if (result == S_OK && discrete)
                {
                    result = discrete->get_Detected(&detected);
                    result = discrete->get_Confidence(&confidence);
                }
            }
            else if (type == GestureType_Continuous)
            {
                IContinuousGestureResult* continuous;
                result = frame->get_ContinuousGestureResult(gestures[id], &continuous);
                if (result == S_OK && continuous)
                {
                    continuous->get_Progress(&progress);
                }
            }

            FGesture g;
            g.Type = TEnumAsByte<EGestureType>(type);
            g.Detected = detected;
            g.Name = FString(gestureName);
            g.Confidence = type == GestureType_Discrete ? confidence : 0;
            g.Progress = type == GestureType_Continuous ? progress : 0;
            s->Gestures.Detected.push_back(g);
        }

        delete[] gestures;
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

void BodySensor::UpdateGestureSource(EBodyNumber skeletonId, UINT64 newTrackingId)
{
    Skeleton* s = &skeletons[skeletonId];
    if (s->Gestures.Source)
        s->Gestures.Source->put_TrackingId(newTrackingId);
}

#pragma endregion
