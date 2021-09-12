// Fill out your copyright notice in the Description page of Project Settings.

#include "PawnBase.h"
#include "Components/CapsuleComponent.h"
#include "ToonTanks/Actors/ProjectileBase.h"
#include "Components/HealthComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
APawnBase::APawnBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	// CreateDefaultSubobject = Just Add Components within this Class

	// Constructor for Collider
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	RootComponent = CapsuleComp; // Every Actor has Root Components 

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(RootComponent);

	TurretMesh  = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	TurretMesh ->SetupAttachment(BaseMesh);

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point"));
	ProjectileSpawnPoint-> SetupAttachment(TurretMesh);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));
}

// Update TurretMesh Rotation to face towards the LookAtTarget passed in from Child Classes.
void APawnBase::RotateTurret(FVector LookAtTarget)
{
	FVector LookAtTargetCleaned = FVector(LookAtTarget.X, LookAtTarget.Y, TurretMesh->GetComponentLocation().Z);
	FVector StartLocation = TurretMesh->GetComponentLocation();
	FRotator TurretRotation = FVector(LookAtTargetCleaned-StartLocation).Rotation();
	TurretMesh->SetWorldRotation(TurretRotation);
}

void APawnBase::Fire()
{
	// Get ProjectileSpawnPoint Location && Rotation -> Spawn Projectile Class at Location firing towards Rotation.
	if(ProjectileClass)
	{
		FVector SpawnLocation = ProjectileSpawnPoint->GetComponentLocation();
		FRotator SpawnRotation = ProjectileSpawnPoint->GetComponentRotation();
		AProjectileBase* TempProjectile = GetWorld()->SpawnActor<AProjectileBase>(ProjectileClass, SpawnLocation, SpawnRotation);
		TempProjectile->SetOwner(this);
		
		// for(int i=0; i<10; ++i)
		// {
		// 	if( i*20 == 60 ){ continue; }
		// 	AProjectileBase* TempProjectile2 = GetWorld()->SpawnActor<AProjectileBase>(ProjectileClass, SpawnLocation + FVector(0,0,20+(i*20)), SpawnRotation);
		// 	AProjectileBase* TempProjectile3 = GetWorld()->SpawnActor<AProjectileBase>(ProjectileClass, SpawnLocation + FVector(0,300,20+(i*20)), SpawnRotation);
		// 	TempProjectile2->SetOwner(this);
		// 	TempProjectile3->SetOwner(this);
		// }


	    // UE_LOG(LogTemp, Display, TEXT("%s is Firing"), *GetActorLabel());
	}
}

// It was used in "ATankGameModeBase::ActorDied"
// overrided in each Pawn Class
void APawnBase::HandleDestruction()
{
	// --Universal Functionality --
	// Play death effects particle, sound and camera shake.

	// Executing this on default would result roation (0,0,0)
	UGameplayStatics::SpawnEmitterAtLocation(this, DeathParticle, GetActorLocation());

	UGameplayStatics::SpawnSoundAtLocation(this, DeathSound, GetActorLocation());
	
	GetWorld()->GetFirstPlayerController()->ClientPlayCameraShake(DeathShake);

	// GameMode Counts How many Turrets are reamin.
	// -- Then do child overrides --
	// --- Pawn Turret : Inform GameMode Turret died Then Destroy() self.
	// --- Pawn Tank   : Inform GameMode Player died Then Hide() all components && stop movement Input.

	// Why you hide Tank istead of Destory it?
	// Because If you do that Camera also Destroyed and It's quite messy to create again.
}