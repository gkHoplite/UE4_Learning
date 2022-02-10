// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerRow.h"
#include "MainMenu.h"
#include <Components/Button.h>

void UServerRow::DelegateForRowButton()
{
	MainMenu->FromServerRowSetIndex(index);
}

void UServerRow::NativeConstruct()
{
	RowButton->OnClicked.AddDynamic(this, &UServerRow::DelegateForRowButton);
}

void UServerRow::FromMainMenuSet(UMainMenu* Menu, uint32 i)
{
	MainMenu = Menu;
	index = i;
}
