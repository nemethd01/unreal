// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "InputActionValue.h"
#include "Tank.generated.h" // ennek kell legalul lennie mindig



// Forward declarations
class UInputMappingContext;
class USpringArmComponent;
class UCameraComponent;
class UInputAction;
class UPawnMovementComponent;


UCLASS()
class TOONTANKS_API ATank : public ABasePawn
{
	GENERATED_BODY()
	
public:
	ATank();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Enhanced Input")
	UInputMappingContext* TankMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Enhanced Input")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Enhanced Input")
	UInputAction* TurnAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Enhanced Input")
	UInputAction* FireAction;

	UPROPERTY(VisibleAnywhere)
	UPawnMovementComponent* MovementComponent;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float Speed = 200.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float TurnSpeed = 100.f;

	// Handle move input
	void Move(const FInputActionValue& Value);
	
	// Handle look input
	// void Look(const FInputActionValue& Value);
	
	// Handle Turn input for Tank
	void Turn(const FInputActionValue& Value);

private:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USpringArmComponent* SpringArm;
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UCameraComponent* Camera;
	
};
