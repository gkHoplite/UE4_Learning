// Fill out your copyright notice in the Description page of Project Settings.

// Aligning order Alphabetically
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Grabber.h" // this module have to be bottom for generator // I don't know whether this component have to be place in fisrt or last

#define OUT // Marking For Out Parameters. It means nothing to complier, so ignored

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// This boolean variable allow components to be ticked every frame. 
	// You can turn these features off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;	
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Display, TEXT("Initializing Grabber!"));

	FindPhysicHandle();
	SetupInputComponent();
}

void UGrabber::FindPhysicHandle() // Check For Physics Handle Component
{
	// Function Templates has Brackets that is for generics.
	// Function 'FindComponentsByClass' would return The First things they find.
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("Physics Handle didn't exist in '%s'"), *GetOwner()->GetName());
	}
}

void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->InputComponent;
	if ( InputComponent )
	{
		UE_LOG(LogTemp, Display, TEXT("Input Components is On '%s'"), *GetOwner()->GetName());

		// BindAction(FName ActionName, EInputEvent KeyEvent, UGrabber *Object, void (UGrabber::*Func)
		// FName : This Variable coordinate with 'Input Settings', Its Interesting this Doesn't use TEXT Macro
		// KeyEvent : Set What kind of action did you choose. [ IE_Pressed, IE_Released, IE_Repeat]
		// *Func : the Function excuted when you press the key. If you add Parenthesis, It would call the Function
		// 		   else, it just let complier know, where to go.
		InputComponent->BindAction("Grab",IE_Pressed, this, &UGrabber::Grap);
		InputComponent->BindAction("Grab",IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Input Components isn't exist in '%s'"), *GetOwner()->GetName());
	}
}

void UGrabber::Grap()
{
	UE_LOG(LogTemp, Display, TEXT("Pressed Input 'Grap'")); 
	FHitResult HitResult = GetFirstPhysicsBodyInput();
	UPrimitiveComponent *ComponentToGrab = HitResult.GetComponent();
	
	//If we hit something then attach the physics handle
	if  (HitResult.GetActor() )
	{
		FVector LineTracerEnd = PlayerViewpointVector + PlayerViewpointRotator.Vector() * Reach;
		
		if(!PhysicsHandle){return;}
		PhysicsHandle->GrabComponentAtLocation(ComponentToGrab, NAME_None, LineTracerEnd);
	}
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Display, TEXT("Released Input 'Grap'"));

	if(!PhysicsHandle){return;}
	PhysicsHandle->ReleaseComponent();
}

FHitResult UGrabber::GetFirstPhysicsBodyInput()
{
	FHitResult Hit;
	// Ray-cast out for a certain distance
	FCollisionQueryParams Params(FName(TEXT("")),false, GetOwner());
	// FCollisionQueryParams(FName InTraceTag, bool bInTraceComplex, const AActor*)
	// You can get info in Ctrl+Click or Seaching on Unreal Document.
	// Fname = You can use this name for check what kinds of Components Do we hit, 
	//		   in this case We will use EColiisonChannel Instead, So We woudn't use it
	// bIntraceComplex = If you want to use some complex work like, It only works after transparent one object, and then
	//					 Query would work this was done before.
	// Actor = Your Collision also work on your own character components, you have to ignore this or it woudn't work as expected
	
	UpdatePlayersViewPoint();
	FVector LineTracerEnd = PlayerViewpointVector + PlayerViewpointRotator.Vector() * Reach;
	GetWorld()->LineTraceSingleByObjectType(OUT Hit, PlayerViewpointVector, LineTracerEnd, ECollisionChannel::ECC_PhysicsBody, Params);

	// Log for checking whether Laytracing meeting a object
	AActor* ActorHit = Hit.GetActor();
	if (ActorHit)
	{
		UE_LOG(LogTemp, Display, TEXT("Line Tracer Hit %s!"),*ActorHit->GetName() );
	}
	return Hit;
}



// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	IsUpdated = false;
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// FVector LineTracerEnd = PlayerViewpointVector + PlayerViewpointRotator.Vector() * Reach;
	// DrawDebugLine(GetWorld(), PlayerViewpointVector, LineTracerEnd, FColor(0,255,0), bPersistentLines, 0.f, 0, 5.f);

	// If Physic handle is attached
	if(PhysicsHandle->GrabbedComponent)
	{
		UpdatePlayersViewPoint();
		FVector LineTracerEnd = PlayerViewpointVector + PlayerViewpointRotator.Vector() * Reach;
		// Move object We are holding

		if(!PhysicsHandle){return;}
		PhysicsHandle->SetTargetLocation(LineTracerEnd);
	}
}

void UGrabber::UpdatePlayersViewPoint()
{
	if(!IsUpdated)
	{
		// So this Vector and Rotator would have variable where you are and where you looking at.
		GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewpointVector, OUT PlayerViewpointRotator);
		IsUpdated = true;
	}
	else
	{
		UE_LOG(LogTemp,Display,TEXT("already Done!"));
	}
}