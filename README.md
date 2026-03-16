# Cyrus Game - FPS with JSON-Based Box Spawning

A professional-grade Unreal Engine 5.6 First-Person Shooter game featuring dynamic JSON data loading, interactive damageable boxes, custom materials with dynamic colors, and a scoring system. This project demonstrates advanced C++ gameplay programming, networking capabilities, and UI implementation in Unreal Engine.

## Project Overview

**Assignment**: Game Developer (Unreal Engine) Technical Test - Cyrus 365  
**Engine**: Unreal Engine 5.6 (or latest)  
**Language**: C++ (with Blueprint extensions)  
**Key Features**:
- Dynamic JSON data fetching from GitHub
- Real-time box spawning with correct properties (color, health, score)
- Interactive shooting mechanics with line-trace detection
- Dynamic material system with color control
- Score tracking and UI display
- Feature-based folder structure with clean code organization

## Technical Implementation

### Architecture Overview

```
CyrusGame/
├── Source/CyrusGame/
│   ├── Public/
│   │   ├── Data/          # JSON data structures and management
│   │   │   └── JsonDataManager.h
│   │   ├── Features/      # Core gameplay features
│   │   │   ├── GameBox.h
│   │   │   ├── GameManager.h
│   │   │   └── WeaponSystem.h
│   │   └── UI/            # User interface widgets
│   │       └── ScoreWidget.h
│   ├── Private/           # Implementation files
│   └── CyrusGame.Build.cs # Module build configuration
├── Content/
│   ├── Maps/              # Game levels
│   ├── Materials/         # Material assets
│   └── Blueprints/        # Blueprint classes and widgets
└── CyrusGame.uproject     # Project file
```

### Core Systems

#### 1. **JSON Data Manager** (`JsonDataManager.h/cpp`)
- **Functionality**: Handles HTTP requests to GitHub repository
- **Method**: Uses native Unreal Engine HTTP module (no external libraries)
- **Features**:
  - Asynchronous HTTP GET request
  - JSON parsing with error handling
  - Data caching in memory
  - Delegate-based event system for success/failure

**Key Methods**:
```cpp
void FetchJsonData()                    // Initiates JSON download
bool ParseJsonData(...)                 // Parses JSON string
FVector ConvertRgbToVector(...)         // Normalizes RGB values
```

**Data Structures**:
```cpp
FBoxTypeData          // Box type definition (color, health, score)
FSpawnableObjectData  // Object spawn information (location, rotation, scale)
FGameData             // Complete game data container
```

#### 2. **Game Box Actor** (`GameBox.h/cpp`)
- **Functionality**: Represents a damageable, interactive box in the world
- **Components**:
  - `UBoxComponent` - Collision and hit detection
  - `UStaticMeshComponent` - Visual representation
  - `UMaterialInstanceDynamic` - Dynamic color control

**Key Features**:
- Health system with damage application
- Automatic destruction on health depletion
- Dynamic material color application based on JSON data
- Score value assignment
- Event broadcasting for destruction

**Key Methods**:
```cpp
void InitializeBox(...)         // Set up box from JSON data
void TakeDamage(int32 Amount)   // Apply damage
void ApplyBoxColor()            // Set material color
void DestroyBox()               // Handle destruction
```

#### 3. **Game Manager** (`GameManager.h/cpp`)
- **Functionality**: Core game logic and orchestration
- **Responsibilities**:
  - JSON data loading coordination
  - Box spawning and management
  - Score accumulation
  - UI updates

**Game Flow**:
1. On BeginPlay: Initialize JSON data manager
2. On JSON loaded: Parse data and spawn boxes
3. On box destroyed: Add score and update UI
4. Display score in real-time

#### 4. **Weapon System** (`WeaponSystem.h/cpp`)
- **Functionality**: First-person shooting mechanics
- **Features**:
  - Enhanced Input System integration
  - Line trace-based bullet detection
  - Configurable fire rate and damage
  - Multiple weapon actions (fire, aim)

**Shooting Mechanism**:
- Uses `LineTraceSingleByChannel` for hit detection
- Performs world collision query from camera position
- Applies damage to hit GameBox actors
- Debug visualization (lines and spheres)

