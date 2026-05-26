# Unreal Engine 5 C++ Mesh Dumper Tool

A C++ Editor Utility Actor for Unreal Engine 5 designed to bulk-spawn and organize static meshes from any project directory into a neat, customizable grid layout in the world. Perfect for **asset auditing, overview map generation, and quick visual previews**.

---


### Editor Details Panel
Configuring grid spacing, directories, alignment options, and triggering spawning via Call-in-Editor buttons

---

## ✨ Features
- 🎛️ **Exposed Editor Actions:** Spawning (`SpawnMeshes`) and cleaning up (`ClearSpawnedMeshes`) are bound to Details Panel buttons for an interactive, blueprint-free editor workflow.
- 📐 **Automatic Floor Alignment:** Computes the bounding box of each static mesh (accounting for any scaling) to place the base of the mesh flush with the dumper's world height—no floating or buried props.
- 🔍 **Dynamic Rescaling:** Optional scaling constraint (`bScaleToMinimumSize`) that dynamically scales up smaller props to a minimum dimension threshold, making tiny assets more viewable alongside large other assets.
- ⚙️ **Fully Customizable Grid:** Control the exact spacing (`Spacing` X and Y), actors per row (`MeshesPerRow`), and customized prefixes (`PrefixToAdd`) for clean Outliner organization.
- 🧹 **Robust Cleanup:** Tracked actors are safely cleared when re-spawning or via the manual cleanup button to prevent cluttering your level.

---

## 🚀 How to Use

1. **Add the Actor to your Scene:**
   Drag and drop the **Mesh Dumper Tool** (`ACpp_EditorTool_MeshDumper`) into your level viewport. Set its position to where you want the grid to start spawning (it spawns relative to the Dumper's location and rotation).

2. **Configure the Properties:**
   Select the Actor and navigate to the **Mesh Dumper** section in the **Details Panel**:
   - **Target Directory:** Click the folder icon or paste the path to scan (e.g., `/Game/Artists/Environment/Props`).
   - **Meshes Per Row:** The maximum number of models placed in a single row before starting a new one.
   - **Spacing (X, Y):** The grid spacing interval in Unreal Units between each model.
   - **Prefix To Add:** Customize the spawned actor names in the World Outliner (defaults to `Dumped_`).
   - **Align To Floor:** Keep this enabled to ensure all meshes sit perfectly on the dumper's plane.
   - **Scale To Minimum Size:** Enable this if you are auditing mixed-size asset packages and want to scale smaller props up to `MinDimensionSize` for better visibility.

3. **Spawn Meshes:**
   Click the **Spawn Meshes** button in the Details Panel. The tool will synchronously scan the directory, process bounds, and instantly instantiate the static mesh actors in the editor.

4. **Clear Spawned Meshes:**
   Click the **Clear Spawned Meshes** button to safely destroy and unregister all spawned actors from the level.

---

## 🛠️ Configuration Details

| Property | Type | Default | Description |
| :--- | :--- | :--- | :--- |
| `TargetDirectory` | `FDirectoryPath` | `/Game/` | The virtual path in your project's Content Browser to scan for static meshes. |
| `MeshesPerRow` | `int32` | `10` | The column count limit for the spawned grid. |
| `Spacing` | `FVector2D` | `(200.0, 200.0)` | The gap distance (X, Y) between the center point of each grid cell. |
| `PrefixToAdd` | `FString` | `"Dumped_"` | A custom prefix applied to spawned actor labels in the Outliner for easy tracking. |
| `bAlignToFloor` | `bool` | `true` | When true, offsets the mesh along the Z axis based on its bounding box to rest on the dumper's surface. |
| `bScaleToMinimumSize` | `bool` | `false` | Enables scaling up small assets so they are at least `MinDimensionSize` on their largest axis. |
| `MinDimensionSize` | `float` | `100.0` | The minimum bounding dimension threshold applied when scaling is active. |

