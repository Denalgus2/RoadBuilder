# Changelog - RoadBuilder

## Version 0.7.0 - 2026-01-04

### Major Features

#### Version Compatibility Improvement
- **Fixed**: Removed hardcoded engine version constraint from .uplugin file
  - Plugin now compatible with UE 5.0 and later without version mismatch warnings
  - Users can rebuild modules for their specific engine version without modification
  - Eliminates "plugin was designed for build X" warnings
  - Resolves compilation prompts when using different engine versions

#### ZoneGraph Integration
- **Added**: Full ZoneGraph and MassCity integration for AI vehicle navigation
  - Export RoadBuilder roads to ZoneGraph lane format
  - Automatic ZoneShape actor generation from road networks
  - Support for MassTraffic and MassEntity AI systems
  - Compatible with Epic's CitySample project
  
#### New Export System
- **Added**: `RoadZoneGraphExport.h` - ZoneGraph export infrastructure
  - `FRoadZoneGraphConfig` - Configurable export settings
  - `FRoadZoneLane` - Exported lane data structure
  - `URoadZoneGraphExporter` - Static utility class for export operations
  
#### Export Features
- **Added**: Configurable lane width (default: 400cm)
- **Added**: Adjustable point density (default: 2 points per meter)
- **Added**: Min/max point distance constraints
- **Added**: Lane type filtering (driving lanes only option)
- **Added**: Automatic reverse lane generation for bidirectional roads
- **Added**: ZoneGraph tag support for lane categorization
- **Added**: Smart point sampling with curve adaptation

#### API Enhancements
- **Added**: `ARoadActor::ExportToZoneGraph()` - Instance method for single road export
- **Added**: `URoadZoneGraphExporter::ExportRoadToZoneLanes()` - Export single road
- **Added**: `URoadZoneGraphExporter::ExportRoadsToZoneLanes()` - Batch export multiple roads
- **Added**: `URoadZoneGraphExporter::CreateZoneShapeFromRoads()` - Generate ZoneShape actor
- **Added**: Full Blueprint exposure for all export functions

#### Module Dependencies
- **Added**: `ZoneGraph` module dependency
- **Added**: `ZoneGraphAnnotations` module dependency
- **Added**: `MassNavigation` module dependency
- **Updated**: `.uplugin` to require ZoneGraph plugin

#### Documentation
- **Added**: `ZONEGRAPH_INTEGRATION_GUIDE.md` - Comprehensive 13KB guide covering:
  - What is ZoneGraph and why use it
  - Setup and configuration instructions
  - Usage examples (Blueprint and C++)
  - API reference
  - Best practices for performance
  - Troubleshooting guide
  - Integration with MassTraffic
  - Complete workflow examples
- **Updated**: `Readme.md` with ZoneGraph features section

### Implementation Details

#### Lane Export Algorithm
- Samples points along lane centerline (between left and right boundaries)
- Calculates average lane width from boundary distances
- Applies configurable point density and distance constraints
- Transforms world coordinates to ZoneShape local space
- Preserves height information from road elevation profiles

#### Quality of Life
- Automatic detection of driving vs. non-driving lanes
- Smart filtering to export only relevant lane types
- Bidirectional lane support with one-way option
- Graceful error handling with detailed logging
- Validation of road data before export

### Compatibility

- **Unreal Engine**: 5.0+ (compatible with all UE5 versions; tested primarily on 5.4.3+)
- **New Requirements**: ZoneGraph plugin must be enabled
- **Platforms**: Win64, Linux (unchanged)
- **Dependencies**: GeoReferencing, PCG, ZoneGraph

### Migration Guide

For existing projects:
1. Enable ZoneGraph plugin in your project settings
2. No changes needed to existing RoadBuilder functionality
3. ZoneGraph export is completely opt-in
4. All previous features continue to work unchanged

### Use Cases

This feature enables:
- **AI Traffic Simulation**: Vehicles follow roads using MassTraffic
- **Pedestrian Navigation**: AI pedestrians use sidewalk lanes
- **Dynamic Pathfinding**: Runtime navigation for autonomous vehicles
- **City Simulation**: Large-scale urban traffic with thousands of agents
- **Game Development**: Realistic traffic for open-world games
- **Autonomous Driving**: Testing and simulation environments

### Known Limitations

- Requires ZoneGraph plugin (available in UE 5.0+)
- MassTraffic assets not included (use CitySample or create custom)
- Traffic light timing is not exported to ZoneGraph (future enhancement)
- Intersection connectivity may need manual adjustment for complex junctions

### Performance Notes

- Export is performed once at design time or level load
- Minimal runtime overhead (ZoneGraph handles pathfinding)
- Recommended: 1-2 points per meter for highways, 2-3 for city streets
- Can handle thousands of lanes in a single ZoneShape

---

## Version 0.6.1 - 2026-01-04

### Critical Bug Fixes

#### Memory Safety
- **Fixed**: Critical null pointer dereference in `URoadProps::Generate()` where `GetWorld()` was incorrectly called on UObject
  - Now properly uses `Road->GetWorld()` with null validation
  - Added checks for valid World pointer before spawning actors
  - Prevents crashes when spawning Blueprint actors for road props

