#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Data/JsonDataManager.h"
#include "GameManager.generated.h"

class AGameBox;
class UScoreWidget;

/**
 * Delegate for game events
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnScoreChanged, int32, NewScore);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBoxesSpawned, int32, BoxCount);

/**
 * Main game mode that manages gameplay, spawning, and scoring
 */
UCLASS()
class CYRUSGAME_API AGameManager : public AGameModeBase
{
	GENERATED_BODY()

public:
	AGameManager();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	/**
	 * Add points to the score
	 */
	UFUNCTION(BlueprintCallable, Category = "Game Manager")
	void AddScore(int32 Points);

	/**
	 * Get current score
	 */
	UFUNCTION(BlueprintCallable, Category = "Game Manager")
	int32 GetCurrentScore() const { return CurrentScore; }

	/**
	 * Get number of active boxes
	 */
	UFUNCTION(BlueprintCallable, Category = "Game Manager")
	int32 GetActiveBoxCount() const { return ActiveBoxes.Num(); }

	// Delegates
	UPROPERTY(BlueprintAssignable, Category = "Game Manager")
	FOnScoreChanged OnScoreChanged;

	UPROPERTY(BlueprintAssignable, Category = "Game Manager")
	FOnBoxesSpawned OnBoxesSpawned;

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	/**
	 * Called when JSON data is loaded
	 */
	UFUNCTION()
	void OnJsonDataLoaded(bool bSuccess, const FGameData& GameData);

	/**
	 * Spawn boxes from JSON data
	 */
	void SpawnBoxesFromData(const FGameData& GameData);

	/**
	 * Handle box destroyed event
	 */
	UFUNCTION()
	void OnBoxDestroyed(int32 Score, FVector Location);

	/**
	 * Find a box type by name
	 */
	const FBoxTypeData* FindBoxType(const FString& TypeName, const FGameData& GameData) const;

private:
	int32 CurrentScore;
	TArray<AGameBox*> ActiveBoxes;
	FGameData LoadedGameData;

	// Reference to score UI widget
	UScoreWidget* ScoreWidget;

	bool bGameInitialized;
};
