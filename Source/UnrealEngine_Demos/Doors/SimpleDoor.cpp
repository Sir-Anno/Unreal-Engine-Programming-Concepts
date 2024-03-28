#include "SimpleDoor.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "Components/TimelineComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ASimpleDoor::ASimpleDoor()
{
	// Set this actor to call Tick() every frame
	PrimaryActorTick.bCanEverTick = true;

	// Enable replication
	bReplicates = true;

	// Default scene root
	SceneRoot = CreateDefaultSubobject<USceneComponent>("DefaultSceneRoot");
	SetRootComponent(SceneRoot);

	// Pivot scene component
	Pivot = CreateDefaultSubobject<USceneComponent>("Pivot");
	Pivot->SetupAttachment(RootComponent);
	Pivot->bVisualizeComponent = true;

	// Door frame mesh
	DoorFrameMesh = CreateDefaultSubobject<UStaticMeshComponent>("DoorFrameMesh");
	DoorFrameMesh->SetupAttachment(RootComponent);

	// Door mesh
	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>("DoorMesh");
	DoorMesh->SetupAttachment(Pivot);

	// Collision box
	CollisionBox = CreateDefaultSubobject<UBoxComponent>("BoxCollision");
	CollisionBox->SetupAttachment(RootComponent);

	// Collision box overlaps
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ASimpleDoor::BeginCollisionOverlap);
	CollisionBox->OnComponentEndOverlap.AddDynamic(this, &ASimpleDoor::EndCollisionOverlap);
}

// Begin play
void ASimpleDoor::BeginPlay()
{
	Super::BeginPlay();

	// Set up timeline
	if (OpenCurve)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindUFunction(this, FName("TimelineUpdate"));

		OpenTimeline.AddInterpFloat(OpenCurve, TimelineProgress);
	}
}

// Tick
void ASimpleDoor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	OpenTimeline.TickTimeline(DeltaSeconds);
}

// Replicated properties
void ASimpleDoor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASimpleDoor, FOpenAngle)
	DOREPLIFETIME(ASimpleDoor, bIsOpen)
}

// Box collision begin overlap
void ASimpleDoor::BeginCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	// Check is server authoritative 
	if (!HasAuthority()) return;

	// Check is character
	if (!Cast<ACharacter>(OtherActor)) return;

	// Compare doors direction to players direction 
	if (GetActorForwardVector().Equals(OtherActor->GetActorForwardVector(), 0.5f))
	{
		FOpenAngle.Yaw = 90.0f;
	}
	else
	{
		FOpenAngle.Yaw = -90.0f;
	}

	bIsOpen = true;
	OnRep_IsOpen(); // Manually call RepNotify as "ReplicatedUsing" function is not automatically called for server
}

// Box collision end overlap
void ASimpleDoor::EndCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// Check is server authoritative 
	if (!HasAuthority()) return;

	// Check is character
	if (!Cast<ACharacter>(OtherActor)) return;

	bIsOpen = false;
	OnRep_IsOpen(); // Manually call RepNotify as "ReplicatedUsing" function is not automatically called for server
}

// bIsOpen RepNotify
void ASimpleDoor::OnRep_IsOpen()
{
	if (bIsOpen)
	{
		OpenTimeline.Play();
	}
	else
	{
		OpenTimeline.Reverse();
	}
}

// Update door rotation
void ASimpleDoor::TimelineUpdate(float Alpha)
{
	FQuat AQuat(FCloseAngle);
	FQuat BQuat(FOpenAngle);
	
	Pivot->SetRelativeRotation(FQuat::Slerp(AQuat, BQuat, Alpha));
}
