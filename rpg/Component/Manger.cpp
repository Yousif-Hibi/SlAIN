// Fill out your copyright notice in the Description page of Project Settings.


#include "Manger.h"

// Sets default values for this component's properties
UManger::UManger()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UManger::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UManger::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UManger::SetState(TEnumAsByte<EChartacterState> NewState)
{
	if (NewState != CurrentState) {

		stateEnd.Broadcast(CurrentState);
		CurrentState=NewState;
		stateBegin.Broadcast(CurrentState);


	}
}

EChartacterState UManger::GetCurrentState()
{
	return CurrentState;
}

void UManger::RestState()
{
	SetState(EChartacterState::Nothing);
}

bool UManger::IsCurrentStateEqualToAny(TArray<TEnumAsByte< EChartacterState>> stateArray)
{
return	stateArray.Contains(CurrentState);

}

void UManger::SetAction(TEnumAsByte<EChartacterAction> NewAction) 
{
	if (NewAction != CurrentAction) {
		CharacterActionEnd.Broadcast(CurrentAction);
		CurrentAction = NewAction;
		
		CharacterActionBegin.Broadcast(CurrentAction);
	}
}

EChartacterAction UManger::GetCurrentAction()
{
	return CurrentAction;
}

bool UManger::IsCurrentActionEqualToAny(TArray<TEnumAsByte<EChartacterAction>> ActionArray)
{
	return ActionArray.Contains(CurrentAction);
}

void UManger::OnStateBegin(EChartacterState CharacterState)
{
}

void UManger::OnStateEnd(EChartacterState CharacterState)
{
}