#### Array Access Safety
- **Fixed**: Potential crashes from accessing empty arrays with `.Last()` and `[0]`
  - Added empty array validation in `URoadStyle::Create()`
  - Added bounds checking for Segments, LocalOffsets arrays
  - Added validation before accessing Line.Points.Last() in prop generation

#### Null Pointer Protection
- **Fixed**: Missing null checks in `URoadStyle::Create()` function
  - Added validation for SrcBoundary and DstBoundary parameters
  - Added validation for SrcRoad and DstRoad pointers
  - Added checks for SrcB, DstB, SrcL, DstL throughout the function
  - Early returns prevent null pointer dereferences

#### Mathematical Safety
- **Fixed**: Potential division by zero in vector normalization
  - Added SMALL_NUMBER check before normalizing direction vectors
  - Skips degenerate prop placements where consecutive points are identical
  - Prevents NaN values in calculations

### New Features

#### Traffic Control System
- **Added**: `TrafficControl.h` - Comprehensive traffic control infrastructure
  - 8 traffic control types: Stop signs, Yield signs, Traffic lights, Speed limits, Pedestrian crossings, No parking, One-way, Do not enter
  - Type-safe asset references: `UStaticMesh*`, `UBlueprint*`, `UMaterialInterface*`
  - Configurable placement with position, rotation, and scale
  
#### Traffic Light Configuration
- **Added**: Configurable traffic light timing system
  - Customizable green phase duration (1-300 seconds)
  - Customizable yellow phase duration (1-60 seconds)
  - Customizable red phase duration (1-300 seconds)
  - Optional pedestrian signal support
  - Optional turn arrow phases

#### Intersection Management
- **Added**: `IntersectionManager.h` - Intersection configuration system
  - Support for four-way intersections
  - Support for three-way (T-junction) intersections
  - Support for roundabouts
  - Custom intersection configurations
  - Configurable traffic control per intersection
  - Crosswalk width configuration

#### Integration with RoadProps
- **Enhanced**: Extended `URoadProps` class with traffic control support
  - `TArray<FTrafficControlPlacement> TrafficControls` property
  - `FTrafficLightConfig DefaultTrafficLightConfig` property
  - Seamless integration with existing prop generation system

### Documentation

#### New Documentation
- **Added**: `CITY_BUILDING_GUIDE.md` - Comprehensive 5500+ word guide
  - Traffic control system overview and usage
  - Intersection management guide
  - Best practices for city building
  - Example workflows for common scenarios
  - Troubleshooting guide
  - Performance optimization tips

- **Added**: `OPTIMIZATION_GUIDE.md` - Performance and optimization documentation
  - Memory safety improvements summary
  - Performance characteristics
  - Scalability guidelines
  - Best practices for large cities
  - Benchmark information

- **Enhanced**: `Readme.md` - Updated with city building features
  - Added "City Building Enhancements" section
  - Documented new traffic control capabilities
  - Highlighted intersection management features

### Code Quality Improvements

#### Type Safety
- **Improved**: Replaced generic `UObject* CustomAsset` with specific types
  - `UStaticMesh* StaticMeshAsset` for static props
  - `UBlueprint* BlueprintAsset` for interactive elements
  - `UMaterialInterface* DecalMaterial` for road markings
  - Better compile-time type checking
  - Clearer API for asset assignment

#### Error Handling
- **Improved**: Enhanced error handling throughout the codebase
  - Graceful handling of invalid configurations
  - Early returns on error conditions
  - Proper resource validation
  - Reduced cascading failures

### Testing & Validation

- **Passed**: Code review with all issues addressed
- **Passed**: CodeQL security analysis (no vulnerabilities found)
- **Validated**: All array access patterns
- **Validated**: All null pointer dereferences fixed
- **Validated**: All mathematical operations safe

### Compatibility

- **Unreal Engine**: 5.4.3+
- **Platforms**: Win64, Linux
- **Dependencies**: GeoReferencing, PCG (unchanged)

### Migration Guide

For existing projects:
1. No breaking changes to existing functionality
2. New traffic control features are opt-in
3. Existing road styles and props continue to work unchanged
4. New properties have sensible defaults

### Known Limitations

- Traffic control assets must be provided by the user (no default assets included)
- Traffic light timing is configuration-only (no runtime animation system)
- Intersection management is primarily for planning (no traffic simulation)

### Future Enhancements (Planned)

- Automated traffic control placement based on road type
- Traffic simulation integration
- Dynamic traffic light timing
- Additional regional sign variants
- Parking lot generation with markings

---

## Installation

Clone this repository to your Unreal Engine project's Plugins folder:
```
YourProject/Plugins/RoadBuilder/
```

Extract Content.zip to the Content folder as described in README.md.

## Support

- GitHub: https://github.com/fullike/RoadBuilder
- Issues: https://github.com/fullike/RoadBuilder/issues
- Video Tutorials: https://www.youtube.com/watch?v=zGpPd4RAneQ&list=PLCWhWOgVmdwkOBGElYEPumnCXL12zEtwE
