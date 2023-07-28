// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/PickupItemInterface.h"
#include "Item.generated.h"

class USphereComponent;
class UNiagaraComponent;
class UNiagaraSystem;
class USoundBase;
class UTexture2D;

enum class EItemState :uint8
{
	EIS_Hovering,
	EIS_Equipped,
	EIS_Inpackage
};

UCLASS()
class SLASH_API AItem : public AActor, public IPickupItemInterface
{
	GENERATED_BODY()

public:
	AItem();

protected:
	virtual void BeginPlay() override;
	UFUNCTION()
		virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	virtual void SpawnPickUpEffect();
	virtual void SpawnPickUpSound();

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* ItemMesh;
	UPROPERTY(VisibleAnywhere)
		USphereComponent* Sphere;
	UPROPERTY(EditAnywhere, Category = Effect)
		UNiagaraComponent* ItemEffect;
	UPROPERTY(EditAnywhere, Category = Effect)
		UNiagaraSystem* PickUpEffect;
	UPROPERTY(EditAnywhere, Category = Sound)
		USoundBase* PickUpSound;

	EItemState ItemState = EItemState::EIS_Hovering;
public:
	virtual void Tick(float DeltaTime) override;
	FORCEINLINE bool CanThrow()const { return ItemState != EItemState::EIS_Equipped; }
	FORCEINLINE void SetItemEquip() { ItemState = EItemState::EIS_Equipped; }
	FORCEINLINE void SetItemInpackage() { ItemState = EItemState::EIS_Inpackage; }
	FORCEINLINE void SetItemHovering() { ItemState = EItemState::EIS_Hovering; }
	FORCEINLINE UStaticMeshComponent* GetStaticMesh()const { return ItemMesh; }
	FORCEINLINE void SetStaticMesh(UStaticMeshComponent* NewMesh) { ItemMesh = NewMesh; }
	FORCEINLINE int32 GetItemNumber()const { return ItemNumber; }
	FORCEINLINE void SetItemNumber(const int32 Number) { ItemNumber = Number; }
	FORCEINLINE void SetItemName(const FName Name) { ItemName = Name; }
	FORCEINLINE UTexture2D* GetItemTexture()const { return ItemTexture; }
	FORCEINLINE void SetItemTexture(UTexture2D* Tex) { if (Tex)ItemTexture = Tex; }
	FORCEINLINE bool GetbIsStack()const { return bIsStack; }
	FORCEINLINE FName GetItemName()const { return ItemName; }

private:
	float RunningTime;
	UPROPERTY(EditAnywhere, Category = Attribute)
		FName ItemName;
	UPROPERTY(EditAnywhere, Category = Attribute)
		float Amplitudef;
	UPROPERTY(EditAnywhere, Category = Attribute)
		float TimeConstant;
	UPROPERTY(EditAnywhere, Category = Attribute)
		bool bIsStack = true;
	UPROPERTY(EditAnywhere, Category = Attribute)
		int32 ItemNumber = 0;
	UPROPERTY(EditAnywhere, Category = Attribute)
		UTexture2D* ItemTexture = nullptr;
};
