# RoadBuilder Plugin — Complete Tutorial

## Table of Contents
1. [Entering Road Mode](#1-entering-road-mode)
2. [Creating Your First Road](#2-creating-your-first-road)
3. [Understanding the Asset Types](#3-understanding-the-asset-types)
4. [Road Styles (URoadStyle)](#4-road-styles-uroadstyle)
5. [Lane Shapes (ULaneShape)](#5-lane-shapes-ulaneshape)
6. [Lane Mark Styles (ULaneMarkStyle)](#6-lane-mark-styles-ulanemark style)
7. [Road Props (URoadProps)](#7-road-props-uroadprops)
8. [Editor Tab Reference](#8-editor-tab-reference)
9. [Step-by-Step: Build a 4-Lane Street with Sidewalks](#9-step-by-step-build-a-4-lane-street-with-sidewalks)
10. [Junctions](#10-junctions)
11. [Custom Markings](#11-custom-markings)
12. [Ground Patches](#12-ground-patches)
13. [Settings Tab](#13-settings-tab)

---

## 1. Entering Road Mode

1. Open your UE 5.7 project with the RoadBuilder plugin enabled.
2. In the top-left toolbar, click the **Modes** dropdown (the selection tool icon).
3. Select **Road** from the list.
4. The Road mode panel appears on the left side with **7 tabs** across the top:

| Tab | Purpose |
|-----|---------|
| **File** | Export (OpenDRIVE .xodr) |
| **Road** | Place and edit road spline points |
| **Junction** | Edit junction connections between roads |
| **Lane** | Edit individual lane properties |
| **Marking** | Add road markings (lines, arrows, symbols) |
| **Ground** | Edit ground patches between roads |
| **Settings** | Global build settings |

When you first enter Road mode, a `RoadScene` actor is automatically spawned in your level. This is the parent container for all roads, junctions, and grounds.

---

## 2. Creating Your First Road

### Set Up the Road Style
1. Click the **Road** tab.
2. In the **Settings panel** below the toolbar, you'll see:
   - **Base Height** — the Z-height of the road plane (default: 0)
   - **Style** — a `URoadStyle` asset reference

3. Set the **Style** to one of the built-in styles. To find them in the Content Browser:
- Click the **Settings** (gear icon) in the top-right of the Content Browser
- Check **Show Plugin Content**
- Navigate to `RoadBuilder Content / RoadStyles`
   
Available styles:
   - `Street-Main-4-Lanes` — 4-lane urban road with sidewalks
   - `Street-Main-6-Lanes` — 6-lane urban road with sidewalks
   - `Highway-Main-4-Lanes` — 4-lane highway (no sidewalks)
   - `Elevated-Main-4-Lanes` — elevated road with barriers
   - `Street-Ramp-1-Lanes` — single-lane on/off ramp
   - etc.

### Place Road Points
1. Select the **Plan** sub-tool (first button in the Road tab toolbar).
2. **Right-click** in the viewport to place road control points. Each right-click adds a new point.
   - The first right-click creates a new `RoadActor` and places the first point.
   - Subsequent right-clicks extend the road with additional points.
   - The road mesh is procedurally generated through all points as a smooth spline.
3. **Left-click** on a road to select it, or left-click on empty space to deselect.
4. **Left-click** on a control point (red dot) to select it and see its properties in the panel:
   - **Pos** — 2D position (X, Y)
   - **MaxRadius** — maximum curvature radius (higher = gentler curves)
   - **CurvatureBlend** — blending factor for curvature transitions
5. **Drag** a selected control point's gizmo (XY axes) to move it.
6. Press **Delete** with a point selected to remove it. Press **Delete** with no point selected (but road selected) to delete the entire road.
7. Press **Escape** to deselect.

### Adjust Road Height
1. Click the **Height** sub-tool (second button in Road tab).
2. Right-click on a road to add height control points.
3. Select a height point to edit:
   - **Dist** — distance along the road
   - **Height** — Z elevation at this point
   - **Range** — blending range for smooth height transitions
4. This creates bridges, overpasses, slopes, etc.

### Other Road Sub-tools
- **Chop** — Split a road at a point into two separate roads.
- **Split** — Split a road laterally along a boundary line (e.g., split a 4-lane road into two 2-lane roads).

---

## 3. Understanding the Asset Types

RoadBuilder uses **4 custom asset types** that you create and configure in the Content Browser. Each controls a different aspect of road appearance:

```
URoadStyle          — "What lanes does the road have?"
  ??? FRoadLaneStyle
       ??? ULaneShape      — "What does each lane look like in cross-section?"
       ??? ULaneMarkStyle  — "What line marking sits on each lane boundary?"
       ??? URoadProps      — "What objects are placed along each lane boundary?"
```

To create any of these assets:
1. **Right-click** in the Content Browser.
2. Under the **RoadBuilder** category, select the asset type.
3. **Double-click** the new asset to open and edit it.

---

## 4. Road Styles (URoadStyle)

A `URoadStyle` defines the **full lane layout** of a road. It's the first thing you pick when placing a road.

### Properties

| Property | Description |
|----------|-------------|
| **LeftLanes** | Array of lanes on the left side (oncoming traffic) |
| **RightLanes** | Array of lanes on the right side (forward traffic) |
| **BaseCurveMark** | `ULaneMarkStyle` for the center line marking |
| **BaseCurveProps** | `URoadProps` for objects placed along the center line |
| **bHasGround** | Whether to auto-generate ground patches along road edges |

### Each Lane Entry (FRoadLaneStyle)

| Field | Description |
|-------|-------------|
| **Width** | Lane width in cm (typical: 350 for driving, 200 for sidewalk) |
| **LaneType** | `Driving`, `Shoulder`, `Sidewalk`, `Median`, or `None` |
| **LaneShape** | `ULaneShape` asset — defines the 3D cross-section geometry |
| **LaneMarking** | `ULaneMarkStyle` asset — the line marking on this lane's outer boundary |
| **Props** | `URoadProps` asset — objects placed along this lane's outer boundary |

### Example: 4-Lane Street with Sidewalks

```
LeftLanes:
  [0] Width=200,  Type=Sidewalk, Shape=Sidewalk,  Marking=None,        Props=None
  [1] Width=350,  Type=Driving,  Shape=Driving,   Marking=WhiteSolid,  Props=None
  [2] Width=350,  Type=Driving,  Shape=Driving,   Marking=WhiteDash,   Props=None

BaseCurveMark: YellowSolidSolid

RightLanes:
  [0] Width=350,  Type=Driving,  Shape=Driving,   Marking=WhiteDash,   Props=None
  [1] Width=350,  Type=Driving,  Shape=Driving,   Marking=WhiteSolid,  Props=None
  [2] Width=200,  Type=Sidewalk, Shape=Sidewalk,  Marking=None,        Props=None
```

Lanes are ordered **from center outward**. The BaseCurveMark is the center dividing line. Each lane's LaneMarking goes on its **outer** boundary edge.

### Alternative: Create Style from Existing Road
Instead of manually configuring a `URoadStyle`, you can:
1. Build a road manually (edit lanes with the Lane tab).
2. In the **Road > Plan** tool, click the **Create** button in the settings panel.
3. This saves the current road's lane layout as a reusable `URoadStyle` asset.

---

## 5. Lane Shapes (ULaneShape)

A `ULaneShape` defines the **3D cross-section profile** of a lane. Think of it as extruding a 2D shape along the road spline.

### Properties

| Property | Description |
|----------|-------------|
| **CrossSections** | Array of `FLaneCrossSection` — each is one material strip |

### Each Cross-Section (FLaneCrossSection)

| Field | Description |
|-------|-------------|
| **Material** | `UMaterialInterface` — the material applied to this strip |
| **Alignment** | How the strip aligns: `Up`, `Down`, `Right`, or `Left` |
| **UVScale** | UV tiling scale (X=along road, Y=across road) |
| **Points** | Array of `FVector2D` — the 2D profile points (X=horizontal, Y=vertical) relative to lane boundaries |
| **ClampZ** | If true, clamps Z values to prevent geometry going underground |

### Built-in Lane Shapes

| Asset | Description |
|-------|-------------|
| **Driving** | Flat road surface — single cross-section: a flat quad from left boundary to right boundary |
| **Driving-Elevated** | Elevated road with barrier walls on edges |
| **Sidewalk** | Raised sidewalk with a curb step. Has 3 cross-sections: top surface, curb face, gutter |
| **Median** | Center median strip |
| **Fade-Elevated** | Transition from ground to elevated |
| **Fade-Tunnel** | Transition into a tunnel |

### How to Make a Custom Lane Shape

Example: A **basic flat driving lane**
```
CrossSections:
  [0]
    Material: YourAsphaltMaterial
    Alignment: Up
    UVScale: (1.0, 1.0)
    Points: [(0, 0), (1, 0)]    // Left edge to right edge, flat
    ClampZ: false
```

Example: A **sidewalk with curb**
```
CrossSections:
  [0] Top surface
    Material: ConcreteMaterial
    Alignment: Up
    Points: [(0, 0.15), (1, 0.15)]    // Flat, raised 15cm

  [1] Curb face (vertical)
    Material: CurbMaterial
    Alignment: Up
    Points: [(0, 0), (0, 0.15)]       // Vertical face at inner edge

  [2] Gutter
    Material: AsphaltMaterial
    Alignment: Up
    Points: [(-0.05, 0), (0, 0)]      // Small slope down to road level
```

The Points use **normalized coordinates**: X=0 is the left boundary of the lane, X=1 is the right boundary. Y is vertical height in meters (multiplied by lane width).

---

## 6. Lane Mark Styles (ULaneMarkStyle)

A `ULaneMarkStyle` controls the **painted line markings** between lanes.

### Properties

| Property | Description |
|----------|-------------|
| **Material** | The line material (e.g., white paint, yellow paint) |
| **MarkType** | Pattern type (see below) |
| **Width** | Line width in cm (default: 12.5) |
| **Separation** | Gap between double lines in cm (default: 12.5) |
| **DashLength** | Length of each dash in cm (default: 150) |
| **DashSpacing** | Gap between dashes in cm (default: 150) |

### Mark Types

| Type | Pattern | Typical Use |
|------|---------|-------------|
| `Dash` | `- - - -` | Lane change allowed |
| `Solid` | `_______` | Lane change prohibited |
| `DashDash` | `= = = =` | Double dashed |
| `DashSolid` | `- ____` | Change allowed from dash side only |
| `SolidDash` | `____ -` | Change allowed from dash side only |
| `SolidSolid` | `??????` | No crossing (center line) |

### Built-in Mark Styles (in `Content/MarkStyles/LaneMark/`)

| Asset | Material | Type |
|-------|----------|------|
| WhiteDash | White | Dash |
| WhiteSolid | White | Solid |
| WhiteDashDash | White | DashDash |
| WhiteSolidSolid | White | SolidSolid |
| WhiteDashSolid | White | DashSolid |
| WhiteSolidDash | White | SolidDash |
| YellowDash | Yellow | Dash |
| YellowSolid | Yellow | Solid |
| YellowSolidSolid | Yellow | SolidSolid |
| ... | ... | ... |

### Other Marking Styles

- **UCrosswalkStyle** — For crosswalk markings. Properties: `Width`, `BorderWidth`, `DashLength`, `DashGap`.
- **UPolygonMarkStyle** — For filled region markings (gore areas). Types: `Solid`, `Striped`, `Crosshatch`, `Chevron`. Found in `Content/MarkStyles/PolygonMark/`.

---

## 7. Road Props (URoadProps)

A `URoadProps` asset places **repeated objects** (static meshes or blueprints) along a road boundary.

### Properties: Array of FRoadProp

| Field | Description |
|-------|-------------|
| **Assets** | Array of `UObject*` — meshes or blueprints to randomly pick from |
| **Offset** | XYZ offset from the boundary line (X=along road, Y=perpendicular, Z=up) |
| **Scale** | Base scale |
| **Rotation** | Base rotation |
| **RandomOffset** | Random variation range for offset |
| **RandomScale** | Random variation range for scale |
| **RandomRotation** | Random variation range for rotation |
| **Spacing** | Distance between instances in cm (e.g., 500 = one every 5 meters) |
| **Start** | Normalized start position (0 = road start, 1 = road end) |
| **End** | Normalized end position |
| **Fill** | If true, fills the entire spacing range instead of single placement |
| **Select** | Selection filter (place every Nth instance) |
| **Base** | Base offset for selection filter |
| **Of** | Divisor for selection filter |

### Example: Street Lights Every 20m
```
Props:
  [0]
    Assets: [StreetLight_Mesh]
    Offset: (0, 50, 0)          // 50cm to the side of the boundary
    Scale: (1, 1, 1)
    Rotation: (0, 0, 0)
    Spacing: 2000                // Every 20 meters
    Start: 0
    End: 1
```

### Example: Guardrails with Random Rotation
```
Props:
  [0]
    Assets: [Guardrail_A, Guardrail_B]    // Randomly picks A or B
    Offset: (0, 0, 0)
    Scale: (1, 1, 1)
    RandomRotation: (0, 5, 0)             // ±2.5° yaw variation
    Spacing: 400                           // Every 4 meters
```

### Built-in Props (in `Content/RoadProps/`)
Pre-made prop sets for common road furniture (varies by content).

---

## 8. Editor Tab Reference

### Road Tab — Sub-tools
| Tool | Description | Controls |
|------|-------------|----------|
| **Plan** | Place/move road control points | RMB=add point, LMB=select, Drag=move, Del=delete |
| **Height** | Add elevation changes | RMB=add height point, edit Dist/Height/Range |
| **Chop** | Cut a road at a point | Click on road to split into two |
| **Split** | Split road laterally along boundary | Click a boundary line to split |

### Junction Tab
| Tool | Description |
|------|-------------|
| **Link** | Click junction connections to select/edit turn radius |

### Lane Tab
| Tool | Description |
|------|-------------|
| **Edit** | Click lane segments to change LaneType, LaneShape |
| **Carve** | Click on a lane to add a segment division point |
| **Width** | Click lane boundaries to add width control points |

### Marking Tab
| Tool | Description |
|------|-------------|
| **Lane** | Click boundaries to change LaneMarkStyle |
| **Point** | Place a static mesh marking at a UV coordinate on the road |
| **Curve** | Draw bezier curve markings (arrows, symbols, custom lines) |

### Ground Tab
| Tool | Description |
|------|-------------|
| **Edit** | Select/move ground patch control points |

---

## 9. Step-by-Step: Build a 4-Lane Street with Sidewalks

### Step 1: Enter Road Mode
- Modes dropdown ? **Road**

### Step 2: Select a Road Style
- Click the **Road** tab
- In the panel, set **Style** to `Street-Main-4-Lanes`
- Set **Base Height** to `0`

### Step 3: Draw the Road
- Select the **Plan** tool
- **Right-click** in the viewport to place the first point
- **Right-click** at another location to place the second point — a road appears
- **Right-click** more to add curves and extend the road
- The road auto-generates: asphalt surface, lane markings, sidewalks with curbs

### Step 4: Draw a Second Road (for an intersection)
- **Left-click** on empty space to deselect the current road
- **Right-click** to start placing a new road that crosses the first
- When two roads overlap, a **Junction** is automatically created

### Step 5: Adjust Junction
- Click the **Junction** tab ? **Link** tool
- Click on junction link curves (the connecting roads inside the intersection)
- Edit the **Radius** property to adjust turn sharpness

### Step 6: Fine-Tune Lanes
- Click the **Lane** tab ? **Edit** tool
- Click on any lane segment to view/change its properties:
  - **LaneType**: Change between Driving, Sidewalk, Shoulder, etc.
  - **LaneShape**: Change the cross-section geometry
- Use **Carve** to split a lane into segments with different properties
- Use **Width** to add width variation points along a lane

### Step 7: Add Markings
- Click **Marking** tab ? **Lane** tool
- Click on boundary lines to change the marking style
- Use **Curve** tool to draw custom markings (turn arrows, text, etc.)
- Use **Point** tool to place mesh objects on the road surface

### Step 8: Adjust Settings
- Click the **Settings** tab
- Configure:
  - **DefaultDrivingShape** / **DefaultSidewalkShape** — default shapes for new lanes
  - **DefaultDashStyle** / **DefaultSolidStyle** — default markings
  - **BuildJunctions** — enable/disable automatic junction generation
  - **BuildProps** — enable/disable prop generation
  - **BuildMassGraph** — enable ZoneGraph generation for MassTraffic
  - **UVScale** — texture tiling scale

---

## 10. Junctions

Junctions are **auto-generated** when roads intersect. The system:
1. Detects where road boundaries cross
2. Creates a junction actor with **gates** (connection points)
3. Generates **link roads** for each possible turn
4. Fills the junction interior with matching road surface
5. Generates gore markings at merge/diverge points

### Turn Restrictions
Select a junction actor in the World Outliner and edit `TurnRestrictions` in the Details panel:
- **FromGateIndex** — which approach is restricted
- **ToGateIndex** — which exit is blocked
Restricted turns will not generate link geometry.

---

## 11. Custom Markings

### Marking Curves (UMarkingCurve)
1. **Marking** tab ? **Curve** tool
2. Right-click on a road surface to place bezier control points
3. Points use **UV coordinates**: U = distance along road, V = lateral offset
4. Set **MarkStyle** to choose the line style
5. For closed shapes (like turn arrows): enable **bClosedLoop** and set a **FillStyle** (polygon marking)

### Marking Points (UMarkingPoint)
1. **Marking** tab ? **Point** tool
2. Right-click on the road to place a mesh at that position
3. Set the **Mesh** property to any static mesh (manhole cover, road stud, etc.)

---

## 12. Ground Patches

Ground patches fill the space between roads (medians, islands, grass areas).

- Many ground patches are **auto-generated** when `bHasGround` is enabled on the road style
- Manual ground editing: **Ground** tab ? **Edit** tool
- Each ground point can be pinned to a road boundary or placed freely
- Set the ground material in **Settings** tab ? **DefaultGroundMaterial**

---

## 13. Settings Tab

The Settings tab exposes `USettings_Global`:

| Setting | Description |
|---------|-------------|
| **DefaultDrivingShape** | Default `ULaneShape` for new driving lanes |
| **DefaultMedianShape** | Default shape for median lanes |
| **DefaultSidewalkShape** | Default shape for sidewalk lanes |
| **DefaultDashStyle** | Default `ULaneMarkStyle` for dashed markings |
| **DefaultSolidStyle** | Default style for solid markings |
| **DefaultGroundMaterial** | Material for auto-generated ground patches |
| **DefaultGoreMarking** | Polygon mark style for gore areas |
| **UVScale** | Global UV texture scale (default: 0.001) |
| **BuildJunctions** | Auto-build junctions on rebuild |
| **BuildProps** | Generate road props on rebuild |
| **BuildMassGraph** | Generate ZoneGraph shapes for MassTraffic AI |
| **NoTurnSignMesh** | Static mesh for no-turn signs (shown at restricted turns) |

---

## Quick Reference: Keyboard Shortcuts

| Key | Action |
|-----|--------|
| **Right Mouse Button** | Add/place point (context-dependent) |
| **Left Mouse Button** | Select road/point/lane/marking |
| **Delete** | Delete selected point, or entire road if no point selected |
| **Escape** | Deselect / go up one level |
| **Ctrl+Z** | Undo |
| **Ctrl+Y** | Redo |
| **Drag gizmo** | Move selected point |
