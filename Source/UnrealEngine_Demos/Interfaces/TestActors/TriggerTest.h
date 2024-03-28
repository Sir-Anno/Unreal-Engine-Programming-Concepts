#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UnrealEngine_Demos/Interfaces/TriggerInterface.h"
#include "TriggerTest.generated.h"

UCLASS()
class UNREALENGINE_DEMOS_API ATriggerTest : public AActor, public ITriggerInterface
{
	GENERATED_BODY()

	// Static mesh
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> StaticMesh;

	// Particle system
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess))
	TObjectPtr<UParticleSystem> ParticleSystem;

	// Trigger sound
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess))
	TObjectPtr<USoundBase> TriggerSound;

public:
	// Sets default values for this actor's properties
	ATriggerTest();

	virtual void Trigger() override;
};


