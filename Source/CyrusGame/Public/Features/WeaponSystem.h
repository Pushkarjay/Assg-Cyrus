#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "WeaponSystem.generated.h"

class UInputComponent;
class UInputAction;
class UInputMappingContext;
class AGameBox;

/**
 * Enhanced character with shooting mechanics for FPS gameplay
 */
UCLASS()
class CYRUSGAME_API AWeaponSystem : public ACharacter
{
	GENERATED_BODY()

public:
	AWeaponSystem();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/**
	 * Perform a raycast shot/fire
	 */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void Fire();

	/**
	 * Set whether the player is aiming down sights
	 */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void SetAiming(bool bNewAiming);

protected:
	// Enhanced Input System
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UEnhancedInputComponent* EnhancedInputComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UEnhancedInputSubsystems* InputSubsystem;

	// Input actions
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* Look;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* Move;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* Fire_IA;

	// Input context
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* InputMappingContext;

	// Weapon properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float FireRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int32 DamagePerShot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float FireRate;

	// Camera component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UCameraComponent* FirstPersonCamera;

private:
	// Input callbacks
	void HandleMove(const FInputActionValue& Value);
	void HandleLook(const FInputActionValue& Value);
	void HandleFirePressed();
	void HandleFireReleased();

	/**
	 * Perform a line trace for bullet hit detection
	 */
	bool PerformLineTrace(FHitResult& OutHitResult);

	bool bIsAiming;
	float LastFireTime;
};
