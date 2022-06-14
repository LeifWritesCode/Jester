// Project Jester
// Jester Blueprint Interface Component
// Implements the Jester public API
// Copyright 2018 Leif Walker-Grant, Inc. All Rights Reserved.
#pragma once

#include "JesterTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "JesterBPLibrary.generated.h"

/* 
*	Function library class.
*	Each function in it is expected to be static and represents blueprint node that can be called in any blueprint.
*
*	When declaring function you can define metadata for the node. Key function specifiers will be BlueprintPure and BlueprintCallable.
*	BlueprintPure - means the function does not affect the owning object in any way and thus creates a node without Exec pins.
*	BlueprintCallable - makes a function which can be executed in Blueprints - Thus it has Exec pins.
*	DisplayName - full name of the node, shown when you mouse over the node and in the blueprint drop down menu.
*				Its lets you name the node using characters not allowed in C++ function names.
*	CompactNodeTitle - the word(s) that appear on the node.
*	Keywords -	the list of keywords that helps you to find node when you search for it using Blueprint drop-down menu. 
*				Good example is "Print String" node which you can find also by using keyword "log".
*	Category -	the category your node will be under in the Blueprint drop-down menu.
*
*	For more info on custom blueprint nodes visit documentation:
*	https://wiki.unrealengine.com/Custom_Blueprint_Node_Creation
*/
UCLASS(DisplayName = "Jester Interface Component")
class UJesterBPLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_UCLASS_BODY()

    /**
    * joint api
    */

    UFUNCTION(BlueprintPure, Category = "Jester|Joint", DisplayName = "Get Distance Between Joints")
    static void GetDistanceBetweenJoints(const TEnumAsByte<EJoint> Start, const TEnumAsByte<EJoint> End, const TEnumAsByte<EBodyNumber> Body, float& Distance);

    UFUNCTION(BlueprintPure, Category = "Jester|Joint", DisplayName = "Get Joint Absolute Position")
    static void GetJointAbsolutePosition(const TEnumAsByte<EJoint> Joint, const TEnumAsByte<EBodyNumber> Body, FVector& Position);

    UFUNCTION(BlueprintPure, Category = "Jester|Joint", DisplayName = "Get Joint Tracked State")
    static void GetJointTrackedState(const TEnumAsByte<EJoint> Joint, const TEnumAsByte<EBodyNumber> Body, TEnumAsByte<ETrackedState>& State);

    UFUNCTION(BlueprintCallable, meta = (Category = "Jester|Joint", DisplayName = "Get Joint Tracked State as Execution", ExpandEnumAsExecs = "State"))
    static void GetJointTrackedStateAsExecution(const TEnumAsByte<EJoint> Joint, const TEnumAsByte<EBodyNumber> Body, TEnumAsByte<ETrackedState>& State);

    UFUNCTION(BlueprintPure, Category = "Jester|Joint", DisplayName = "Get Joint Delta Position")
    static void GetJointDeltaPosition(const TEnumAsByte<EJoint> Joint, const TEnumAsByte<EBodyNumber> Body, FVector& Position);

    UFUNCTION(BlueprintPure, Category = "Jester|Joint", DisplayName = "Get Joint Orientation")
    static void GetJointOrientation(const TEnumAsByte<EJoint> Joint, const TEnumAsByte<EBodyNumber> Body, FRotator& Orientation);

    UFUNCTION(BlueprintPure, Category = "Jester|Joint", DisplayName = "Get Joint Relative Position")
    static void GetJointRelativePosition(const TEnumAsByte<EJoint> Start, const TEnumAsByte<EBodyNumber> StartBody, const TEnumAsByte<EJoint> End, const TEnumAsByte<EBodyNumber> EndBody, FVector& Position);

    UFUNCTION(BlueprintPure, Category = "Jester|Joint", DisplayName = "GetPitchAdjustedJointPosition")
    static void GetPitchAdjustedJointPosition(const TEnumAsByte<EJoint> Joint, const TEnumAsByte<EBodyNumber> Body, FVector& Position);

    /**
    * hand api
    */

    UFUNCTION(BlueprintPure, Category = "Jester|Hand", DisplayName = "Get Hand State")
    static void GetHandState(const TEnumAsByte<EHand> Hand, const TEnumAsByte<EBodyNumber> Body, TEnumAsByte<EHandTrackedState>& State);

    UFUNCTION(BlueprintCallable, meta = (Category = "Jester|Hand", DisplayName = "Get Hand State as Execution", ExpandEnumAsExecs = "State"))
    static void GetHandStateAsExecution(const TEnumAsByte<EHand> Hand, const TEnumAsByte<EBodyNumber> Body, TEnumAsByte<EHandTrackedState>& State);

    /**
     * body tracking
     */

    UFUNCTION(BlueprintPure, Category = "Jester|Body", DisplayName = "Is Body Tracked")
    static void IsBodyTracked(const TEnumAsByte<EBodyNumber> Body, bool& IsTracked);

    UFUNCTION(BlueprintPure, Category = "Jester|Body", DisplayName = "Get Nearest Body")
    static void GetNearestBody(TEnumAsByte<EBodyNumber>& Body);

    UFUNCTION(BlueprintPure, Category = "Jester|Body", DisplayName = "Get Centered Body")
    static void GetCenteredBody(TEnumAsByte<EBodyNumber>& Body);
 
    UFUNCTION(BlueprintPure, Category = "Jester|Body", DisplayName = "Get Tracked Bodies")
    static void GetTrackedBodies(TArray<TEnumAsByte<EBodyNumber>>& Bodies);

    /**
    * lean
    */

    UFUNCTION(BlueprintPure, Category = "Jester|Lean", DisplayName = "Get Lean Amount")
    static void GetLeanAmount(const TEnumAsByte<EBodyNumber> Body, FVector2D& Lean);

    /**
    * device state
    */

    UFUNCTION(BlueprintPure, Category = "Jester|Device", DisplayName = "Is Device Awake")
    static void IsDeviceAwake(bool& IsAwake);
 
    UFUNCTION(BlueprintPure, Category = "Jester|Device", DisplayName = "Get Device Pitch")
    static void GetDevicePitch(float& Pitch);

    /**
     * utility
     */

    UFUNCTION(BlueprintPure, Category = "Jester|Utility", DisplayName = "Convert CSP to Unreal Coord")
    static void ConvertCoordCameraToUnreal(const FVector& in, FVector& out);

    UFUNCTION(BlueprintPure, Category = "Jester|Utility", DisplayName = "Convert Unreal Coord to CSP")
    static void ConvertCoordUnrealToCamera(const FVector& in, FVector& out);
};
