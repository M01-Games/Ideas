// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSCollectable.generated.h"

UCLASS()
class IDEAS_API AFPSCollectable : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFPSCollectable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* CollectableMesh;

	UPROPERTY(EditAnywhere)
		class UBoxComponent* CollisionComponent;

	//Is the collectable a medkit 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool medKit = false;

	//Is the collectable an ammobox 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool ammoBox = false;

	//Is the collectable a document
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool docs = false;

	//Is the Health increase
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float HealIncrease = 25.0f;
	
	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& Hit);
};
