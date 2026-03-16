#include "Data/JsonDataManager.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"
#include "Json.h"
#include "JsonUtilities.h"
#include "Containers/List.h"

void UJsonDataManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	bDataLoaded = false;
}

void UJsonDataManager::FetchJsonData()
{
	if (!FModuleManager::Get().IsModuleLoaded("HTTP"))
	{
		FModuleManager::Get().LoadModule("HTTP");
	}

	FHttpModule& HttpModule = FHttpModule::Get();
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = HttpModule.CreateRequest();

	HttpRequest->SetURL(JSON_URL);
	HttpRequest->SetVerb(TEXT("GET"));
	HttpRequest->SetHeader(TEXT("User-Agent"), TEXT("CyrusGame/1.0"));
	HttpRequest->SetHeader(TEXT("Accept"), TEXT("application/json"));

	// Bind response handler
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UJsonDataManager::OnHttpResponse);

	if (!HttpRequest->ProcessRequest())
	{
		FString ErrorMsg = TEXT("Failed to process HTTP request");
		OnJsonLoadError.Broadcast(ErrorMsg, -1);
		UE_LOG(LogTemp, Error, TEXT("JsonDataManager: %s"), *ErrorMsg);
	}
}

void UJsonDataManager::OnHttpResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!bWasSuccessful || !Response.IsValid())
	{
		FString ErrorMsg = TEXT("HTTP request failed");
		int32 ResponseCode = Response.IsValid() ? Response->GetResponseCode() : -1;
		OnJsonLoadError.Broadcast(ErrorMsg, ResponseCode);
		UE_LOG(LogTemp, Error, TEXT("JsonDataManager: %s (Code: %d)"), *ErrorMsg, ResponseCode);
		return;
	}

	FString JsonString = Response->GetContentAsString();
	
	if (JsonString.IsEmpty())
	{
		FString ErrorMsg = TEXT("Empty JSON response");
		OnJsonLoadError.Broadcast(ErrorMsg, Response->GetResponseCode());
		UE_LOG(LogTemp, Error, TEXT("JsonDataManager: %s"), *ErrorMsg);
		return;
	}

	FGameData ParsedData;
	if (!ParseJsonData(JsonString, ParsedData))
	{
		FString ErrorMsg = TEXT("Failed to parse JSON data");
		OnJsonLoadError.Broadcast(ErrorMsg, -2);
		UE_LOG(LogTemp, Error, TEXT("JsonDataManager: %s"), *ErrorMsg);
		return;
	}

	// Cache the data and mark as loaded
	CachedGameData = ParsedData;
	bDataLoaded = true;

	UE_LOG(LogTemp, Warning, TEXT("JsonDataManager: Successfully loaded %d box types and %d objects"), 
		ParsedData.BoxTypes.Num(), ParsedData.Objects.Num());

	// Broadcast success
	OnJsonDataLoaded.Broadcast(true, CachedGameData);
}

bool UJsonDataManager::ParseJsonData(const FString& JsonString, FGameData& OutGameData)
{
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);

	if (!FJsonSerializer::Deserialize(Reader, JsonObject) || !JsonObject.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("JsonDataManager: Failed to deserialize JSON"));
		return false;
	}

	// Parse box types
	TArray<TSharedPtr<FJsonValue>> TypesArray = JsonObject->GetArrayField(TEXT("types"));
	OutGameData.BoxTypes.Reserve(TypesArray.Num());

	for (const TSharedPtr<FJsonValue>& TypeValue : TypesArray)
	{
		TSharedPtr<FJsonObject> TypeObj = TypeValue->AsObject();
		if (!TypeObj.IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("JsonDataManager: Skipping invalid type entry"));
			continue;
		}

		FBoxTypeData BoxType;
		BoxType.TypeName = TypeObj->GetStringField(TEXT("name"));
		BoxType.Health = static_cast<int32>(TypeObj->GetNumberField(TEXT("health")));
		BoxType.Score = static_cast<int32>(TypeObj->GetNumberField(TEXT("score")));

		TArray<TSharedPtr<FJsonValue>> ColorArray = TypeObj->GetArrayField(TEXT("color"));
		if (ColorArray.Num() == 3)
		{
			TArray<double> RgbValues;
			for (const TSharedPtr<FJsonValue>& ColorVal : ColorArray)
			{
				RgbValues.Add(ColorVal->AsNumber());
			}
			BoxType.BoxColor = ConvertRgbToVector(RgbValues);
		}

		OutGameData.BoxTypes.Add(BoxType);
	}

	// Parse objects
	TArray<TSharedPtr<FJsonValue>> ObjectsArray = JsonObject->GetArrayField(TEXT("objects"));
	OutGameData.Objects.Reserve(ObjectsArray.Num());

	for (const TSharedPtr<FJsonValue>& ObjValue : ObjectsArray)
	{
		TSharedPtr<FJsonObject> ObjData = ObjValue->AsObject();
		if (!ObjData.IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("JsonDataManager: Skipping invalid object entry"));
			continue;
		}

		FSpawnableObjectData SpawnObj;
		SpawnObj.TypeName = ObjData->GetStringField(TEXT("type"));

		// Parse transform
		TSharedPtr<FJsonObject> TransformObj = ObjData->GetObjectField(TEXT("transform"));
		if (TransformObj.IsValid())
		{
			// Location
			TArray<TSharedPtr<FJsonValue>> LocationArray = TransformObj->GetArrayField(TEXT("location"));
			if (LocationArray.Num() == 3)
			{
				SpawnObj.Location = FVector(
					LocationArray[0]->AsNumber(),
					LocationArray[1]->AsNumber(),
					LocationArray[2]->AsNumber()
				);
			}

			// Rotation (roll, pitch, yaw)
			TArray<TSharedPtr<FJsonValue>> RotationArray = TransformObj->GetArrayField(TEXT("rotation"));
			if (RotationArray.Num() == 3)
			{
				SpawnObj.Rotation = FVector(
					RotationArray[0]->AsNumber(),
					RotationArray[1]->AsNumber(),
					RotationArray[2]->AsNumber()
				);
			}

			// Scale
			TArray<TSharedPtr<FJsonValue>> ScaleArray = TransformObj->GetArrayField(TEXT("scale"));
			if (ScaleArray.Num() == 3)
			{
				SpawnObj.Scale = FVector(
					ScaleArray[0]->AsNumber(),
					ScaleArray[1]->AsNumber(),
					ScaleArray[2]->AsNumber()
				);
			}
		}

		OutGameData.Objects.Add(SpawnObj);
	}

	return true;
}

FVector UJsonDataManager::ConvertRgbToVector(const TArray<double>& RgbArray) const
{
	if (RgbArray.Num() != 3)
	{
		return FVector(1.0f, 1.0f, 1.0f); // Default white
	}

	// Normalize RGB values from 0-255 to 0-1
	FVector NormalizedColor;
	NormalizedColor.X = static_cast<float>(FMath::Clamp(RgbArray[0] / 255.0, 0.0, 1.0));
	NormalizedColor.Y = static_cast<float>(FMath::Clamp(RgbArray[1] / 255.0, 0.0, 1.0));
	NormalizedColor.Z = static_cast<float>(FMath::Clamp(RgbArray[2] / 255.0, 0.0, 1.0));

	return NormalizedColor;
}
