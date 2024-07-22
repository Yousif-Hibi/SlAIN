// Fill out your copyright notice in the Description page of Project Settings.


#include "C_HumanoidEnemy.h"
#include "C_AIController.h"

void AC_HumanoidEnemy::Tick(float DeltaTime)
{
	ChangeHealth();
	Track();
}
void AC_HumanoidEnemy::SetPatrolPath(APatrolPath* NewPatrolPath)
{
	PatrolPath = NewPatrolPath;
}

