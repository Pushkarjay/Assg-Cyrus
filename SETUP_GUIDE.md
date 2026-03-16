# Setup Guide - Complete Instructions

This document provides step-by-step instructions to build and run the Cyrus Game project.

## Prerequisites

### Required Software
- **Unreal Engine 5.6** - [Download from Epic Games](https://www.epicgames.com/unrealengine)
- **Visual Studio 2022** (Community Edition is free) - [Download](https://visualstudio.microsoft.com/downloads/)
- **Git** (for version control) - [Download](https://git-scm.com/)
- **Windows 10/11** (or Mac/Linux for cross-platform support)

### Recommended Hardware
- **Processor**: Intel i7 or equivalent (6+ cores)
- **RAM**: 16 GB minimum
- **Storage**: 100 GB available space (for engine + project)
- **GPU**: NVIDIA GTX 1060 or equivalent

## Installation Steps

### Step 1: Install Unreal Engine 5.6

1. Open **Epic Games Launcher**
2. Click "Unreal Engine" in the left sidebar
3. Click "Library"
4. Click "Install Engine"
5. Select version **5.6** (or latest)
6. Choose installation location (requires ~100 GB)
7. Complete installation (this may take 30-60 minutes)

### Step 2: Install Visual Studio 2022

1. Open **Visual Studio Installer**
2. Click "Install"
3. Select Workloads:
   - ✅ Desktop development with C++
   - ✅ Game development with C++
4. Click "Install"
5. Wait for completion

### Step 3: Clone or Extract the Project

The project is located at: `e:\Projects\Working\Assg Cyrus\`

If using Git:
```batch
cd e:\Projects\Working
git clone <repository-url> Assg_Cyrus
cd Assg_Cyrus
```

### Step 4: Generate Visual Studio Project Files

1. Locate `CyrusGame.uproject` in the project folder
2. Right-click on `CyrusGame.uproject`
3. Select **"Generate Visual Studio project files"**
4. Wait for generation to complete

Expected files created:
- `CyrusGame.sln` (Visual Studio Solution)
- `Binaries/` folder
- `Intermediate/` folder

### Step 5: Compile C++ Code

**Option A: Using Unreal Editor (Recommended for first-time)**

1. Double-click `CyrusGame.uproject`
2. Select C++ project when prompted
3. Click "Yes" to rebuild binaries
4. Wait for compilation (5-15 minutes depending on system)
5. Unreal Editor will launch when complete

**Option B: Using Visual Studio**

1. Open `CyrusGame.sln` in Visual Studio
2. Set build configuration to **"Development Editor | Win64"** (top toolbar)
3. Right-click "CyrusGame" in Solution Explorer
4. Select "Build"
5. Wait for compilation to complete
6. Return to Unreal Editor and refresh

### Step 6: Create Required Blueprint Classes

Once Unreal Editor is open:

#### 6a: Create Game Manager Blueprint
1. In Content Browser, navigate to **Content > Blueprints**
2. Right-click > **Blueprint Class**
3. Search for `AGameManager`
4. Select it and click "Create"
5. Rename to `BP_GameManager`
6. Click "Compile" and "Save"

#### 6b: Create Score Widget Blueprint
1. Right-click in **Content > Blueprints**
2. Select **Widget Blueprint**
3. Search for `UScoreWidget`
4. Select it and click "Create"
5. Rename to `W_ScoreWidget`

**Now design the widget layout**:
- In the Designer view, drag a TextBlock from the Palette
- Rename it to `ScoreText`
- Set text to "Score: 0"
- Add another TextBlock named `BoxCountText`
- Set anchors and positions as desired

6. Click "Compile" and "Save"

#### 6c: Create the Main Game Level
1. File > **New Level**
2. Choose **Empty Level** or **Basic**
3. In the Outliner, click **Create** button
4. Search for `WeaponSystem` and add it
5. In Details, set Transform:
   - Location: (0, 0, 100)
   - Rotation: (0, 0, 0)
6. Add another actor: Search for `BP_GameManager`
7. Add it to the level
8. File > **Save As**
9. Navigate to **Content/Maps**
10. Name it `MainMap` and save

### Step 7: Configure Project Settings

1. Edit > **Project Settings**
2. Search for **"Default Game Mode"**
3. Set `Default Game Mode Override` to `BP_GameManager`

4. Search for **"Default Character"**
5. Set `Default Pawn Class` to `AWeaponSystem_C`

6. Close Project Settings

### Step 8: Verify Configuration

1. Window > **World Settings**
2. Verify:
   - **Game Mode Override**: `BP_GameManager`
   - **Game State**:  (auto-selected)

3. Close World Settings

### Step 9: Test the Project

1. Click **Play** button in the Unreal Editor toolbar
2. You should see:
   - First-person camera perspective
   - Boxes spawning in the world (from JSON data)
   - Score display in top-left corner
3. Test controls:
   - **WASD**: Move
   - **Mouse**: Look around
   - **Left Click**: Shoot
   - **Right Click**: Aim (zoom in)

Expected behavior:
- Boxes appear with different colors
- Shooting boxes reduces their health
- Box disappears when health reaches 0
- Score increases each time a box is destroyed

### Step 10: Compile and Package

Once satisfied with testing:

1. Click **Compile**
2. Click **Refresh**
3. File > **Cook Content for Windows**
4. Wait for cooking process to complete

## Troubleshooting

### Issue: "Cannot find Unreal Engine"
**Solution**:
- Ensure UE 5.6 is installed at default Epic Games location
- Right-click `.uproject` > "Associate with Unreal Engine 5.6"
- Regenerate Visual Studio files

### Issue: Compilation Errors (C++ Syntax)
**Solution**:
- Verify Visual Studio has C++ development tools installed
- Close Unreal Editor
- Delete `Intermediate/` and `Binaries/` folders
- Right-click `.uproject` > "Generate Visual Studio project files"
- Rebuild in Visual Studio

### Issue: "Module 'HTTP' failed to load"
**Solution**:
- Module is built-in, verify `.Build.cs` file includes:
  ```cpp
  PublicDependencyModuleNames.AddRange(new string[] { "HTTP", "Json", "JsonUtilities" });
  ```
- Recompile project

### Issue: Boxes Not Spawning
**Solution**:
- Check JSON URL is accessible: https://raw.githubusercontent.com/CyrusCHAU/Varadise-Technical-Test/refs/heads/main/data.json
- Verify `JsonDataManager` is fetching data (check Output Log)
- Ensure `BP_GameManager` exists and is set as Game Mode
- Check `AGameBox` class is compiled correctly

### Issue: Shooting Doesn't Damage Boxes
**Solution**:
- Verify `WeaponSystem` is the default pawn
- Check line trace is hitting boxes (debug lines should appear)
- Ensure boxes have `UBoxComponent` with collision enabled
- Check damage amount is > 0 in `WeaponSystem`

### Issue: UI Score Not Updating
**Solution**:
- Verify `W_ScoreWidget` has `ScoreText` TextBlock
- Check widget is added to viewport in `GameManager`
- Ensure delegates are bound correctly
- Check Output Log for widget creation errors

## Post-Setup Verification Checklist

- [ ] Unreal Engine 5.6 installed and running
- [ ] C++ project compiles without errors
- [ ] Visual Studio command line tools working
- [ ] Blueprints created: `BP_GameManager`, `W_ScoreWidget`
- [ ] Main level created and configured
- [ ] Game mode set to `BP_GameManager`
- [ ] Default pawn set to `AWeaponSystem_C`
- [ ] Game launches in Play mode
- [ ] Boxes spawn from JSON data
- [ ] Shooting damages boxes
- [ ] Score updates on screen
- [ ] Game runs smoothly (60 FPS+)

## Building for Distribution

To package the game for others:

1. File > **Package Project**
2. Select **Windows (64-bit)**
3. Choose output directory
4. Wait for packaging (5-10 minutes)
5. Packaged project will be in: `[Output]/CyrusGame/Binaries/Win64/`

To run packaged version:
```bash
cd [Output]/CyrusGame/Binaries/Win64
CyrusGame.exe
```

## Next Steps

1. **Play the game**: Shoot boxes, break records
2. **Record gameplay**: Use OBS or Nvidia Share for video
3. **Upload to YouTube**: Set video to public view
4. **Push to GitHub**: Commit all project files
5. **Submit assignment**: Email GitHub link to hiring@cyrus365.com

---

**Support**: Refer to [README.md](README.md) for more technical details
**Questions**: Contact Cyrus 365 hiring team

