#include "C_QuestKnight.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"
#include <Components/TextBlock.h>
#include "TimerManager.h"
#include "HAL/PlatformFileManager.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Kismet/GameplayStatics.h"
#include "rpg/C_SaveGame.h"
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
#include "rpg/AI/PatrolPath.h"
#include "Components/Button.h"    
#include "rpg/Actors/C_PickupActor.h"
#include "rpg/SoulsLikeCharacter.h"
#include "rpg/Dungen/C_door.h"
#include "EngineUtils.h" 






AC_QuestKnight::AC_QuestKnight(const FObjectInitializer& ObjectInitializer)
{
    static ConstructorHelpers::FClassFinder<UUserWidget> QuestText(TEXT("/Game/Static/quests/WB_QuestText.WB_QuestText_C"));
    if (QuestText.Succeeded())
    {
        widget = QuestText.Class;
        
    }
    static ConstructorHelpers::FClassFinder<UUserWidget> QuestTextAnswer(TEXT("/Game/Static/quests/WB_QuestAnswer"));
    if (QuestTextAnswer.Succeeded())
    {
        widgetanswer = QuestTextAnswer.Class; 

    }
    else {
        UE_LOG(LogTemp, Error, TEXT("Failed to find   QuestTextBlock"));
    }
    Loadgame();
       fileDialog = {
        TEXT("In the desolate and ravaged landscape of The Rustborn Wastes, the remnants of a once-glorious world stand as stark reminders of its past conflicts. The land, now divided into distinct regions, each governed by one of the elemental clans, bears witness to the aftermath of ancient wars. These clans, bound by their elemental conduits, hold dominion over their territories, their powers a blend of earth's primal forces. The Clan, among these, is the most formidable, its stronghold a bastion of strength. As a knight from a far-off land, you journey to this fractured world with a singular aim: to claim the  Conduit, a source of immense power, and reshape the balance within these war-torn lands."),
        TEXT("Long ago, in a desperate bid to secure  their dominion and harness the elemental powers more effectively, the clans of The Rustborn Wastes devised a series of powerful enchantments. These magics, intended to bind and channel elemental forces, inadvertently transformed their most devoted followers into humanoid goats—creatures both wise and formidable. These beings, known as the Gauron, now serve as the clan’s most elite guardians and emissaries. Possessing a unique blend of human intellect and goat-like resilience, they have become key figures in the protection of the conduits. Their transformation was a testament to the depth of their commitment, and now, they stand as both allies and obstacles in your quest to obtain the Iron Conduit."),
        TEXT("As you venture deeper into the Ironclad Clan’s citadel, you encounter the Gauron, who serve as both protectors and gatekeepers of the conduit. The path to the Iron Conduit is fraught with challenges, but through clever strategy and bravery, you manage to forge a crucial alliance with a sympathetic Gauron. This ally, seeing the righteousness of your cause, aids you in navigating the citadel's defenses and overcoming its trials. Together, you confront the clan’s leaders in a climactic battle, showcasing your skill and determination. With the help of your new ally, you successfully claim the Iron Conduit, securing its power and marking a pivotal moment in the transformation of The Rustborn Wastes.")
    };
}

void AC_QuestKnight::TalkToNPC()
{
    IInteractive_CI::TalkToNPC();
    UC_HTTPGoogleApi *callApi= NewObject<UC_HTTPGoogleApi>(this);;
    DialogIndex++;
    if (Dialog.Num() > DialogIndex)
    {
        UE_LOG(LogTemp, Warning, TEXT("%s"), *Dialog[DialogIndex]);
       
       
       callApi->SynthesizeSpeech(Dialog[DialogIndex]);
       if (Dialog[DialogIndex].Contains("?"))
       {
           // Show Yes/No buttons on the UI
          ShowYesNoOptions(Dialog[DialogIndex]);
         
       }
       else {
           ShowWidget(Dialog[DialogIndex]);
       }
    }
    else
    {
        if (PartNumber < 3) {
            Savegame();
            DialogIndex = -1;
            Dialog.Empty();
            PartNumber++;
            fileToRead();
            
        }
        else {
            moveDoors();
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

void AC_QuestKnight::Savegame()
{
    FString SaveSlotName = TEXT("PlayerSaveSlot");
    const int32 UserIndex = 0;

    UC_SaveGame* SaveGameInstance;

    // Check if the save game already exists
    if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, UserIndex))
    {
        // If it exists, load the existing save game
        SaveGameInstance = Cast<UC_SaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, UserIndex));
    }
    else
    {
        // If it does not exist, create a new save game instance
        SaveGameInstance = Cast<UC_SaveGame>(UGameplayStatics::CreateSaveGameObject(UC_SaveGame::StaticClass()));
    }

    if (SaveGameInstance)
    {


        SaveGameInstance->Questline = PartNumber;
        SaveGameInstance->QuestRotation = this->QuestRotation;
        SaveGameInstance->QuestLocation = this->QuestLocation;


       
        // Save the updated game instance
        UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveSlotName, UserIndex);







    }


}
void AC_QuestKnight::Loadgame()
{

    // Load the save game instance from the specified slot
    UC_SaveGame* LoadGameInstance = Cast<UC_SaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("PlayerSaveSlot"), 0));

    if (LoadGameInstance)
    {
        // PartNumber= LoadGameInstance->Questline;
         this->QuestRotation = LoadGameInstance->QuestRotation ;
         this->QuestLocation = LoadGameInstance->QuestLocation;
        
      
    }
    else {
        Savegame();
    }

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

