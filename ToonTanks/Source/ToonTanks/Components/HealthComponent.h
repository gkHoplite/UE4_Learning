// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

class ATankGameModeBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOONTANKS_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float DefaultHealth = 100.f;

	// Health would be purely updated in c++ based on Default Health.
	float Health = 0.f;

	// GameMode is for Printing out Message on the screen.
	ATankGameModeBase* GameModeRef;

private:


public:	
	// Sets default values for this component's properties
	UHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Damage Events for the owning Pawn
	// Implementing this Like AProjectileBase::OnHit. Just follow the Macro
	UFUNCTION() // This is Delegate So you needs this.
	void TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* InstigatedBy, AActor* DamageCauser);
};
