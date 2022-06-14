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
    // TODO: GetDistanceBetweenJoints
}

void UJesterBPLibrary::GetJointAbsolutePosition(const TEnumAsByte<EJoint> Joint, const TEnumAsByte<EBodyNumber> Body, FVector& Position)
{
    Position = FJesterModule::GetCurrentInstance().BodySensor.GetJoint(Body, Joint).Position;
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
    // TODO: GetJointDeltaPosition
}

void UJesterBPLibrary::GetJointOrientation(const TEnumAsByte<EJoint> Joint, const TEnumAsByte<EBodyNumber> Body, FRotator& Orientation)
{
    // TODO: GetJointOrientation
}

void UJesterBPLibrary::GetJointRelativePosition(const TEnumAsByte<EJoint> Start, const TEnumAsByte<EBodyNumber> StartBody, const TEnumAsByte<EJoint> End, const TEnumAsByte<EBodyNumber> EndBody, FVector& Position)
{
    // TODO: GetJointRelativePosition
}

void UJesterBPLibrary::GetPitchAdjustedJointPosition(const TEnumAsByte<EJoint> Joint, const TEnumAsByte<EBodyNumber> Body, FVector& Position)
{
    // TODO: GetPitchAdjustedJointPosition
}

#pragma endregion

#pragma region hand

void UJesterBPLibrary::GetHandState(const TEnumAsByte<EHand> Hand, const TEnumAsByte<EBodyNumber> Body, TEnumAsByte<EHandTrackedState>& State)
{

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
    // TODO: GetNearestBody
}

void UJesterBPLibrary::GetCenteredBody(TEnumAsByte<EBodyNumber>& Body)
{
    // TODO: GetCenteredBody
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
    // TODO: GetDevicePitch
}

#pragma endregion

#pragma region utility

void UJesterBPLibrary::ConvertCoordCameraToUnreal(const FVector& in, FVector& out)
{
    // adjust from CSP unit (1m) to UE4 unit (1cm)
    FVector inAdjusted(in * FVector(100));

    out.X = -inAdjusted.Z;
    out.Y = inAdjusted.X;
    out.Z = inAdjusted.Y;
}

void UJesterBPLibrary::ConvertCoordUnrealToCamera(const FVector& in, FVector& out)
{
    // adjust from UE4 unit (1cm) to CSP unit (1m)
    FVector inAdjusted(in / FVector(100));

    out.X = inAdjusted.Y;
    out.Y = inAdjusted.Z;
    out.Z = -inAdjusted.X;
}

#pragma endregion

#pragma region gesture

void UJesterBPLibrary::GetDiscreteGestureResult(const EBodyNumber& Body, const FString& Name, bool& Found, bool& Detected, float& Confidence)
{
    Found = FJesterModule::GetCurrentInstance().BodySensor.GetDiscreteGestureResult(Body, Name, Detected, Confidence);
}

void UJesterBPLibrary::GetContinuousGestureResult(const EBodyNumber& Body, const FString& Name, bool& Found, float& Progress)
{
    Found = FJesterModule::GetCurrentInstance().BodySensor.GetContinuousGestureResult(Body, Name, Progress);
}

#pragma endregion
