// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "GameModes/TankGameModeBase.h"
#include "Kismet/GameplayStatics.h"
// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false; // Care this!
	// ...
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	Health = DefaultHealth; 
	GameModeRef = Cast<ATankGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	// Whenever the OnTakeAnyDamage is Called, TakeDamage Function will all so be called.
	// Add this function on Beginplay instead of Constructer.
	// It was relates "AProjectileBase::OnHit" Belong to that 
	// "UGameplayStatics::ApplyDamage" is implemeted 
	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::TakeDamage);
	// If you pass TakeDamage(), it would execute and pass the result as a argument.
}

void UHealthComponent::TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* InstigatedBy, AActor* DamageCauser) 
{
	if(Damage == 0 || Health <= 0 ){ return; }
	// Blocking Cheat to get the Health beyond Max.
	// FMath Can't Convert Integaer to Float, So you needs to clarify it.
	Health = FMath::Clamp(Health - Damage, 0.f, DefaultHealth);
	
	UE_LOG(LogTemp, Warning, TEXT("Health: %f"), Health);

	// I know Health value can't be reached to Negative 
	// Just for implementation that will be done in the futrue for death state.
	if(Health <= 0)
	{
		if(!GameModeRef)
		{
			UE_LOG(LogTemp, Warning, TEXT("Health Components has No GameModeRef "));
			return; 
		}
		GameModeRef->ActorDied(GetOwner());
	}

	// You didn't pass any arguments to Owner of this script For recording Death State.
	// Because this game is so simple no needs to do that.
	// And it was considered in GameMode The Way to detect the destroying.
}