#### 5. **Score Widget** (`ScoreWidget.h/cpp`)
- **Functionality**: UI for score and game status display
- **Features**:
  - Real-time score updates
  - Box count display
  - Destruction feedback animation
  - C++ base class with Blueprint extension support

## Data Format (JSON)

The game fetches JSON data from: `https://raw.githubusercontent.com/CyrusCHAU/Varadise-Technical-Test/refs/heads/main/data.json`

**Expected JSON Structure**:
```json
{
  "types": [
    {
      "name": "BoxType1",
      "color": [255, 0, 0],
      "health": 10,
      "score": 100
    }
  ],
  "objects": [
    {
      "type": "BoxType1",
      "transform": {
        "location": [100, 200, 300],
        "rotation": [0, 0, 0],
        "scale": [1, 1, 1]
      }
    }
  ]
}
```

## Setup and Build Instructions

### Prerequisites
- **Unreal Engine 5.6** (or latest version)
- **Visual Studio 2022** (or 2019 with latest updates)
- **Windows 10/11** (or Mac/Linux for cross-platform)
- **Git** (for version control)

### Step 1: Install Unreal Engine 5.6

1. Download Epic Games Launcher
2. Install Unreal Engine 5.6
3. Accept default installation options

### Step 2: Clone/Extract Project

```bash
cd "e:\Projects\Working\"
# Project is already in "Assg Cyrus" directory
```

### Step 3: Generate Visual Studio Project Files

1. Right-click `CyrusGame.uproject`
2. Select "Generate Visual Studio project files"
3. Wait for completion

### Step 4: Compile C++ Code

**Option A: Using Unreal Editor**
1. Open CyrusGame.uproject
2. Click "Yes" when prompted to rebuild binaries
3. Wait for compilation to complete

**Option B: Using Visual Studio**
1. Open `CyrusGame.sln` in Visual Studio
2. Set build configuration to "Development Editor" (Win64)
3. Press F5 or Build > Build Solution
4. Wait for compilation

### Step 5: Create Blueprint Assets

#### Create Game Manager Blueprint
1. In Unreal Editor, navigate to Content > Blueprints
2. Create new Blueprint based on `AGameManager`
3. Name it `BP_GameManager`
4. Compile and save

#### Create Score Widget Blueprint
1. In Content > Blueprints, create new Widget Blueprint
2. Select `UScoreWidget` as parent class
3. Name it `W_ScoreWidget`
4. Design the UI:
   - Add TextBlock named `ScoreText`
   - Add TextBlock named `BoxCountText`
   - Add TextBlock named `FeedbackText`
5. Compile and save

#### Create Game Level
1. File > New Level (Basic)
2. Add `AWeaponSystem` character to the level
3. Add `BP_GameManager` to level (set as GameMode)
4. Set player start location
5. Save as `/Game/Maps/MainMap`

### Step 6: Configure Project Settings

1. Edit > Project Settings
2. Search "Default Character"
3. Set `Default Pawn Class` to `AWeaponSystem`
4. Search "Game Mode"
5. Set `Default Game Mode` to `BP_GameManager`

### Step 7: Compile and Run

1. Click "Compile" in Unreal Editor
2. Click "Play" to start game
3. Use mouse to look around
4. WASD to move
5. Left Mouse Button to shoot
6. Right Mouse Button to aim

## Gameplay Instructions

### Controls
- **WASD**: Move character
- **Mouse**: Look around
- **Left Mouse Button**: Fire/Shoot
- **Right Mouse Button**: Aim down sights
- **ESC**: Pause/Menu (not implemented in this version)

### Objective
- Shoot boxes to destroy them
- Collect points from destroyed boxes
- Track score displayed on screen
- Try to destroy all boxes on the map

### Scoring
- Each box has a score value (from JSON)
- Destroying a box awards its score value
- Score is displayed in real-time on the UI
- Feedback text shows "+XXX Points!" when box is destroyed

## Code Quality Highlights

### Best Practices Implemented
✅ **Feature-Based Organization**: Code organized by functionality (Data, Features, UI)  
✅ **Clear Separation**: Public/Private folder structure for encapsulation  
✅ **Documentation**: Comprehensive comments and Doxygen-style documentation  
✅ **Error Handling**: HTTP errors, JSON parse errors, null checks  
✅ **Memory Management**: Proper UObject lifecycle management  
✅ **Delegates**: Event-driven architecture using Unreal delegates  
✅ **Modular Design**: Systems can be independently tested and modified  
✅ **Blueprint Integration**: C++ classes can be extended in Blueprint  

