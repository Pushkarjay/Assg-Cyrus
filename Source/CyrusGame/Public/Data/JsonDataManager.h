#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "JsonDataManager.generated.h"

/**
 * Structure representing a single box type from JSON
 */
USTRUCT(BlueprintType)
struct FBoxTypeData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "Box Data")
	FString TypeName;

	UPROPERTY(BlueprintReadWrite, Category = "Box Data")
	FVector BoxColor; // RGB normalized 0-1

	UPROPERTY(BlueprintReadWrite, Category = "Box Data")
	int32 Health;

	UPROPERTY(BlueprintReadWrite, Category = "Box Data")
	int32 Score;
};

/**
 * Structure representing a single spawnable object
 */
USTRUCT(BlueprintType)
struct FSpawnableObjectData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "Object Data")
	FString TypeName;

	UPROPERTY(BlueprintReadWrite, Category = "Object Data")
	FVector Location; // in centimeters

	UPROPERTY(BlueprintReadWrite, Category = "Object Data")
	FVector Rotation; // roll, pitch, yaw in degrees

	UPROPERTY(BlueprintReadWrite, Category = "Object Data")
	FVector Scale;
};

/**
 * Complete game data structure loaded from JSON
 */
USTRUCT(BlueprintType)
struct FGameData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "Game Data")
	TArray<FBoxTypeData> BoxTypes;

	UPROPERTY(BlueprintReadWrite, Category = "Game Data")
	TArray<FSpawnableObjectData> Objects;
};

/**
 * Delegate for notifying when JSON data is loaded
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnJsonDataLoaded, bool, bSuccess, const FGameData&, GameData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnJsonLoadError, const FString&, ErrorMessage, int32, ErrorCode);

/**
 * Manages JSON data fetching and parsing from online source
 */
UCLASS()
class CYRUSGAME_API UJsonDataManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	/**
	 * Fetch JSON data from the online GitHub source
	 */
	UFUNCTION(BlueprintCallable, Category = "JSON Data")
	void FetchJsonData();

	/**
	 * Get the loaded game data
	 */
	UFUNCTION(BlueprintCallable, Category = "JSON Data")
	const FGameData& GetGameData() const { return CachedGameData; }

	/**
	 * Check if data has been loaded
	 */
	UFUNCTION(BlueprintCallable, Category = "JSON Data")
	bool IsDataLoaded() const { return bDataLoaded; }

	// Delegates
	UPROPERTY(BlueprintAssignable, Category = "JSON Data")
	FOnJsonDataLoaded OnJsonDataLoaded;

	UPROPERTY(BlueprintAssignable, Category = "JSON Data")
	FOnJsonLoadError OnJsonLoadError;

private:
	/**
	 * Called when HTTP response is received
	 */
	void OnHttpResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	/**
	 * Parse JSON string into game data structure
	 */
	bool ParseJsonData(const FString& JsonString, FGameData& OutGameData);

	/**
	 * Convert RGB array from JSON to normalized FVector
	 */
	FVector ConvertRgbToVector(const TArray<double>& RgbArray) const;

private:
	FGameData CachedGameData;
	bool bDataLoaded;

	// JSON URL
	static constexpr const TCHAR* JSON_URL = TEXT("https://raw.githubusercontent.com/CyrusCHAU/Varadise-Technical-Test/refs/heads/main/data.json");
};
