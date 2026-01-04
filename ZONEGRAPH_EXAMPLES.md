# RoadBuilder ZoneGraph Examples

This file contains practical code examples for using RoadBuilder's ZoneGraph integration.

## C++ Examples

### Example 1: Basic Road Export

```cpp
#include "RoadActor.h"
#include "RoadZoneGraphExport.h"

void ExportSingleRoad(ARoadActor* Road)
{
    // Create default configuration
    FRoadZoneGraphConfig Config;
    
    // Export the road
    TArray<FRoadZoneLane> Lanes;
    if (Road->ExportToZoneGraph(Config, Lanes))
    {
        UE_LOG(LogTemp, Log, TEXT("Exported %d lanes from road"), Lanes.Num());
        
        // Inspect exported lanes
        for (int32 i = 0; i < Lanes.Num(); i++)
        {
            const FRoadZoneLane& Lane = Lanes[i];
            UE_LOG(LogTemp, Log, TEXT("Lane %d: %d points, width %.2f"), 
                i, Lane.Points.Num(), Lane.Width);
        }
    }
}
```

### Example 2: Custom Configuration

```cpp
void ExportHighwayWithCustomSettings(ARoadActor* Highway)
{
    FRoadZoneGraphConfig Config;
    
    // Highway-specific settings
    Config.LaneWidth = 450.0f;              // Wider lanes (4.5m)
    Config.PointsPerMeter = 1.5f;           // Lower detail for performance
    Config.MinPointDistance = 75.0f;        // Larger minimum spacing
    Config.MaxPointDistance = 600.0f;       // Allow longer segments
    Config.bExportDrivingLanesOnly = true;  // Skip shoulders
    Config.bGenerateReverseLanes = false;   // One-way highway
    
    TArray<FRoadZoneLane> Lanes;
    if (Highway->ExportToZoneGraph(Config, Lanes))
    {
        UE_LOG(LogTemp, Log, TEXT("Exported highway with %d lanes"), Lanes.Num());
    }
}
```

### Example 3: Export Multiple Roads to Single ZoneShape

```cpp
#include "EngineUtils.h"

void CreateCityZoneGraph(UWorld* World)
{
    // Gather all road actors
    TArray<ARoadActor*> CityRoads;
    for (TActorIterator<ARoadActor> It(World); It; ++It)
    {
        ARoadActor* Road = *It;
        if (Road->Tags.Contains(FName("City")))
        {
            CityRoads.Add(Road);
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("Found %d city roads"), CityRoads.Num());
    
    // Configure export for city streets
    FRoadZoneGraphConfig Config;
    Config.LaneWidth = 350.0f;
    Config.PointsPerMeter = 2.5f;
    Config.bGenerateReverseLanes = true;
    
    // Create ZoneShape
    FVector SpawnLocation(0, 0, 100); // Slightly elevated for visibility
    AZoneShape* ZoneShape = URoadZoneGraphExporter::CreateZoneShapeFromRoads(
        World, 
        CityRoads, 
        Config, 
        SpawnLocation
    );
    
    if (ZoneShape)
    {
        ZoneShape->SetActorLabel(TEXT("City_ZoneGraph"));
        UE_LOG(LogTemp, Log, TEXT("Created ZoneShape at %s"), 
            *SpawnLocation.ToString());
    }
}
```

### Example 4: Batch Export with Error Handling

```cpp
void BatchExportRoads(const TArray<ARoadActor*>& Roads)
{
    FRoadZoneGraphConfig Config;
    TArray<FRoadZoneLane> AllLanes;
    
    int32 SuccessCount = URoadZoneGraphExporter::ExportRoadsToZoneLanes(
        Roads, 
        Config, 
        AllLanes
    );
    
    UE_LOG(LogTemp, Log, TEXT("Successfully exported %d/%d roads"), 
        SuccessCount, Roads.Num());
    UE_LOG(LogTemp, Log, TEXT("Total lanes: %d"), AllLanes.Num());
    
    // Process lanes
    for (const FRoadZoneLane& Lane : AllLanes)
    {
        if (Lane.bIsReverse)
        {
            UE_LOG(LogTemp, Verbose, TEXT("Reverse lane with %d points"), 
                Lane.Points.Num());
        }
    }
}
```

