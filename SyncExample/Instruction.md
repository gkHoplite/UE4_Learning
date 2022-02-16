# 1. Creating A Go-Kart Pawn #
1. Create C++ Pawn and Creating BP inheriting C++ Pawn
2. Editing BP Pawn
![img](./img/79.SetBPKart.png)

3. Create BP GameMode Inheriting Default C++ GameMode
4. Set Project Settings GameMode
![img](./img/79.CreateBPGameMode.png)
![img](./img/79.SetGameModeSettings.png)

5. Implemeting Movement Logic for Car
```c++
AKart::AKart()
{
	Speed = 1000.f;
}

void AKart::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector LocationToMove = Velocity * Speed * DeltaTime;
	AddActorWorldOffset(LocationToMove);
}

void AKart::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	InputComponent->BindAxis(FName("MoveForward"), this, &AKart::MoveForward);
}

void AKart::MoveForward(float Value)
{
	Velocity = Value * GetActorForwardVector();
}
```

# 2 Understanding Forces And Movement #
## How to Simulate Physics?
1. Transition is Accumulating Velocity with Time. So 
	- __Transition = Velocity * Time__
2. Velocity is Acculating Acceleration with Time. So
	- __Velocity = Acceleration * Time__
3. You can take Acceleration with Force, How many Power is pushing the Mass
	- __Acceleration = Power / Mass__

## Get the Equation for Calculate
|Transition = |Velocity * Time|
|-|-|
|Transition = |Acceleration * Time * Time|
|Transition = |Power/Mass * Time * Time|


## Implementing in on Code
```c++
void AKart::Tick(float DeltaTime)
{
	// Acceleration = Delta Velocity / Delta Time
	FVector Force = GetActorForwardVector() * Throttle * MaxDrivingForce;

	// (a = F / m) == (F = m * a)
	FVector Acceleration = Force / Mass;
	
	// Acceleration = Delta Velocity / Delta Time
	// acceleration is the change in speed divided by change in time
	Velocity += Acceleration * DeltaTime;

	float TransRealWorldUnit= 100.f;
	// (Transition = Velocity * Time) == (Velocity = Delta transition / Delta Time)
	FVector Translation = Velocity * DeltaTime * TransRealWorldUnit; // 
	AddActorWorldOffset(Translation);
}
```

# 3 Blocking Movement Without Physics #
## Setting up collision volumes.
![img](./img/81.SetCollision.png)

## Sweeping with `AddActorWorldOffset`.
1. Turn On bSweep for Working Collision
2. Check blocking with FHitResult.

```c++
void AKart::Tick(float DeltaTime)
{
	FHitResult HitResult;
	/*@param bSweep  Whether we sweep to the destination location, triggering overlaps along the way and stopping short of 
	 *				 the target if blocked by something.Only the root component is sweptand checked for blocking collision, 
	 *				 child components move without sweeping.If collision is off, this has no effect. */
	AddActorWorldOffset(Translation, true, &HitResult);
	if (HitResult.IsValidBlockingHit()) {
		Velocity = FVector(0.f);
	}
}
```

# 4 Rotations With Quaternions #
## Angle axis rotations with FQuat.
![img](img\82.AngleAxisRotation.png)

## Adding rotations actors.
1. Set Rotation Angle Degree
2. Get FQuat Rotation with Upwards Direction and transform it into radians. 
3. Applying Rotation info to Actor
```c++
void AKart::Tick(float DeltaTime)
{
	// Rotation Angle is Degree , RotationDelta takes Radians
	float RotationAngle = MaxRotatingForce * DeltaTime * SteeringThrow;
	FQuat RotationDelta(GetActorUpVector(), FMath::DegreesToRadians(RotationAngle));

	Velocity = RotationDelta.RotateVector(Velocity);
	AddActorWorldRotation(RotationDelta);
}
```

## Rotating our velocity.
# 5 Simulating Air Resistance #
## Understanding air resistance.
![img](img\83.AirResistanceFormula.gif)

