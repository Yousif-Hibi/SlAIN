// Fill out your copyright notice in the Description page of Project Settings.


#include "C_Dungeons.h"
#include "C_room.h"
#include "rpg/C_fillerWall.h"
#include "Math/UnrealMathUtility.h"
#include "Components/BoxComponent.h"
#include "rpg/Dungen/C_MeshToSpawn.h"
#include "rpg/Dungen/C_SpawnOnWall.h"
#include "rpg/Dungen/C_door.h"
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

// Sets default values
AC_Dungeons::AC_Dungeons()
{
    // Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AC_Dungeons::BeginPlay()
{
    Super::BeginPlay();
    SpawnStartRoom();
    
   
}

// Called every frame
void AC_Dungeons::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AC_Dungeons::SpawnStartRoom()
{
    room = GetWorld()->SpawnActor<AC_room>(roomClass1, GetActorLocation(), FRotator::ZeroRotator);
    if (room) {
        if (USceneComponent* canSpawnRoom = room->Exits) {
            canSpawnRoom->GetChildrenComponents(false, SpawnDiractions);
            if (!SpawnDiractions.IsEmpty()) {
                UE_LOG(LogTemp, Warning, TEXT("Got the arrows"));
            }
            for (USceneComponent* SpawnDirection : SpawnDiractions) {
                if (SpawnDirection) {
                    FRotator Rotation = SpawnDirection->GetComponentRotation();
                    FVector Offset =  Rotation.Vector();
                 //   Rotation.Yaw =+ 90.f;
                    FVector Location = SpawnDirection->GetComponentLocation() + Offset;
                    //Location.Z += 250.f;

                    FActorSpawnParameters SpawnParams;
                    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
                    auto* NewDoor = GetWorld()->SpawnActor<AC_door>(doorClass, Location, Rotation, SpawnParams);
                    if (NewDoor) {
                        NewDoor->SetActorScale3D(FVector(1.2f, 1.2f, 1.2f));
                       
                    }
                }
            }
        }

        if (USceneComponent* canSpawnPoints = room->floorSpawnPoints) {
            if (canSpawnPoints->GetNumChildrenComponents() > 0) {
                canSpawnPoints->GetChildrenComponents(false, SpawnPointsList);
              

                if (USceneComponent* canSpawnWallPoints = room->wallSpawnPoints) {
                    if (canSpawnWallPoints->GetNumChildrenComponents() > 0) {
                        canSpawnWallPoints->GetChildrenComponents(false, SpawnOnWallList);
                        SendPostQestRequest("x");
                       

                    }
                    else {
                        UE_LOG(LogTemp, Warning, TEXT("floorSpawnPoinnts has no children"));
                    }
                }
                else {
                    UE_LOG(LogTemp, Warning, TEXT("floorSpawnPoinnts is null"));
                }

            }
            else {
                UE_LOG(LogTemp, Warning, TEXT("floorSpawnPoinnts has no children"));
            }
        }
        else {
            UE_LOG(LogTemp, Warning, TEXT("floorSpawnPoinnts is null"));
        }
        

    }
}

void AC_Dungeons::SpawnNextRoom()
{
    for (int32 x = 0; x < 20; x++) {
        if (SpawnDiractions.Num() > 0) {
            int32 RandomDirection = FMath::RandRange(0, SpawnDiractions.Num() - 1);
            USceneComponent* RandomComponent = SpawnDiractions[RandomDirection];
            if (RandomComponent) {
                if (RoomList.Num() > 0) {
                    int32 RandomRoomIndex = FMath::RandRange(0, RoomList.Num() - 1);
                    TSubclassOf<AC_room> RandomRoomClass = RoomList[RandomRoomIndex];

                    if (RandomRoomClass) {
                        FActorSpawnParameters SpawnParams;
                        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
                        Randomroom = GetWorld()->SpawnActor<AC_room>(RandomRoomClass, RandomComponent->GetComponentLocation(), RandomComponent->GetComponentRotation(), SpawnParams);

                        if (checkOverLap(Randomroom)) {
                            SpawnDiractions.RemoveAt(RandomDirection);
                            Randomroom->Destroy();
                            x--;
                        }
                        else {
                            if (USceneComponent* canSpawnPoints = Randomroom->floorSpawnPoints) {
                                TArray<USceneComponent*> NewPoints;
                                canSpawnPoints->GetChildrenComponents(false, SpawnPointsList);
                                if (USceneComponent* canSpawnWallPoints = Randomroom->wallSpawnPoints) {
                                    TArray<USceneComponent*> NewWallPoints;
                                    canSpawnWallPoints->GetChildrenComponents(false, SpawnOnWallList);
                                    spawnWallMeshatLocation();
                                    spawnMeshatLocation();
                                }
                            }
                            



                            SpawnDiractions.RemoveAt(RandomDirection);
                            if (USceneComponent* canSpawnRoom = Randomroom->Exits) {
                                TArray<USceneComponent*> NewExits;
                                canSpawnRoom->GetChildrenComponents(false, NewExits);
                                if (!NewExits.IsEmpty()) {
                                    SpawnDiractions.Append(NewExits);
                                    UE_LOG(LogTemp, Warning, TEXT("Got the arrows, %d"), SpawnDiractions.Num());
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    FillWalls();
    
}

bool AC_Dungeons::checkOverLap(AC_room* NewRoom)
{
    bool bIsOverlapping = false;
    TArray<USceneComponent*> CollisionComponents;
    NewRoom->collisons->GetChildrenComponents(false, CollisionComponents);

    for (USceneComponent* CollisionComponent : CollisionComponents) {
        if (UBoxComponent* BoxComponent = Cast<UBoxComponent>(CollisionComponent)) {
            TArray<FOverlapResult> OverlapResults;
            FCollisionQueryParams QueryParams;
            QueryParams.AddIgnoredActor(NewRoom);

            GetWorld()->OverlapMultiByChannel(
                OverlapResults,
                BoxComponent->GetComponentLocation(),
                FQuat::Identity,
                ECC_WorldDynamic,
                FCollisionShape::MakeBox(BoxComponent->GetScaledBoxExtent()),
                QueryParams
            );

            if (OverlapResults.Num() > 0) {
                bIsOverlapping = true;
                return true;
            }
        }
    }
    return false;
}

void AC_Dungeons::FillWalls()
{
    if (!FillerWallClass) {
        UE_LOG(LogTemp, Warning, TEXT("FillerWallClass is not set"));
        return;
    }
    if (room && room->GetRootComponent()) {
        USceneComponent* RootComponentT = room->GetRootComponent();
        FVector StartLocation = RootComponentT->GetComponentLocation();
        FRotator Rotation = RootComponentT->GetComponentRotation();
        FVector Offset = -1000.f * Rotation.Vector();
        FVector EndLocation = StartLocation + Offset;
        EndLocation.Z += 250.f;

        FHitResult HitResult;
        FCollisionQueryParams CollisionParams;
        CollisionParams.AddIgnoredActor(this); // Ignore the dungeon actor itself
        CollisionParams.AddIgnoredActor(room); // Ignore the room actor

        bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, CollisionParams);

        DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, 5.0f);

        if (!bHit) {
            EndLocation.X += 100.0f;
            FActorSpawnParameters SpawnParams;
            SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
            AC_fillerWall* NewWall = GetWorld()->SpawnActor<AC_fillerWall>(FillerWallClass, EndLocation, Rotation, SpawnParams);
            if (NewWall) {
                NewWall->SetActorScale3D(FVector(1.0f, 6.0f, 7.0f));
            }
        }
        else {
            UE_LOG(LogTemp, Warning, TEXT("Collision detected, not spawning filler wall"));
        }
    }

    for (USceneComponent* SpawnDirection : SpawnDiractions) {
        if (SpawnDirection) {
            FRotator Rotation = SpawnDirection->GetComponentRotation();
            FVector Offset = -1000.f * Rotation.Vector();
            FVector Location = SpawnDirection->GetComponentLocation() + Offset;
            Location.Z += 250.f;

            FActorSpawnParameters SpawnParams;
            SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
            AC_fillerWall* NewWall = GetWorld()->SpawnActor<AC_fillerWall>(FillerWallClass, Location, Rotation, SpawnParams);
            if (NewWall) {
                NewWall->SetActorScale3D(FVector(1.0f, 6.0f, 7.0f));
            }
        }
    }
}



void AC_Dungeons::spawnMeshatLocation()
{
    if (SpawnPointsList.Num() > 0) {
        for (int i = 0; i < 4; i++) {
            int32 RandomSpawnPointInt = FMath::RandRange(0, SpawnPointsList.Num() - 1);
            UE_LOG(LogTemp, Warning, TEXT("Got the spawn points, %d"), SpawnPointsList.Num());
            USceneComponent* RandomSpawnPoint = SpawnPointsList[RandomSpawnPointInt];

            if (RandomSpawnPoint && meshClass) {
                FActorSpawnParameters SpawnParams;
                SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
                
                   
                // Spawn the mesh actor at the location
                auto MeshActor = GetWorld()->SpawnActor<AC_MeshToSpawn>(meshClass, RandomSpawnPoint->GetComponentLocation(), RandomSpawnPoint->GetComponentRotation(), SpawnParams);
          
                if (MeshActor) {
                    MeshActor->getItemToSpawn(ParsedFloorItems[i]);
                    SpawnPointsList.RemoveAt(RandomSpawnPointInt);
                    UE_LOG(LogTemp, Warning, TEXT("Spawned mesh actor at location"));
                }
                else {
                    UE_LOG(LogTemp, Warning, TEXT("Failed to spawn mesh actor"));
                } 
            }
        }
    }
    else {
        UE_LOG(LogTemp, Warning, TEXT("No spawn   points available"));
    }
}

void AC_Dungeons::spawnWallMeshatLocation()
{
    if (SpawnOnWallList.Num() > 0) {
        for (int i = 0; i < 3; i++) {
            int32 RandomSpawnPointInt = FMath::RandRange(0, SpawnOnWallList.Num() - 1);
            UE_LOG(LogTemp, Warning, TEXT("Got the spawn points, %d"), SpawnOnWallList.Num());
            USceneComponent* RandomSpawnPoint = SpawnOnWallList[RandomSpawnPointInt];

            if (RandomSpawnPoint && wallMeshClass) {
                FActorSpawnParameters SpawnParams;
                SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;


                // Spawn the mesh actor at the location
                auto wallMeshActor = GetWorld()->SpawnActor<AC_SpawnOnWall>(wallMeshClass, RandomSpawnPoint->GetComponentLocation(), RandomSpawnPoint->GetComponentRotation(), SpawnParams);

                if (wallMeshActor) {
                    wallMeshActor->getItemToSpawn(ParsedWallItems[i]);
                    SpawnOnWallList.RemoveAt(RandomSpawnPointInt);
                    UE_LOG(LogTemp, Warning, TEXT("Spawned mesh actor at location"));
                }
                else {
                    UE_LOG(LogTemp, Warning, TEXT("Failed to spawn mesh actor"));
                }
            }
        }
    }  
    else {
        UE_LOG(LogTemp, Warning, TEXT("No spawn   points available"));
    }
}
void AC_Dungeons::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
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

void AC_Dungeons::SendPostRequest(FString ApiEndpoint, FString JsonContent)
{
    FHttpModule* Http = &FHttpModule::Get();
    if (!Http) { return; }

    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();
    Request->OnProcessRequestComplete().BindUObject(this, &AC_Dungeons::OnResponseReceived);
    Request->SetURL(ApiEndpoint);
    Request->SetVerb("POST");
    Request->SetHeader("Content-Type", "application/json");
    Request->SetContentAsString(JsonContent);
    Request->ProcessRequest();
}

void AC_Dungeons::SendPostQestRequest(FString FileContent)
{
    FString APIKey;
    FString ConfigFilePath = FPaths::Combine(FPaths::ProjectConfigDir(), TEXT("Secrets.ini"));
    ConfigFilePath = FConfigCacheIni::NormalizeConfigIniPath(ConfigFilePath);

    if (GConfig->GetString(TEXT("APIKeys"), TEXT("GeminiaiKey"), APIKey, ConfigFilePath))
    {
      

            FString JsonPart1 = TEXT("{\"contents\":[{\"parts\":[{\"text\":\"");
            FString JsonPart2 = TEXT("  i want you to choose what to spawn on wall amd floor, 3 things to spawn in wall and 4 things to spawn on floor,  ");
            FString JsonPart3 = TEXT("  on the wall you can chose from :  lamp1 , lamp2 ,brazier,cage , statue . ");
            FString JsonPart4 = TEXT("  on the floor you can chose from :   tree , grass ,brokenfloor1 ,brokenfloor2 ,  well ,   barrels , boxes ,  lavaBarrel .  ");

            FString JsonPart5 = TEXT(" only choose from the list , and response with : wall : , , . flloor : , , ,  . \"}]}]}");

            FString JsonPayload = JsonPart1 + JsonPart2 + JsonPart3 + JsonPart4+  JsonPart5;
            UE_LOG(LogTemp, Warning, TEXT("sdfsdfsdfsdfsd %s."), *JsonPayload);

            FString ApiEndpoint = TEXT("https://generativelanguage.googleapis.com/v1beta/models/gemini-1.5-flash-latest:generateContent?key=");
            FString FullEndpoint = ApiEndpoint + APIKey;
            SendPostRequest(FullEndpoint, JsonPayload);
        
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed   to retrieve GeminiaiKey from Secrets.ini"));
    }
}
void AC_Dungeons::ProcessReceivedText(const FString& ReceivedText)
{
    UE_LOG(LogTemp, Warning, TEXT("Received Text: %s"), *ReceivedText);

    ParsedWallItems.Empty();
    ParsedFloorItems.Empty();

    TArray<FString> Lines;
    ReceivedText.ParseIntoArrayLines(Lines);

    if (Lines.Num() > 0)
    {
        for (const FString& Line : Lines)
        {
            FString Key;
            FString Value;

            if (Line.Split(TEXT(":"), &Key, &Value))
            {
                Key = Key.TrimStartAndEnd();
                Value = Value.TrimStartAndEnd();

                TArray<FString> Items;
                Value.ParseIntoArray(Items, TEXT(","), true);

                if (Key.Equals(TEXT("wall"), ESearchCase::IgnoreCase))
                {
                    ParsedWallItems.Append(Items);
                }
                else if (Key.Equals(TEXT("floor"), ESearchCase::IgnoreCase))
                {
                    ParsedFloorItems.Append(Items);
                }
            }
        }
    }
    spawnWallMeshatLocation();
    spawnMeshatLocation();
    UE_LOG(LogTemp, Warning, TEXT("Wall Items: %s"), *FString::Join(ParsedWallItems, TEXT(", ")));
    UE_LOG(LogTemp, Warning, TEXT("Floor Items: %s"), *FString::Join(ParsedFloorItems, TEXT(", ")));
    SpawnNextRoom();
}