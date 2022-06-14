// Project Jester
// Jester Blueprint Interface Component
// Implements the Jester public API
// Copyright 2018 Leif Walker-Grant, Inc. All Rights Reserved.

#include "JesterBPLibrary.h"
#include "Jester.h"
#include "Runtime/Core/Public/Modules/ModuleManager.h"

#pragma region ctor/dtor

UJesterBPLibrary::UJesterBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

#pragma endregion

#pragma region joint

void UJesterBPLibrary::GetDistanceBetweenJoints(const TEnumAsByte<EJoint> Start, const TEnumAsByte<EJoint> End, const TEnumAsByte<EBodyNumber> Body, float& Distance)
{
    FVector a, b;
    ConvertCoordCameraToUnreal(FJesterModule::GetCurrentInstance().BodySensor.GetJoint(Body, Start).Position, a);
    ConvertCoordCameraToUnreal(FJesterModule::GetCurrentInstance().BodySensor.GetJoint(Body, End).Position, b);
    Distance = FVector::Distance(a, b);
}

void UJesterBPLibrary::GetDirectionBetweenJoints(const TEnumAsByte<EJoint> Start, const TEnumAsByte<EJoint> End, const TEnumAsByte<EBodyNumber> Body, FVector& Vector)
{
    FVector a, b;
    ConvertCoordCameraToUnreal(FJesterModule::GetCurrentInstance().BodySensor.GetJoint(Body, Start).Position, a);
    ConvertCoordCameraToUnreal(FJesterModule::GetCurrentInstance().BodySensor.GetJoint(Body, End).Position, b);
    Vector = b - a;
}

void UJesterBPLibrary::GetJointAbsolutePosition(const TEnumAsByte<EJoint> Joint, const TEnumAsByte<EBodyNumber> Body, FVector& Position)
{
    ConvertCoordCameraToUnreal(FJesterModule::GetCurrentInstance().BodySensor.GetJoint(Body, Joint).Position, Position);
}

void UJesterBPLibrary::GetJointTrackedState(const TEnumAsByte<EJoint> Joint, const TEnumAsByte<EBodyNumber> Body, TEnumAsByte<ETrackedState>& State)
{
    State = FJesterModule::GetCurrentInstance().BodySensor.GetJoint(Body, Joint).TrackingState;
}

void UJesterBPLibrary::GetJointTrackedStateAsExecution(const TEnumAsByte<EJoint> Joint, const TEnumAsByte<EBodyNumber> Body, TEnumAsByte<ETrackedState>& State)
{
    GetJointTrackedState(Joint, Body, State);
}

void UJesterBPLibrary::GetJointDeltaPosition(const TEnumAsByte<EJoint> Joint, const TEnumAsByte<EBodyNumber> Body, FVector& Position)
{
    // TODO: GetJointDeltaPosition - need to track last two frames for this one
}

void UJesterBPLibrary::GetJointOrientation(const TEnumAsByte<EJoint> Joint, const TEnumAsByte<EBodyNumber> Body, FRotator& Orientation)
{
    // TODO: GetJointOrientation - need conversion data
    // INFI: GetJointOrientation - the joints are oriented relative to the last bone, e.g. shoulders are oriented relative to upper-back
    FVector4 o = FJesterModule::GetCurrentInstance().BodySensor.GetJoint(Body, Joint).Orientation;
}

void UJesterBPLibrary::GetJointRelativePosition(const TEnumAsByte<EJoint> Start, const TEnumAsByte<EBodyNumber> StartBody, const TEnumAsByte<EJoint> End, const TEnumAsByte<EBodyNumber> EndBody, FVector& Position)
{
    // TODO: GetJointRelativePosition - in kinect space, vector difference between start and end
}

void UJesterBPLibrary::GetPitchAdjustedJointPosition(const TEnumAsByte<EJoint> Joint, const TEnumAsByte<EBodyNumber> Body, FVector& Position)
{
    // TODO: GetPitchAdjustedJointPosition - requires pitch
}

#pragma endregion

#pragma region hand

void UJesterBPLibrary::GetHandState(const TEnumAsByte<EHand> Hand, const TEnumAsByte<EBodyNumber> Body, TEnumAsByte<EHandTrackedState>& State)
{
    //TODO: GetHandState - BodySensor needs to exponse hand state
    State = FJesterModule::GetCurrentInstance().BodySensor.GetHand(Body, Hand).State;
}

void UJesterBPLibrary::GetHandStateAsExecution(const TEnumAsByte<EHand> Hand, const TEnumAsByte<EBodyNumber> Body, TEnumAsByte<EHandTrackedState>& State)
{
    GetHandState(Hand, Body, State);
}

#pragma endregion

