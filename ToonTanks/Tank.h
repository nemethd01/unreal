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

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Enhanced Input")
	UInputMappingContext* TankMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Enhanced Input")
	UInputAction* MoveAction;

	UPROPERTY(VisibleAnywhere)
	UPawnMovementComponent* MovementComponent;

	// Handle move input
	void Move(const FInputActionValue& Value);
	// Handle look input
	void Look(const FInputActionValue& Value);
	
private:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USpringArmComponent* SpringArm;
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UCameraComponent* Camera;
	
};
