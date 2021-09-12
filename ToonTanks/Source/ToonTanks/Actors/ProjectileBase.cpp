// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileBase.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "Kismet/GameplayStatics.h" // For UGameplayStatics::ApplyDamage
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AProjectileBase::AProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));

	// From this code, Every class Instanced from this have 'OnComponentHit'
	// It register every collision mesh was hit
	// Whenever hit, you would execute Onhit Function.
	ProjectileMesh->OnComponentHit.AddDynamic(this, &AProjectileBase::OnHit);

	// this didn't belong to Scene hierarchy
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	
	// If you want to needs velocity, set this values diffrently.
	ProjectileMovement->InitialSpeed = MovementSpeed;
	ProjectileMovement->MaxSpeed = MovementSpeed;
	// After Initialized three seconds, It will be removed. 
	InitialLifeSpan = 3.0f;

	ParticleTrail = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle Trail"));
	ParticleTrail->SetupAttachment(ProjectileMesh);
}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	UGameplayStatics::PlaySoundAtLocation(this, LaunchSound, GetActorLocation());
}

// No Usage on here
// void AProjectileBase::Tick(float DeltaTime)
// {
// 	Super::Tick(DeltaTime);
// }


// This Function is Called Whenver StaticMeshComponent's OnComponentHit function is Called
void AProjectileBase::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
				FVector NormalImpulse, const FHitResult& Hit)
{
	AActor* MyOwner = GetOwner();
	if(!MyOwner){ return; }
	
	// OtherActor == this : Actor could has various components. if two components has collider and hit, Ignore!
	if(OtherActor && OtherActor != this && MyOwner != OtherActor)
	{
		// UE_LOG(LogTemp, Display, TEXT("Hit comp: %s, OtherAc: %s, OtherComp: %s, Hit Ac: %s "), *HitComp->GetName(), *OtherActor->GetName(), *OtherComp->GetName(), *Hit.GetActor()->GetName());

	// @param DamagedActor - Actor that will be damaged.
	// @param BaseDamage - The base damage to apply.
	// @param EventInstigator - Controller that was responsible for causing this damage
	// 						  - To seperate player's Team, or Checking this Event was done between A.I
	// @param DamageCauser - Actor that actually caused the damage (e.g. the grenade that exploded)
	// @param DamageTypeClass - Class that describes the  damage that was done.
	// @return Actual damage the ended up being applied to the actor.
		UGameplayStatics::ApplyDamage(OtherActor, Damage, MyOwner->GetInstigatorController(), this, DamageType);
		UGameplayStatics::SpawnEmitterAtLocation(this, HitParticle, GetActorLocation());
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());
		GetWorld()->GetFirstPlayerController()->ClientPlayCameraShake(HitShake);
		Destroy();
	}
}