#pragma region body tracking

void UJesterBPLibrary::IsBodyTracked(const TEnumAsByte<EBodyNumber> Body, bool& IsTracked)
{
    IsTracked = FJesterModule::GetCurrentInstance().BodySensor.IsBodyTracked(Body);
}

void UJesterBPLibrary::GetNearestBody(TEnumAsByte<EBodyNumber>& Body)
{
    // TODO: GetNearestBody - needs an is tracked value, if no bodies return Body_0 and istracked=false
    // INFO: GetNearestBody is relative to sensor
}

void UJesterBPLibrary::GetCenteredBody(TEnumAsByte<EBodyNumber>& Body)
{
    // TODO: GetCenteredBody - needs an is tracked value, if no bodies return Body_0 and istracked=false
    // INFO: GetCenteredBody is relative to sensor X/Y, Z doesn't factor in.
}

void UJesterBPLibrary::GetTrackedBodies(TArray<TEnumAsByte<EBodyNumber>>& Bodies)
{
    for (int i = 0; i < BodyNumber_Count; ++i)
        if (FJesterModule::GetCurrentInstance().BodySensor.IsBodyTracked((EBodyNumber)i))
            Bodies.Add((EBodyNumber)i);
}

#pragma endregion

#pragma region lean

void UJesterBPLibrary::GetLeanAmount(const TEnumAsByte<EBodyNumber> Body, FVector2D& Lean)
{
    Lean = FJesterModule::GetCurrentInstance().BodySensor.GetLean(Body);
}

#pragma endregion

#pragma region device state

// INFO: IsDeviceAwake is probably wrong, can't just be a case of is open as that will be true so long as Jester is running
void UJesterBPLibrary::IsDeviceAwake(bool& IsAwake)
{
    IKinectSensor* sensor;
    BOOLEAN awake = false;
    HRESULT result = GetDefaultKinectSensor(&sensor);
    if (result == S_OK)
    {
        sensor->get_IsOpen(&awake);
    }
    IsAwake = awake;
}

void UJesterBPLibrary::GetDevicePitch(float& Pitch)
{
    FVector4 fcp = FJesterModule::GetCurrentInstance().BodySensor.GetFloorClipPlane();
    Pitch = FMath::Atan(fcp.Z / fcp.Y) * (180.0f / 3.14159265359f);
}

void UJesterBPLibrary::GetDeviceHeight(float& Height)
{
    // height of device is just W
    Height = FJesterModule::GetCurrentInstance().BodySensor.GetFloorClipPlane().W;
}

#pragma endregion

#pragma region utility

void UJesterBPLibrary::ConvertCoordCameraToUnreal(const FVector in, FVector& out)
{
    // adjust from CSP unit (1m) to UE4 unit (1cm)
    FVector inAdjusted(in * FVector(100));

    out.X = -inAdjusted.Z;
    out.Y = inAdjusted.X;
    out.Z = inAdjusted.Y;
}

void UJesterBPLibrary::ConvertCoordUnrealToCamera(const FVector in, FVector& out)
{
    // adjust from UE4 unit (1cm) to CSP unit (1m)
    FVector inAdjusted(in / FVector(100));

    out.X = inAdjusted.Y;
    out.Y = inAdjusted.Z;
    out.Z = -inAdjusted.X;
}

#pragma endregion

#pragma region gesture

void UJesterBPLibrary::GetDiscreteGestureResult(const EBodyNumber Body, const FString Name, bool& Found, bool& Detected, float& Confidence)
{
    Found = FJesterModule::GetCurrentInstance().BodySensor.GetDiscreteGestureResult(Body, Name, Detected, Confidence);
}

void UJesterBPLibrary::GetContinuousGestureResult(const EBodyNumber Body, const FString Name, bool& Found, float& Progress)
{
    Found = FJesterModule::GetCurrentInstance().BodySensor.GetContinuousGestureResult(Body, Name, Progress);
}

// this doesn't differentiate between continuous and discrete, which is an issue since the idea is to
// seamlessly pull gesture data from a for
// TODO: GetRecognisedGesturesAsArray should support differentiating between types
// INFO: GetRecognisedGesturesAsArray multi-type support can be done using pins, two arrays - one for each type.
void UJesterBPLibrary::GetRecognisedGesturesAsArray(const EBodyNumber Body, const float ProgressThreshold, TArray<FString>& Gestures)
{
    std::vector<FGesture> g = FJesterModule::GetCurrentInstance().BodySensor.GetGestures(Body);
    for (int i = 0; i < g.size(); ++i)
        if (g[i].Detected || g[i].Progress >= ProgressThreshold)
            Gestures.Add(g[i].Name);
}

#pragma endregion
