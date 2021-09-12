// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PawnBase.generated.h"

// Forward Declaration, Include file will be imported on code file
// It reduce compile time
class UCapsuleComponent; 
class AProjectileBase;
class UHealthComponent;

UCLASS()
class TOONTANKS_API APawnBase : public APawn
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category="Components", meta = (AllowPrivateAccess="true")) // Collision
	UCapsuleComponent* CapsuleComp;

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category="Components", meta = (AllowPrivateAccess="true")) // Mesh For Turret Head and Base
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category="Components", meta = (AllowPrivateAccess="true"))
	UStaticMeshComponent* TurretMesh;

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category="Components", meta = (AllowPrivateAccess="true")) // USceneComponent is Just Collection of Data
	USceneComponent* ProjectileSpawnPoint; // For Visuality Where to start Projectile

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category="Components", meta = (AllowPrivateAccess="true")) // USceneComponent is Just Collection of Data
	UHealthComponent* HealthComponent;

	UPROPERTY(EditAnywhere, Category = "Effects")
	UParticleSystem* DeathParticle;

	UPROPERTY(EditAnywhere, Category="Effects")
	USoundBase* DeathSound;

	UPROPERTY(EditAnywhere, Category="Effects")
	TSubclassOf<UCameraShake> DeathShake;

public:
	// Sets default values for this pawn's properties
	APawnBase();

	virtual void HandleDestruction();

protected:
	// Overriden or added implementation on child class
	void RotateTurret(FVector LookAtTarget);
	
	void Fire();

// Projectile
private:
	// Filtering Only SubClass of AProjectileBase
	UPROPERTY(EditAnywhere, BluePrintReadOnly, Category = "Projectile Type", meta = (AllowPrivateAccess="true"))
	TSubclassOf<AProjectileBase> ProjectileClass;
};
