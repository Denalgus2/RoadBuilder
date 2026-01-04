![](./Snapshot.png)

# RoadBuilder

RoadBuilder is a plugin for UnrealEngine that helps you to make complex roads in Unreal Editor.

RoadBuilder allows you to edit every detail of your road, including lane type, width, lane markings, traffic signs, zebra crossings, and more, while its built-in road styles allow you to quickly create various types of roads, such as street, highway, ramps, elevated roads and tunnels.

RoadBuilder can also create ground meshes and boundary spline for PCG system to generate more detail.

The roads created by RoadBuilder can be exported to OpenDrive format for use in various autonomous driving simulation environments.

### Features

- Spiral and curvature blending supported road planning.
- Various built-in road styles (street, highway, elevated, tunnel) and corresponding road props generation.
- Road chopping and splitting
- Junctions, ramps and forks generation.
- Lane carving and offset adjustment.
- Various lane marking and traffic signs support.
- Gore area generation.
- Seamless and refined ground mesh generation along with spline boundary for PCG graph
- SVG file import.
- OpenDrive (xodr) format export.
- **ZoneGraph Integration**: Export roads to Unreal Engine 5's ZoneGraph system for AI navigation with MassEntity and MassTraffic

### ZoneGraph & AI Navigation

- **Automatic Zone Export**: Convert RoadBuilder roads to ZoneGraph lanes for AI vehicle navigation
- **MassTraffic Compatible**: Full integration with Unreal Engine's MassTraffic system for realistic traffic simulation
- **Configurable Export**: Control lane width, point density, and which lanes to export
- **ZoneShape Generation**: Automatically create ZoneShape actors from road networks
- **Blueprint Support**: Full Blueprint API for ZoneGraph export workflows
- **CitySample Ready**: Works seamlessly with Epic's CitySample project

See [ZONEGRAPH_INTEGRATION_GUIDE.md](./ZONEGRAPH_INTEGRATION_GUIDE.md) for detailed usage instructions.

### City Building Enhancements

- **Traffic Control System**: Support for traffic lights, stop signs, yield signs, and various traffic control elements
- **Intersection Management**: Configure four-way, three-way (T-junctions), and roundabout intersections
- **Traffic Light Configuration**: Customizable timing for green, yellow, and red phases
- **Pedestrian Features**: Enhanced crosswalk support with configurable widths
- **Traffic Sign Types**: Stop signs, yield signs, speed limits, no parking, one-way, and do not enter signs
- **City Planning**: Improved error handling and validation for complex city road networks

These enhancements make RoadBuilder ideal for creating complete city road systems with proper traffic control infrastructure.

### Compatibility

- **Unreal Engine**: Compatible with UE 5.0 and later (tested primarily on 5.4.3+)
- **Platforms**: Windows (Win64), Linux
- **Required Plugins**: GeoReferencing, PCG, ZoneGraph (for ZoneGraph features)

When loading the plugin in a different engine version, Unreal Engine will prompt you to rebuild the modules. Simply click "Yes" to rebuild, and the plugin will be compiled for your engine version.

### Troubleshooting

**"Modules are missing or built with another version" error:**

If you see this error message when loading the plugin in UE 5.4.3:
1. Delete any `Binaries` and `Intermediate` folders in the plugin directory if they exist
2. When prompted by the editor, click "Yes" to rebuild the modules
3. The plugin will automatically compile for your engine version

This error typically occurs when:
- The plugin was previously compiled for a different engine version
- Pre-built binaries exist that don't match your engine version
- The editor needs to rebuild the plugin for the first time

The plugin source code is fully compatible with UE 5.4.3 and will compile successfully when rebuilt.

### Video tutorials

- https://www.youtube.com/watch?v=zGpPd4RAneQ&list=PLCWhWOgVmdwkOBGElYEPumnCXL12zEtwE

### How to use

- Clone this repo to <u>project/plugins/RoadBuilder</u> folder
- Extract [Content.zip](https://drive.google.com/file/d/1Es8YJwpEvMFxoh7SPojt0weMluLXaYZE/view?usp=sharing) to <u>project/plugins/RoadBuilder/Content</u> folder