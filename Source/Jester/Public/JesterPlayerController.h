// Project Jester
// A player controller exposing Jester-specific events
// Copyright 2018 Leif Walker-Grant, Inc. All Rights Reserved.
#pragma once

#include "Runtime/Engine/Classes/GameFramework/PlayerController.h"
#include "JesterPlayerController.generated.h"

// a (very) basic player controller extension enabling Jester to communicate a change (rather than rely on polling)
UCLASS(Blueprintable, BlueprintType)
class AJesterPlayerController : public APlayerController
{
    GENERATED_UCLASS_BODY()

public:
    // executed whenever a kinect frame is processed that changed the internal state
    UFUNCTION(BlueprintImplementableEvent, Category = "Jester|Events", DisplayName = "Kinect Frame Received")
    void OnKinectFrame();
};
