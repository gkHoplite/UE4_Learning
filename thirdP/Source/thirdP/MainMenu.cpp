// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "Components/Button.h"
#include <Components/WidgetSwitcher.h>
#include <Components/EditableTextBox.h>
#include "PuzzleGameInstance.h"
#include "ServerRow.h"
#include <Components/TextBlock.h>

#include "Kismet/KismetSystemLibrary.h"

UMainMenu::UMainMenu(const FObjectInitializer& ObjectInitializer)// :Super(ObjectInitializer)
{
	/* UI For ServerList in Scroll box */
	ConstructorHelpers::FClassFinder<UUserWidget>ServerRowBPClass(TEXT("/Game/UI/WBP_ServerRow")); // Only in Constructor
	if (ensure(ServerRowBPClass.Class != nullptr)) // pointer to class for instantiation
	{
		ServerRowClass = ServerRowBPClass.Class;
		UE_LOG(LogTemp, Warning, TEXT("Detecting %s"), *ServerRowClass.Get()->GetName());
	}
}

void UMainMenu::UpdateServerList(TArray<FServerData> ServerDatas)
{
	if (GetWorld()->GetGameViewport() == nullptr || ServerRowClass == nullptr) { return; }
	
	ServerList->ClearChildren();

	uint16 index = 0;
	for (const FServerData& ServerData: ServerDatas) {
		UServerRow* Row = CreateWidget<UServerRow>(this, ServerRowClass); // Call Native Constructor
		if (ensure(Row != nullptr)) {
			Row->ServerName->SetText(FText::FromString(ServerData.ServerName));
			Row->HostUserName->SetText(FText::FromString(ServerData.HostUserName));
			FString SessionPlayerState = FString::Printf(TEXT("%d/%d"), ServerData.CurPlayers, ServerData.MaxPlayers);
			Row->SessionPlayerState->SetText(FText::FromString(SessionPlayerState));

			Row->FromMainMenuSet(this, index++);
			ServerList->AddChild(Row);
		}
	}
}

void UMainMenu::FromServerRowSetIndex(uint16 i)
{
	/* Turn Old Button's Color to normal */
	if (SelectedIndex.IsSet()) {
		auto* Before = Cast<UServerRow>(ServerList->GetChildAt(SelectedIndex.GetValue()));
		if (Before != nullptr) { Before->SwapColorForClick(false); }
	}

	SelectedIndex = i;
	
	/* Turn Old Button's Color to Special */
	auto* After = Cast<UServerRow>(ServerList->GetChildAt(i));
	if (After != nullptr) { After->SwapColorForClick(true); }
}

void UMainMenu::DelegateForHostButton()
{
	//UPuzzleGameInstance* GameInstance = Cast<UPuzzleGameInstance>(GetGameInstance());
	//GameInstance->Host();
	//UE_LOG(LogTemp, Warning, TEXT("Instance add %x"), GameInstance);

	if (ensure(MenuInterface != nullptr)) {
		FName HostingNameStr =  FName(*HostingName->Text.ToString());
		MenuInterface->Host(HostingNameStr);
	}
}

void UMainMenu::DelegateForJoinButton()
{
	//MenuSwitcher->SetActiveWidgetIndex(2);
	MenuSwitcher->SetActiveWidget(JoinOverlay);
}

void UMainMenu::DelegateForExtraButton()
{
	if (ensure(MenuInterface != nullptr)) {
		MenuInterface->Play("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap");
	}
}

void UMainMenu::DelegateForAddrButton()
{
	if (SelectedIndex.IsSet() && MenuInterface != nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("selected index %d"), SelectedIndex.GetValue());
		MenuInterface->Join(SelectedIndex.GetValue());
	}
}

void UMainMenu::DelegateForCancelJoinButton()
{
	MenuSwitcher->SetActiveWidgetIndex(0);
}

void UMainMenu::DelegateForExitButton()
{
	// nullptr controlled by GameInstance
	Cast<UPuzzleGameInstance>(GetGameInstance())->CloseMenu();
}

void UMainMenu::DelegateForQuitButton()
{
	GWorld->GetFirstPlayerController()->ConsoleCommand("quit");

//#if WITH_EDITOR
//	GWorld->GetFirstPlayerController()->ConsoleCommand("quit");
//#else	
//	FGenericPlatformMisc::RequestExit(false); // Exiting Editor also
//#endif
}

void UMainMenu::DelegateForUpdateButton()
{
	if (MenuInterface == nullptr) { return; }
	MenuInterface->Update();
}

void UMainMenu::DelegateForCancelHostButton()
{
	MenuSwitcher->SetActiveWidgetIndex(0);
}

void UMainMenu::DelegateForHostMenuButton()
{
	MenuSwitcher->SetActiveWidgetIndex(1);
}

bool UMainMenu::Initialize()
{
	UE_LOG(LogTemp, Warning, TEXT(__FUNCTION__));
	if(Super::Initialize() ==false) return false;
	return true;
}

void UMainMenu::NativeConstruct()
{
	UE_LOG(LogTemp, Warning, TEXT(__FUNCTION__));
	Super::NativeConstruct();

	/* Binding Button to Function */
	HostButton->OnClicked.AddDynamic(this, &UMainMenu::DelegateForHostButton);
	JoinButton->OnClicked.AddDynamic(this, &UMainMenu::DelegateForJoinButton);
	ExtraButton->OnClicked.AddDynamic(this, &UMainMenu::DelegateForExtraButton);
	ExitButton->OnClicked.AddDynamic(this, &UMainMenu::DelegateForExitButton);

	AddrButton->OnClicked.AddDynamic(this, &UMainMenu::DelegateForAddrButton);
	CancelButtonJoin->OnClicked.AddDynamic(this, &UMainMenu::DelegateForCancelJoinButton);
	QuitButton->OnClicked.AddDynamic(this, &UMainMenu::DelegateForQuitButton);
	
	UpdateButton->OnClicked.AddDynamic(this, &UMainMenu::DelegateForUpdateButton);
	
	CancelButtonHost->OnClicked.AddDynamic(this, &UMainMenu::DelegateForCancelHostButton);
	HostMenuButton->OnClicked.AddDynamic(this, &UMainMenu::DelegateForHostMenuButton);
}

void UMainMenu::NativeDestruct()
{
	UE_LOG(LogTemp, Warning, TEXT(__FUNCTION__));
	
	DelegateForExitButton(); // tracking nullptr in GameInstance

	Super::NativeDestruct();
}

void UMainMenu::AddToScreen(ULocalPlayer* Player, int32 ZOrder)
{
	Super::AddToScreen(Player, ZOrder);
	//FWorldDelegates::LevelRemovedFromWorld.RemoveAll(this);
}

//void UMainMenu::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)
//{
//	Super::OnLevelRemovedFromWorld(InLevel, InWorld);
//
//	UWorld* World = GetWorld();
//	if (!World) { return; }
//	APlayerController* PlayerController = World->GetFirstPlayerController();
//	if (!PlayerController) { return; }
//
//	FInputModeGameOnly InputModeData;
//
//	PlayerController->SetInputMode(InputModeData);
//	PlayerController->bShowMouseCursor = false;
//}