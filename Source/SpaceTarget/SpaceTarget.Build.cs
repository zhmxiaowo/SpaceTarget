// Copyright 4DAGE Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;
using Tools.DotNETCommon;

public class SpaceTarget : ModuleRules
{
	public SpaceTarget(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		bEnableUndefinedIdentifierWarnings = false;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "AugmentedReality", "GeometricObjects" });
		PrivateDependencyModuleNames.AddRange(new string[] { "Json", "JsonUtilities", "Engine", "RenderCore", "RHI", });

        //The path for the header files
        PublicIncludePaths.AddRange(new string[] { "$(ModuleDir)/Public" });

        //The path for the source files
        PrivateIncludePaths.AddRange(new string[] { "SpaceTarget/Private" });

		//string tffilePath = PluginDirectory.Replace("\\","/").Replace("Plugins/SpaceTarget","") + "Content/SpaceTarget/SpaceTargetAssets/EncARHelper.tflite";
		//Log.TraceInformation(tffilePath);
		if (Target.Platform == UnrealTargetPlatform.Android)
		{
			PrivateDependencyModuleNames.AddRange(new string[] { "Launch", "GoogleARCoreBase" });

			AdditionalPropertiesForReceipt.Add(new ReceiptProperty("AndroidPlugin", Path.Combine(ModuleDirectory, "SpaceTarget_APL.xml")));

			//unreal memory bug ,we need to link .so file manually
			PublicAdditionalLibraries.Add("$(ModuleDir)/Libs/Android/arm64-v8a/libsfm.so");
			PublicAdditionalLibraries.Add("$(ModuleDir)/Libs/Android/armeabi-v7a/libsfm.so");
		}
		else if (Target.Platform == UnrealTargetPlatform.IOS)
		{
			AdditionalPropertiesForReceipt.Add("IOSPlugin", Path.Combine(ModuleDirectory, "SpaceTarget_IPL.xml"));
			PrivateDependencyModuleNames.AddRange(new string[] { "Launch", "AppleARKit" });
            PublicAdditionalFrameworks.Add(new Framework("FKARSFM", "$(ModuleDir)/Libs/iOS/FKARSFM.embeddedframework.zip"));
            PublicFrameworks.AddRange(new string[] { "CoreML" , "Accelerate" });

			// Now get the base of UE4's modules dir (could also be Developer, Editor, ThirdParty)


			// now you can include the module's private paths!
			// as an example, you can expose UE4's abstraction of D3D11, located in Source/Runtime/Windows/D3D11RHI
		}

	}
}
