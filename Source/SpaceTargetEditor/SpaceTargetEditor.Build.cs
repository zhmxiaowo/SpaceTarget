// Copyright 4DAGE Games, Inc. All Rights Reserved.
using UnrealBuildTool;
using Tools.DotNETCommon;

public class SpaceTargetEditor : ModuleRules
{
	public SpaceTargetEditor(ReadOnlyTargetRules Target) : base(Target)
	{

        //PrecompileForTargets = PrecompileTargetsType.Editor;
        //bPrecompile = true;
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore","SpaceTarget" ,"HTTP"});
		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore", "EditorStyle" });

		//PublicDependencyModuleNames.AddRange(new string[] { "SpaceTarget" });

		Log.TraceInformation(string.Format("SpaceTargetEditor Target.Type: {0}", Target.Type.ToString()));
        if (Target.Type == TargetType.Editor)
        {
            PrivateDependencyModuleNames.AddRange(new string[] { "UnrealEd", "PropertyEditor" });//
        }



        //The path for the header files
        PublicIncludePaths.AddRange(new string[] { "$(ModuleDir)/Public" });

        //The path for the source files
        PrivateIncludePaths.AddRange(new string[] { "SpaceTargetEditor/Private" });

		//PrivateDependencyModuleNames.AddRange(new string[] { "UnrealEd" });
		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
