#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Data/JsonDataManager.h"
#include "GameBox.generated.h"

class UMaterialInstanceDynamic;
class UBoxComponent;

/**
 * Delegate for box destroyed event
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBoxDestroyed, int32, Score, FVector, Location);

/**
 * Actor representing a damageable box in the game
 */
UCLASS()
class CYRUSGAME_API AGameBox : public AActor
{
	GENERATED_BODY()

public:
	AGameBox();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	/**
	 * Initialize the box with data from JSON
	 */
	UFUNCTION(BlueprintCallable, Category = "Game Box")
	void InitializeBox(const FSpawnableObjectData& SpawnData, const FBoxTypeData& TypeData);

	/**
	 * Apply damage to the box
	 */
	UFUNCTION(BlueprintCallable, Category = "Game Box")
	void TakeDamage(int32 Amount = 1);

	/**
	 * Get current health
	 */
	UFUNCTION(BlueprintCallable, Category = "Game Box")
	int32 GetCurrentHealth() const { return CurrentHealth; }

	/**
	 * Get score value
	 */
	UFUNCTION(BlueprintCallable, Category = "Game Box")
	int32 GetScoreValue() const { return ScoreValue; }

	// Delegate for box destruction
	UPROPERTY(BlueprintAssignable, Category = "Game Box")
	FOnBoxDestroyed OnBoxDestroyed;

protected:
	// Root collision component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* CollisionBox;

	// Static mesh component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* BoxMesh;

	// Material instance for dynamic color
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UMaterialInstanceDynamic* DynamicMaterial;

	// Box properties
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Box Properties")
	int32 MaxHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Box Properties")
	int32 CurrentHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Box Properties")
	int32 ScoreValue;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Box Properties")
	FVector BoxColorValue;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Box Properties")
	FString TypeName;

private:
	/**
	 * Destroy the box
	 */
	void DestroyBox();

	/**
	 * Apply color to box material
	 */
	void ApplyBoxColor();

	// Base material to use for boxes
	UPROPERTY(EditAnywhere, Category = "Materials")
	UMaterial* BaseMaterial;

	bool bIsDestroyed;
};
