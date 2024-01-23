// Copyright Paul Brocklehurst 2022

using UnrealBuildTool;

public class OneLastGrindEditor : ModuleRules
{
    public OneLastGrindEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "OneLastGrind",
            "DetailCustomizations",
            "Slate",
            "SlateCore",
            "UnrealEd",
            "PropertyEditor",
            "EditorStyle"
        });
    }
}
