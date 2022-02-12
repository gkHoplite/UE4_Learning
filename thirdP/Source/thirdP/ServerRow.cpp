// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerRow.h"
#include "MainMenu.h"
#include <Components/Button.h>
#include <Components/TextBlock.h>

void UServerRow::OnClickedRowButton()
{
	MainMenu->FromServerRowSetIndex(index);
}

void UServerRow::OnHoveredRowButton()
{
	ServerName->SetColorAndOpacity(FLinearColor::Gray);
}

void UServerRow::OnUnHoveredRowButton()
{
	ServerName->SetColorAndOpacity(UnHoveredColor);
}

void UServerRow::NativeConstruct()
{
	RowButton->OnClicked.AddDynamic(this, &UServerRow::OnClickedRowButton);
	RowButton->OnHovered.AddDynamic(this, &UServerRow::OnHoveredRowButton);
	RowButton->OnUnhovered.AddDynamic(this, &UServerRow::OnUnHoveredRowButton);

	UnHoveredColor = FLinearColor::White;
}

void UServerRow::FromMainMenuSet(UMainMenu* Menu, uint16 i)
{
	MainMenu = Menu;
	index = i;
}

void UServerRow::SwapColorForClick(bool isNew)
{
	if (isNew) {
		//ServerName->SetColorAndOpacity(FLinearColor::Black);
		UnHoveredColor = FLinearColor::Black;
	}
	else {
		ServerName->SetColorAndOpacity(FLinearColor::White);
		UnHoveredColor = FLinearColor::White;
	}
}
