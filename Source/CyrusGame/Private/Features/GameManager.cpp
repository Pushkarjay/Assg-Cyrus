#include "Features/GameManager.h"
#include "Features/GameBox.h"
#include "UI/ScoreWidget.h"
#include "Data/JsonDataManager.h"
#include "Kismet/GameplayStatics.h"

AGameManager::AGameManager()
{
	PrimaryActorTick.bCanEverTick = true;

	CurrentScore = 0;
	bGameInitialized = false;
}

void AGameManager::BeginPlay()
{
	Super::BeginPlay();

	// Get JSON data manager
	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		UJsonDataManager* JsonManager = GameInstance->GetSubsystem<UJsonDataManager>();
		if (JsonManager)
		{
			// Bind to data loaded delegate
			JsonManager->OnJsonDataLoaded.AddDynamic(this, &AGameManager::OnJsonDataLoaded);

			// Start fetching JSON data
			JsonManager->FetchJsonData();

			UE_LOG(LogTemp, Warning, TEXT("GameManager: Fetching JSON data..."));
		}
	}

	// Create and setup score widget
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (UClass* ScoreWidgetClass = FindObject<UClass>(ANY_PACKAGE, TEXT("ScoreWidget_C")))
		{
			ScoreWidget = CreateWidget<UScoreWidget>(PlayerController, ScoreWidgetClass);
			if (ScoreWidget)
			{
				ScoreWidget->AddToViewport();
			}
		}
	}
}

void AGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGameManager::OnJsonDataLoaded(bool bSuccess, const FGameData& GameData)
{
	if (!bSuccess)
	{
		UE_LOG(LogTemp, Error, TEXT("GameManager: Failed to load JSON data"));
		return;
	}

	LoadedGameData = GameData;
	SpawnBoxesFromData(GameData);
	bGameInitialized = true;
}

void AGameManager::SpawnBoxesFromData(const FGameData& GameData)
{
	if (GameData.Objects.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("GameManager: No objects to spawn from JSON data"));
		return;
	}

	for (const FSpawnableObjectData& SpawnData : GameData.Objects)
	{
		// Find the box type
		const FBoxTypeData* BoxTypePtr = FindBoxType(SpawnData.TypeName, GameData);
		if (!BoxTypePtr)
		{
			UE_LOG(LogTemp, Warning, TEXT("GameManager: Box type '%s' not found"), *SpawnData.TypeName);
			continue;
		}

		// Spawn the box actor
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = nullptr;

		AGameBox* NewBox = GetWorld()->SpawnActor<AGameBox>(AGameBox::StaticClass(), SpawnParams);
		if (NewBox)
		{
			NewBox->InitializeBox(SpawnData, *BoxTypePtr);
			NewBox->OnBoxDestroyed.AddDynamic(this, &AGameManager::OnBoxDestroyed);
			ActiveBoxes.Add(NewBox);

			UE_LOG(LogTemp, Warning, TEXT("GameManager: Spawned box at (%.0f, %.0f, %.0f)"),
				SpawnData.Location.X, SpawnData.Location.Y, SpawnData.Location.Z);
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("GameManager: Spawned %d boxes from JSON data"), ActiveBoxes.Num());
	OnBoxesSpawned.Broadcast(ActiveBoxes.Num());
}

void AGameManager::OnBoxDestroyed(int32 Score, FVector Location)
{
	AddScore(Score);

	// Remove from active boxes
	for (int32 i = 0; i < ActiveBoxes.Num(); ++i)
	{
		if (ActiveBoxes[i]->GetActorLocation().Equals(Location, 10.0f))
		{
			ActiveBoxes.RemoveAt(i);
			break;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("GameManager: Box destroyed! Score += %d, Total = %d, Remaining boxes = %d"),
		Score, CurrentScore, ActiveBoxes.Num());
}

const FBoxTypeData* AGameManager::FindBoxType(const FString& TypeName, const FGameData& GameData) const
{
	for (const FBoxTypeData& BoxType : GameData.BoxTypes)
	{
		if (BoxType.TypeName.Equals(TypeName))
		{
			return &BoxType;
		}
	}
	return nullptr;
}

void AGameManager::AddScore(int32 Points)
{
	CurrentScore += Points;
	OnScoreChanged.Broadcast(CurrentScore);

	if (ScoreWidget)
	{
		ScoreWidget->UpdateScore(CurrentScore);
	}
}

void AGameManager::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
