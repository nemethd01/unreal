// Fill out your copyright notice in the Description page of Project Settings.


#include "Tower.h"
#include "Tank.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

void ATower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (InFireRange())
	{
		// If in range, rotate turret toward tank
		RotateTurret(Tank->GetActorLocation());
	}
}

void ATower::HandleDestruction()
{
	Super::HandleDestruction();
	Destroy();
}


void ATower::BeginPlay()
{
	Super::BeginPlay();
	Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));

	GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &ATower::CheckFireCondition, FireRate, true);
}

void ATower::CheckFireCondition()
{
	if (InFireRange())
	{
		Fire();
	}
}

bool ATower::InFireRange()
{
	if (Tank)
	{
		// Find the distance to the tank
		float Distance = FVector::Dist(GetActorLocation(), Tank->GetActorLocation());

		// Check to see if the tank is in range
		if (Distance <= FireRange)
		{
			// If in range, return true
			return true;
		}
	}

	return false;
}
