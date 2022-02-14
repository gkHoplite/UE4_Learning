// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/HUD.h"
#include "SyncExampleHud.generated.h"


UCLASS(config = Game)
class ASyncExampleHud : public AHUD
{
	GENERATED_BODY()

public:
	ASyncExampleHud();

	/** Font used to render the vehicle info */
	UPROPERTY()
	UFont* HUDFont;

	// Begin AHUD interface
	virtual void DrawHUD() override;
	// End AHUD interface
};
