#include "Features/GameBox.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/Material.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Engine/StaticMesh.h"

AGameBox::AGameBox()
{
	PrimaryActorTick.bCanEverTick = false;

	// Create root collision component
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	RootComponent = CollisionBox;
	CollisionBox->SetCollisionEnabled(ECC_WorldDynamic);
	CollisionBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	CollisionBox->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	// Create mesh component
	BoxMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoxMesh"));
	BoxMesh->SetupAttachment(RootComponent);
	BoxMesh->SetCollisionEnabled(ECC_NoCollision);

	// Load default cube mesh
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMeshAsset(TEXT("/Engine/BasicShapes/Cube"));
	if (CubeMeshAsset.Succeeded())
	{
		BoxMesh->SetStaticMesh(CubeMeshAsset.Object);
	}

	// Load default material
	static ConstructorHelpers::FObjectFinder<UMaterial> MaterialAsset(TEXT("/Engine/EngineMaterials/WorldGridMaterial"));
	if (MaterialAsset.Succeeded())
	{
		BaseMaterial = MaterialAsset.Object;
	}

	bIsDestroyed = false;
	CurrentHealth = 100;
	MaxHealth = 100;
	ScoreValue = 10;
}

void AGameBox::BeginPlay()
{
	Super::BeginPlay();

	// Create dynamic material instance
	if (BoxMesh && BaseMaterial)
	{
		DynamicMaterial = UMaterialInstanceDynamic::Create(BaseMaterial, this);
		BoxMesh->SetMaterial(0, DynamicMaterial);
	}

	// Setup collision for bullet detection
	if (CollisionBox)
	{
		CollisionBox->SetGenerateOverlapEvents(false);
	}
}

void AGameBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGameBox::InitializeBox(const FSpawnableObjectData& SpawnData, const FBoxTypeData& TypeData)
{
	TypeName = SpawnData.TypeName;
	MaxHealth = TypeData.Health;
	CurrentHealth = TypeData.Health;
	ScoreValue = TypeData.Score;
	BoxColorValue = TypeData.BoxColor;

	// Set scale
	SetActorScale3D(SpawnData.Scale);

	// Set location (convert from cm to UU if needed)
	FVector LocationUU = SpawnData.Location;
	SetActorLocation(LocationUU);

	// Set rotation (convert from degrees to rotator)
	FRotator ActorRotation(
		SpawnData.Rotation.Y,  // Pitch
		SpawnData.Rotation.Z,  // Yaw
		SpawnData.Rotation.X   // Roll
	);
	SetActorRotation(ActorRotation);

	// Set collision box size
	if (CollisionBox)
	{
		CollisionBox->SetBoxExtent(FVector(50.0f, 50.0f, 50.0f) * SpawnData.Scale);
	}

	// Apply color
	ApplyBoxColor();

	UE_LOG(LogTemp, Warning, TEXT("GameBox initialized: Type=%s, Health=%d, Score=%d, Color=%.2f,%.2f,%.2f"),
		*TypeName, CurrentHealth, ScoreValue, BoxColorValue.X, BoxColorValue.Y, BoxColorValue.Z);
}

void AGameBox::TakeDamage(int32 Amount)
{
	if (bIsDestroyed)
	{
		return;
	}

	CurrentHealth -= Amount;
	UE_LOG(LogTemp, Warning, TEXT("GameBox took damage. Remaining health: %d"), CurrentHealth);

	if (CurrentHealth <= 0)
	{
		DestroyBox();
	}
}

void AGameBox::DestroyBox()
{
	if (bIsDestroyed)
	{
		return;
	}

	bIsDestroyed = true;
	UE_LOG(LogTemp, Warning, TEXT("GameBox destroyed! Score awarded: %d"), ScoreValue);

	// Broadcast destroy event
	OnBoxDestroyed.Broadcast(ScoreValue, GetActorLocation());

	// Destroy actor with delay for visual effect
	GetWorld()->GetTimerManager().SetTimer(
		FTimerHandle(),
		[this]()
		{
			Destroy();
		},
		0.1f,
		false
	);
}

void AGameBox::ApplyBoxColor()
{
	if (!DynamicMaterial)
	{
		return;
	}

	// Apply color to material
	FLinearColor BoxLinearColor(BoxColorValue.X, BoxColorValue.Y, BoxColorValue.Z, 1.0f);
	DynamicMaterial->SetVectorParameterValue(TEXT("BaseColor"), BoxLinearColor);
	DynamicMaterial->SetVectorParameterValue(TEXT("Color"), BoxLinearColor);
	DynamicMaterial->SetVectorParameterValue(TEXT("BoxColor"), BoxLinearColor);

	UE_LOG(LogTemp, Warning, TEXT("Applied color to box: R=%.2f, G=%.2f, B=%.2f"),
		BoxColorValue.X, BoxColorValue.Y, BoxColorValue.Z);
}
