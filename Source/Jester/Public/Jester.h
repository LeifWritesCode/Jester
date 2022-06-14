// Project Jester
// Jester module entry point
// Copyright 2018 Leif Walker-Grant, Inc. All Rights Reserved.
#pragma once

#include "Containers/Ticker.h"
#include "ModuleManager.h"
#include "BodySensor.h"

class FJesterModule : public IModuleInterface
{
public:

    /** IModuleInterface implementation */
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

    static FJesterModule& GetCurrentInstance();

    float MultiplyByTenTest(float input);
    BodySensor& GetBodySensor();
private:
    // updates the sensors every frame
    virtual bool Tick(float deltaTime);

    // tick registration bits
    FTickerDelegate tickDelegate;
    FDelegateHandle tickDelegateHandle;

    // the skeleton and gesture sensor
    BodySensor bodySensor;
};
