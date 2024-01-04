// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSCharacter.h"
#include "IdeasGameModeBase.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AFPSCharacter::AFPSCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a first person camera component.
	FPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	check(FPSCameraComponent != nullptr);

	// Attach the camera component to our capsule component.
	FPSCameraComponent->SetupAttachment(CastChecked<USceneComponent, UCapsuleComponent>(GetCapsuleComponent()));

	// Position the camera slightly above the eyes.
	FPSCameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f + BaseEyeHeight));

	// Enable the pawn to control camera rotation.
	FPSCameraComponent->bUsePawnControlRotation = true;

	// Create a first person mesh component for the owning player.
	FPSMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
	check(FPSMesh != nullptr);

	// Only the owning player sees this mesh.
	FPSMesh->SetOnlyOwnerSee(true);

	// Attach the FPS mesh to the FPS camera.
	FPSMesh->SetupAttachment(FPSCameraComponent);

	// Disable some environmental shadows to preserve the illusion of having a single mesh.
	FPSMesh->bCastDynamicShadow = false;
	FPSMesh->CastShadow = false;

	// Create a first person mesh component for the owning player.
	GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMesh"));
	check(GunMesh != nullptr);

	// Only the owning player sees this mesh.
	GunMesh->SetOnlyOwnerSee(true);

	// Disable some environmental shadows to preserve the illusion of having a single mesh.
	GunMesh->bCastDynamicShadow = false;
	GunMesh->CastShadow = false;

	// The owning player doesn't see the regular (third-person) body mesh.
	GetMesh()->SetOwnerNoSee(true);

	//Setting Gun Values
	maxTotalAmmo = 120;
	maxClipAmmo = 30;
	totalAmmo = 120;
	clipAmmo = 30;
	reloadTime = 1.0f;
}

// Called when the game starts or when spawned
void AFPSCharacter::BeginPlay()
{
	Super::BeginPlay();

	check(GEngine != nullptr);

	// Display a debug message for five seconds. 
	// The -1 "Key" value argument prevents the message from being updated or refreshed.
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("We are using FPSCharacter."));

	GunMesh->AttachToComponent(FPSMesh,
		FAttachmentTransformRules::SnapToTargetNotIncludingScale,
		TEXT("b_RightWeapon"));

	GunMesh->SetRelativeLocation(FVector(3.985038f, 0.763133f, 5.375207f));
	GunMesh->SetRelativeRotation(FRotator(71.406314f, 124.010001f, 33.820759f));

	SprintSpeedMultiplier = 2.0f;
}

// Called every frame
void AFPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up "movement" bindings.
	PlayerInputComponent->BindAxis("MoveForward", this, &AFPSCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFPSCharacter::MoveRight);

	// Set up "look" bindings.
	PlayerInputComponent->BindAxis("Turn", this, &AFPSCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &AFPSCharacter::AddControllerPitchInput);

	// Set up "action" bindings.
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AFPSCharacter::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AFPSCharacter::StopJump);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AFPSCharacter::StartSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AFPSCharacter::StopSprint);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AFPSCharacter::StartCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &AFPSCharacter::StopCrouch);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AFPSCharacter::Fire);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AFPSCharacter::ReloadWeapon);

}

void AFPSCharacter::MoveForward(float Value)
{
	// Find out which way is "forward" and record that the player wants to move that way.
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	if (!bPressedSprint)
		Value *= 0.3;
	AddMovementInput(Direction, Value);
}

void AFPSCharacter::MoveRight(float Value)
{
	// Find out which way is "right" and record that the player wants to move that way.
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	if (!bPressedSprint)
		Value *= 0.3;
	AddMovementInput(Direction, Value);
}

void AFPSCharacter::StartJump()
{
	bPressedJump = true;
}

void AFPSCharacter::StopJump()
{
	bPressedJump = false;
}

void AFPSCharacter::StartSprint()
{
	bPressedSprint = true;
}

void AFPSCharacter::StopSprint()
{
	bPressedSprint = false;
}

void AFPSCharacter::StartCrouch()
{
	bPressedCrouch = true;
	SetActorRelativeScale3D(FVector(1.0, 1.0, 0.75));
	GunMesh->SetRelativeLocation(FVector(3.663145, 2.116888, 8.020947));
	GunMesh->SetRelativeRotation(FRotator(74.528212, 131.925329, 41.418362));
}

void AFPSCharacter::StopCrouch()
{
	bPressedCrouch = false;
	SetActorRelativeScale3D(FVector(1.0, 1.0, 1.0));
	GunMesh->SetRelativeLocation(FVector(3.985038f, 0.763133f, 5.375207f));
	GunMesh->SetRelativeRotation(FRotator(71.406314f, 124.010001f, 33.820759f));
}

void AFPSCharacter::ReloadWeapon()
{
	if (clipAmmo != maxClipAmmo)
	{
		if (totalAmmo - (maxClipAmmo - clipAmmo) >= 0)
		{
			totalAmmo -= (maxClipAmmo - clipAmmo);
			clipAmmo = maxClipAmmo;
		}
		else
		{
			clipAmmo += totalAmmo;
			totalAmmo = 0;
		}
	}
}

void AFPSCharacter::Fire()
{
	if (clipAmmo > 0)
	{
		clipAmmo -= 1;
		// Attempt to fire a projectile.
		if (ProjectileClass)
		{
			// Get the camera transform.
			FVector CameraLocation;
			FRotator CameraRotation;
			GetActorEyesViewPoint(CameraLocation, CameraRotation);

			// Set MuzzleOffset to spawn projectiles slightly in front of the camera.
			MuzzleOffset.Set(100.0f, 0.0f, -30.0f);

			// Transform MuzzleOffset from camera space to world space.
			FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);

			// Skew the aim to be slightly upwards.
			FRotator MuzzleRotation = CameraRotation;
			MuzzleRotation.Pitch += 5.0f;

			UWorld* World = GetWorld();
			if (World)
			{
				FActorSpawnParameters SpawnParams;
				SpawnParams.Owner = this;
				SpawnParams.Instigator = GetInstigator();

				// Spawn the projectile at the muzzle.
				AFPSProjectile* Projectile = World->SpawnActor<AFPSProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
				if (Projectile)
				{
					// Set the projectile's initial trajectory.
					FVector LaunchDirection = MuzzleRotation.Vector();
					Projectile->FireInDirection(LaunchDirection);
				}
			}
		}
	}
	else if (totalAmmo > 0) 
	{
		ReloadWeapon();
	}
	else 
	{
		//Restart game
		AIdeasGameModeBase* MyGameMode = Cast<AIdeasGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

		if (MyGameMode)
		{
			MyGameMode->RestartGamePlay(false);
		}

		Destroy();
	}
}

void AFPSCharacter::DealDamage(float DamageAmount)
{
	Health -= DamageAmount;

	if (Health <= 0.0f) 
	{
		//Restart game
		AIdeasGameModeBase* MyGameMode = Cast<AIdeasGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

		if (MyGameMode)
		{
			MyGameMode->RestartGamePlay(false);
		}

		Destroy();

	}
}
