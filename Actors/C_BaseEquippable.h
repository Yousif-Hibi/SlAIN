// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "C_BaseEquippable.generated.h"

UCLASS(Blueprintable)
class  RPG_API AC_BaseEquippable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AC_BaseEquippable();
private:
	UPROPERTY(EditAnywhere)
	bool bIsEquipped = false;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UPROPERTY()
	USceneComponent* Root;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* ItemStaticMesh;

	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* ItemSkeletalMesh;

	UFUNCTION(BlueprintCallable , BlueprintPure, Category = "ItemMesh")
	UPrimitiveComponent* GetItemMesh();












	

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Initialization")
	FName AttachSocketName;

	UFUNCTION(BlueprintCallable)
	bool IsEquipped();

	UFUNCTION(BlueprintCallable)
	void SetIsEquipped(bool IsEquipped);

	UFUNCTION(BlueprintCallable)
	void OnUnequipped();

	UFUNCTION(BlueprintCallable)
	virtual void OnEquipped();
	virtual void OnAIEquipped();
	

	UFUNCTION(BlueprintCallable , Category = "ItemMesh")
	void AttachActor(FName SocketName);

};
