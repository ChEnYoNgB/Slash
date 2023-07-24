// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Bird.generated.h"
class UCapsuleComponent;
class USkeletalMeshComponent;
class USpringArmComponent;
class UCameraComponent;
UCLASS()
class SLASH_API ABird : public APawn
{
	GENERATED_BODY()

public:
	ABird();

protected:

	virtual void BeginPlay() override;
	void MoveForward(float Value);
	void Turn(float Value);
	void LookUp(float Value);

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
private:
	UPROPERTY(VisibleAnywhere)
		UCapsuleComponent* Capsule;
	UPROPERTY(VisibleAnywhere)
		USkeletalMeshComponent* BirdMesh;
	UPROPERTY(VisibleAnywhere)
		USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere)
		UCameraComponent* ViewCamera;
};