### Code Examples

**JSON Fetching (async)**:
```cpp
void UJsonDataManager::FetchJsonData()
{
    FHttpModule& HttpModule = FHttpModule::Get();
    TSharedRef<IHttpRequest> HttpRequest = HttpModule.CreateRequest();
    HttpRequest->SetURL(JSON_URL);
    HttpRequest->SetVerb(TEXT("GET"));
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &UJsonDataManager::OnHttpResponse);
    HttpRequest->ProcessRequest();
}
```

**Line Trace Shooting**:
```cpp
bool AWeaponSystem::PerformLineTrace(FHitResult& OutHitResult)
{
    FVector CameraLocation = FirstPersonCamera->GetComponentLocation();
    FVector TraceEnd = CameraLocation + (FirstPersonCamera->GetForwardVector() * FireRange);
    return GetWorld()->LineTraceSingleByChannel(OutHitResult, CameraLocation, TraceEnd, ECC_WorldDynamic, QueryParams);
}
```

**Dynamic Material Color**:
```cpp
void AGameBox::ApplyBoxColor()
{
    if (DynamicMaterial)
    {
        FLinearColor BoxLinearColor(BoxColorValue.X, BoxColorValue.Y, BoxColorValue.Z, 1.0f);
        DynamicMaterial->SetVectorParameterValue(TEXT("BaseColor"), BoxLinearColor);
    }
}
```

## Troubleshooting

### Common Issues

**Issue: "Missing DLL errors on startup"**
- **Solution**: Regenerate Visual Studio files and recompile

**Issue: "Boxes not spawning"**
- **Solution**: Check browser console for JSON fetch errors
- Verify GitHub URL is accessible
- Check `JsonDataManager` logs

**Issue: "Shooting doesn't work"**
- **Solution**: Verify `WeaponSystem` is set as default pawn
- Check if line trace is hitting boxes correctly
- Enable debug visualization in `PerformLineTrace`

**Issue: "Material color not showing"**
- **Solution**: Ensure material has `BaseColor` parameter
- Check RGB values are normalized (0-1)
- Verify dynamic material instance is created

## Performance Optimization

### Implemented Optimizations
- Object pooling ready (can be added for box spawning)
- Efficient line traces (single channel, ignore self)
- Material instances (dynamic, not creating new materials)
- Async HTTP requests (doesn't block game thread)

### Future Optimizations
- Object pooling for frequently destroyed boxes
- Frustum culling for many boxes
- LOD system for distant boxes
- Network replication optimization

## Submission Contents

### GitHub Repository Structure
```
CyrusGame/
├── Source/              # C++ source code
├── Content/             # Unreal assets (maps, materials, blueprints)
├── CyrusGame.uproject   # Project file
├── README.md            # This file
├── SETUP_GUIDE.md       # Detailed setup instructions
└── .gitignore           # Git ignore file
```

### Files Included
- ✅ Complete C++ source code
- ✅ Project configuration files
- ✅ Comprehensive documentation
- ✅ Setup and build instructions
- ✅ Code comments and best practices
- ✅ Error handling and logging
- ✅ Material color system
- ✅ Scoring system
- ✅ UI implementation

## Video Walkthrough

A recorded gameplay video demonstrating:
1. Game launching and box spawning
2. Shooting mechanics
3. Color variations on boxes
4. Score accumulation
5. Destruction of boxes
6. Final score display

Video uploaded to YouTube (link in submission)

## Contact & Support

For questions regarding the implementation:
- **Email**: Contact via Cyrus 365 platform
- **Issues**: Refer to troubleshooting section
- **Code Style**: Follows Unreal Engine Coding Standards

## License

This project is created for the Cyrus 365 internship assignment. All code is original and follows professional development standards.

---

**Assignment Deadline**: March 16, 2026  
**Submission Status**: Ready for review  
**Engine Version**: UE 5.6  
**Language**: C++17  
**Platform**: Windows (portable to Mac/Linux)

