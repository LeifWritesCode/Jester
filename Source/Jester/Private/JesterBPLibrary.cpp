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

#pragma region test functions

void UJesterBPLibrary::MultiplyByTenTest(const float Param, float& Result)
{
    Result = FJesterModule::GetCurrentInstance().MultiplyByTenTest(Param);
}

#pragma endregion

#pragma region joint

void UJesterBPLibrary::GetDistanceBetweenJoints(const TEnumAsByte<EJoint> Start, const TEnumAsByte<EJoint> End, const TEnumAsByte<EBodyNumber> Body, float& Distance)
{

}

void UJesterBPLibrary::GetJointAbsolutePosition(const TEnumAsByte<EJoint> Joint, const TEnumAsByte<EBodyNumber> Body, FVector& Position)
{

}

void UJesterBPLibrary::GetJointTrackedState(const TEnumAsByte<EJoint> Joint, const TEnumAsByte<EBodyNumber> Body, TEnumAsByte<ETrackedState>& State)
{

}

void UJesterBPLibrary::GetJointTrackedStateAsExecution(const TEnumAsByte<EJoint> Joint, const TEnumAsByte<EBodyNumber> Body, TEnumAsByte<ETrackedState>& State)
{
    GetJointTrackedState(Joint, Body, State);
}

void UJesterBPLibrary::GetJointDeltaPosition(const TEnumAsByte<EJoint> Joint, const TEnumAsByte<EBodyNumber> Body, FVector& Position)
{

}

void UJesterBPLibrary::GetJointOrientation(const TEnumAsByte<EJoint> Joint, const TEnumAsByte<EBodyNumber> Body, FRotator& Orientation)
{

}

void UJesterBPLibrary::GetJointRelativePosition(const TEnumAsByte<EJoint> Start, const TEnumAsByte<EBodyNumber> StartBody, const TEnumAsByte<EJoint> End, const TEnumAsByte<EBodyNumber> EndBody, FVector& Position)
{

}

void UJesterBPLibrary::GetPitchAdjustedJointPosition(const TEnumAsByte<EJoint> Joint, const TEnumAsByte<EBodyNumber> Body, FVector& Position)
{

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

}

void UJesterBPLibrary::GetNearestBody(TEnumAsByte<EBodyNumber>& Body)
{

}

void UJesterBPLibrary::GetCenteredBody(TEnumAsByte<EBodyNumber>& Body)
{

}

void UJesterBPLibrary::GetTrackedBodies(TArray<TEnumAsByte<EBodyNumber>>& Bodies)
{

}

#pragma endregion

#pragma region lean

void UJesterBPLibrary::GetLeanAmount(const TEnumAsByte<EBodyNumber> Body, float& Lean)
{

}

#pragma endregion

#pragma region device state

void UJesterBPLibrary::IsDeviceAwake(bool& IsAwake)
{

}

void UJesterBPLibrary::GetDevicePitch(float& Pitch)
{

}

#pragma endregion
