#include "C_QuestKnight.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"
#include <Components/TextBlock.h>
#include "TimerManager.h"
#include "HAL/PlatformFileManager.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

#include "rpg/HTTP/C_HTTPGoogleApi.h"
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
AC_QuestKnight::AC_QuestKnight(const FObjectInitializer& ObjectInitializer)
{
    static ConstructorHelpers::FClassFinder<UUserWidget> QuestText(TEXT("/Game/Static/quests/WB_QuestText.WB_QuestText_C"));
    if (QuestText.Succeeded())
    {
        widget = QuestText.Class;
    }
}

void AC_QuestKnight::TalkToNPC()
{
    IInteractive_CI::TalkToNPC();
    UC_HTTPGoogleApi *callApi= NewObject<UC_HTTPGoogleApi>(this);;
    DialogIndex++;
    if (Dialog.Num() > DialogIndex)
    {
        UE_LOG(LogTemp, Warning, TEXT("%s"), *Dialog[DialogIndex]);
        ShowWidget(Dialog[DialogIndex]);
       callApi->SynthesizeSpeech(Dialog[DialogIndex]);
    }
    else
    {
        if (PartNumber <= 3) {
            DialogIndex = -1;
            Dialog.Empty();
            bAllowedTOTelleport = true;
            PartNumber++;
            fileToRead();
        }
    }
}

void AC_QuestKnight::ShowWidget(FString str)
{
    if (UWorld* World = GetWorld())
    {
        // Remove the current widget if it exists
        if (CurrentWidget)
        {
            CurrentWidget->RemoveFromParent();
            CurrentWidget = nullptr;
        }

        if (widget)
        {
            CurrentWidget = CreateWidget<UUserWidget>(World, widget);
            if (CurrentWidget)
            {
                CurrentWidget->AddToViewport();

                // Find the TextBlock in the widget
                UTextBlock* QuestTextBlock = Cast<UTextBlock>(CurrentWidget->GetWidgetFromName(TEXT("questText")));
                if (QuestTextBlock)
                {
                    QuestTextBlock->SetText(FText::FromString(str));
                }
                else
                {
                    UE_LOG(LogTemp, Error, TEXT("Failed to find QuestTextBlock"));
                }

                // Set a timer to remove the widget after 5 seconds
                GetWorldTimerManager().SetTimer(WidgetTimerHandle, this, &AC_QuestKnight::RemoveCurrentWidget, 5.0f, false);
            }
        }
    }
}

FString AC_QuestKnight::ReadStringFromFile(FString FilePath, bool& bOutSuccess, FString& OutInfoMessage)
{
    // Ensure the file path uses forward slashes
    FilePath.ReplaceInline(TEXT("\\"), TEXT("/"));

    UE_LOG(LogTemp, Warning, TEXT("Reading file: %s"), *FilePath);

    if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*FilePath))
    {
        bOutSuccess = false;
        OutInfoMessage = FString::Printf(TEXT("File does not exist - '%s'"), *FilePath);
        return "";
    }

    FString RetString = "";
    if (!FFileHelper::LoadFileToString(RetString, *FilePath))
    {
        bOutSuccess = false;
        OutInfoMessage = FString::Printf(TEXT("Failed to load file - '%s'"), *FilePath);
        return "";
    }

    bOutSuccess = true;
    OutInfoMessage = FString::Printf(TEXT("Read the file - '%s'"), *FilePath);
    return RetString;
}

void AC_QuestKnight::WriteStringToFile(FString FilePath, FString String, bool& bOutSuccess, FString& OutInfoMessage)
{
    if (!FFileHelper::SaveStringToFile(String, *FilePath))
    {
        bOutSuccess = false;
        OutInfoMessage = FString::Printf(TEXT("Failed to save file - '%s'"), *FilePath);
        return;
    }
    bOutSuccess = true;
    OutInfoMessage = FString::Printf(TEXT("Wrote to file - '%s'"), *FilePath);
}

void AC_QuestKnight::RemoveCurrentWidget()
{
    if (CurrentWidget)
    {
        CurrentWidget->RemoveFromParent();
        CurrentWidget = nullptr;
    }
}

void AC_QuestKnight::CreateQuestFile()
{
    bool bSuccess;
    FString InfoMessage;
    FString FilePath = FPaths::ProjectContentDir() + TEXT("Static/quests/QuestText/KnightQuestPart3.txt");
    FString Content = TEXT("This is the first line of the quest file.\nYou... you're not one of them, are you?\nI see it in your eyes, the same emptiness I bear. You are Forsaken, just like me.\nLong ago, I had a family, a life, before the Iron Clan tore it all away.\nThey wield their metallic powers with cruelty, crushing anyone who stands in their way.\nI vowed revenge, but without power, what can one like me do?\nYet, here you stand, perhaps together we can topple the Iron Conduit and avenge the fallen.");
    WriteStringToFile(FilePath, Content, bSuccess, InfoMessage);

    if (!bSuccess)
    {
        UE_LOG(LogTemp, Error, TEXT("%s"), *InfoMessage);
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("%s"), *InfoMessage);
    }
}

