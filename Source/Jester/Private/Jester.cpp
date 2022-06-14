// Project Jester
// Types used by Jester internally
// Consists of high level abstractions, such as a skeleton
// Copyright 2018 Leif Walker-Grant, Inc. All Rights Reserved.

#include "Jester.h"

#define LOCTEXT_NAMESPACE "FJesterModule"

// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
void FJesterModule::StartupModule()
{
    // register the tick function, so that we may ensure the sensors are always up to date
    tickDelegate = FTickerDelegate::CreateRaw(this, &FJesterModule::Tick);
    tickDelegateHandle = FTicker::GetCoreTicker().AddTicker(tickDelegate);
}

// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
// we call this function before unloading the module.
void FJesterModule::ShutdownModule()
{
    FTicker::GetCoreTicker().RemoveTicker(tickDelegateHandle);
}

// gets the currently running instance of jester
FJesterModule& FJesterModule::GetCurrentInstance()
{
    return FModuleManager::GetModuleChecked<FJesterModule>("Jester");
}

float FJesterModule::MultiplyByTenTest(float input)
{
    return input * 10;
}

// gets the current body sensor
BodySensor& FJesterModule::GetBodySensor()
{
    return bodySensor;
}

// Called each frame, updates the state of any registered sensors
bool FJesterModule::Tick(float deltaTime)
{
    bool result = true;

    if (!bodySensor.Tick(deltaTime))
    {
        /* do something */
        result = false;
    }

    return result;
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FJesterModule, Jester)
