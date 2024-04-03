#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UnrealEngine_Demos/Interfaces/InteractInterface.h"
#include "ColorCube.generated.h"

UCLASS()
class UNREALENGINE_DEMOS_API AColorCube : public AActor, public IInteractInterface
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> StaticMesh;

	UPROPERTY()
	TObjectPtr<UMaterialInterface> Material;

	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> MaterialInstance;

	UPROPERTY(ReplicatedUsing = OnRep_Color)
	FLinearColor Color;

public:
	// Sets default values for this actor's properties
	AColorCube();

	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual void Interact() override;

protected:
	void SetColor();

	UFUNCTION()
	void OnRep_Color();
};
