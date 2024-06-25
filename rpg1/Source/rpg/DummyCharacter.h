// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Interfaces/Targeting_CI.h"
#include "CoreMinimal.h"
#include "SoulsLikeCharacter.h"
#include "DummyCharacter.generated.h"

/**
 * 
 */

class AC_BaseEquippable;

UCLASS()
class RPG_API ADummyCharacter : public ASoulsLikeCharacter, public ITargeting_CI
{
	GENERATED_BODY()
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public :
	ADummyCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual bool CanBeTarget();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf <AC_BaseEquippable> Equip;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UUserWidget * LockOnWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf< UUserWidget> LockOnWidgetClass;
	

	 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<  AC_BaseEquippable>> StartingEquipmeent;





	void PerformDeath();
	void PerformDeathAfterDelay() ;

	void OnStateBegin(EChartacterState CharacterState);
	
	void OnStateEnd(EChartacterState CharacterState);
	virtual bool CanRecieveDamage()override;

	void OnActionBegin(EChartacterAction CharacterAction);

	void OnActionEnd(EChartacterAction CharacterAction);
	
	UFUNCTION(BlueprintImplementableEvent)
	void target();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool OnTargete();
	
};
