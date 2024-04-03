#include "ColorCube.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AColorCube::AColorCube()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Enable replication
	bReplicates = true;

	// Static mesh
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	StaticMesh->ComponentTags.Add("PlayerInteractable");
	RootComponent = StaticMesh;
}

// Begin play
void AColorCube::BeginPlay()
{
	Super::BeginPlay();

	MaterialInstance = StaticMesh->CreateDynamicMaterialInstance(0, StaticMesh->GetMaterial(0));
}

// Replicated properties
void AColorCube::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AColorCube, Color)
}

// Interact interface
void AColorCube::Interact()
{
	// Check is server authoritative
	if(!HasAuthority()) return;
	
	SetColor();
	OnRep_Color(); // Manually call RepNotify as "ReplicatedUsing" function is not automatically called for server
}

void AColorCube::SetColor()
{
	Color =  FLinearColor(UKismetMathLibrary::HSVToRGB(UKismetMathLibrary::RandomFloatInRange(0, 359), 1, 1, 1));
}

// Color RepNotify
void AColorCube::OnRep_Color()
{
	MaterialInstance->SetVectorParameterValue("Color", Color);
}
