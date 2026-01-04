# City Building Guide for RoadBuilder

## Overview

RoadBuilder now includes enhanced features specifically designed for creating complete city road networks with traffic control infrastructure. This guide will help you utilize these features to build realistic urban environments.

## Traffic Control System

### Traffic Control Types

The following traffic control elements are now supported:

1. **Stop Signs** - Place at intersections requiring full stops
2. **Yield Signs** - Place where traffic must yield to other roads
3. **Traffic Lights** - Automated intersection control with configurable timing
4. **Speed Limit Signs** - Display speed restrictions
5. **Pedestrian Crossings** - Marked crosswalks with proper signage
6. **No Parking Signs** - Parking restriction indicators
7. **One Way Signs** - Directional traffic flow indicators
8. **Do Not Enter Signs** - Restricted access markers

### Adding Traffic Controls to Roads

Traffic controls can be added to any road through the RoadProps system:

1. Select your road actor in the Unreal Editor
2. Navigate to the "Traffic Control" category in the details panel
3. Add entries to the "Traffic Controls" array
4. Configure each control's type, location, rotation, and scale

### Traffic Light Configuration

Traffic lights support customizable timing:

- **Green Duration**: Time the light remains green (1-300 seconds)
- **Yellow Duration**: Warning phase duration (1-60 seconds)
- **Red Duration**: Time the light remains red (1-300 seconds)
- **Pedestrian Signal**: Optional walk/don't walk signals
- **Turn Arrow**: Optional left/right turn arrow phases

## Intersection Management

### Intersection Types

Four intersection types are supported:

1. **Four-Way Intersections**: Standard cross intersections
2. **Three-Way (T-Junctions)**: T-shaped intersections
3. **Roundabouts**: Circular traffic flow
4. **Custom**: Flexible configuration for unique layouts

### Intersection Configuration

Configure intersections through the FIntersectionConfig structure:

```
IntersectionType: Choose from Four-Way, Three-Way, Roundabout, or Custom
HasTrafficLights: Enable/disable traffic light control
HasStopSigns: Enable/disable stop sign control
HasYieldSigns: Enable/disable yield sign control
HasCrosswalks: Enable/disable pedestrian crosswalks
CrosswalkWidth: Set the width of crosswalks (default: 400cm)
TrafficLightConfig: Configure traffic light timing
```

## Best Practices for City Building

### 1. Planning Your Road Network

- Start with main arterial roads
- Add secondary streets connecting to main roads
- Use the junction system to create proper intersections
- Plan traffic flow direction early

### 2. Traffic Control Placement

- Place stop signs at low-traffic intersections
- Use traffic lights for high-traffic intersections
- Add yield signs for merge lanes and roundabout entrances
- Ensure all intersections have appropriate control measures

### 3. Pedestrian Considerations

- Add crosswalks at all intersections
- Place pedestrian crossing signs near crosswalks
- Consider pedestrian signal timing in traffic light configuration
- Maintain consistent crosswalk widths (typically 300-500cm)

### 4. Performance Optimization

- Use instanced static meshes for repeated signs (automatically handled)
- Group similar traffic controls together
- Use appropriate LOD settings for traffic control assets
- Leverage the PCG system for additional city details

## Error Prevention

The updated RoadBuilder includes enhanced error checking:

- Null pointer validation for all traffic control placements
- Array bounds checking prevents crashes
- World context validation ensures proper spawning
- Blueprint asset validation before spawning

## Example Workflow

### Creating a Four-Way Intersection with Traffic Lights

1. Create four road actors meeting at a point
2. Use the junction system to connect them
3. In the junction settings, enable "HasTrafficLights"
4. Configure the TrafficLightConfig with appropriate timings:
   - GreenDuration: 30 seconds
   - YellowDuration: 3 seconds
   - RedDuration: 30 seconds
5. Enable "HasCrosswalks" for pedestrian safety
6. Set CrosswalkWidth to 400cm

### Creating a Residential Street with Stop Signs

1. Create your residential road network
2. At each intersection, add FTrafficControlPlacement entries
3. Set ControlType to "StopSign"
4. Position signs at appropriate distances from intersections
5. Add "PedestrianCrossing" controls near schools or parks

## Troubleshooting

### Traffic Controls Not Appearing

- Verify that assets are assigned in the TrafficControls array
- Check that CustomAsset is set for each control
- Ensure the road actor has a valid world context
- Verify BuildProps is enabled in global settings

### Performance Issues

- Reduce the number of unique traffic control assets
- Use material instances instead of unique materials
- Enable instancing for repeated meshes
- Consider using simplified collision for signs

## Future Enhancements

Planned features for future releases:

- Automated traffic control placement based on road type
- Traffic simulation integration
- Dynamic traffic light timing based on traffic flow
- Additional sign types and regional variants
- Parking lot generation with proper markings

## Additional Resources

- Video tutorials: https://www.youtube.com/watch?v=zGpPd4RAneQ&list=PLCWhWOgVmdwkOBGElYEPumnCXL12zEtwE
- GitHub repository: https://github.com/fullike/RoadBuilder
- Report issues: https://github.com/fullike/RoadBuilder/issues
