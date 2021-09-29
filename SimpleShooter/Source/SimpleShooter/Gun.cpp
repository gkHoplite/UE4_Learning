// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h" // For Display ViewPort

#define OUT

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
}

void AGun::PullTrigger() 
{
	// UE_LOG(LogTemp, Warning, TEXT("You are Shooting"));
	
	// Mesh is Type of USkeletalMesh derived from USceneComponent 
	// FName = You can see the name of it in BP_edit of hierarchy
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, TEXT("MuzzleFlashSocket"));
	UGameplayStatics::SpawnSoundAttached(MuzzleSound, Mesh, TEXT("MuzzleFlashSocket"));

	FHitResult Hit;
	FVector ShotDirection;
	// Pass Params on LineTracing
	bool bSuccess = GunTrace(Hit,ShotDirection);
	if( bSuccess)
	{
		// Where this shot is coming from.
		// DrawDebugPoint(GetWorld(), Hit.Location, 20, FColor::Red, true);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),
		ImpactEffect, Hit.Location, ShotDirection.Rotation());

		UGameplayStatics::SpawnSoundAtLocation(GetWorld(),
		ImpactSound, Hit.Location);

		AActor* HitActor = Hit.GetActor();
		if( HitActor != nullptr )
		{
			// Constructor For FPointDamageEvent
			FPointDamageEvent DamageEvent(Damage, Hit, ShotDirection, nullptr);

			AController *OwnerController = GetOwnerController();
			if(OwnerController == nullptr) return;

			// HitActor->TakeDamage(Damage, DamageEvent, GetWorld()->GetFirstPlayerController(), this);
			HitActor->TakeDamage(Damage, DamageEvent, OwnerController, this);

			UE_LOG(LogTemp, Warning, TEXT("Hit and %s"), *Hit.GetActor()->GetName());
		}
	}
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AGun::GunTrace(FHitResult &Hit, FVector &ShotDirection) 
{
	AController *OwnerController = GetOwnerController();
	if(OwnerController == nullptr) return false;

	FVector Location;
	FRotator Rotation;
	OwnerController->GetPlayerViewPoint(OUT Location, OUT Rotation);
	
	// Where this shot is coming from.
	ShotDirection = -Rotation.Vector();
	FVector End = Location + Rotation.Vector() * MaxRange;
	// Showing Camera
	// Fourth Argument is Field of view in this case refering 90 degrees.
	// DrawDebugCamera(GetWorld(), Location, Rotation,90.f, 1.f, FColor::Red, true);


	// You have to use GetWorld() for LineTrace() function.
	// If you struggle with arguments Just Press F12 on that.
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());

	return GetWorld()->LineTraceSingleByChannel(OUT Hit, Location, End, ECollisionChannel::ECC_GameTraceChannel1, Params);

}

AController* AGun::GetOwnerController() const
{
	// The Owner of Gun is ShooterCharacter. See this on AShooterCharacter::BeginPlay()
	// Only Pawn has Controller that was implemented implicitly
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if(OwnerPawn == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("OwnerPawn is nullptr"));
		return nullptr;
	}
	return OwnerPawn->GetController();
}