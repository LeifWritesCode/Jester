// Project Jester
// Jester module entry point
// Copyright 2018 Leif Walker-Grant, Inc. All Rights Reserved.

#include "Jester.h"
#include "JesterPlayerController.h"
#include "Runtime/CoreUObject/Public/UObject/UObjectIterator.h"

#define LOCTEXT_NAMESPACE "FJesterModule"

// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
void FJesterModule::StartupModule()
{
    SetKinectOnePersonSystemEngagement();

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

// Called each frame, updates the state of any registered sensors
bool FJesterModule::Tick(float deltaTime)
{
    bool result = true;

    if (!BodySensor.Tick(deltaTime))
    {
        /* do something */
        result = false;
    }

    // if the internal state of Jester has changed, inform all Jester-based controllers
    if (result)
    {
        for (TObjectIterator<AJesterPlayerController> iterator; iterator; ++iterator)
        {
            iterator->OnKinectFrame();
        }
    }

    return true;
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FJesterModule, Jester)
