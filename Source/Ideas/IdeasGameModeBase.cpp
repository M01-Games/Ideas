// Copyright Epic Games, Inc. All Rights Reserved.

#include "IdeasGameModeBase.h"
#include "Kismet/GameplayStatics.h"

void AIdeasGameModeBase::StartPlay()
{
	Super::StartPlay();

	check(GEngine != nullptr);

	// Display a debug message for five seconds. 
	// The -1 "Key" value argument prevents the message from being updated or refreshed.
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Hello World, this is FPSGameMode!"));

	GetWorldTimerManager().SetTimer(CountDownTimerHandle, this, &AIdeasGameModeBase::CountDownTimer, 1.0f, true, 1.0f);

}

void AIdeasGameModeBase::RestartGamePlay(bool Won)
{
	if(Won)
	{
		ResetLevel();
	}
	else
	{
		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(CountDownTimerHandle, this, &AIdeasGameModeBase::ResetLevel, 3.0f);
	}

}

void AIdeasGameModeBase::ResetLevel()
{
	UGameplayStatics::OpenLevel(GetWorld(), "FPSMap");
}

void AIdeasGameModeBase::CountDownTimer()
{
	TimerCount--;

	if(TimerCount == 0)
	{
		GetWorldTimerManager().ClearTimer(CountDownTimerHandle);
		ResetLevel();
	}

}