## Calculating force due to air resistance.
- AirResistance Affecting Force relating to Movement
```c++
FVector AKart::GetAirResistance()
{
	//Velocity.SizeSquared() == FMath::Square(Velocity.Size());
	FVector AirResistance = Velocity.GetSafeNormal() * Velocity.SizeSquared() * DragCoefficient;
	return -1 * AirResistance;
}

void AKart::Tick(float DeltaTime)
{
	FVector Force = GetActorForwardVector() * Throttle * MaxDrivingForce;
	
	/* Modified */
	Force += GetAirResistance();

	FVector Acceleration = Force / Mass;
}
```
# 6 Simulating Rolling Resistance #
## What is rolling resistance?

![img](img/84.RollingResistance.png)
- Rolling Resistance = RR Coefficient * Normal Force
- [Explanation About Rolling Resistance](https://www.youtube.com/watch?v=_S2lyaMgBQ8)
- __Rolling Resistance vary with Mass__

## Implementing Rolling Resistance
1. Finding the gravity in Unreal.
2. Implementing rolling resistance.
3. Example rolling resistance coeffients. 
```c++
FVector AKart::GetRollingResistance()
{
	// GetGravityZ = -980.f, default unit is centimeter
	float ReactingGravityForce = -1 * GetWorld()->GetGravityZ() / TransUnit;

	float NormalForce = Mass * ReactingGravityForce;
	
	return -1 * Velocity.GetSafeNormal() * RollingResistanceCoefficient * NormalForce;
}

void AKart::Tick(float DeltaTime)
{
	FVector Force = GetActorForwardVector() * Throttle * MaxDrivingForce;
	Force += GetAirResistance();

	/* Add Rolling Resistance */
	Force += GetRollingResistance();

	FVector Acceleration = Force / Mass;
}
```
# 7 Steering And Turning Circles #
## Why we get turning circles Calculating our rotation geometry.
![img](img\85.CarRotation.png)

## Usage for Dot Product
![img](./img/85.Consine.png)
- Dot Product is useful, when calculating Cosines Theta btw two vector
- By using this Calculating Degrees or radians btw two Vector


## Correcting steering behaviour.
![img](./img/ArcLengthFormula.png)
```
Arc = Theta * radius
Delta Location = Delta Rotation  * radius
Delta Rotation = Delta Location / Radius
```

```c++
void AKart::Tick(float DeltaTime)
{
/* Rotation */
	// Rotation Angle is Degree , RotationDelta takes Radians
	float DeltaLocation = FVector::DotProduct(GetActorForwardVector(), Velocity) * DeltaTime;
	float RotationAngle = DeltaLocation / TurningRadius * SteeringThrow;
	FQuat RotationDelta(GetActorUpVector(), RotationAngle);

	Velocity = RotationDelta.RotateVector(Velocity);
	AddActorWorldRotation(RotationDelta);
}
```
- [Physics for Advanced Racing Game, like Drafting](https://www.udemy.com/course/unrealmultiplayer/learn/lecture/8274656#questions/12231310)

- [How to implement exact Physics?](https://www.udemy.com/course/unrealmultiplayer/learn/lecture/8274656#questions/10831926)


# 8 Server Functions & Cheat Protection #
## How to change state from the client.
- Request Server to change state with RPC
- RPC : Remote Procedure Calls

## Introduction to RPC server functions.
1. Add UFUNCTION Specifier
- UFUNCTION(Server, Reliable, WithValidation) takes the declaration of Server_MoveForward and Creates 2 declarations in its place - _Implementation and _Validation

```c++
UFUNCTION(Server, Reliable, WithValidation)
void ServerMoveForward(float Value);
```
- Call this function on Clients make things happened simultanously in Server

2. Add Implementation and Validate.
```c++
void AKart::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	InputComponent->BindAxis(FName("MoveForward"), this, &AKart::ServerMoveForward);
}
// Implementation and Validate Executed on ServerSide.
void AKart::ServerMoveForward_Implementation(float Value)
{
	Throttle = Value;
}

bool AKart::ServerMoveForward_Validate(float Value)
{
	return FMath::Abs(Value)<=1.0;
}
```

|Specifier|Details  |
|-|-|
|Server|The function is only executed on the server. Declares an additional function named the same as the main function, but with _Implementation added to the end, which is where code should be written. The autogenerated code will call the _Implementation method when necessary.|
|Reliable|The function is replicated over the network, and is guaranteed to arrive regardless of bandwidth or network errors. Only valid when used in conjunction with Client or Server.|
|WithValidation|Declares an additional function named the same as the main function, but with _Validate added to the end. This function takes the same parameters, and returns a bool to indicate whether or not the call to the main function should proceed.|


- Q: PuzzlePlatforms didn't use RPC and client state update without it. what is different here?
	- The CharacterMovement component is calling RPCs for us in the background. This is the beauty of using components, they can do a lot for us.


# 9 AutonomousProxy vs SimulatedProxy #

# How Server Replication works?
- The server doesn't replicate actors every update. This would take way too much bandwidth and CPU resources. Instead, the server will replicate actors at a frequency specified on the AActor::NetUpdateFrequency property.

## What are Actor roles?
![img](./img/87.ActorRole.png)
1. Server Control All the Pawn and replicate down to clients.
2. But Pawn takes input from client itself is exception. 
	|Actor Role| Details|
	|-|-|
	|Autonomous Proxy | Pawn updated on thier own client |
	|Simulated Proxy  | Pawn replicated down by Server |
	|Authority        | Pawn controlled by Server |
3. Role and Remote Role
- Role is Your Pawn's Actor Role.
- Remote Role is indicating Other machine's point of view what the Pawn's Actor Role is

## Updating the `AutonomousProxy`.
1. UFUNCTION(Server) didn't work in Autonomous.
- It means Server didn't updates your pawn
2. So First, executing function on local machine, then Request to Server to updates it.
```c++
void AKart::Tick(float DeltaTime)
{
	/* Display Actor Role */
	FString ActorRoleName = UEnum::GetValueAsString(GetLocalRole());
	DrawDebugString(GWorld, FVector(0.f), ActorRoleName, this, FColor::White, 0);
}

void AKart::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	InputComponent->BindAxis(FName("MoveForward"), this, &AKart::MoveForward);
	InputComponent->BindAxis(FName("MoveRight"), this, &AKart::MoveRight);
}

void AKart::MoveForward(float Value)
{
	Throttle = Value;
	ServerMoveForward(Value);
}

void AKart::MoveRight(float Value)
{
	SteeringThrow = Value;
	ServerMoveRight(Value);
}
```

# 10 Sources Of Simulation Error #
## How to Fix Spawn Error?
- Random Spwaning cause Error, So Specifiying it solve problem.
```c++
#include <GameFramework/PlayerStart.h>
#include <Kismet/GameplayStatics.h>
AActor* ASyncExampleGameMode::ChoosePlayerStart_Implementation(AController* Player) {

	// create array with all PlayerStart Actors
	TArray<AActor*> PlayerStarts;
	TSubclassOf< APlayerStart > PlayerStart = APlayerStart::StaticClass();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), PlayerStart, PlayerStarts);

	//Loop through Array, spawn only if the spawn is not already taken
	for (auto Spawn : PlayerStarts) {
		if (Cast< APlayerStart>(Spawn)->PlayerStartTag != "Taken") {
			Cast< APlayerStart>(Spawn)->PlayerStartTag = "Taken";
			return Spawn;
		}
	}
	return Super::ChoosePlayerStart_Implementation(Player);
}
```

## What Makes Sync Errors on Transform?
![img](img\88.WhatCauseSimulationError.png)
- If Set Collision Preset On BP_Car to BlockAllDynamic, then desynchronization occured easily. If Set this BlockAll, It's hard to find desynchronization
- Because Autonomous has Collider but the other machine didn't simulate it's collider. So hitting Sth makes Desynchronization so easily.
- All the Transform calculated with DeltaTime, Because DeltaTime is Set by frame on each computer makes desynchronization!

## Investigate approaches to eliminating them.
1. Overwrite Transform from the Server
	- Makes Players Movement flawless
2. Synchronise DeltaTime
	- Diverse factor like Networks condition could ruin this.

# 11 Replicating Variables From The Server #
## Overview of property replication.
## Replicating the actor position.
## Setting and reading the property.
## Replicating the actor rotation.
# 12 Triggering Code On Replication #
## Deep dive on property replication.
## Setting the network update interval.
## Notify on replicate.
## Simulate between updates.
# 13 Smooth Simulated Proxies #
## Replicating velocity.
## Why is movement jerky?
## Replicating control input to SimulatedProxy.
# 14 Simulating Lag And Packet Loss #
## What is lag?
## Simulating lag and packet loss.
## Why does lag cause glitching?
# 15 Replay Autonomous Moves #
## Why do we reset when accelerating?
## Keeping AutonomousProxy ahead of the Server.
## What information needs to be sent to the server.
## Compare our different simulation approaches.
# 16 Planning Client-Side Prediction #
## Pseudocode for client prediction.
## Adding structs for synchronisation.
# 17 Replicating Structs #
## What do we have already?
## Replicating state via a struct.
## Sending the `Move` struct via RPC.
# 18 Simulating A Move #
## The `SimulateMove` signature.
## Updating the canonical state.
## Implement `SimulateMove`.
# 19 Unacknowledged Move Queue #
## `TArray` for the Move queue.
## Tidying the move creation code.
## Printing the queue length.
## Removing acknowledged moves.
# 20 Simulating Unacknowledged Moves #
## Simulate all moves.
## Testing for smoothness.
## How can we still make it glitch?
# 21 Fixing SimulatedProxy Prediction #
## Ensuring the Server simulates once.
## Local prediction on the client.
## Making smoother predictions.
# 22 Refactoring Into Components #
## Red-Green-Refactor process.
## How to spot your "code smells".
## Identifying a suitable refactor.
## Planning our refactor.
# 23 Extracting A Movement Component #
## Create and name the component.
## Move member declarations across.
## Move function implementations.
## Fix build errors.
# 24 Extracting A Replication Component #
## Creating the component.
## Enable replication.
## Move member declarations across.
## Move function implementations.
## Fix build errors.
# 25 Decoupling Movement & Replication #
## What happens if we disable the replicator?
## Allow the Movement Component to tick.
## Getting the information to replicate.
# 26 Linear Interpolation For Position #
## How does linear interpolation work?
## Overview of client interpolation.
## Pseudocode for client interpolation.
# 27 FMath::Lerp For Client Interpolation #
## Ensure movement replication is off.
## Updating the time variables.
## `FMath::Lerp` vs `FMath::LerpStable`.
## Implementing the pseudocode.
# 28 FQuat::Slerp For Rotation #
## `Slerp` vs `Lerp`.
## Store tranform instead of location.
## Implementing `Slerp`ed location.
# 29 Hermite Cubic Spline Interpolation #
## Understanding jarring movement.
## How velocity can help or hinder.
## A brief overview of polynomials.
## Introducing the Hermite Cubic Spline.
# 30 FMath::CubicInterp For Velocity #
## Slopes, derivatives and velocity.
## Using `CubicInterp` for location.
## Using `CubicInterpDerivative` for velocity.
# 31 Refactoring With Structs #
## Assessing the existing code.
## Creating a plain C#### struct.
## Pulling out methods.
# 32 Client Interpolation Mesh Offset ##

## Understanding mesh offseting.
## Set up the mesh offset root component.
## Manipulating the offset instead.

# 33 Advanced Cheat Protection #

## Bounding the inputs.
## Stressing our DeltaTime.
## Tracking simulation time.