// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "C_HTTPGoogleApi.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UC_HTTPGoogleApi : public UObject
{
	GENERATED_BODY()

public:
	void onSendPostRequest(FString ApiEndpoint, FString JsonContent);
	void onResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void SynthesizeSpeech(FString TextToSpeak);
	FString FilterOnlyLetters(const FString& Input);
};
