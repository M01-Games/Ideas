// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSExtraction.h"

#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "FPSVip.h"
#include "IdeasGameModeBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFPSExtraction::AFPSExtraction()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Creates the staticmesh
	ExtractionMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door Mesh"));
	RootComponent = ExtractionMesh;

	//Creates the Collision box
	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Component")); 
	CollisionComponent->SetupAttachment(ExtractionMesh);
}

// Called when the game starts or when spawned
void AFPSExtraction::BeginPlay()
{
	Super::BeginPlay();
	
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AFPSExtraction::OnHit);
}

// Called every frame
void AFPSExtraction::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFPSExtraction::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	AFPSVip* vip = Cast<AFPSVip>(OtherActor);

	if (vip)
	{
		//Restart game
		AIdeasGameModeBase* MyGameMode = Cast<AIdeasGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

		if (MyGameMode)
		{
			MyGameMode->RestartGamePlay(false);
		}
	}
}

