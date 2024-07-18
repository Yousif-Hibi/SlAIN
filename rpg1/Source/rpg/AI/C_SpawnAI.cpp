#include "C_SpawnAI.h"
#include "Engine/World.h"
#include "C_mageAI.h"
#include "C_HumanoidEnemy.h"
#include "PatrolPath.h"
#include "Kismet/GameplayStatics.h"
#include "rpg/Component/C_StatsComponent.h"
#include "rpg/Component/C_CombatComponent.h"
#include "rpg/Actors/C_magicSpell.h"
#include "rpg/Actors/C_BaseMagicWeapon.h"
#include "rpg/Actors/C_BaseWeapon.h"
#include "rpg/AI/C_MasterAI.h"
#include "rpg/Enums/E_CharacterAction.h"
#include "TimerManager.h"
#include "HAL/PlatformFileManager.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
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
#include "rpg/SoulsLikeCharacter.h"
// Sets default values
AC_SpawnAI::AC_SpawnAI()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Ensure MageClass is initialized (optional, typically done in Editor)
	// MageClass = AC_mageAI::StaticClass();
}

// Called when the game starts or when spawned
void AC_SpawnAI::BeginPlay()
{
	Super::BeginPlay();
	SendPostQestRequest("x");
	//spawnMage(1);
	//spawnKnight(1);

}

// Called every frame
void AC_SpawnAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AC_SpawnAI::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	UE_LOG(LogTemp, Log, TEXT("Main Text: worked here "));
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
					UE_LOG(LogTemp, Warning, TEXT("Main Text: %s"), *MainText);

					// Process the received text
					ProcessReceivedText(MainText);
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

void AC_SpawnAI::SendPostRequest(FString ApiEndpoint, FString JsonContent)
{
	FHttpModule* Http = &FHttpModule::Get();
	if (!Http) { return; }

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &AC_SpawnAI::OnResponseReceived);
	Request->SetURL(ApiEndpoint);
	Request->SetVerb("POST");
	Request->SetHeader("Content-Type", "application/json");
	Request->SetContentAsString(JsonContent);
	Request->ProcessRequest();
}

void AC_SpawnAI::SendPostQestRequest(FString FileContent)
{
	FString APIKey;
	FString ConfigFilePath = FPaths::Combine(FPaths::ProjectConfigDir(), TEXT("Secrets.ini"));
	ConfigFilePath = FConfigCacheIni::NormalizeConfigIniPath(ConfigFilePath); 

	if (GConfig->GetString(TEXT("APIKeys"), TEXT("GeminiaiKey"), APIKey, ConfigFilePath))
	{
		if (ASoulsLikeCharacter* PlayerCharacter = Cast<ASoulsLikeCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))) {
			int32 d = 0;
			float points = PlayerCharacter->StatsComponents->GetBaseStateValue(Estat::Health) ;
			if (PlayerCharacter->CombatComponent->GetMainWeapon()) {
				d = PlayerCharacter->CombatComponent->GetMainWeapon()->Damege;
			}
			int32 IntPoints = static_cast<int32>(points)+d;
			UE_LOG(LogTemp, Warning, TEXT("sdfsdfsdfsdfsd %d."), IntPoints);

			FString JsonPart1 = TEXT("{\"contents\":[{\"parts\":[{\"text\":\"");
			FString JsonPart2 = TEXT("  i want you to choose between making Adding More NPCs & Varying Strength or Creating a Powerful Elite ");
			FString JsonPart3 =  FString::Printf(TEXT("I want to spawn 1-4 npcs between Knights and mages. Can you tell me how many to spawn, and you have %d points between everything that spawns. "), IntPoints)  ;
			FString JsonPart4 = TEXT(" The points will be divided between the number of knights or number of mages that you give, by filling this and responding only with this but each on a line");
			FString JsonPart5 = TEXT(" Knight: . magi: .knighthealthpoints: . magihealthpoints: .knightdamagepoints: . magidamagepoints: . \"}]}]}");

			FString JsonPayload = JsonPart1 + JsonPart2 + JsonPart3 + JsonPart4 + JsonPart5;
			UE_LOG(LogTemp, Warning, TEXT("sdfsdfsdfsdfsd %s."),* JsonPayload);
			  
			FString ApiEndpoint = TEXT("https://generativelanguage.googleapis.com/v1beta/models/gemini-1.5-flash-latest:generateContent?key=");
			FString FullEndpoint = ApiEndpoint + APIKey;
			SendPostRequest(FullEndpoint, JsonPayload);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed   to retrieve GeminiaiKey from Secrets.ini"));
	}
}

