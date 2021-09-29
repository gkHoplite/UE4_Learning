// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "GameFramework/PlayerController.h"
#include "Components/InputComponent.h"
#include "Components/SkinnedMeshComponent.h" // For GetMesh()->HideBoneByName()
#include "Gun.h"
#include "Components/CapsuleComponent.h"
#include "SimpleShooterGameModeBase.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();


	// It was same like
	// TypeHolder ChooseOne = int
	// GunClass ChooseOne = AGun 
	Gun = GetWorld()->SpawnActor<AGun>(GunClass);

	GetMesh()->HideBoneByName(TEXT("weapon_R"),EPhysBodyOp::PBO_None); // Error UE4.25 and VSCode

	// You can check FAttachmentTransfromRules with "F12" and you can also add your cutom
	// Third Argument is ignored when Mesh is StaticMeshComponent, In this Case that is Skeletal
	Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform,TEXT("WeaponSocket"));

/*
	1. You can change above option like this, then it would work like a drone.
	Gun = GetWorld()->SpawnActor<AGun>(GunClass, FVector(0, 0, 100), FRotator::ZeroRotator);
	Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform,TEXT("WeaponSocket"));

	2. Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale,TEXT("WeaponSocket"));
	this Work as well like above, but it doesn't have any option.
*/

	// It's Interesting Between Actor's relationship still has hierarchy.
	// You can use it, and it is so useful for implemnting 	multiplayer and in damage functionality
	// If you considering multiplayer, The 'this' have to be changed.
	Gun->SetOwner(this);

	Health = MaxHealth;
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AShooterCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &APawn::AddControllerYawInput);

	// For GamePad Controller
	PlayerInputComponent->BindAxis(TEXT("LookUpRate"), this, &AShooterCharacter::LookUpRate);
	PlayerInputComponent->BindAxis(TEXT("LookRightRate"), this, &AShooterCharacter::LookRightRate);

	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Walk"), EInputEvent::IE_Pressed, this, &AShooterCharacter::ToggleWalk);
	PlayerInputComponent->BindAction(TEXT("Shoot"), EInputEvent::IE_Pressed, this, &AShooterCharacter::Shoot);
}

float AShooterCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) 
{
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	DamageToApply = FMath::Min(Health, DamageToApply);
	
	Health -= DamageToApply;

	UE_LOG(LogTemp, Warning, TEXT("Health : %f"), Health);

	if(IsDead())
	{
		// This Should have to be located front of DetachFromControllerPendingDestroy
		// else Controller will be destroy, and Pawnkill method didn't catch it always.
		ASimpleShooterGameModeBase *GameMode = GetWorld()->GetAuthGameMode<ASimpleShooterGameModeBase>();
		if(GameMode != nullptr){ GameMode->PawnKilled(this); }

		DetachFromControllerPendingDestroy(); 

		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	return DamageToApply;
}	

bool AShooterCharacter::IsDead() const
{
	return Health <= 0;
}

void AShooterCharacter::MoveForward(float AxisValue)
{
	if(isWalk)
	{
		AddMovementInput(GetActorForwardVector() * AxisValue * 0.1);
	}
	else
	{
		AddMovementInput(GetActorForwardVector() * AxisValue);
	}
}

void AShooterCharacter::MoveRight(float AxisValue)
{
	if(isWalk)
	{
		AddMovementInput(GetActorRightVector() * AxisValue * 0.1);

	}
	else
	{
		AddMovementInput(GetActorRightVector() * AxisValue);
	}
}

void AShooterCharacter::LookUpRate(float AxisValue) 
{
	AddControllerPitchInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::LookRightRate(float AxisValue) 
{
	AddControllerYawInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::ToggleWalk() 
{
	if(isWalk)
	{
		isWalk = false;
	}
	else
	{
		isWalk = true;
	}
}

void AShooterCharacter::Shoot() 
{
	Gun->PullTrigger();
}

float AShooterCharacter::GetHealthPercent() const
{
	return Health / MaxHealth;
}


//void AShooterCharacter::LookRight(float AxisValue)
//{
//	AddControllerPitchInput(AxisValue);
//}
//void AShooterCharacter::LookUp(float AxisValue)
//{
//	AddControllerPitchInput(AxisValue);
//}

