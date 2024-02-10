// Fill out your copyright notice in the Description page of Project Settings.

#include "Blueprint/UserWidget.h"
#include "C_PlayerController.h"
#include "WC_MainHUD.h"


void AC_PlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	UWC_MainHUD* playerHUD = CreateWidget<UWC_MainHUD>(this, MainHUD);
	if (playerHUD != nullptr) {
		playerHUD->AddToViewport();
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("it null"));
	}
}
