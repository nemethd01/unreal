// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "Tank.generated.h"



// Forward declarations
class UInputMappingContext;
class USpringArmComponent;
class UCameraComponent;


UCLASS()
class TOONTANKS_API ATank : public ABasePawn
{
	GENERATED_BODY()
	
public:
	ATank();

protected:
	virtual void BeginPlay() override;
	
	void MoveForward(float Value);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Enhanced Input")
	UInputMappingContext* InputMapping;
	
private:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USpringArmComponent* SpringArm;
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UCameraComponent* Camera;
	
};
