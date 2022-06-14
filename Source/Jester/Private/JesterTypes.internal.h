// Project Jester
// Types used by Jester internally
// Consists of high level abstractions, such as a skeleton
// Copyright 2018 Leif Walker-Grant, Inc. All Rights Reserved.
#pragma once

#include "Kinect.VisualGestureBuilder.h"
#include "JesterTypes.h"

// represents a pairing between body and hand - necessary to replicate in UE4
struct HandPair
{
    // the hand represented by this pair
    FHand Hand;

    // the pairing interface provided by kinect
    IBodyHandPair* Pairing;
};

// represents a source of gesture data, per skeleton
struct GestureSource
{
    // the handle on which to wait for any gestures
    WAITABLE_HANDLE WaitHandle;

    // the source from which to obtain gesturs
    IVisualGestureBuilderFrameSource* Source;

    // the frame reader from which to obtain gestures
    IVisualGestureBuilderFrameReader* Reader;
};

// internal representation of a player skeleton
// this is not exposed to the programmer directly
struct Skeleton
{
    // is the player currently tracked
    bool Tracked;

    // the ID of the skeleton
    EBodyNumber BodyID;

    // the (internal) ID of the skeleton for gesture recognition
    uint64 TrackingID;

    // the joints and orientations of said joints
    FJoint Joints[Joint_Count];
    FJointOrientation Orientations[Joint_Count];

    // hands associated with this skeleton
    HandPair Hands[Hand_Count];

    // back-forward and left-right lean in (x,y) pair
    FVector2D Lean;

    // the gestures tracked by this skeleton
    GestureSource Gestures;
};
