// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
// #include "DrawDebugHelpers.h"

ATank::ATank()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	
	MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MovementComponent"));
}

// Called every frame
void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FHitResult HitResult;

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		// Ellenőrizzük, hogy sikerült-e elérni
		if (PlayerController->GetHitResultUnderCursor(ECC_Visibility, false, HitResult))
		{
			/*DrawDebugSphere(
				GetWorld(),
				HitResult.ImpactPoint,
				25.f,
				12,
				FColor::Red,
				false,
				-1.f
			);*/

			RotateTurret(HitResult.ImpactPoint);
		}
	}
}

void ATank::BeginPlay()
{
	Super::BeginPlay();

	// Get the player controller
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		// Get the local player subsystem
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// Clear out existing mapping, and add our mapping
			Subsystem->ClearAllMappings();
			Subsystem->AddMappingContext(TankMappingContext, 0);
		}
	}
}

/*void ATank::Move(const FInputActionValue& Value)
 // Ez egy egyszerűbb és olvashatóbb megközelítés, de nem optimalizált. Egy teljesítményigényesebb játéknál, ahol többször van használva ez a megközelítés, ott lehetnek optimalizációs problémák.
 // Ezzel szemben a Matrixos megoldás optimalizáltabb, de komplexebb megközelítése van.
{
	const FVector2d MoveVector = Value.Get<FVector2d>();
	const FRotator MovementRotation(0, Controller->GetControlRotation().Yaw, 0);

	// Forward/Backward direction
	if (MoveVector.Y != 0.f)
	{
		// Get forward vector
		const FVector Direction = MovementRotation.RotateVector(FVector::ForwardVector);
 
		AddMovementInput(Direction, MoveVector.Y);
	}
 
	// Right/Left direction
	if (MoveVector.X != 0.f)
	{
		// Get right vector
		const FVector Direction = MovementRotation.RotateVector(FVector::RightVector);
 
		AddMovementInput(Direction, MoveVector.X);
	}
}*/

void ATank::Move(const FInputActionValue& Value)
{
	const FVector2d MoveVector = Value.Get<FVector2d>();
	
	if (Controller)
	{
		// FPS - Ez a megközelítés a kamerát vagy a játékos irányítását veszi alapul, így a mozgás a kamera/vezérlő irányába történik, nem pedig a tank aktuális fizikai pozíciójának forgása alapján. Ez általában jellemző egy "FPS" vagy "TPP" játékban, ahol a kamera vezérli a mozgást, és az irányítás érzése kevésbé hangsúlyos a járművek mozgásához képest.
		// const FRotator MovementRotation(0, Controller->GetControlRotation().Yaw, 0);

		// Ez a jármű szerű megközelítés - Get the tank's current rotation (yaw) to move in the forward/backward direction of the tank
		const FRotator MovementRotation = GetActorRotation(); 

		
		// Get forward and right vectors
		const FVector ForwardDirection = FRotationMatrix(MovementRotation).GetUnitAxis(EAxis::X);
		
		// const FVector RightDirection = FRotationMatrix(MovementRotation).GetUnitAxis(EAxis::Y);
		
		// Forward/Backward direction with speed
		if (MoveVector.Y != 0.f)
		{
			AddMovementInput(ForwardDirection, MoveVector.Y * Speed * GetWorld()->GetDeltaSeconds());
		}

		// Right/Left direction with speed
		/*if (MoveVector.X != 0.f)
		{
			AddMovementInput(RightDirection, MoveVector.X * Speed * GetWorld()->GetDeltaSeconds());
		}*/
	}
}

void ATank::Turn(const FInputActionValue& Value)
{
	// Tank rotation on the yaw axis (left/right)
	float TurnValue = Value.Get<float>();

	FRotator RotationDelta = FRotator(0.f, TurnValue * TurnSpeed * GetWorld()->GetDeltaSeconds(), 0.f);

	AddActorLocalRotation(RotationDelta, true);
}


void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATank::Move);
		EnhancedInputComponent->BindAction(TurnAction, ETriggerEvent::Triggered, this, &ATank::Turn);
	}
}