# Changelog - RoadBuilder City Creation Optimization

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
