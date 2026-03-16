# Quick Start - Get Running in 5 Minutes

## Fastest Path to Playable Game

### Prerequisites
✅ Windows 10/11  
✅ UE 5.6 installed  
✅ Visual Studio 2022  

### 5-Step Setup

#### Step 1: Generate Project Files (1 min)
```batch
# Right-click CyrusGame.uproject
# Select "Generate Visual Studio project files"
# Wait for completion
```

#### Step 2: Compile Code (5-10 min)
```batch
# Open CyrusGame.sln in Visual Studio
# Set to "Development Editor | Win64"
# Press F5 or Build → Build Solution
# Wait for completion
```

#### Step 3: Open in Editor (1 min)
```batch
# Double-click CyrusGame.uproject
# Click "Play" when editor opens
```

#### Step 4: Create Blueprints (5 min)
In Unreal Editor:
1. Right-click Content/Blueprints → Blueprint Class → AGameManager → `BP_GameManager`
2. Right-click Content/Blueprints → Widget Blueprint → UScoreWidget → `W_ScoreWidget`
3. Add TextBlocks: `ScoreText`, `BoxCountText`

#### Step 5: Configure Level (5 min)
1. File → New Level
2. Add actors: WeaponSystem, BP_GameManager
3. Save as Content/Maps/MainMap
4. Edit → Project Settings → Search "Game Mode" → Set to `BP_GameManager`

### Play!
- Click **Play**
- Shoot boxes with Left Mouse
- Watch score increase
- Destroy all boxes!

---

## For Detailed Setup
👉 See **SETUP_GUIDE.md**

## For Issues
👉 See **README.md** → Troubleshooting

## For Submission
👉 See **SUBMISSION.md**

**Total Time**: ~20-30 minutes for first-time setup  
**Runtime**: 60+ FPS on modern systems

