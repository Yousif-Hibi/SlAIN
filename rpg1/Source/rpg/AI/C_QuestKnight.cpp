#include "C_QuestKnight.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"
#include <Components/TextBlock.h>
#include "TimerManager.h"
#include "HAL/PlatformFileManager.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include <Blueprint/AIBlueprintHelperLibrary.h>
#include "HAL/PlatformProcess.h"




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

	DialogIndex++;
	if (Dialog.Num() > DialogIndex)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *Dialog[DialogIndex]);
		ShowWidget(Dialog[DialogIndex]);
	}
	else
	{
		
		if (PartNumber <= 3) {
			DialogIndex = -1;
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
			CurrentWidget->RemoveFromViewport();
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





FString AC_QuestKnight::RunPythonScript(FString ScriptPath, FString TextToSend)
{
	FString PythonExecutable = TEXT("python"); // Path to the Python interpreter executable
	FString Arguments = FString::Printf(TEXT("\"%s\" \"%s\""), *ScriptPath, *TextToSend); // Include text as an argument

	FString Output;
	int32 ExitCode = -1; // Initialize the exit code variable

	bool bSuccess = FPlatformProcess::ExecProcess(*PythonExecutable, *Arguments, &ExitCode, &Output, nullptr);

	if (bSuccess)
	{
		UE_LOG(LogTemp, Warning, TEXT("Python script executed successfully"));
		UE_LOG(LogTemp, Warning, TEXT("Python script output: %s"), *Output);
		UE_LOG(LogTemp, Warning, TEXT("Python script exit code: %d"), ExitCode);
		return Output;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to execute Python script"));
	}
	return TextToSend;
}







void AC_QuestKnight::RemoveCurrentWidget()
{
	if (CurrentWidget)
	{
		CurrentWidget->RemoveFromViewport();
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
	//CreateQuestFile();
	UE_LOG(LogTemp, Warning, TEXT("sssssssssssssss %d"), PartNumber);
	FString FilePath = FString::Printf(TEXT("Static/quests/QuestText/KnightQuestPart%d.txt"), PartNumber);
	// Correcting the file path by ensuring it's relative to the project's content directory
	FString FullFilePath = FPaths::ProjectContentDir() + FilePath;

	// Log the file path for debugging purposes
	UE_LOG(LogTemp, Warning, TEXT("Attempting to read file at path: %s"), *FullFilePath);

	FString FileContent = ReadStringFromFile(FullFilePath, bSuccess, InfoMessage);
	FString PythonScriptPath = FPaths::ProjectContentDir() + TEXT("python/main.py");
	FString AiAgustedContent =RunPythonScript(PythonScriptPath, FileContent);
	UE_LOG(LogTemp, Warning, TEXT("Successfully read linessssssssss: %s"), *AiAgustedContent);
	if (bSuccess)
	{
		TArray<FString> Lines;
		AiAgustedContent.ParseIntoArrayLines(Lines);
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
			UE_LOG(LogTemp, Error, TEXT("File is empty: %s"), *InfoMessage);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s"), *InfoMessage);
	}
	
}

void AC_QuestKnight::BeginPlay()
{
	Super::BeginPlay();
	
	fileToRead();
	

}
