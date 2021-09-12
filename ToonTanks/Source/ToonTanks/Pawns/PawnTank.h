// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PawnBase.h"
#include "PawnTank.generated.h"

class USpringArmComponent;
class UCameraComponent;
/**
 * 
 */
UCLASS()
class TOONTANKS_API APawnTank : public APawnBase
{
	GENERATED_BODY()

private:

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Components", meta = (AllowPrivateAccess="true"))
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Components", meta = (AllowPrivateAccess="true"))
	UCameraComponent* Camera;

	FVector MovementDirection;
	FQuat ROtationDirection; 
	
	UPROPERTY(EditAnyWhere, BluePrintReadWrite, Category = "Movement", meta = (AllowPrivateAccess="true"))
	float MoveSpeed = 1000.f;
	UPROPERTY(EditAnyWhere, BluePrintReadWrite, Category = "Movement", meta = (AllowPrivateAccess="true"))
	float RotateSpeed = 300.f;

	// Add Sound
	UPROPERTY(EditAnywhere, Category="Effects")
	USoundBase* TankTrackSound;

	APlayerController *PlayerContorllerRef;
	bool bIsPlayerAlive = true;

	void CalculateMoveInput(float Value);
	void CalculateRotateInput(float Value);

	void Move();
	void Rotate();

public:
	 
	APawnTank();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void HandleDestruction() override;

	bool GetIsPlayerAlive();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
