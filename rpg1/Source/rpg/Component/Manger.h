// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "rpg/Enums/E_ChartacterState.h"

#include "rpg/Enums/E_CharacterAction.h"

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Manger.generated.h"



DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStateBegin, EChartacterState, CharacterState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStateEnd, EChartacterState, CharacterState);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterActionBegin, EChartacterAction, CharacterState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterActionEnd, EChartacterAction, CharacterState);



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPG_API UManger : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UManger();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	UFUNCTION()
	void SetState(TEnumAsByte< EChartacterState> State);

	UFUNCTION()
	EChartacterState GetCurrentState();

	UFUNCTION()
	void RestState();

	UFUNCTION()
	bool IsCurrentStateEqualToAny(TArray<TEnumAsByte< EChartacterState>> stateArray);


	UFUNCTION()
	void SetAction(TEnumAsByte< EChartacterAction> NewAction);

	UFUNCTION()
	EChartacterAction GetCurrentAction();

	UFUNCTION()
	bool IsCurrentActionEqualToAny(TArray<TEnumAsByte< EChartacterAction>> ActionArray);

	UPROPERTY(BlueprintAssignable)
	FOnStateBegin stateBegin;

	UPROPERTY(BlueprintAssignable)
	FOnStateEnd stateEnd;

	UPROPERTY(BlueprintAssignable)
	FOnCharacterActionBegin CharacterActionBegin;
	UPROPERTY(BlueprintAssignable)
	FOnCharacterActionEnd CharacterActionEnd;

	UFUNCTION()
	void OnStateBegin(EChartacterState CharacterState);
	UFUNCTION()
	void OnStateEnd(EChartacterState CharacterState);

private:
	UPROPERTY(EditAnywhere)
	TEnumAsByte< EChartacterState> CurrentState;

	UPROPERTY(EditAnywhere)
	TEnumAsByte< EChartacterAction> CurrentAction;

};
