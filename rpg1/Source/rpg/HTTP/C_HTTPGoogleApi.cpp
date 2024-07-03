// Fill out your copyright notice in the Description page of Project Settings.


#include "C_HTTPGoogleApi.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundWaveProcedural.h"
#include "AudioDevice.h"   
#include "Misc/Base64.h"
#include <Blueprint/AIBlueprintHelperLibrary.h>
#include "HAL/PlatformProcess.h"
#include "TextToSpeech.h"
#include "Async/Async.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Json.h"
#include "JsonUtilities.h"
#include "Misc/ConfigCacheIni.h"



void UC_HTTPGoogleApi::onSendPostRequest(FString ApiEndpoint, FString JsonContent)
{
    FHttpModule* Http = &FHttpModule::Get();
    if (!Http) { return; }

    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();
    Request->OnProcessRequestComplete().BindUObject(this, &UC_HTTPGoogleApi::onResponseReceived);
    Request->SetURL(ApiEndpoint);
    Request->SetVerb("POST");
    Request->SetHeader("Content-Type", "application/json");
    Request->SetContentAsString(JsonContent);
    Request->ProcessRequest();
}

void UC_HTTPGoogleApi::onResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
    if (bWasSuccessful && Response.IsValid())
    {
        // Parse JSON response from Google TTS API
        FString ResponseString = Response->GetContentAsString();
        TSharedPtr<FJsonObject> JsonObject;
        TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseString);
        UE_LOG(LogTemp, Warning, TEXT("Received audio content: %s"), *ResponseString);

        if (FJsonSerializer::Deserialize(Reader, JsonObject))
        {
            // Extract audio content from the JSON response
            FString AudioContentBase64 = JsonObject->GetStringField("audioContent");

            // Decode base64 audio content
            TArray<uint8> AudioContentBytes;
            FBase64::Decode(AudioContentBase64, AudioContentBytes);

            // Create a sound wave from the decoded audio data
            USoundWaveProcedural* ProceduralSoundWave = NewObject<USoundWaveProcedural>();
            if (ProceduralSoundWave)
            {
                ProceduralSoundWave->SetSampleRate(24000); // Set your desired sample rate
                ProceduralSoundWave->NumChannels = 1; // Mono audio
                ProceduralSoundWave->Duration = AudioContentBytes.Num() / (ProceduralSoundWave->GetSampleRateForCurrentPlatform() * ProceduralSoundWave->NumChannels * sizeof(int16));
                ProceduralSoundWave->SoundGroup = SOUNDGROUP_Voice; // Set appropriate sound group
                ProceduralSoundWave->bLooping = false; // Non-looping sound

                // Copy the decoded audio data to the sound wave
                ProceduralSoundWave->QueueAudio(AudioContentBytes.GetData(), AudioContentBytes.Num());
                UObject* x= GetWorld();
                // Play the procedural sound wave
                UGameplayStatics::PlaySound2D(x, ProceduralSoundWave);
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("Failed to create procedural sound wave"));
            }
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to deserialize JSON response from Google TTS API"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to send POST request to Google TTS API"));
    }
}

void UC_HTTPGoogleApi::SynthesizeSpeech(FString TextToSpeak)
{
    TextToSpeak = FilterOnlyLetters(TextToSpeak);
    UE_LOG(LogTemp, Warning, TEXT("new text: %s"), *TextToSpeak);
    FString APIKey;
    FString ConfigFilePath = FPaths::Combine(FPaths::ProjectConfigDir(), TEXT("Secrets.ini"));
    ConfigFilePath = FConfigCacheIni::NormalizeConfigIniPath(ConfigFilePath);

    UE_LOG(LogTemp, Warning, TEXT("Secrets.ini path: %s"), *ConfigFilePath);

    if (GConfig->GetString(TEXT("APIKeys"), TEXT("GoogleCloudKey"), APIKey, ConfigFilePath))
    {
        FString ApiEndpoint = TEXT("https://texttospeech.googleapis.com/v1beta1/text:synthesize?key=");
        FString FullEndpoint = ApiEndpoint + APIKey;

        // Construct the JSON payload
       // FString JsonPayload = FString::Printf(TEXT("{\"input\":{\"text\":\"%s\"},\"voice\":{\"languageCode\":\"en-US\",\"name\":\"en-US-Wavenet-D\"},\"audioConfig\":{\"audioEncoding\":\"LINEAR16\"}}"), *TextToSpeak);
        FString JsonPayload = FString::Printf(
            TEXT("{\"input\":{\"text\":\"%s\"},\"voice\":{\"languageCode\":\"en-US\",\"name\":\"en-US-Wavenet-D\"},\"audioConfig\":{\"audioEncoding\":\"LINEAR16\",\"pitch\":-5.0,\"speakingRate\":0.9}}"),
            *TextToSpeak);

        onSendPostRequest(FullEndpoint, JsonPayload);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to retrieve Google TTS API key from Secrets.ini"));
    }
}

FString UC_HTTPGoogleApi::FilterOnlyLetters(const FString& Input)
{
    FString Result;

    for (TCHAR Character : Input)
    {
        if (FChar::IsAlpha(Character)) // Check if the character is a letter
        {
            Result.AppendChar(Character);
        }
    }

    return Result;
}