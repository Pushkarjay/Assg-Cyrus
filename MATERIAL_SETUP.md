# Cyrus Game - Material Setup Guide

This guide explains how to set up the dynamic color material system for the boxes.

## Material Creation Steps

### Creating the Box Material

1. **In Unreal Editor**:
   - Right-click in Content > Materials folder
   - Select "Material"
   - Name it `M_Box`
   - Double-click to open material editor

2. **Material Graph Setup**:
   - Delete the default "M_Box" connection nodes (if any)
   - In the Material Details panel:
     - Set `Material Domain` to `Surface`
     - Set `Blend Mode` to `Opaque`

3. **Add Nodes** (connect right-to-left to Main Material Output):

   **Node 1: Base Color Parameter**
   - Right-click > Search "VectorParameter"
   - Create `VectorParameter` node
   - Set Default Value to (0.5, 0.5, 0.5) (medium gray)
   - Name it `BaseColor`
   - Connect output to Main Material Output's `Base Color` pin

   **Node 2: Metallic**
   - Right-click > Search "ScalarParameter"
   - Create `ScalarParameter` node  
   - Set Default to 0.0
   - Name it `Metallic`
   - Connect to Main Material Output's `Metallic` pin

   **Node 3: Roughness**
   - Right-click > Search "ScalarParameter"
   - Create `ScalarParameter` node
   - Set Default to 0.8
   - Name it `Roughness`
   - Connect to Main Material Output's `Roughness` pin

4. **Compile and Save**:
   - Click "Apply"
   - Click "Save"

### Material Instance Creation

1. **Create Material Instance**:
   - Right-click on `M_Box` in Content Browser
   - Select "Create Material Instance"
   - Name it `M_Box_Instance`

2. **In Material Instance**:
   - Under "Vector Parameter Values"
   - Enable `BaseColor` checkbox
   - This will be used by the C++ code

## C++ Integration

The `AGameBox::ApplyBoxColor()` function automatically:

```cpp
// Creates dynamic material instance
DynamicMaterial = UMaterialInstanceDynamic::Create(BaseMaterial, this);

// Applies color from JSON
FLinearColor BoxLinearColor(BoxColorValue.X, BoxColorValue.Y, BoxColorValue.Z, 1.0f);
DynamicMaterial->SetVectorParameterValue(TEXT("BaseColor"), BoxLinearColor);
```

## Color Format in JSON

Colors are stored as RGB values (0-255):
```json
"color": [255, 0, 0]  // Red
"color": [0, 255, 0]  // Green
"color": [0, 0, 255]  // Blue
```

The code normalizes them to 0-1 range:
```cpp
FVector ConvertRgbToVector(const TArray<double>& RgbArray)
{
    NormalizedColor.X = RgbArray[0] / 255.0;  // 0-1
    NormalizedColor.Y = RgbArray[1] / 255.0;  // 0-1
    NormalizedColor.Z = RgbArray[2] / 255.0;  // 0-1
}
```

## Testing Material System

1. Play the game
2. Boxes should spawn with colors from JSON data
3. Each box will have a unique color based on its type
4. Material parameters are applied per-box instance (dynamic material)

## Troubleshooting

**Problem**: Boxes appear white/default color
- **Solution**: Check if material parameters are named correctly in material editor
- Verify JSON color data is valid (0-255 range)
- Check logs for "ApplyBoxColor" messages

**Problem**: All boxes same color
- **Solution**: Verify JSON data has different colors for different types
- Check `ConvertRgbToVector()` normalization logic

**Problem**: Material doesn't exist
- **Solution**: Regenerate Visual Studio files
- Ensure `BaseMaterial` is loaded correctly in constructor

## Advanced: Custom Material Nodes

For more advanced effects, you can add to the material:

- **Emissive Material**: Add to make boxes glow
- **Normal Map**: For texture detail
- **Specular**: For shiny surfaces
- **Opacity Mask**: For semi-transparent boxes

All of these can be controlled by modifying the `M_Box` material and adding additional parameters to the C++ code.

