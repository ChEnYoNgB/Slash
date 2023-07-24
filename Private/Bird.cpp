// Fill out your copyright notice in the Description page of Project Settings.


#include "Bird.h"
#include"Components/CapsuleComponent.h"
#include"Components/SkeletalMeshComponent.h"
#include"GameFramework/SpringArmComponent.h"
#include"Camera/CameraComponent.h"

ABird::ABird()
{

	PrimaryActorTick.bCanEverTick = true;

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->SetCapsuleHalfHeight(20.f);
	Capsule->SetCapsuleRadius(15.f);
	SetRootComponent(Capsule);

	BirdMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BirdMesh"));
	BirdMesh->SetupAttachment(Capsule);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(Capsule);
	SpringArm->TargetArmLength = 300.f;

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("SpringArmViewCamera"));
	ViewCamera->SetupAttachment(SpringArm);

	AutoPossessPlayer = EAutoReceiveInput::Player0;
}


void ABird::BeginPlay()
{
	Super::BeginPlay();

}

void ABird::MoveForward(float Value)
{
	if (Controller && Value != 0)
	{
		FVector Forward = GetActorForwardVector();
		AddMovementInput(Forward, Value);
	}
}
void ABird::Turn(float Value)
{
	AddControllerYawInput(Value);
}
void ABird::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}
void ABird::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ABird::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(FName("MoveForward"), this, &ABird::MoveForward);
	PlayerInputComponent->BindAxis(FName("Turn"), this, &ABird::Turn);
	PlayerInputComponent->BindAxis(FName("LookUp"), this, &ABird::LookUp);

}

