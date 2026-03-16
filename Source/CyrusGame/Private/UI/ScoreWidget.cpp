#include "UI/ScoreWidget.h"
#include "Features/GameManager.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UScoreWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Try to bind widgets if they exist
	if (UCanvasPanel* Canvas = Cast<UCanvasPanel>(GetRootWidget()))
	{
		// Look for score text
		for (int32 i = 0; i < Canvas->GetChildrenCount(); ++i)
		{
			if (UTextBlock* TextBlock = Cast<UTextBlock>(Canvas->GetChildAt(i)))
			{
				if (TextBlock->GetName().Contains(TEXT("ScoreText")))
				{
					ScoreText = TextBlock;
				}
				else if (TextBlock->GetName().Contains(TEXT("BoxCountText")))
				{
					BoxCountText = TextBlock;
				}
				else if (TextBlock->GetName().Contains(TEXT("FeedbackText")))
				{
					FeedbackText = TextBlock;
				}
			}
		}
	}

	// Find game manager
	AGameManager* Manager = Cast<AGameManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AGameManager::StaticClass()));
	if (Manager)
	{
		GameManager = Manager;
		Manager->OnScoreChanged.AddDynamic(this, &UScoreWidget::UpdateScore);
		Manager->OnBoxesSpawned.AddDynamic(this, &UScoreWidget::UpdateBoxCount);
		
		// Set initial values
		UpdateScore(Manager->GetCurrentScore());
		UpdateBoxCount(Manager->GetActiveBoxCount());
	}

	CurrentScore = 0;
	CurrentBoxCount = 0;
	RefreshScoreDisplay();
	RefreshBoxCountDisplay();
}

void UScoreWidget::NativeDestruct()
{
	if (GameManager)
	{
		GameManager->OnScoreChanged.RemoveDynamic(this, &UScoreWidget::UpdateScore);
		GameManager->OnBoxesSpawned.RemoveDynamic(this, &UScoreWidget::UpdateBoxCount);
	}

	Super::NativeDestruct();
}

void UScoreWidget::UpdateScore(int32 NewScore)
{
	CurrentScore = NewScore;
	RefreshScoreDisplay();
}

void UScoreWidget::UpdateBoxCount(int32 BoxCount)
{
	CurrentBoxCount = BoxCount;
	RefreshBoxCountDisplay();
}

void UScoreWidget::ShowDestructionFeedback(int32 Score)
{
	if (FeedbackText)
	{
		FString FeedbackStr = FString::Printf(TEXT("+%d Points!"), Score);
		FeedbackText->SetText(FText::FromString(FeedbackStr));
		FeedbackText->SetVisibility(ESlateVisibility::Visible);

		// Hide after 2 seconds
		GetWorld()->GetTimerManager().SetTimer(
			FeedbackTimerHandle,
			this,
			&UScoreWidget::HideFeedbackText,
			2.0f,
			false
		);
	}
}

void UScoreWidget::RefreshScoreDisplay()
{
	if (ScoreText)
	{
		FString ScoreStr = FString::Printf(TEXT("Score: %d"), CurrentScore);
		ScoreText->SetText(FText::FromString(ScoreStr));
	}
}

void UScoreWidget::RefreshBoxCountDisplay()
{
	if (BoxCountText)
	{
		FString CountStr = FString::Printf(TEXT("Boxes: %d"), CurrentBoxCount);
		BoxCountText->SetText(FText::FromString(CountStr));
	}
}

void UScoreWidget::HideFeedbackText()
{
	if (FeedbackText)
	{
		FeedbackText->SetVisibility(ESlateVisibility::Hidden);
	}
}
