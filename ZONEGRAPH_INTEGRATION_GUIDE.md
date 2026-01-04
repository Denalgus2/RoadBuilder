# ZoneGraph Integration Guide

## Overview

RoadBuilder now includes native support for Unreal Engine 5's ZoneGraph system, enabling AI cars and pedestrians to navigate along roads created with RoadBuilder. This integration allows you to automatically convert RoadBuilder roads into ZoneGraph navigation lanes for use with MassEntity, MassTraffic, and the CitySample framework.

## What is ZoneGraph?

ZoneGraph is Unreal Engine 5's spline-based navigation system designed for large-scale AI movement, particularly for vehicles and pedestrians in urban environments. It's a core component of the CitySample project and MassEntity framework, providing:

- High-performance navigation for thousands of AI agents
- Lane-based pathfinding for vehicles
- Integration with MassTraffic for realistic traffic simulation
- Support for complex intersection navigation
- Dynamic lane switching and overtaking

## Features

### Automatic Lane Export

- **Driving Lane Detection**: Automatically exports only driving lanes by default
- **Bidirectional Support**: Can generate reverse lanes for two-way traffic
- **Width Calculation**: Automatically calculates appropriate lane widths from road geometry
- **Point Sampling**: Configurable point density along lane splines

### Flexible Configuration

- **Lane Width**: Customize zone lane width (default: 400cm)
- **Point Density**: Control points per meter for smooth curves (default: 2)
- **Distance Constraints**: Set minimum and maximum distances between points
- **Lane Filtering**: Choose which lane types to export
- **Tag Support**: Apply ZoneGraph tags to exported lanes

### Integration Methods

1. **Direct Export**: Export roads to `FRoadZoneLane` data structures
2. **ZoneShape Creation**: Automatically create ZoneShape actors from roads
3. **Blueprint Support**: Full Blueprint exposure for workflow integration

## Setup

### 1. Enable Required Plugins

Ensure the following plugins are enabled in your project:

1. **ZoneGraph** (Engine plugin)
2. **MassEntity** (Engine plugin) 
3. **MassAI** (Engine plugin, for AI navigation)
4. **MassTraffic** (CitySample plugin, for traffic simulation)

These are automatically added as dependencies in RoadBuilder's .uplugin file.

### 2. Module Dependencies

RoadBuilder now includes these modules in its Build.cs:
- `ZoneGraph`
- `ZoneGraphAnnotations`
- `MassNavigation`

No additional configuration is needed - these are set up automatically.

## Usage

### Method 1: Blueprint Export

Export a single road to zone lanes:

```blueprint
// Get your road actor
Road Actor -> Export to Zone Graph
  Config: Road Zone Graph Config
  Out Lanes: Array of Road Zone Lane

// Use the exported lanes for custom processing
```

### Method 2: Create ZoneShape Actor

Automatically create a ZoneShape from multiple roads:

```blueprint
// Create Zone Shape from Roads (Static Function)
URoadZoneGraphExporter::CreateZoneShapeFromRoads
  World: World Context
  Road Actors: Array of ARoadActor
  Config: FRoadZoneGraphConfig
  Spawn Location: FVector (optional)
  
// Returns: AZoneShape actor ready for ZoneGraph building
```

### Method 3: C++ Export

```cpp
#include "RoadActor.h"
#include "RoadZoneGraphExport.h"

void ExportRoadsToZoneGraph(ARoadActor* Road)
{
    // Configure export settings
    FRoadZoneGraphConfig Config;
    Config.LaneWidth = 400.0f;
    Config.PointsPerMeter = 2.0f;
    Config.bExportDrivingLanesOnly = true;
    Config.bGenerateReverseLanes = true;
    
    // Export to zone lanes
    TArray<FRoadZoneLane> ZoneLanes;
    if (Road->ExportToZoneGraph(Config, ZoneLanes))
    {
        UE_LOG(LogTemp, Log, TEXT("Exported %d lanes"), ZoneLanes.Num());
    }
}
```

### Method 4: Multiple Roads to Single ZoneShape

```cpp
void CreateZoneGraphFromRoadNetwork(UWorld* World, TArray<ARoadActor*> Roads)
{
    FRoadZoneGraphConfig Config;
    Config.LaneWidth = 400.0f;
    Config.PointsPerMeter = 2.0f;
    
    AZoneShape* ZoneShape = URoadZoneGraphExporter::CreateZoneShapeFromRoads(
        World, 
        Roads, 
        Config, 
        FVector::ZeroVector
    );
    
    if (ZoneShape)
    {
        // Zone shape is ready - build the ZoneGraph
        // This happens automatically in editor, or call:
        // ZoneGraph->BuildZoneGraph();
    }
}
```

## Configuration Options

### FRoadZoneGraphConfig