### Example 5: Actor Component for Automatic Export

```cpp
// YourRoadNetworkComponent.h
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RoadZoneGraphExport.h"
#include "YourRoadNetworkComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class YOUR_API UYourRoadNetworkComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ZoneGraph")
    FRoadZoneGraphConfig ExportConfig;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ZoneGraph")
    bool bAutoExportOnBeginPlay = false;
    
    UFUNCTION(BlueprintCallable, Category = "ZoneGraph")
    void ExportAllRoads();
    
protected:
    virtual void BeginPlay() override;
};

// YourRoadNetworkComponent.cpp
#include "YourRoadNetworkComponent.h"
#include "RoadActor.h"
#include "EngineUtils.h"

void UYourRoadNetworkComponent::BeginPlay()
{
    Super::BeginPlay();
    
    if (bAutoExportOnBeginPlay)
    {
        ExportAllRoads();
    }
}

void UYourRoadNetworkComponent::ExportAllRoads()
{
    UWorld* World = GetWorld();
    if (!World) return;
    
    TArray<ARoadActor*> Roads;
    for (TActorIterator<ARoadActor> It(World); It; ++It)
    {
        Roads.Add(*It);
    }
    
    AZoneShape* ZoneShape = URoadZoneGraphExporter::CreateZoneShapeFromRoads(
        World, 
        Roads, 
        ExportConfig
    );
    
    if (ZoneShape)
    {
        UE_LOG(LogTemp, Log, TEXT("Auto-exported %d roads to ZoneGraph"), 
            Roads.Num());
    }
}
```

## Blueprint Examples

### Example 1: Simple Button to Export Roads

**Blueprint Setup:**
1. Create a Blueprint actor or widget
2. Add a button/event
3. In the event graph:

```
Event (Button Click / Begin Play / etc.)
  |
  v
Get All Actors Of Class
  - Actor Class: Road Actor
  |
  v
Create Zone Shape From Roads (URoadZoneGraphExporter)
  - World Context: Get World
  - Road Actors: (from previous node)
  - Config: Make FRoadZoneGraphConfig
    - Lane Width: 400.0
    - Points Per Meter: 2.0
    - Export Driving Lanes Only: true
    - Generate Reverse Lanes: true
  - Spawn Location: Make Vector (0, 0, 100)
  |
  v
Branch (Is Valid?)
  - True: Print String "ZoneGraph created successfully"
  - False: Print String "Failed to create ZoneGraph"
```

### Example 2: Export Specific Tagged Roads

```
Event Begin Play
  |
  v
Get All Actors Of Class (Road Actor)
  |
  v
For Each Loop
  |
  v
Actor Has Tag? (Tag: "Highway")
  - True: Add to Array (Highway Roads)
  - False: Skip
  |
  v
(After loop completes)
  |
  v
Create Zone Shape From Roads
  - Road Actors: Highway Roads Array
  - Config: (Highway settings)
  |
  v
Set Actor Label
  - New Label: "Highway_ZoneGraph"
```

### Example 3: Custom Config per Road Type

```
Function: Get Config For Road Type
  - Input: Road Type (Enum or String)
  - Output: FRoadZoneGraphConfig
  
  Switch on Road Type:
    - Highway:
        Lane Width: 450
        Points Per Meter: 1.5
        Generate Reverse: false
    - City Street:
        Lane Width: 350
        Points Per Meter: 2.5
        Generate Reverse: true
    - Residential:
        Lane Width: 300
        Points Per Meter: 2.0
        Generate Reverse: true
```

### Example 4: Level Blueprint Auto-Export

```
Event Begin Play
  |
  v
Delay (0.5 seconds) // Wait for level to load
  |
  v
Get All Actors Of Class (Road Actor)
  |
  v
Branch (Array Length > 0?)
  - True:
      |
      v
      Create Zone Shape From Roads
      |
      v
      Save Zone Shape Reference (to variable)
  - False:
      Print String "No roads found in level"
```

## Console Commands

You can also create console commands for testing:

