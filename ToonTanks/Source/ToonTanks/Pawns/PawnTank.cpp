// Fill out your copyright notice in the Description page of Project Settings.

#include "PawnTank.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h" 

// It's Not Necessary to include this components. But If you needs Reference, Use it!
// #include "Components/InputComponent.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

#define OUT

APawnTank::APawnTank()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Sprint Arm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}

// Called when the game starts or when spawned
void APawnTank::BeginPlay()
{
	Super::BeginPlay();
	// Find and Assign active Player Controller Converting this to APlayerConroller type
	PlayerContorllerRef = Cast<APlayerController>(GetController());
}

// Hiding instead of Destroying, Because we want to keep Cameracomponent.
void APawnTank::HandleDestruction()
{
	Super::HandleDestruction();
	
	// It not only detroy the Pawn but also Controller. 
	// It causes error because you would call 
	// Destroy(); Don't Use this Function. Just for Mark this codes reamins
	
	// Hide Player
	bIsPlayerAlive = false;
	// Hiding Every Visual Components attached to this actor.
	SetActorHiddenInGame(true);
	// Save resourse and Not updated in tick
	SetActorTickEnabled(false);
}

// You can get the value but can't set the value. 
bool APawnTank::GetIsPlayerAlive() 
{
	return bIsPlayerAlive;
}

// Called every frame
void APawnTank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// You can Change the Order Rotate-Move.
	// Which way is better depends on your feeling. 
	Rotate();
	Move();
	// You can Put this two Function on Cal Function Implement of InputComponet
	// Because it would exeute every frame, Everything is same as Before 
	// But extracting gives you control on action. You could use this Later on.

	if(PlayerContorllerRef)
	{
		FHitResult TraceHitResult;
		// Find the Cursor's Location
		// First Arguemnts is for checking in those condition. So only check on sight.
		// Second is Did you needs Complex collision? if not using simple Collision.
		// Third is Out Parameter
		// https://docs.unrealengine.com/4.27/en-US/InteractiveExperiences/Physics/Collision/Reference/
		PlayerContorllerRef->GetHitResultUnderCursor(ECC_Visibility, false, OUT TraceHitResult);

		// 'HitResult.Location' == 'HitResult.ImpactPoint' 
		FVector HitLocation = TraceHitResult.ImpactPoint; // Same as 

		RotateTurret(HitLocation);
	}
}

// Called to bind functionality to input
void APawnTank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// BindAxis - Considered when needs incremental changes
	// It always in work like ticks, If you didn't pressed anything then return Zero to Delegate Function.
	PlayerInputComponent->BindAxis("MoveForward", this, &APawnTank::CalculateMoveInput);
	PlayerInputComponent->BindAxis("Turn", this, &APawnTank::CalculateRotateInput);

	// BindAction - Single event Just turned on when you pressed.
	PlayerInputComponent->BindAction("Fire",IE_Pressed,this, &APawnTank::Fire);
}

void APawnTank::CalculateMoveInput(float Value)
{	// DeltatimeSeconds for Independancy with each frame
	MovementDirection = FVector(Value * MoveSpeed * GetWorld()->DeltaTimeSeconds, 0, 0);
}
void APawnTank::CalculateRotateInput(float Value)
{
	float RotateAmount = Value * RotateSpeed * GetWorld()->DeltaTimeSeconds;
	FRotator Rotation = FRotator(0,RotateAmount,0);
	ROtationDirection = FQuat(Rotation);
}
void APawnTank::Move()
{
	// bSweep = true : Sweep over when touching the collider
	// 			false : Phasing through when touching the collider
	AddActorLocalOffset(MovementDirection, true);
}
void APawnTank::Rotate()
{
	AddActorLocalRotation(ROtationDirection, true);
}