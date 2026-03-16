#include "Features/WeaponSystem.h"
#include "Features/GameBox.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Input/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

AWeaponSystem::AWeaponSystem()
{
	PrimaryActorTick.bCanEverTick = true;

	// Don't rotate character with camera
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	GetCharacterMovement()->MaxAcceleration = 2400.0f;

	// Create camera boom
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 0.0f;
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->bInheritPitch = true;
	CameraBoom->bInheritYaw = true;

	// Create first person camera
	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FirstPersonCamera->bUsePawnControlRotation = false;

	// Weapon properties
	FireRange = 5000.0f;
	DamagePerShot = 1;
	FireRate = 0.1f;

	bIsAiming = false;
	LastFireTime = 0.0f;
}

void AWeaponSystem::BeginPlay()
{
	Super::BeginPlay();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			PlayerController->GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			// Input context must be created in Blueprint
			// Subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}
}

void AWeaponSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeaponSystem::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Bind fire action
		if (Fire_IA)
		{
			EnhancedInputComponent->BindAction(Fire_IA, ETriggerEvent::Started, this, &AWeaponSystem::HandleFirePressed);
			EnhancedInputComponent->BindAction(Fire_IA, ETriggerEvent::Completed, this, &AWeaponSystem::HandleFireReleased);
		}

		// Bind look action
		if (Look)
		{
			EnhancedInputComponent->BindAction(Look, ETriggerEvent::Triggered, this, &AWeaponSystem::HandleLook);
		}

		// Bind move action
		if (Move)
		{
			EnhancedInputComponent->BindAction(Move, ETriggerEvent::Triggered, this, &AWeaponSystem::HandleMove);
		}
	}
}

void AWeaponSystem::Fire()
{
	// Check fire rate
	if (GetWorld()->GetTimeSeconds() - LastFireTime < FireRate)
	{
		return;
	}

	LastFireTime = GetWorld()->GetTimeSeconds();

	FHitResult HitResult;
	if (PerformLineTrace(HitResult))
	{
		AActor* HitActor = HitResult.GetActor();
		if (AGameBox* GameBox = Cast<AGameBox>(HitActor))
		{
			GameBox->TakeDamage(DamagePerShot);
			UE_LOG(LogTemp, Warning, TEXT("WeaponSystem: Hit GameBox!"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("WeaponSystem: Hit actor %s"), *HitActor->GetName());
		}
	}
}

void AWeaponSystem::SetAiming(bool bNewAiming)
{
	bIsAiming = bNewAiming;

	if (bIsAiming)
	{
		// Zoom in
		FirstPersonCamera->SetFieldOfView(60.0f);
	}
	else
	{
		// Zoom out
		FirstPersonCamera->SetFieldOfView(90.0f);
	}
}

void AWeaponSystem::HandleMove(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// Move forward/backward
		if (MovementVector.Y != 0.0f)
		{
			AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		}

		// Move right/left
		if (MovementVector.X != 0.0f)
		{
			AddMovementInput(GetActorRightVector(), MovementVector.X);
		}
	}
}

void AWeaponSystem::HandleLook(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// Look up/down
		AddControllerPitchInput(LookAxisVector.Y);

		// Look left/right
		AddControllerYawInput(LookAxisVector.X);
	}
}

void AWeaponSystem::HandleFirePressed()
{
	Fire();
}

void AWeaponSystem::HandleFireReleased()
{
	// Can be used for automatic fire stop
}

bool AWeaponSystem::PerformLineTrace(FHitResult& OutHitResult)
{
	if (!FirstPersonCamera)
	{
		return false;
	}

	FVector CameraLocation = FirstPersonCamera->GetComponentLocation();
	FVector CameraForward = FirstPersonCamera->GetForwardVector();
	FVector TraceEnd = CameraLocation + (CameraForward * FireRange);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		OutHitResult,
		CameraLocation,
		TraceEnd,
		ECC_WorldDynamic,
		QueryParams
	);

	// Debug visualization
	if (bHit)
	{
		DrawDebugLine(
			GetWorld(),
			CameraLocation,
			OutHitResult.ImpactPoint,
			FColor::Green,
			false,
			0.1f,
			0,
			2.0f
		);

		DrawDebugSphere(
			GetWorld(),
			OutHitResult.ImpactPoint,
			10.0f,
			8,
			FColor::Red,
			false,
			0.1f,
			0,
			2.0f
		);
	}
	else
	{
		DrawDebugLine(
			GetWorld(),
			CameraLocation,
			TraceEnd,
			FColor::Red,
			false,
			0.1f,
			0,
			1.0f
		);
	}

	return bHit;
}
