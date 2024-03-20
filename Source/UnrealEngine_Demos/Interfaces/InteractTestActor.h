// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractInterface.h"
#include "GameFramework/Actor.h"
#include "InteractTestActor.generated.h"

UCLASS()
class UNREALENGINE_DEMOS_API AInteractTestActor : public AActor, public IInteractInterface
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> Mesh;

public:
	// Sets default values for this actor's properties
	AInteractTestActor();

protected:
	virtual void Interact() override;
};
