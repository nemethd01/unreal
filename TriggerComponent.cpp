// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerComponent.h"

UTriggerComponent::UTriggerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}

// Called when the game starts
void UTriggerComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	/*TArray<AActor*> Actors;
	GetOverlappingActors(Actors);*/

	// test
	/*if (Actors.Num() > 0)
	{
		FString ActorName = Actors[0]->GetActorNameOrLabel();
		UE_LOG(LogTemp, Display, TEXT("Overlapping Actor name: %s"), *ActorName);
	}*/

	// while loop
	/*int32 Index = 0;
	while (Index < Actors.Num())
	{
		FString ActorName = Actors[Index]->GetActorNameOrLabel();
		UE_LOG(LogTemp, Display, TEXT("Overlapping Actor name: %s"), *ActorName);
		++Index;
	}*/

	// for loop
	/*for (int32 i = 0; i < Actors.Num(); i++)
	{
		FString ActorName = Actors[i]->GetActorNameOrLabel();
		UE_LOG(LogTemp, Display, TEXT("Overlapping Actor name: %s"), *ActorName);
	}*/

	// foreach
	/*for (AActor* Actor : Actors)
	{
		if (Actor->ActorHasTag(AcceptableActorTag))
		{
			UE_LOG(LogTemp, Display, TEXT("Unlocking"));
		}
	}*/
	
	if (Mover == nullptr)
	{
		return;
	}
	
	AActor* Actor = GetAcceptableActor();
	if (Actor != nullptr)
	{
		UPrimitiveComponent* Component = Cast<UPrimitiveComponent>(Actor->GetRootComponent());
		if (Component != nullptr)
		{
			Component->SetSimulatePhysics(false);
		}
		// ez az AttachToComponent alaphelyzetbe állíja az actort, ami jó, mert bárhova lerakjuk, felénk fog nézni és egyenes lesz, viszont a méretét is alaphelyzetbe állítja ami nagyon nem jó
		// Actor->AttachToComponent(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
		Actor->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);
		Actor->SetActorRelativeRotation(FRotator::ZeroRotator);
		Mover->SetShouldMove(true);
	}
	else
	{
		Mover->SetShouldMove(false);
	}
	
}

void UTriggerComponent::SetMover(UMover* NewMover)
{
	Mover = NewMover;
}

AActor* UTriggerComponent::GetAcceptableActor() const
{
	TArray<AActor*> Actors;
	GetOverlappingActors(Actors);
	
	for (AActor* Actor : Actors)
	{
		bool HasAcceptableTag = Actor->ActorHasTag(AcceptableActorTag);
		bool IsGrabbed = Actor->ActorHasTag("Grabbed");
		if (HasAcceptableTag && !IsGrabbed)
		{
			// UE_LOG(LogTemp, Display, TEXT("Unlocking"));
			return Actor;
		}
	}

	return nullptr;
}