| Property | Type | Default | Description |
|----------|------|---------|-------------|
| LaneWidth | float | 400.0 | Width of zone lanes in cm (4 meters) |
| PointsPerMeter | float | 2.0 | Number of sample points per meter |
| MinPointDistance | float | 50.0 | Minimum distance between points in cm |
| MaxPointDistance | float | 500.0 | Maximum distance between points in cm |
| bExportDrivingLanesOnly | bool | true | Export only driving lanes, skip sidewalks |
| bGenerateReverseLanes | bool | true | Create reverse lanes for bidirectional roads |
| LaneTags | FZoneGraphTagMask | Empty | Tags to apply to exported lanes |

### Adjusting Point Density

Higher point density = smoother curves but more data:
- **Urban Streets**: 2-3 points per meter
- **Highways**: 1-2 points per meter
- **Tight Curves**: 3-4 points per meter

```cpp
Config.PointsPerMeter = 3.0f; // More points for tighter curves
Config.MinPointDistance = 30.0f; // Closer points for detail
```

## Workflow Example

### Complete City Road Network Export

1. **Create Your Road Network**
   - Use RoadBuilder to design your city roads
   - Add intersections, ramps, and lanes
   - Ensure proper lane configuration

2. **Select Roads for Export**
   ```cpp
   TArray<ARoadActor*> CityRoads;
   // Gather all road actors in your level
   for (TActorIterator<ARoadActor> It(World); It; ++It)
   {
       CityRoads.Add(*It);
   }
   ```

3. **Configure Export Settings**
   ```cpp
   FRoadZoneGraphConfig Config;
   Config.LaneWidth = 350.0f; // Narrower urban streets
   Config.PointsPerMeter = 2.5f;
   Config.bExportDrivingLanesOnly = true;
   ```

4. **Create ZoneShape**
   ```cpp
   AZoneShape* CityZones = URoadZoneGraphExporter::CreateZoneShapeFromRoads(
       World, CityRoads, Config
   );
   ```

5. **Build ZoneGraph**
   - In editor: ZoneGraph builds automatically
   - At runtime: Call build methods on ZoneGraphSubsystem

6. **Set Up MassTraffic**
   - Create MassTraffic spawners
   - Configure vehicle types
   - Link to ZoneGraph lanes

## Integration with MassTraffic

Once you have exported your roads to ZoneGraph:

1. **Add Zone Graph Builder**
   - Place `AZoneGraphData` actor in level
   - It will collect all ZoneShape actors
   - Build the graph in the editor

2. **Configure Mass Traffic**
   - Add `UMassTrafficSubsystem` configuration
   - Set up vehicle spawning parameters
   - Configure traffic density

3. **Vehicle Spawning**
   ```cpp
   // MassTraffic will automatically spawn vehicles on zone lanes
   // Configure in your MassTraffic settings:
   // - Vehicle types and frequency
   // - Spawn density per lane
   // - Traffic behavior rules
   ```

## Best Practices

### Road Design

- **Consistent Lane Widths**: Keep lane widths uniform for better AI navigation
- **Smooth Curves**: Use adequate smoothness settings in RoadBuilder
- **Proper Intersections**: Ensure lanes connect properly at intersections
- **Height Profiles**: Maintain reasonable road grades for vehicle physics

### Export Configuration

- **Performance**: Lower point density for better performance with many lanes
- **Accuracy**: Higher point density for complex curves and intersections
- **Lane Filtering**: Export only necessary lane types to reduce data

### Debugging

1. **Visualize Zone Lanes**
   - Enable ZoneGraph debugging in editor
   - Use `showdebug zonegraph` console command
   - Check lane connectivity at intersections

2. **Validate Export**
   ```cpp
   TArray<FRoadZoneLane> Lanes;
   Road->ExportToZoneGraph(Config, Lanes);
   
   for (const FRoadZoneLane& Lane : Lanes)
   {
       UE_LOG(LogTemp, Log, TEXT("Lane: %d points, width: %.2f"), 
           Lane.Points.Num(), Lane.Width);
   }
   ```

3. **Common Issues**
   - **No lanes exported**: Check `bExportDrivingLanesOnly` and lane types
   - **Broken connections**: Verify road continuity at intersections
   - **Poor performance**: Reduce point density or number of exported lanes

## API Reference

### URoadZoneGraphExporter

Static utility class for exporting roads to ZoneGraph format.

#### ExportRoadToZoneLanes
```cpp
static bool ExportRoadToZoneLanes(
    ARoadActor* RoadActor,
    const FRoadZoneGraphConfig& Config,
    TArray<FRoadZoneLane>& OutLanes
);
```
Export a single road to zone lane data.

#### ExportRoadsToZoneLanes
```cpp
static int32 ExportRoadsToZoneLanes(
    const TArray<ARoadActor*>& RoadActors,
    const FRoadZoneGraphConfig& Config,
    TArray<FRoadZoneLane>& OutLanes
);
```
Export multiple roads, returns count of successfully exported roads.

