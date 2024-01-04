// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSVip.h"
#include "Components/BoxComponent.h"
#include "FPSCharacter.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

// Sets default values
AFPSVip::AFPSVip()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DamageCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Damage Collision"));
	DamageCollision->SetupAttachment(RootComponent);

	AIPerComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception Component"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));

	SightConfig->SightRadius = 1250.0f;
	SightConfig->LoseSightRadius = 1280.0f;
	SightConfig->PeripheralVisionAngleDegrees = 90.0f;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->SetMaxAge(0.1f);

	AIPerComp->ConfigureSense(*SightConfig);
	AIPerComp->SetDominantSense(SightConfig->GetSenseImplementation());
	AIPerComp->OnPerceptionUpdated.AddDynamic(this, &AFPSVip::OnSensed);

	CurrentVelocity = FVector::ZeroVector;
	MovementSpeed = 375.0f;

	DistanceSquared = BIG_NUMBER;
}

// Called when the game starts or when spawned
void AFPSVip::BeginPlay()
{
	Super::BeginPlay();

	DamageCollision->OnComponentBeginOverlap.AddDynamic(this, &AFPSVip::OnHit);

	BaseLocation = this->GetActorLocation();
	
}

// Called every frame
void AFPSVip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!CurrentVelocity.IsZero())
	{
		Newlocation = GetActorLocation() + CurrentVelocity * DeltaTime;

		if (BackToBaseLocation)
		{
			if ((Newlocation - BaseLocation).SizeSquared2D() < DistanceSquared)
			{
				DistanceSquared = (Newlocation - BaseLocation).SizeSquared2D();
			}
			else
			{
				CurrentVelocity = FVector::ZeroVector;
				DistanceSquared = BIG_NUMBER;
				BackToBaseLocation = false;

				SetNewRotation(GetActorForwardVector(), GetActorLocation());
			}
		}

		SetActorLocation(Newlocation);

	}
}

// Called to bind functionality to input
void AFPSVip::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


void AFPSVip::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bForSweep, const FHitResult& Hit)
{
	AFPSCharacter* Char = Cast<AFPSCharacter>(OtherActor);
	if (Char)
	{
		Char->DealDamage(DamageValue);
	}
}

void AFPSVip::OnSensed(const TArray<AActor*>& UpdatedActors)
{
	for (int i = 0; i < UpdatedActors.Num(); i++)
	{
		FActorPerceptionBlueprintInfo Info;
		AIPerComp->GetActorsPerception(UpdatedActors[i], Info);
		AFPSCharacter* Char = Cast<AFPSCharacter>(UpdatedActors[i]); //Sets a pointer to the players character

		if (Info.LastSensedStimuli[0].WasSuccessfullySensed()) //Checls if the last dectetion of an actor was successful
		{
			if (UpdatedActors[i] == Char) //Checks if the player was the last detected actor
			{
				FVector dir = UpdatedActors[i]->GetActorLocation() - GetActorLocation();
				dir.Z = 0.0f;

				CurrentVelocity = dir.GetSafeNormal() * MovementSpeed;

				SetNewRotation(UpdatedActors[i]->GetActorLocation(), GetActorLocation());
			}
		}
	}
}

void AFPSVip::SetNewRotation(FVector TargetPosition, FVector CurrentPosition)
{
	FVector NewDirection = TargetPosition - CurrentPosition;
	NewDirection.Z = 0.0f;

	EnemyRotation = NewDirection.Rotation();

	SetActorRotation(EnemyRotation);


}

void AFPSVip::DealDamage(float DamageAmount)
{
	Health -= DamageAmount;

	if (Health <= 0.0f)
	{
		//Restart game


		Destroy();

	}
}