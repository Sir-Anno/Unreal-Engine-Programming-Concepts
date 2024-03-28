#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "UnrealEngine_Demos/Interfaces/TriggerInterface.h"
#include "FloorButton.generated.h"

class USphereComponent;
class UCurveFloat;

UCLASS()
class UNREALENGINE_DEMOS_API AFloorButton : public AActor, public ITriggerInterface
{
	GENERATED_BODY()
	
	// Ring mesh
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> RingMesh;

	// Button mesh
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> ButtonMesh;

	// Collision sphere
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<USphereComponent> CollisionSphere;

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

public:
	// Sets default values for this actor's properties
	AFloorButton();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Sphere collision begin overlap
	UFUNCTION()
	void BeginCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
	// Sphere collision end overlap
	UFUNCTION()
	void EndCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// Trigger
	UFUNCTION(NetMulticast, Reliable)
	void Trigger();

	// Reset
	UFUNCTION(NetMulticast, Reliable)
	void Reset();

	// Timeline update
	UFUNCTION()
	void TimelineUpdate(float Alpha);

	// Timeline finished
	UFUNCTION()
	void TimelineFinished();
};
