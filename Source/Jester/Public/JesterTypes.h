// Project Jester
// Types used by Jester that are also exposed to the Blueprint API
// Consists of core tracking types, such as joints and hands
// Copyright 2018 Leif Walker-Grant, Inc. All Rights Reserved.
#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Runtime/Core/Public/Math/Vector.h"
#include "Runtime/Core/Public/Math/Vector4.h"
#include "Runtime/Core/Public/Math/Vector2D.h"
#include "JesterTypes.generated.h"

UENUM(BlueprintType)
enum EHand
{
    Hand_None       = 0 UMETA(DisplayName = "None"),
    Hand_LeftHand   = 1 UMETA(DisplayName = "Left Hand"),
    Hand_RightHand  = 2 UMETA(DisplayName = "Right Hand")
};
const unsigned int Hand_Count = (Hand_RightHand + 1);

UENUM(BlueprintType)
enum EHandTrackedState
{
    HandTrackedState_None       = 0 UMETA(DisplayName = "None"),
    HandTrackedState_NotTracked = 1 UMETA(DisplayName = "Not Tracked"),
    HandTrackedState_Open       = 2 UMETA(DisplayName = "Open"),
    HandTrackedState_Closed     = 3 UMETA(DisplayName = "Closed"),
    HandTrackedState_Lasso      = 4 UMETA(DisplayName = "Lasso")
};
const unsigned int HandTrackedState_Count = (HandTrackedState_Lasso + 1);

// an enum describing the trackable bodies
UENUM(BlueprintType)
enum EBodyNumber
{
    Body_0 = 0 UMETA(DisplayName = "Body 0"),
    Body_1 = 1 UMETA(DisplayName = "Body 1"),
    Body_2 = 2 UMETA(DisplayName = "Body 2"),
    Body_3 = 3 UMETA(DisplayName = "Body 3"),
    Body_4 = 4 UMETA(DisplayName = "Body 4"),
    Body_5 = 5 UMETA(DisplayName = "Body 5")
};
const unsigned int BodyNumber_Count = (Body_5 + 1);

// an enum describing the tracked joints
UENUM(BlueprintType)
enum EJoint
{
    Joint_SpineBase     = 0  UMETA(DisplayName = "Base of Spine"),
    Joint_SpineMid      = 1  UMETA(DisplayName = "Mid Point of Spine"),
    Joint_Neck          = 2  UMETA(DisplayName = "Neck"),
    Joint_Head          = 3  UMETA(DisplayName = "Head"),
    Joint_ShoulderLeft  = 4  UMETA(DisplayName = "Left Shoulder"),
    Joint_ElbowLeft     = 5  UMETA(DisplayName = "Left Elbow"),
    Joint_WristLeft     = 6  UMETA(DisplayName = "Left Wrist"),
    Joint_HandLeft      = 7  UMETA(DisplayName = "Left Hand"),
    Joint_ShoulderRight = 8  UMETA(DisplayName = "Right Shoulder"),
    Joint_ElbowRight    = 9  UMETA(DisplayName = "Right Elbow"),
    Joint_WristRight    = 10 UMETA(DisplayName = "Right Wrist"),
    Joint_HandRight     = 11 UMETA(DisplayName = "Right Hand"),
    Joint_HipLeft       = 12 UMETA(DisplayName = "Left Hip"),
    Joint_KneeLeft      = 13 UMETA(DisplayName = "Left Knee"),
    Joint_AnkleLeft     = 14 UMETA(DisplayName = "Left Ankle"),
    Joint_FootLeft      = 15 UMETA(DisplayName = "Left Foot"),
    Joint_HipRight      = 16 UMETA(DisplayName = "Right Hip"),
    Joint_KneeRight     = 17 UMETA(DisplayName = "Right Knee"),
    Joint_AnkleRight    = 18 UMETA(DisplayName = "Right Ankle"),
    Joint_FootRight     = 19 UMETA(DisplayName = "Right Foot"),
    Joint_SpineShoulder = 20 UMETA(DisplayName = "Top of Spine"),
    Joint_HandTipLeft   = 21 UMETA(DisplayName = "Tip of the Left Hand"),
    Joint_ThumbLeft     = 22 UMETA(DisplayName = "Left Thumb"),
    Joint_HandTipRight  = 24 UMETA(DisplayName = "Tip of the Right Hand"),
    Joint_ThumbRight    = 25 UMETA(DisplayName = "Right Thumb"),
};
const unsigned int Joint_Count = (Joint_ThumbRight + 1);

// an enum describing possible tracking states
UENUM(BlueprintType)
enum ETrackedState
{
    TrackedState_NotTracked = 0 UMETA(DisplayName = "Not Tracked"),
    TrackedState_Inferred   = 1 UMETA(DisplayName = "Inferred"),
    TrackedState_Tracked    = 2 UMETA(DisplayName = "Tracked")
};
const unsigned int TrackedState_Count = (TrackedState_Tracked + 1);

USTRUCT(BlueprintType)
struct FHand
{
    GENERATED_BODY();

    UPROPERTY()
    TEnumAsByte<EHand> Hand;

    UPROPERTY()
    TEnumAsByte<EHandTrackedState> State;
};

// represents a skeletal joint in camera space
USTRUCT(BlueprintType)
struct FJoint
{
    GENERATED_BODY()

    UPROPERTY()
    TEnumAsByte<EJoint> Type;

    UPROPERTY()
    FVector Position;

    UPROPERTY()
    TEnumAsByte<ETrackedState> TrackingState;
};

// represents orientation of a skeletal joint in camera space
// two joints and an orientation make a bone
USTRUCT(BlueprintType)
struct FJointOrientation
{
    GENERATED_BODY()

    UPROPERTY()
    TEnumAsByte<EJoint> Type;

    UPROPERTY()
    FVector4 Orientation;
};
