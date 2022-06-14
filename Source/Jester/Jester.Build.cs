// Project Jester
// Jester build rules
// Copyright 1998-2018 Leif Walker-Grant, Inc. All Rights Reserved.

using System;
using System.IO;
using UnrealBuildTool;

public class Jester : ModuleRules
{
    private string KinectSdkPath
    {
        get
        {
            return Path.Combine(ModuleDirectory, "KinectSDK");
        }
    }

    private bool LinkKinect(ReadOnlyTargetRules target)
    {
        bool supported = true;

        if (target.Platform != UnrealTargetPlatform.Win64)
        {
            Console.WriteLine("Jester must be built in 64-bit mode.");
            supported = false;
        }

        if (supported)
        {
            PublicIncludePaths.Add(Path.Combine(KinectSdkPath, "inc"));
            PublicLibraryPaths.Add(Path.Combine(KinectSdkPath, "lib"));
            PublicAdditionalLibraries.Add("Kinect20.Face.Lib");
            PublicAdditionalLibraries.Add("Kinect20.Fusion.Lib");
            PublicAdditionalLibraries.Add("Kinect20.Lib");
            PublicAdditionalLibraries.Add("Kinect20.VisualGestureBuilder.Lib");
        }

        return supported;
    }

	public Jester(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        if (!Directory.Exists(KinectSdkPath))
        {
            Console.WriteLine(String.Format("Kinect SDK not found at {0}, aborting", KinectSdkPath));
            return;
        }

        LinkKinect(Target);

        PublicIncludePaths.AddRange(
			new string[] {
				"Jester/Public"
				
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				"Jester/Private",
				
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",

				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