void AC_QuestKnight::ShowYesNoOptions(FString str)
{

    if (UWorld* World = GetWorld())
    {
        if (CurrentWidget)
        {
            CurrentWidget->RemoveFromParent();
            CurrentWidget = nullptr;
        }



        if (widgetanswer)
        {

            UUserWidget* YesNoWidgetInstance = CreateWidget<UUserWidget>(World, widgetanswer);
            if (YesNoWidgetInstance)
            {
                APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
                if (PlayerController)
                {
                    PlayerController->bShowMouseCursor = true;
                    PlayerController->SetInputMode(FInputModeUIOnly());
                }
                CurrentWidget = CreateWidget<UUserWidget>(World, widgetanswer);
                if (CurrentWidget)
                {
                    CurrentWidget->AddToViewport();
                    UButton* YesButton = Cast<UButton>(CurrentWidget->GetWidgetFromName(TEXT("yes")));
                    UButton* NoButton = Cast<UButton>(CurrentWidget->GetWidgetFromName(TEXT("no")));

                    if (YesButton)
                    {
                        YesButton->OnClicked.AddDynamic(this, &AC_QuestKnight::OnYesSelected);
                    }

                    if (NoButton)
                    {
                        NoButton->OnClicked.AddDynamic(this, &AC_QuestKnight::OnNoSelected);
                    }
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
                   // GetWorldTimerManager().SetTimer(WidgetTimerHandle, this, &AC_QuestKnight::RemoveCurrentWidget, 5.0f, false);


                // Bind buttons to functions
                    


                }

            }
        }
    }
}
void AC_QuestKnight::OnYesSelected()
{
   
    // Handle the logic when Yes is selected
    UE_LOG(LogTemp, Warning, TEXT("Yes Selected"));
    // Continue with the quest or take specific action
    RemoveCurrentWidget();
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PlayerController)
    {
        PlayerController->bShowMouseCursor = false;
        PlayerController->SetInputMode(FInputModeGameOnly());
    }
    if (auto* const PlayerCharacter =Cast<ASoulsLikeCharacter> (UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))) {
        ReputationIndex++;
        PlayerCharacter->ReputationIndex = ReputationIndex;
    }

    if (ReputationIndex == 10) {
        giveRewored();
    }
    UE_LOG(LogTemp, Warning, TEXT("reputation , %d"),ReputationIndex);
}   

void AC_QuestKnight::OnNoSelected()
{
    // Handle the logic when No is selected
    UE_LOG(LogTemp, Warning, TEXT("No Selected"));
    // Continue with the quest or take specific action
    RemoveCurrentWidget();
    if (auto* const PlayerCharacter = Cast<ASoulsLikeCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))) {
        ReputationIndex--;
        PlayerCharacter->ReputationIndex = ReputationIndex;
    }
   
    UE_LOG(LogTemp, Warning, TEXT("reputation , %d"), ReputationIndex);
}

void AC_QuestKnight::giveRewored()
{
    FVector Location= GetActorLocation();
    Location.X += 50;
    auto * reword = GetWorld()->SpawnActor<AC_PickupActor>(condoiet, Location, FRotator::ZeroRotator);
}

void AC_QuestKnight::Teleport(FVector Location)
{
   
    if (bAllowedTOTelleport) {
       // SetActorLocation(Location);
        bAllowedTOTelleport = false;
       
    }
}

void AC_QuestKnight::fileToRead()
{
    bool bSuccess;
    FString InfoMessage;

    // Construct the file path based on PartNumber
    FString FilePath = FString::Printf(TEXT("Static/quests/QuestText/KnightQuestPart%d.txt"), PartNumber);
    FString FullFilePath = FPaths::ProjectContentDir() + FilePath;

    // Log the file path for debugging purposes
    UE_LOG(LogTemp, Warning, TEXT("Attempting to read file at path: %s"), *FullFilePath);

    FString FileContent = ReadStringFromFile(FullFilePath, bSuccess, InfoMessage);

    if (bSuccess)
    {
        // File read successfully, send request with file content
        SendPostQestRequest(FileContent);
    }
    else
    {
        // Failed to read file, fallback to fileDialog
        UE_LOG(LogTemp, Warning, TEXT("Failed to read file: %s"), *InfoMessage);

        // Use fileDialog content if available
        if (fileDialog.Num() > 0 && PartNumber <= fileDialog.Num())
        {
            FString DialogContent = fileDialog[PartNumber - 1]; // Adjust index to match PartNumber
            SendPostQestRequest(DialogContent);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("No fallback content available in fileDialog"));
        }
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
        FString JsonPayload = FString::Printf(TEXT("{\"contents\":[{\"parts\":[{\"text\":\"give me nothing but the 6 short Dialogue sentences and 3  in a yes and no form qustions as a part of the dialogue , no numbers, no pretext, make it soulsborne style, %s\"}]}]}"), *FileContent);


        FString ApiEndpoint = TEXT("https://generativelanguage.googleapis.com/v1beta/models/gemini-1.5-flash-latest:generateContent?key=");
        FString FullEndpoint = ApiEndpoint + APIKey;
        SendPostRequest(FullEndpoint, JsonPayload);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to retrieve GeminiaiKey from Secrets.ini"));
    }
}

void AC_QuestKnight::moveDoors()
{
    for (TActorIterator<AC_door> DoorItr(GetWorld()); DoorItr; ++DoorItr)
    {
        // Get the current location of the door
        FVector CurrentLocation = DoorItr->GetActorLocation();

        // Move the door 300 units in the Z direction
        FVector NewLocation = CurrentLocation + FVector(0, 0, -300);

        // Set the new location of the door
        DoorItr->SetActorLocation(NewLocation);
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
