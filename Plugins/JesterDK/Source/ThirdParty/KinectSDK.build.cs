// Fill out your copyright notice in the Description page of Project Settings.

using System;
using System.IO;
using UnrealBuildTool;

public class KinectSDK : ModuleRules
{
    private static readonly string KINECT_SDK = "KinectSDK";
    private static readonly string LIBRARIES = "lib";
    private static readonly string INCLUDES = "inc";
    private static readonly string BINARIES = "bin";

    public KinectSDK(ReadOnlyTargetRules targetRules)
        : base(targetRules)
    {
        Type = ModuleType.External;

        if (targetRules.Platform  != UnrealTargetPlatform.Win64)
        {
            throw new NotSupportedException();
        }

        // Add a macro so that we can see this module has been loaded
        PublicDefinitions.Add("WITH_KINECTSDK=1");

        // Add the headers
        PublicIncludePaths.Add(Path.Combine(ModuleDirectory, KINECT_SDK, INCLUDES));

        // Add the import libraries for code linkage
        PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, KINECT_SDK, LIBRARIES, "Kinect20.lib"));
        PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, KINECT_SDK, LIBRARIES, "Kinect20.Face.lib"));
        PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, KINECT_SDK, LIBRARIES, "Kinect20.Fusion.lib"));
        PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, KINECT_SDK, LIBRARIES, "Kinect20.VisualGestureBuilder.lib"));

        // Delay load the DLLs so that jester can load them properly at boot
        PublicDelayLoadDLLs.Add("Kinect20.dll");
        PublicDelayLoadDLLs.Add("Kinect20.Face.dll");
        PublicDelayLoadDLLs.Add("Kinect20.Fusion.dll");
        PublicDelayLoadDLLs.Add("Kinect20.VisualGestureBuilder.dll");
        PublicDelayLoadDLLs.Add("AdaBoostTech.dll");
        PublicDelayLoadDLLs.Add("RFRProgressTech.dll");

        // Ensure that the DLLs are staged along with the executable
        RuntimeDependencies.Add(Path.Combine(ModuleDirectory, KINECT_SDK, BINARIES, "Kinect20.dll"));
        RuntimeDependencies.Add(Path.Combine(ModuleDirectory, KINECT_SDK, BINARIES, "Kinect20.Face.dll"));
        RuntimeDependencies.Add(Path.Combine(ModuleDirectory, KINECT_SDK, BINARIES, "Kinect20.Fusion.dll"));
        RuntimeDependencies.Add(Path.Combine(ModuleDirectory, KINECT_SDK, BINARIES, "Kinect20.VisualGestureBuilder.dll"));
        RuntimeDependencies.Add(Path.Combine(ModuleDirectory, KINECT_SDK, BINARIES, "AdaBoostTech.dll"));
        RuntimeDependencies.Add(Path.Combine(ModuleDirectory, KINECT_SDK, BINARIES, "RFRProgressTech.dll"));
    }
}
