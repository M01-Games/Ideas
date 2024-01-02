// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "IdeasGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class IDEAS_API AIdeasGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	

private:
	void ResetLevel();

	FTimerHandle CountDownTimerHandle = FTimerHandle();

	void CountDownTimer();


public:
	void RestartGamePlay(bool Won);

	UPROPERTY(BlueprintReadOnly)
		int TimerCount = 600;

	virtual void StartPlay() override;
};
