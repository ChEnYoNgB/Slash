// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PickupItemInterface.generated.h"


UINTERFACE(MinimalAPI)
class UPickupItemInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class SLASH_API IPickupItemInterface
{
	GENERATED_BODY()


public:
	virtual void PickUp();
};
