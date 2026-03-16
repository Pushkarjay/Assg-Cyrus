#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CyrusGameCharacter.generated.h"

UCLASS()
class CYRUSGAME_API ACyrusGameCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ACyrusGameCharacter();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
