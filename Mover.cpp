// Fill out your copyright notice in the Description page of Project Settings.


#include "Mover.h"

#include "Math/UnrealMathUtility.h"

// Sets default values for this component's properties
UMover::UMover()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMover::BeginPlay()
{
	Super::BeginPlay();

	OriginalLocation = GetOwner()->GetActorLocation();
	
}


// Called every frame
void UMover::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AActor* Owner = GetOwner(); // Get an actor pointer that points at the owner

	// pointer alapok
	// optiomalizálatlan megoldás kommentekkel:
	/*float MyFloat = 5;
	float* YourFloat = &MyFloat;
	float FloatValue = *YourFloat; // itt a * referenciaként van, nem a pointert jelzi - ez azt csinálja, hogy oda adja az értéket ami azon a lokáción van

	// UE_LOG(LogTemp, Warning, TEXT("YourFloat value: %f"), YourFloat); // Ez nem lesz jó, mert a pointer nem float ezért nem fogja kiírni
	UE_LOG(LogTemp, Warning, TEXT("YourFloat value: %f"), FloatValue); // Ez nem lesz jó, mert a pointer nem float ezért nem fogja kiírni*/

	// helyes megoldás
	/*float MyFloat = 5;
	float* YourFloat = &MyFloat;

	UE_LOG(LogTemp, Warning, TEXT("YourFloat value: %f"), *YourFloat);*/

	
	// actor owner pointer value

	// FString Name = Owner->GetActorNameOrLabel();
	//
	// FVector OwnerLocation = Owner->GetActorLocation();
	// FString OwnerLocationString = OwnerLocation.ToCompactString();
	//
	// UE_LOG(LogTemp, Warning, TEXT("Mover Owner: %s with location: %s"), *Name, *OwnerLocationString);
	//
	// UE_LOG(LogTemp, Warning, TEXT("Owner actor pointer address: %u"), Owner);

	FVector TargetLocation = OriginalLocation;
	if (ShouldMove)
	{
		TargetLocation = OriginalLocation + MoveOffset;
	}
	FVector CurrentLocation = GetOwner()->GetActorLocation();
	float Speed =MoveOffset.Length() / MoveTime;
	
	FVector NewLocation = FMath::VInterpConstantTo(CurrentLocation, TargetLocation, DeltaTime, Speed);

	GetOwner()->SetActorLocation(NewLocation);
}

void UMover::SetShouldMove(bool NewSetShouldMove)
{
	ShouldMove = NewSetShouldMove;
}
