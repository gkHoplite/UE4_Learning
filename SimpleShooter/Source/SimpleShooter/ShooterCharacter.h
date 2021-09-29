// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterCharacter.generated.h"

class AGun;

UCLASS()
class SIMPLESHOOTER_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Overriding
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);

	UFUNCTION(BlueprintPure)
	bool IsDead() const;
	
	void Shoot(); // It was used in BTTask_Shoot, So public.


	UFUNCTION(BlueprintPure)
	float GetHealthPercent() const;
	
private:
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void LookUpRate(float AxisValue);
	void LookRightRate(float AxisValue);
	//void LookUp(float AxisValue);
	//void LookRight(float AxisValue);
	void ToggleWalk();

	UPROPERTY()
	bool isWalk = false;

	UPROPERTY(EditAnywhere)
	float RotationRate = 70;

	// It was only used in BeginPlay, Later Don' have to be Modified
	UPROPERTY(EditDefaultsOnly)
	// Only Allowed for Derived BP_Class from 'AGun.h'
	TSubclassOf<AGun> GunClass;

	UPROPERTY(EditDefaultsOnly)
	// Only Allowed for Derived BP_Class from 'AGun.h'
	TSubclassOf<AGun> GunClass2;

	UPROPERTY()
	AGun* Gun;

	UPROPERTY(EditDefaultsOnly) // It block to modifying it after beginplay
	float MaxHealth = 100.f;

	UPROPERTY(VisibleAnywhere) // For Debugging.
	float Health = 1.f;

};
