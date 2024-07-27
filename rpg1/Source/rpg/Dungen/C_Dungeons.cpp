// Fill out your copyright notice in the Description page of Project Settings.


#include "C_Dungeons.h"
#include "C_room.h"
#include "rpg/C_fillerWall.h"
#include "Math/UnrealMathUtility.h"
#include "Components/BoxComponent.h"
// Sets default values
AC_Dungeons::AC_Dungeons()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AC_Dungeons::BeginPlay()
{
	Super::BeginPlay();
	SpawnStartRoom();
	SpawnNextRoom();
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
		if (USceneComponent * canSpawnRoom = room->Exits) {
			
			canSpawnRoom->GetChildrenComponents(false, SpawnDiractions);
			if (!SpawnDiractions.IsEmpty()) {
				UE_LOG(LogTemp,Warning,TEXT("got the arraws"));
			}




		}
	}

}

void AC_Dungeons::SpawnNextRoom()
{
	for (int32 x = 0; x < 10; x++) {
		if (SpawnDiractions.Num() > 0)
		{
			int32 RandomDiraction = FMath::RandRange(0, SpawnDiractions.Num() - 1);
			USceneComponent* RandomComponent = SpawnDiractions[RandomDiraction];
			if (RandomComponent) {
				if (RoomList.Num() > 0)
				{
					int32 RandomroomIndex = FMath::RandRange(0, RoomList.Num() - 1);
					TSubclassOf<AC_room> RandomRoomClass = RoomList[RandomroomIndex];

					if (RandomRoomClass)
					{
						FActorSpawnParameters SpawnParams;
						SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
						Randomroom = GetWorld()->SpawnActor<AC_room>(RandomRoomClass, RandomComponent->GetComponentLocation(), RandomComponent->GetComponentRotation(), SpawnParams);
						checkOverLap(Randomroom);
						if (Randomroom)
						{
							if (checkOverLap(Randomroom)) {
								SpawnDiractions.RemoveAt(RandomDiraction);
								Randomroom->Destroy();
								x=x-1;
							}
							else {
								SpawnDiractions.RemoveAt(RandomDiraction);
								if (USceneComponent* canSpawnRoom = Randomroom->Exits) {
									TArray<USceneComponent*> NewExits;
									canSpawnRoom->GetChildrenComponents(false, NewExits);
									if (!SpawnDiractions.IsEmpty()) {
										if (!NewExits.IsEmpty()) {
											SpawnDiractions.Append(NewExits);
											UE_LOG(LogTemp, Warning, TEXT("got the arraws,%d"), SpawnDiractions.Num());
										}
									}

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

	for (USceneComponent* CollisionComponent : CollisionComponents)
	{
		if (UBoxComponent* BoxComponent = Cast<UBoxComponent>(CollisionComponent))
		{
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

			if (OverlapResults.Num() > 0)
			{
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

		// Perform collision check
		FHitResult HitResult;
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(this); // Ignore the dungeon actor itself
		CollisionParams.AddIgnoredActor(room); // Ignore the room actor

		bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, CollisionParams);

		// Debug line to visualize the trace
		DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, 5.0f);

		if (!bHit) {
			// No collision, spawn the wall
			EndLocation.X += 100.0f;
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			AC_fillerWall* NewWall = GetWorld()->SpawnActor<AC_fillerWall>(FillerWallClass, EndLocation, Rotation, SpawnParams);
			if (NewWall) {
				NewWall->SetActorScale3D(FVector(1.0f, 4.0f, 4.0f));
			}
		}
		else {
			// Collision detected
			UE_LOG(LogTemp, Warning, TEXT("Collision detected, not spawning filler wall"));
		}
	}
	for (USceneComponent* SpawnDirection : SpawnDiractions) {
		if (SpawnDirection) {
			FRotator Rotation = SpawnDirection->GetComponentRotation();
			FVector Offset = -1000.f * Rotation.Vector();
			FVector Location = SpawnDirection->GetComponentLocation() + Offset;
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			AC_fillerWall* NewWall = GetWorld()->SpawnActor<AC_fillerWall>(FillerWallClass, Location, Rotation, SpawnParams);
			if (NewWall) {
				NewWall->SetActorScale3D(FVector(1.0f, 4.0f, 4.0f));
			}
		}
	}
}
