// Copyright Epic Games, Inc. All Rights Reserved.

#include "JesterDK.h"
#include "Core.h"
#include "Modules/ModuleManager.h"
#include "Interfaces/IPluginManager.h"

#include "Windows/AllowWindowsPlatformTypes.h"
#include "Kinect/Kinect.h"
#include "Windows/HideWindowsPlatformTypes.h"

#define LOCTEXT_NAMESPACE "FJesterDKModule"

void FJesterDKModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	// Get the base directory of this plugin
	FString BaseDir = IPluginManager::Get().FindPlugin("JesterDK")->GetBaseDir();

	// Add on the relative location of the third party dll and load it
	FString LibraryPath = FPaths::Combine(*BaseDir, TEXT("Source/ThirdParty/KinectSDK/bin/Kinect20.dll"));

	void* ExampleLibraryHandle = !LibraryPath.IsEmpty() ? FPlatformProcess::GetDllHandle(*LibraryPath) : nullptr;

	if (ExampleLibraryHandle)
	{
		// Call the test function in the third party library that opens a message box
 		SetKinectOnePersonSystemEngagement();
		FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("Kinect SDK", "Loaded Kinect SDK binaries and set single-person system-engagement mode, innit"));
	}
	else
	{
		FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("Kinect SDK", "Failed to load Kinect SDK binaries"));
	}
}

void FJesterDKModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FJesterDKModule, JesterDK)