# 1. Hosting and Joining
# Git merge #
    git fetch /path/to/project-a master
    git merge --allow-unrelated-histories FETCH_HEAD
    git remote remove project-a


# How to use Command #
[Command-Line Arguments for UE4](https://docs.unrealengine.com/4.27/en-US/ProductionPipelines/CommandLineArguments/)

> start { window tab title } { execution file } { Params }
## Server ##
    start "" "C:/Program Files/Epic Games/UE_4.27/Engine/Binaries/Win64/UE4Editor.exe" "D:/Unreal/thirdP/thirdP.uproject" "/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap" -server -log -nostream

## Client ##
    start "" "C:/Program Files/Epic Games/UE_4.27/Engine/Binaries/Win64/UE4Editor.exe" "D:/Unreal/thirdP/thirdP.uproject" 127.0.0.1 -game -WINDOWED -ResX=800 -ResY=900 -WinX=0 -WinY=20 -log -nosteam


# Feature for Movable Actor
- UE4 combine actor's transform with attaching. it also affect the caculation btw client and server.
- Actor move when setting Mobility option to Movable.
``` c++
AMovingPlatform::AMovingPlatform() {
    PrimaryActorTick.bCanEverTick = true;
    SetMobility(EComponentMobility::Movable);
}
```

## Gizmo
- Add Gizmo on FVector
- The Gizmo's FVector is Local not global

```c++
UPROPERTY(EditAnywhere, meta = (MakeEditWidget=true))
FVector TargetLocation;
```
![img](./img/1.10_Widget.png)

# Server's Role
- Calculate Actor and it's movement on Server and replicate it to clients.
- It allow to control all of clients in server. calculation works only in Server

## Replication and Authority
- Actor is container transfer info btw client and server.
- Server send updates of replicated actor and it's replicated property.
- Updates Actor only in Client is possible but whenever Server didn't agree with it, Set the status to that server allow.

## Replicates Information only in Server
- Only server has authority and could replicates actor and it's property. Every actor has authority function return wheter run in server or not. Use this for Server functionality.


if working on server, set this actor to be replicated
``` c++
AMovingPlatform::AMovingPlatform() {
    ...    
    if (HasAuthority()) {
        SetReplicates(true);
        SetReplicateMovement(true);
    }
}
```

Calculating Movement only in server.
```c++
void AMovingPlatform::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (HasAuthority()) {
        FVector Location = GetActorLocation();
        Location += FVector(MovePerSec * DeltaTime, 0, 0);
        SetActorLocation(Location);

        if(Location.X > 800.f ){
            MovePerSec *= -1;
        }
        if (Location.X < 0.f) {
            MovePerSec *= -1;
        }
    }
}
```

[FTransform](https://docs.unrealengine.com/4.27/en-US/API/Runtime/Core/Math/FTransform/)

- TargetLocation is Local needs translating to global. Use Actor's Transform and translating local to global. 
- SafeNormal is normalized vector return Zero if the value is small enough lossing it's information.
- this way works with any scaling and rotation.

### `Translate to global` ###
```c++
void AMovingPlatform::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (HasAuthority()) {
        FVector Location = GetActorLocation();
        FVector Direction = GetTransform().TransformPosition(TargetLocation)  - GetActorLocation();
        Direction = Direction.GetSafeNormal();
        Location += MovePerSec * DeltaTime * Direction;
        SetActorLocation(Location);
        if (Location.X > 800.f) {
            MovePerSec *= -1;
        }
        else if (Location.X < 0.f) {
            MovePerSec *= -1;
        }
    }
}
```

### `Use Local` ###
``` c++
void AMovingPlatform::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (HasAuthority()) {
        FVector Location = GetActorLocation();
        Location += MovePerSec * DeltaTime * TargetLocation.GetSafeNormal();
        SetActorLocation(Location);
        if (Location.X > 800.f) {
            MovePerSec *= -1;
        }
        else if (Location.X < 0.f) {
            MovePerSec *= -1;
        }
    }
}

```

## assert ##
[UE4 Documentation for assert](https://docs.unrealengine.com/4.27/en-US/ProgrammingAndScripting/ProgrammingWithCPP/Assertions/)

__ensure__ : marcro check null pointer. it doesn't make crash, log on console.


## UPROPERTY Specifier ##
- It's interesting that visibleAnywhere allow to change it's property.

|VisibleAnywhere|EditAnywhere|
|-|-|
|fixed|reassignable|
|changable property | changable property


``` c++
UPROPERTY(VisibleAnywhere)
UPROPERTY(EditAnywhere)
```

## Dynamic Event ##
Delegates
```c++
// .h
TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &APlatformTrigger::OnOverLapBegin);
```

```c++
void APlatformTrigger::OnOverLapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)

```


# Game Instance #
Game Instance apply Logic to different level.
- Good for console command flags
- Use this for hosting server and joining

1. Create Game Instance Class in C++
2. Project Settings -> Game Instance -> Choose custom Game Instance
- Each Player has own Game Instance

## Init() vs Constructor ##
- Start editor call constructor
- Start game call Constructor and Init in sequential order.


>Tips: Create default constructor with parent's signature. much safer.
```c++
class THIRDP_API UPuzzleGameInstance : public UGameInstance{}

UPuzzleGameInstance::UPuzzleGameInstance(const class FObjectInitializer& ObjectInitializer);

UGameInstance::UGameInstance(const FObjectInitializer& ObjectInitializer);
```


## Exec Keyword ##
In game backslash( ` ) helps to execute command and UFUNCTION(Exec) create a command with function name.
```c++
    UFUNCTION(Exec)
    void Host();
```
 ___theses Class use Exec keyword.___
- PlayerControllers
- Pawn Possessed by Controller
- HUDs
- Cheat Managers
- Game Modes
- Game Instances

## Log on Screen ##
- c-style printf with format specifier
 ```c++
FString X = FString::Printf(TEXT("%s %s "), TEXT(__FUNCTION__),*Address);
FString Y = TEXT(__FUNCTION__) + FString("192.168.111.0");
 ```

- set unique key to -1 means making new log line
```c++
void UPuzzleGameInstance::Join(const FString& Address)
{
    UEngine* Engine = GetEngine();
    if (ensure(Engine != nullptr)) {
        Engine->AddOnScreenDebugMessage(-1, 5, FColor::Cyan, FString::Printf(TEXT("%s %s "), TEXT(__FUNCTION__),*Address));
    }
}
```


# Switching Map

## Server Travel
use ServerTravel for two main reason
1. Move to Map
2. Hosting Server with 'listen' keyword
3. Move each player to another map one by one.

```c++
void UPuzzleGameInstance::Host()
{
    UWorld* World = GetWorld(); // GWorld instead
    if (ensure(World != nullptr)) {
        World->ServerTravel("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap?listen");
    }
}
```

## Clien Travel
Use for connecting to server
```c++
void UPuzzleGameInstance::Join(const FString& Address)
{
    if (ensure(GWorld != nullptr)) {
        APlayerController* PController = GetFirstLocalPlayerController(GWorld);
        PController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
    }
}

```

## share with others
[itch.io](https://itch.io/)
use itch's application protect your machine from any random executable.
sandbox mode prohibiting executable to access out of it.


# __2. UI System__
## Widget BluePrint for display
Anchor adjust relative location of the data.
![img](.\img\2.25_Anchor.png)

## C++ code for functionality
The Parent class of WBP is UUserWidget

![img](.\img\2.27_WBPClass.png)

## Include BluePrint in C++ Code

> add UMG Module on your project in {file}.Build.cs
``` c#
PublicDependencyModuleNames.AddRange(new string[] { "UMG" });
```
> include UserWidget.h
> take every subclass of it for referencing
``` c++
//ProjectInstance.cpp
#include <Blueprint/UserWidget.h>
UPuzzleGameInstance::UPuzzleGameInstance(const FObjectInitializer & ObjectInitializer){
    ConstructorHelpers::FClassFinder<UUserWidget>MenuClass(TEXT("/Game/UI/WBP_MainMenu"));
    //if (MenuClass.Succeeded()) 
    if(ensure(MenuClass.Class!= nullptr)) // pointer to class for instantiation
    {
        UIMenu = MenuClass.Class;
        UE_LOG(LogTemp, Warning, TEXT("Detecting %s"), *UIMenu.Get()->GetName());
    }

}

```
> Class "TSubclassOf" help to get the subclass belong to specific class

-  TSubclassOf is a template that takes a type and restricts the type we can assign to MenuClass in the blueprint. What we are say here is that the MenuClass variable should hold a class itself (not a pointer or and instance of a class).
```c++
// ProjectInstance.h
private:
    UPROPERTY()
    TSubclassOf<class UUserWidget> UIMenu;
```

> Place it on beginplay or OnStart
- OnStart called  when gameinstance is starting. GameInstance created before the world do.
- Call UUserWidget on init or constructor is too early. the viewport is before creating. call this on beginplay or Onstart
- UE4 Editor didn't call StartGameInstanced. it only works on Game only clients
```c++
// For the sake of clean code, remove every if statements for check nullptr.
void UPuzzleGameInstance::OnStart()
{
    Super::OnStart();
    UUserWidget* Menu = CreateWidget<UUserWidget>(this, UIMenu);
    Menu->AddToViewport(0);
    UE_LOG(LogTemp, Warning, TEXT("Create Widget on your screen %s"), TEXT(__FUNCTION__));
}

void UPuzzleGameInstance::CloseMenu()
{
    GetWorld()->GetGameViewport()->RemoveAllViewportWidgets();
}
```

BluePrint version is quite interesting.
![img](\D:\Unreal\UE4_Learning\thirdP\img\2.27BluePrint.png)


## Turn on Mouse
FInputModeDataBase
ㄴFInputModeUIOnly
ㄴFInputModeGameAndUI
ㄴFInputModeGameOnly

```c++
void UPuzzleGameInstance::CloseMenu()
{
    GetFirstLocalPlayerController(GWorld)->SetShowMouseCursor(false);
    GetFirstLocalPlayerController(GWorld)->SetInputMode(FInputModeGameOnly());
}

void UPuzzleGameInstance::OpenMenu()
{
    GetFirstLocalPlayerController(GWorld)->SetShowMouseCursor(true);
    FInputModeGameAndUI FInputMode;
    FInputMode.SetHideCursorDuringCapture(false); // solve shaking in camera
    GetFirstLocalPlayerController(GWorld)->SetInputMode(FInputMode);
}
```