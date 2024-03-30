#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "SimpleDoor.generated.h"

class UBoxComponent;
class UCurveFloat;

UCLASS()
class UNREALENGINE_DEMOS_API ASimpleDoor : public AActor
{
	GENERATED_BODY()

	// Default scene root
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> SceneRoot;
	
	// Pivot
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> Pivot;

	// Door frame mesh
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> DoorFrameMesh;

	// Door mesh
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> DoorMesh;

	// Collision box
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> CollisionBox;

	// Is open
	UPROPERTY(ReplicatedUsing = OnRep_IsOpen)
	bool bIsOpen;

	// Rotation amount
	UPROPERTY(Replicated)
	FRotator FCloseAngle;
	UPROPERTY(Replicated)
	FRotator FOpenAngle;

	// Curve float
	UPROPERTY(EditAnywhere)
	TObjectPtr<UCurveFloat> OpenCurve;

	// Timeline
	FTimeline OpenTimeline;

public:
	// Sets default values for this actor's properties
	ASimpleDoor();
	
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

protected:
	// Box collision begin overlap
	UFUNCTION()
	void BeginCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
	// Box collision end overlap
	UFUNCTION()
	void EndCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// bIsOpen RepNotify
	UFUNCTION()
	void OnRep_IsOpen();

	// Timeline update
	UFUNCTION()
	void TimelineUpdate(float Alpha);
};
