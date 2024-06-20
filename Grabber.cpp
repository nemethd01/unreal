// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	/*UPhysicsHandleComponent* PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	
	if (PhysicsHandle != nullptr)
	{
		UE_LOG(LogTemp, Display, TEXT("Got Physics Handle: %s"), *PhysicsHandle->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No Physics Handle found"));
	}*/
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();

	if (PhysicsHandle && PhysicsHandle->GetGrabbedComponent())
	{
		FVector TargetLocation = GetComponentLocation() + GetForwardVector() * HoldDistance;
		PhysicsHandle->SetTargetLocationAndRotation(TargetLocation, GetComponentRotation());
	}
	
}

/*void UGrabber::PrintDamage(const float& Damage)
{
	UE_LOG(LogTemp, Warning, TEXT("Damage: %f"), Damage);
};

bool UGrabber::HasDamage(float& OutDamage)
{
	OutDamage = 5;
	return true;
};*/


void UGrabber::Grab()
{
	/*
	// Grabber rotation vectors
FRotator MyRotation = GetComponentRotation();
FString RotationString = MyRotation.ToCompactString();

UE_LOG(LogTemp, Warning, TEXT("Grabber rotation: %s"), *RotationString);


	// World timesec
float TimeSec = GetWorld()->TimeSeconds;
UE_LOG(LogTemp, Display, TEXT("Time: %f"), TimeSec);*/

	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();
	if (PhysicsHandle == nullptr)
	{
		return;
	}

	FHitResult HitResult;
	bool HasHit = GetGrabbableInReach(HitResult);

	if (HasHit)
	{
		UPrimitiveComponent* HitComponent = HitResult.GetComponent();
		HitComponent->SetSimulatePhysics(true);
		HitComponent->WakeAllRigidBodies();
		AActor* HitActor = HitResult.GetActor();
		HitActor->Tags.Add("Grabbed");
		HitActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		PhysicsHandle->GrabComponentAtLocationWithRotation(
			HitComponent,
			NAME_None,
			HitResult.ImpactPoint,
			GetComponentRotation()
		);
	}
	
	/* // ez itt a nullptr return vizsgálata előtti rész volt + DrawDebugSphere
	 if (HasHit)
	{
		DrawDebugSphere(GetWorld(), HitResult.Location, 10, 10, FColor::Green, false, 2);
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10, 10, FColor::Red, false, 2);
		// itt pointert kell használni több okból is: Referencia értéknél nem lehet az érték null, itt viszont van hogy nincs hit. A másik, pedig hogy az AActorban többféle típusú objektum is lehet
		AActor* HitActor = HitResult.GetActor();
		UE_LOG(LogTemp, Display, TEXT("Hitted actor name: %s"), *HitActor->GetActorNameOrLabel());
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("No actor hit"))
	}*/

	
	// reference vs pointer
	/*float Damage = 0;
	float& DamageRef = Damage;
	DamageRef = 5;

	UE_LOG(LogTemp, Display, TEXT("DamageRef: %f, Damage: %f"), DamageRef, Damage);*/

	
	// PrintDamage(Damage);
	/*if (HasDamage(Damage))
	{
		PrintDamage(Damage);
	}*/
}

void UGrabber::Release()
{
	// UE_LOG(LogTemp, Display, TEXT("Released grabber"))
	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();

	// Az ez alatt lévő (nem kikommentezett) kód optimalizáltabban vizsgálja, hogy nullptr bármelyik változó
	/*if (PhysicsHandle == nullptr)
	{
		return;
	}*/
	// először mindig az alap pointert kell vizsgálni, különben ki crashel az engine
	if (PhysicsHandle && PhysicsHandle->GetGrabbedComponent())
	{
		PhysicsHandle->GetGrabbedComponent()->WakeAllRigidBodies();
		AActor* GrabbedActor = PhysicsHandle->GetGrabbedComponent()->GetOwner();
		GrabbedActor->Tags.Remove("Grabbed");
		PhysicsHandle->ReleaseComponent();
	}
	
	
}

UPhysicsHandleComponent* UGrabber::GetPhysicsHandle() const
{
	UPhysicsHandleComponent* Result = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (Result == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Grabber requires a UphysicsHandleComponent!"));
	}
	
	return Result;
}

bool UGrabber::GetGrabbableInReach(FHitResult& OutHitResult) const
{
	FVector Start = GetComponentLocation();
	FVector End = Start + GetForwardVector() * MaxGrabDistance;
	DrawDebugLine(GetWorld(), Start, End, FColor::Red);
	DrawDebugSphere(GetWorld(), End, 10, 10, FColor::Blue, false, 5); // radius: hány cm

	FCollisionShape Sphere = FCollisionShape::MakeSphere(GrabRadius);
	return GetWorld()->SweepSingleByChannel(
		OutHitResult,
		Start, End,
		FQuat::Identity,
		ECC_GameTraceChannel2,
		Sphere
	);
	
}

