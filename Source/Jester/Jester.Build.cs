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
            return Utils.ResolveEnvironmentVariable("%KINECTSDK20_DIR%");
        }
    }

    private string ModuleRoot
    {
        get
        {
            return Path.Combine(ModuleDirectory, @"..\..\");
        }
    }

    private string PlatformBitness { get; set; }
    private string PlatformTarget { get; set; }

    private void ImportAndCopy(FileInfo file)
    {
        if (!File.Exists(file.FullName))
            throw new FileNotFoundException("Failed to import requested module.", file.FullName);

        Console.WriteLine("\tAdding dependency on {0}.", file.FullName);
        RuntimeDependencies.Add(file.FullName);
        string dest = Path.Combine(ModuleRoot, "binaries", PlatformTarget, file.Name);

        if (File.Exists(dest))
        {
            Console.WriteLine("\tDeleting existing file {0}.", dest);
            File.Delete(dest);
        }

        Console.WriteLine("\tCopying {0} to {1}.", file.FullName, dest);
        File.Copy(file.FullName, dest);

        Console.WriteLine("\tDeferring load of {0} to runtime.", file.Name);
        PublicDelayLoadDLLs.Add(file.Name);
    }

    private void ImportFromDirectoryMatching(string dir, string match)
    {
        foreach (string f in Directory.GetFiles(dir))
        {
            if (f.Contains(match))
                ImportAndCopy(new FileInfo(f));
        }
    }

    private void LinkKinect(ReadOnlyTargetRules target)
    {
        switch (target.Platform)
        {
            case UnrealTargetPlatform.Win64:
                Console.WriteLine("Preparing Jester for 64-bit build.");
                PlatformBitness = "x64";
                PlatformTarget = "Win64";
                break;

            case UnrealTargetPlatform.Win32:
                Console.WriteLine("Building Jester for 32-bit build.");
                PlatformBitness = "x86";
                PlatformTarget = "Win32";
                break;

            default:
                throw new PlatformNotSupportedException("Jester only supports Windows x86/x64 targets.");
        }

        string libPath = Path.Combine(KinectSdkPath, "lib", PlatformBitness);
        string incPath = Path.Combine(KinectSdkPath, "inc");
        string redistPath = Path.Combine(KinectSdkPath, "redist");
        string vgbPath = Path.Combine(redistPath, "VGB", PlatformBitness);
        string vgbTechPath = Path.Combine(vgbPath, "vgbtechs");
        string corePath = @"C:\Windows\System32\";
        string dllPrefix = @"Kinect20";
        string dllSuffix = @".dll";

        Console.WriteLine("Including Kinect SDK headers from {0}", incPath);
        PublicIncludePaths.Add(incPath);

        Console.WriteLine("Importing Kinect SDK static libraries from {0}", libPath);
        PublicLibraryPaths.Add(libPath);
        foreach (string f in Directory.GetFiles(libPath))
        {
            Console.WriteLine("\tAdding additional library {0}", f);
            PublicAdditionalLibraries.Add(f);
        }

        Console.WriteLine("Importing Kinect Visual Gesture Builder SDK from {0}", vgbPath);
        ImportFromDirectoryMatching(vgbPath, dllPrefix);

        Console.WriteLine("Importing Kinect Visual Gesture Builder Tech SDK from {0}", vgbTechPath);
        ImportFromDirectoryMatching(vgbTechPath, dllSuffix);

        Console.WriteLine("Importing Kinect SDK Core from {0}", corePath);
        ImportAndCopy(new FileInfo(Path.Combine(corePath, "Kinect20.dll")));
    }

	public Jester(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        if (string.IsNullOrEmpty(KinectSdkPath) || !Directory.Exists(KinectSdkPath))
            throw new FileNotFoundException("Kinect v2 SDK not found. Please check your installation.");

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
