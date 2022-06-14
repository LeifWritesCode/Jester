// Project Jester
// Jester module entry point
// Copyright 2018 Leif Walker-Grant, Inc. All Rights Reserved.
#pragma once

#include "Containers/Ticker.h"
#include "ModuleManager.h"
#include "Sensors/BodySensor.h"

class FJesterModule : public IModuleInterface
{
public:

    /** IModuleInterface implementation */
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

    static FJesterModule& GetCurrentInstance();

    // the skeleton and gesture sensor
    BodySensor BodySensor;
private:
    // updates the sensors every frame
    virtual bool Tick(float deltaTime);

    // tick registration bits
    FTickerDelegate tickDelegate;
    FDelegateHandle tickDelegateHandle;

};