#### CreateZoneShapeFromRoads
```cpp
static AZoneShape* CreateZoneShapeFromRoads(
    UWorld* World,
    const TArray<ARoadActor*>& RoadActors,
    const FRoadZoneGraphConfig& Config,
    const FVector& SpawnLocation = FVector::ZeroVector
);
```
Create a ZoneShape actor from roads, ready for ZoneGraph building.

### ARoadActor Extensions

#### ExportToZoneGraph
```cpp
UFUNCTION(BlueprintCallable, Category = "RoadBuilder|ZoneGraph")
bool ExportToZoneGraph(
    const FRoadZoneGraphConfig& Config,
    TArray<FRoadZoneLane>& OutLanes
);
```
Instance method to export this road to zone lanes.

## Examples

### Example 1: Simple Highway Export

```cpp
// Get highway road
ARoadActor* Highway = GetHighwayRoad();

// Configure for highway (fewer points, wider lanes)
FRoadZoneGraphConfig Config;
Config.LaneWidth = 450.0f; // Wider highway lanes
Config.PointsPerMeter = 1.5f; // Less detail needed
Config.bGenerateReverseLanes = false; // One-way highway

// Export
TArray<FRoadZoneLane> Lanes;
Highway->ExportToZoneGraph(Config, Lanes);
```

### Example 2: Urban Grid with Intersections

```cpp
// Gather all city streets
TArray<ARoadActor*> Streets;
for (TActorIterator<ARoadActor> It(World); It; ++It)
{
    if (It->Tags.Contains("CityStreet"))
    {
        Streets.Add(*It);
    }
}

// Urban configuration
FRoadZoneGraphConfig Config;
Config.LaneWidth = 350.0f;
Config.PointsPerMeter = 2.5f;
Config.bGenerateReverseLanes = true;

// Create zone shape for entire city
AZoneShape* CityZones = URoadZoneGraphExporter::CreateZoneShapeFromRoads(
    World, Streets, Config, FVector(0, 0, 100)
);
```

### Example 3: Blueprint Implementation

Create a Blueprint actor with this setup:

1. Add event to trigger export
2. Get all road actors in level
3. Call `CreateZoneShapeFromRoads`
4. Handle the returned ZoneShape

```blueprint
Event BeginPlay
  -> Get All Actors Of Class (ARoadActor)
  -> Create Zone Shape From Roads
     - World Context: Self
     - Road Actors: (from Get All Actors)
     - Config: Make FRoadZoneGraphConfig
  -> Print String: "ZoneGraph created"
```

## Troubleshooting

### Compilation Errors

**Error**: "Cannot find ZoneGraph modules"
- **Solution**: Ensure ZoneGraph plugin is enabled in your .uproject file
- Regenerate project files

**Error**: "Module 'RoadBuilder' could not be loaded"
- **Solution**: This plugin requires UE 5.4.3 or newer
- Check EngineVersion in .uplugin file

### Runtime Issues

**Issue**: No lanes exported
- Check that roads have driving lanes
- Verify `bExportDrivingLanesOnly` setting
- Ensure road lane types are set correctly

**Issue**: ZoneShape is empty
- Verify roads are valid and have lane data
- Check logs for export errors
- Ensure World context is valid

**Issue**: AI vehicles not spawning
- Confirm ZoneGraph is built (check ZoneGraphData actor)
- Verify MassTraffic is configured
- Check that zones have proper tags

## Performance Considerations

### Large City Networks

For cities with hundreds of roads:
- Use lower point density (1.0-1.5 points/meter)
- Export in chunks rather than all at once
- Consider multiple ZoneShape actors for different city districts
- Use level streaming for very large worlds

### Memory Usage

Approximate memory per lane:
- **Low detail** (1 pt/m): ~500 bytes per 100m
- **Medium detail** (2 pt/m): ~1 KB per 100m
- **High detail** (4 pt/m): ~2 KB per 100m

For a city with 1000 lanes averaging 200m each:
- Low: ~1 MB
- Medium: ~2 MB  
- High: ~4 MB

## Future Enhancements

Planned features for future releases:
- Automatic intersection connection detection
- Traffic light timing export to ZoneGraph annotations
- Pedestrian crossing zones
- Parking area zones
- Dynamic lane closure/routing
- Integration with OpenDrive export

## Support

- **GitHub**: https://github.com/fullike/RoadBuilder
- **Issues**: https://github.com/fullike/RoadBuilder/issues
- **Video Tutorials**: https://www.youtube.com/watch?v=zGpPd4RAneQ&list=PLCWhWOgVmdwkOBGElYEPumnCXL12zEtwE

## References

- [Unreal Engine ZoneGraph Documentation](https://docs.unrealengine.com/5.4/en-US/zone-graph-in-unreal-engine/)
- [MassEntity System](https://docs.unrealengine.com/5.4/en-US/mass-entity-in-unreal-engine/)
- [City Sample Project](https://www.unrealengine.com/marketplace/en-US/product/city-sample)
