using UnrealBuildTool;

public class CastleBreaker : ModuleRules
{
    public CastleBreaker(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "AIModule", "UMG", "NavigationSystem", "GameplayTasks" });
    }
}
