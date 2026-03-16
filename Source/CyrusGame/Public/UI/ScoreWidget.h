#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "ScoreWidget.generated.h"

class AGameManager;

/**
 * UI Widget for displaying player score
 * Base class implemented in C++, can be extended in Blueprint for styling
 */
UCLASS()
class CYRUSGAME_API UScoreWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	/**
	 * Update the score display
	 */
	UFUNCTION(BlueprintCallable, Category = "UI")
	void UpdateScore(int32 NewScore);

	/**
	 * Update the box count display
	 */
	UFUNCTION(BlueprintCallable, Category = "UI")
	void UpdateBoxCount(int32 BoxCount);

	/**
	 * Show destruction feedback (text animation)
	 */
	UFUNCTION(BlueprintCallable, Category = "UI")
	void ShowDestructionFeedback(int32 Score);

protected:
	// Score text block
	UPROPERTY(meta = (BindWidget), Category = "UI")
	UTextBlock* ScoreText;

	UPROPERTY(meta = (BindWidget), Category = "UI")
	UTextBlock* BoxCountText;

	UPROPERTY(meta = (BindWidget), Category = "UI")
	UTextBlock* FeedbackText;

private:
	/**
	 * Internal update for score display
	 */
	void RefreshScoreDisplay();

	/**
	 * Internal update for box count display
	 */
	void RefreshBoxCountDisplay();

	int32 CurrentScore;
	int32 CurrentBoxCount;

	// Reference to game manager
	AGameManager* GameManager;

	// Feedback timer
	FTimerHandle FeedbackTimerHandle;

	/**
	 * Hide feedback text after delay
	 */
	UFUNCTION()
	void HideFeedbackText();
};