void AC_QuestKnight::Teleport(FVector Location)
{
    if (bAllowedTOTelleport) {
        SetActorLocation(Location);
        bAllowedTOTelleport = false;
    }
}

void AC_QuestKnight::fileToRead()
{
    bool bSuccess;
    FString InfoMessage;
    UE_LOG(LogTemp, Warning, TEXT("sssssssssssssss %d"), PartNumber);
    FString FilePath = FString::Printf(TEXT("Static/quests/QuestText/KnightQuestPart%d.txt"), PartNumber);
    // Correcting the file path by ensuring it's relative to the project's content directory
    FString FullFilePath = FPaths::ProjectContentDir() + FilePath;

    // Log the file path for debugging purposes
    UE_LOG(LogTemp, Warning, TEXT("Attempting to read file at path: %s"), *FullFilePath);

    FString FileContent = ReadStringFromFile(FullFilePath, bSuccess, InfoMessage);
    UE_LOG(LogTemp, Warning, TEXT("Successfully read linessssssssss: %s"), *FileContent);

    if (bSuccess)
    {
       
        SendPostQestRequest(FileContent);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("%s"), *InfoMessage);
    }
}

void AC_QuestKnight::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
    if (bWasSuccessful && Response.IsValid())
    {
        FString ResponseString = Response->GetContentAsString();
        UE_LOG(LogTemp, Log, TEXT("Response: %s"), *ResponseString);

        // Parse JSON response
        TSharedPtr<FJsonObject> JsonObject;
        TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseString);
        if (FJsonSerializer::Deserialize(Reader, JsonObject))
        {
            // Extract text from the JSON structure
            const TArray<TSharedPtr<FJsonValue>>& CandidatesArray = JsonObject->GetArrayField("candidates");
            if (CandidatesArray.Num() > 0)
            {
                const TSharedPtr<FJsonValue>& CandidateValue = CandidatesArray[0];
                const TSharedPtr<FJsonObject>& CandidateObject = CandidateValue->AsObject();
                const TSharedPtr<FJsonObject>& ContentObject = CandidateObject->GetObjectField("content");
                const TArray<TSharedPtr<FJsonValue>>& PartsArray = ContentObject->GetArrayField("parts");
                if (PartsArray.Num() > 0)
                {
                    const TSharedPtr<FJsonValue>& PartsValue = PartsArray[0];
                    const TSharedPtr<FJsonObject>& PartsObject = PartsValue->AsObject();
                    FString MainText = PartsObject->GetStringField("text");

                    // Use or store the extracted main text
                    UE_LOG(LogTemp, Log, TEXT("Main Text: %s"), *MainText);

                    // Process the received text
                    ProcessReceivedText(MainText) ;
                }
            }
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to deserialize JSON response"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Request failed"));
    }
}


void AC_QuestKnight::SendPostRequest(FString ApiEndpoint, FString JsonContent)
{
    FHttpModule* Http = &FHttpModule::Get();
    if (!Http) { return; }

    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();
    Request->OnProcessRequestComplete().BindUObject(this, &AC_QuestKnight::OnResponseReceived);
    Request->SetURL(ApiEndpoint);
    Request->SetVerb("POST");
    Request->SetHeader("Content-Type", "application/json");
    Request->SetContentAsString(JsonContent);
    Request->ProcessRequest();
}




void AC_QuestKnight::SendPostQestRequest(FString FileContent)
{
    FString APIKey;
    FString ConfigFilePath = FPaths::Combine(FPaths::ProjectConfigDir(), TEXT("Secrets.ini"));
    ConfigFilePath = FConfigCacheIni::NormalizeConfigIniPath(ConfigFilePath);

    if (GConfig->GetString(TEXT("APIKeys"), TEXT("GeminiaiKey"), APIKey, ConfigFilePath))
    {
        FString JsonPayload = FString::Printf(TEXT("{\"contents\":[{\"parts\":[{\"text\":\"give me nothing but the 9 short Dialogue sentences, no numbers, no pretext, make it soulsborne style, %s\"}]}]}"), *FileContent);


        FString ApiEndpoint = TEXT("https://generativelanguage.googleapis.com/v1beta/models/gemini-1.5-flash-latest:generateContent?key=");
        FString FullEndpoint = ApiEndpoint + APIKey;
        SendPostRequest(FullEndpoint, JsonPayload);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to retrieve GeminiaiKey from Secrets.ini"));
    }
}





















void AC_QuestKnight::ProcessReceivedText(const FString& ReceivedText)
{
    TArray<FString> Lines;
    ReceivedText.ParseIntoArrayLines(Lines);
    if (Lines.Num() > 0)
    {
        for (const FString& Line : Lines)
        {
            Dialog.Add(Line);
            UE_LOG(LogTemp, Warning, TEXT("Successfully read line: %s"), *Line);
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Received text is empty"));
    }
}

void AC_QuestKnight::BeginPlay()
{
    Super::BeginPlay();

    fileToRead();
}
