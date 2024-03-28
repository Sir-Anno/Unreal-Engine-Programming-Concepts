#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "UnrealEngine_Demos/Interfaces/InteractInterface.h"
#include "PushButton.generated.h"

UCLASS()
class UNREALENGINE_DEMOS_API APushButton : public AActor, public IInteractInterface
{
	GENERATED_BODY()

	// Ring mesh
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> RingMesh;

	// Button mesh
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> ButtonMesh;

	// Activation sound
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true));
	TObjectPtr<USoundBase> ActivationSound;

	// Curve float
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCurveFloat> PushCurve;

	// Timeline
	FTimeline PushTimeline;
	UPROPERTY(Replicated)
	TEnumAsByte<ETimelineDirection::Type> TimelineDirection;

	// Actors to trigger
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TArray<AActor*> ActorsToTrigger;

	bool bCanTrigger = true;

public:
	// Sets default values for this actor's properties
	APushButton();

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Interact interface
	virtual void Interact() override;

	// Trigger
	UFUNCTION(NetMulticast, Reliable)
	void Trigger();

	// Timeline update
	UFUNCTION()
	void TimelineUpdate(float Alpha);

	// Timeline finished
	UFUNCTION()
	void TimelineFinished();
};