```cpp
// In your game module or plugin

#include "Engine/Console.h"

// Register console command
static FAutoConsoleCommand ExportRoadsCommand(
    TEXT("RoadBuilder.ExportZoneGraph"),
    TEXT("Export all roads to ZoneGraph"),
    FConsoleCommandDelegate::CreateLambda([]()
    {
        UWorld* World = GEngine->GetWorldFromContextObject(
            GEngine->GameViewport, 
            EGetWorldErrorMode::LogAndReturnNull
        );
        
        if (World)
        {
            TArray<ARoadActor*> Roads;
            for (TActorIterator<ARoadActor> It(World); It; ++It)
            {
                Roads.Add(*It);
            }
            
            FRoadZoneGraphConfig Config;
            AZoneShape* ZoneShape = URoadZoneGraphExporter::CreateZoneShapeFromRoads(
                World, Roads, Config
            );
            
            if (ZoneShape)
            {
                GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green,
                    FString::Printf(TEXT("Exported %d roads"), Roads.Num()));
            }
        }
    })
);
```

Then in console: `RoadBuilder.ExportZoneGraph`

## Common Patterns

### Pattern 1: Export on Level Load (Game Mode)

```cpp
void AYourGameMode::BeginPlay()
{
    Super::BeginPlay();
    
    // Delay to ensure level is fully loaded
    FTimerHandle TimerHandle;
    GetWorldTimerManager().SetTimer(TimerHandle, [this]()
    {
        ExportLevelRoads();
    }, 0.5f, false);
}

void AYourGameMode::ExportLevelRoads()
{
    // Your export logic here
}
```

### Pattern 2: Selective Export by Tags

```cpp
void ExportRoadsByTag(UWorld* World, FName Tag)
{
    TArray<ARoadActor*> TaggedRoads;
    for (TActorIterator<ARoadActor> It(World); It; ++It)
    {
        if (It->Tags.Contains(Tag))
        {
            TaggedRoads.Add(*It);
        }
    }
    
    // Export tagged roads
    if (TaggedRoads.Num() > 0)
    {
        FRoadZoneGraphConfig Config;
        URoadZoneGraphExporter::CreateZoneShapeFromRoads(
            World, TaggedRoads, Config
        );
    }
}
```

### Pattern 3: Progressive Export (Avoid Hitches)

```cpp
void ProgressiveExport(UWorld* World)
{
    // Export roads in batches to avoid frame hitches
    static int32 CurrentIndex = 0;
    static TArray<ARoadActor*> AllRoads;
    
    if (CurrentIndex == 0)
    {
        // First call - gather all roads
        for (TActorIterator<ARoadActor> It(World); It; ++It)
        {
            AllRoads.Add(*It);
        }
    }
    
    const int32 BatchSize = 10;
    int32 EndIndex = FMath::Min(CurrentIndex + BatchSize, AllRoads.Num());
    
    TArray<ARoadActor*> Batch;
    for (int32 i = CurrentIndex; i < EndIndex; i++)
    {
        Batch.Add(AllRoads[i]);
    }
    
    // Export batch
    FRoadZoneGraphConfig Config;
    TArray<FRoadZoneLane> Lanes;
    URoadZoneGraphExporter::ExportRoadsToZoneLanes(Batch, Config, Lanes);
    
    CurrentIndex = EndIndex;
    
    if (CurrentIndex >= AllRoads.Num())
    {
        UE_LOG(LogTemp, Log, TEXT("Progressive export complete!"));
        CurrentIndex = 0;
        AllRoads.Empty();
    }
}
```

## Tips

1. **Performance**: Start with lower point density (1.0-1.5) and increase if needed
2. **Testing**: Use console visualization (`showdebug zonegraph`) to verify export
3. **Tagging**: Tag your roads for easy filtering (e.g., "Highway", "City", "Parking")
4. **Validation**: Check lane counts and widths after export
5. **Iteration**: Export is fast - don't hesitate to re-export during development

## Troubleshooting

**No lanes exported:**
- Check if roads have driving lanes
- Verify `bExportDrivingLanesOnly` setting
- Ensure roads are valid and have geometry

**ZoneShape is empty:**
- Check logs for error messages
- Verify World context is valid
- Ensure roads array is not empty

**Performance issues:**
- Reduce `PointsPerMeter`
- Increase `MinPointDistance`
- Export in batches for large networks
