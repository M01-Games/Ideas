// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "FPSProjectile.h"
#include "FPSCharacter.generated.h"

UCLASS()
class IDEAS_API AFPSCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPSCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Projectile class to spawn.
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class AFPSProjectile> ProjectileClass;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Handles input for moving forward and backward.
	UFUNCTION()
		void MoveForward(float Value);

	// Handles input for moving right and left.
	UFUNCTION()
		void MoveRight(float Value);

	// Sets jump flag when key is pressed.
	UFUNCTION()
		void StartJump();

	// Clears jump flag when key is released.
	UFUNCTION()
		void StopJump();

	// Sets Sprint flag when key is pressed.
	UFUNCTION()
		void StartSprint();

	// Clears Sprint flag when key is released.
	UFUNCTION()
		void StopSprint();

	// Sprint flag set to false.
	bool bPressedSprint = false;

	// Sets Sprint flag when key is pressed.
	UFUNCTION()
	void StartCrouch();

	// Clears Sprint flag when key is released.
	UFUNCTION()
	void StopCrouch();

	// Sprint flag set to false.
	bool bPressedCrouch = false;

	//Reloads the weapon
	void ReloadWeapon();

	// Function that handles firing projectiles.
	UFUNCTION()
		void Fire();

	//The total amount of ammo that can be carried for the weapon. 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		int maxTotalAmmo;

	//The total amount of ammo that can be in the weapon.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon) 
		int maxClipAmmo;

	//The total amount of ammo being carried for the weapon. 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		int totalAmmo;

	//The amount of ammo in the weapon.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		int clipAmmo;

	//The time it takes to reload the weapon.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		float reloadTime;

	// FPS camera.
	UPROPERTY(VisibleAnywhere)
		UCameraComponent* FPSCameraComponent;

	// First-person mesh (arms), visible only to the owning player.
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		USkeletalMeshComponent* FPSMesh;

	// Gun mesh, visible only to the owning player.
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		USkeletalMeshComponent* GunMesh;

	// Gun muzzle offset from the camera location.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector MuzzleOffset;

	// The players full health
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Health = 100.0f;

	// Sprint multipler that will be added to the characters movement.
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float SprintSpeedMultiplier;

	void DealDamage(float DamageAmount);

	void HealPlayer(float HealAmount);
};
