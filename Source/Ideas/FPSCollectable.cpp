// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSCollectable.h"

#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "FPSCharacter.h"

// Sets default values
AFPSCollectable::AFPSCollectable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Creates the staticmesh
	CollectableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Collectable Mesh"));
	RootComponent = CollectableMesh;

	//Creates the Collision box
	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Component"));
	CollisionComponent->SetupAttachment(CollectableMesh);

}

// Called when the game starts or when spawned
void AFPSCollectable::BeginPlay()
{
	Super::BeginPlay();
	
	//CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AFPSCollectable::OnHit);
}

// Called every frame
void AFPSCollectable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void AFPSCollectable::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	AFPSCharacter* Char = Cast<AFPSCharacter>(OtherActor);
	if (medKit == true)
	{
		if (Char->Health != 100)
		{
			Char->HealPlayer(HealIncrease);
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("HEALTH"));
			if (Char->Health > 100)
			{
				Char->Health = 100;
			}
		}
	}
	else if (ammoBox == true)
	{
		Char->totalAmmo += Char->maxClipAmmo;
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("AMMO"));
	}
	else if (docs == true)
	{
		Char->docsCollected++;
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("DOCS"));
	}
	
}
