#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UnrealEngine_Demos/Interfaces/InteractInterface.h"
#include "ExplosiveBarrel.generated.h"

UCLASS()
class UNREALENGINE_DEMOS_API AExplosiveBarrel : public AActor, public IInteractInterface
{
	GENERATED_BODY()

	// Static mesh
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> BarrelMesh;

	// Particle emitter
	UPROPERTY(EditAnywhere)
	TObjectPtr<UParticleSystem> ExplosionParticle;

	// Sound cue
	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> ExplosionSound;

public:
	// Sets default values for this actor's properties
	AExplosiveBarrel();

	virtual void Interact() override;
};
