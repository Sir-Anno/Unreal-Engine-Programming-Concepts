#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UnrealEngine_Demos/Interfaces/InteractInterface.h"
#include "InteractTest.generated.h"

UCLASS()
class UNREALENGINE_DEMOS_API AInteractTest : public AActor, public IInteractInterface
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> StaticMesh;

public:
	// Sets default values for this actor's properties
	AInteractTest();

	virtual void Interact() override;
};