void AC_SpawnAI::spawnMage(int32 numToSpawn)
{
	for (int32 i = 0; i < numToSpawn; ++i)
	{
		AC_mageAI* SpawnedMage = GetWorld()->SpawnActor<AC_mageAI>(MageClass, GetActorLocation(), FRotator::ZeroRotator);
		if (SpawnedMage)
		{
			APatrolPath* ClosestPath = FindClosestPatrolPath();
			if (ClosestPath)
			{
				// Assign the patrol path
				SpawnedMage->SetPatrolPath(ClosestPath);
				SpawnedMage->StatsComponent->SetMaxStateValue(Estat::Health, magiHealth);
				SpawnedMage->StatsComponent->SetBaseStateValue(Estat::Health, magiHealth);
				SpawnedMage->StatsComponent->ModifyCurrentStatValue(Estat::Health, magiHealth, false);
				auto* weapon = Cast<AC_BaseMagicWeapon>(SpawnedMage->CombatComponent->GetMainWeapon());
				if (weapon)
				{
					weapon->MagicDamegeSet(magiDamage);
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Weapon  is null"));
				}
				UE_LOG(LogTemp, Warning, TEXT("Mage %d spawned and closest patrol path assigned."), i);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("No patrol path found for Mage %d."), i);
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to spawn Mage %d."), i);
		}
	}

}

void AC_SpawnAI::spawnKnight(int32 numToSpawn)
{

	for (int32 i = 0; i < numToSpawn; ++i)
	{
		AC_HumanoidEnemy* SpawnedKnight = GetWorld()->SpawnActor<AC_HumanoidEnemy>(KnightClass, GetActorLocation(), FRotator::ZeroRotator);
		if (SpawnedKnight)
		{
			APatrolPath* ClosestPath = FindClosestPatrolPath();
			if (ClosestPath)
			{
				// Assign the patrol path
				SpawnedKnight->SetPatrolPath(ClosestPath);
				SpawnedKnight->StatsComponent->SetMaxStateValue(Estat::Health, knightHealth);
				SpawnedKnight->StatsComponent->SetBaseStateValue(Estat::Health, knightHealth);
				SpawnedKnight->StatsComponent->ModifyCurrentStatValue(Estat::Health, knightHealth, false);
				SpawnedKnight->mainWeapon->setDmg( knightDamage);
				
				UE_LOG(LogTemp, Warning, TEXT("Mage %d spawned and closest patrol path assigned."), i);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("No patrol path found for Mage %d."), i);
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to spawn Mage %d."), i);
		}
	}

}
void AC_SpawnAI::ProcessReceivedText(const FString& ReceivedText)
{

	TArray<FString> Lines;
	ReceivedText.ParseIntoArrayLines(Lines);
	if (Lines.Num() > 0)
	{
		for (const FString& Line : Lines)
		{
			TArray<FString> KeyValue;
			Line.ParseIntoArray(KeyValue, TEXT(": "), true);

			if (KeyValue.Num() == 2)
			{
				FString Key = KeyValue[0].ToLower();
				int32 Value = FCString::Atoi(*KeyValue[1]);

				if (Key == TEXT("knight"))
				{
					NumknightsToSpawn = Value;
				}
				else if (Key == TEXT("magi"))
				{
					NumMagesToSpawn = Value;
				}

				else if (Key == TEXT("knighthealthpoints"))
				{
					if (NumknightsToSpawn > 0)
						knightHealth = (Value / NumknightsToSpawn) * 10;
				}
				else if (Key == TEXT("knightdamagepoints"))
				{
					if (NumknightsToSpawn > 0)
						knightDamage = (Value / NumknightsToSpawn) * 5;
				}
				else if (Key == TEXT("magihealthpoints"))
				{
					if (NumMagesToSpawn > 0)
						magiHealth = (Value / NumMagesToSpawn) * 10;
				}

				else if (Key == TEXT("magidamagepoints"))
				{
					if (NumMagesToSpawn > 0)
						magiDamage = (Value / NumMagesToSpawn) * 5;
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Unknown key: %s"), *Key);
				}
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Invalid line format: %s"), *Line);
			}
		}
		spawnKnight(NumknightsToSpawn);
		spawnMage(NumMagesToSpawn);

		UE_LOG(LogTemp, Warning, TEXT("NumKnightsToSpawn: %d"), NumknightsToSpawn);
		UE_LOG(LogTemp, Warning, TEXT("NumMagesToSpawn: %d"), NumMagesToSpawn);
		UE_LOG(LogTemp, Warning, TEXT("KnightHealth: %d"), knightHealth);
		UE_LOG(LogTemp, Warning, TEXT("MagiHealth: %d"), magiHealth);
		UE_LOG(LogTemp, Warning, TEXT("KnightDamage: %d"), knightDamage);
		UE_LOG(LogTemp, Warning, TEXT("MagiDamage: %d"), magiDamage);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Received text is empty"));
	}

}

APatrolPath* AC_SpawnAI::FindClosestPatrolPath() const
{
	TArray<AActor*> PatrolPaths;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APatrolPath::StaticClass(), PatrolPaths);

	APatrolPath* ClosestPath = nullptr;
	float MinDistance = TNumericLimits<float>::Max();
	FVector SpawnLocation = GetActorLocation();

	for (AActor* Actor : PatrolPaths)
	{
		APatrolPath* PatrolPath = Cast<APatrolPath>(Actor);
		if (PatrolPath)
		{
			float Distance = FVector::Dist(SpawnLocation, PatrolPath->GetActorLocation());
			if (Distance < MinDistance)
			{
				MinDistance = Distance;
				ClosestPath = PatrolPath;
			}
		}
	}

	return ClosestPath;
}
