// Fill out your copyright notice in the Description page of Project Settings.


#include "PatrolPath.h"
// Sets default values
APatrolPath::APatrolPath()
{
 
	
PrimaryActorTick.bCanEverTick = false;
	

}

FVector APatrolPath::GetPatrolPoint(int const index) const
{
	return PatrolPath[index]; 
}

int APatrolPath::num() const
{
	return PatrolPath.Num();
}


