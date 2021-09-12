// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileBase.generated.h"

class UProjectileMovementComponent;

UCLASS()
class TOONTANKS_API AProjectileBase : public AActor
{
	GENERATED_BODY()

private:
// COMPONENTS
	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category="Components", meta = (AllowPrivateAccess="true"))
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category="Components", meta = (AllowPrivateAccess="true"))
	UStaticMeshComponent* ProjectileMesh;

	// This is such like Scene Components, attachable on Actor
	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category="Components", meta = (AllowPrivateAccess="true"))
	UParticleSystemComponent* ParticleTrail;

// VARIABLES
	// Just a way to store a type specificly for UObject.
	// Provide Type Safe access to specific class pointers. We would use Inherited class of UDamageType
	UPROPERTY(EditDefaultsOnly, Category="Damage")
	TSubclassOf<UDamageType> DamageType;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category="Move", meta = (AllowPrivateAccess="true"))
	float MovementSpeed = 4000.f;
	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category="Damage", meta = (AllowPrivateAccess="true"))
	float Damage = 50.f;
	
	UPROPERTY(EditAnywhere, Category="Effects")
	UParticleSystem* HitParticle;
	UPROPERTY(EditAnywhere, Category="Effects")
	USoundBase* HitSound;
	UPROPERTY(EditAnywhere, Category="Effects")
	USoundBase* LaunchSound;
	
	UPROPERTY(EditAnywhere, Category="Effects")
	TSubclassOf<UCameraShake> HitShake;
// FUNCTIONS
	/*
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams( 
		FComponentHitSignature, UPrimitiveComponent*, HitComponent, AActor*, OtherActor, 
		UPrimitiveComponent*, OtherComp, FVector, NormalImpulse, const FHitResult&, Hit );
	*/
	UFUNCTION() // This Macro is for Delegate Function
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
				FVector NormalImpulse, const FHitResult& Hit); 
	// Last two argument is for Extrathings for more Complexed Usage.


public:	
	// Sets default values for this actor's properties
	AProjectileBase();

	// You don't needs the function in projectile base.
	// virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
