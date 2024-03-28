#include "FloorButton.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TimelineComponent.h"
#include "Net/UnrealNetwork.h"
#include "UnrealEngine_Demos/Interfaces/TriggerInterface.h"

// Sets default values
AFloorButton::AFloorButton()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Enable replication
	bReplicates = true;

	// Ring mesh
	RingMesh = CreateDefaultSubobject<UStaticMeshComponent>("RingMesh");

	// Button mesh
	ButtonMesh = CreateDefaultSubobject<UStaticMeshComponent>("ButtonMesh");
	ButtonMesh->SetupAttachment(RingMesh);

	// Collision sphere
	CollisionSphere = CreateDefaultSubobject<USphereComponent>("CollisionSphere");
	CollisionSphere->SetupAttachment(ButtonMesh);

	// Collision sphere overlap
	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AFloorButton::BeginCollisionOverlap);
	CollisionSphere->OnComponentEndOverlap.AddDynamic(this, &AFloorButton::AFloorButton::EndCollisionOverlap);
}

// Called when the game starts or when spawned
void AFloorButton::BeginPlay()
{
	Super::BeginPlay();

	// Set up timeline
	if (PushCurve)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindUFunction(this, FName("TimelineUpdate"));
		PushTimeline.AddInterpFloat(PushCurve, TimelineProgress);

		FOnTimelineEvent TimelineFinished;
		TimelineFinished.BindUFunction(this, FName("TimelineFinished"));
		PushTimeline.SetTimelineFinishedFunc(TimelineFinished);

		PushTimeline.SetPropertySetObject(this);
		PushTimeline.SetDirectionPropertyName(FName("TimelineDirection"));
	}
}

// Called every frame
void AFloorButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	PushTimeline.TickTimeline(DeltaTime);
}

// Replicated properties
void AFloorButton::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFloorButton, TimelineDirection);
}

// Sphere collision begin overlap
void AFloorButton::BeginCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Check is server authoritative 
	if (!HasAuthority()) return;

	// Check is character
	if (!Cast<ACharacter>(OtherActor)) return;

	Trigger();
}

// Sphere collision end overlap
void AFloorButton::EndCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// Check is server authoritative 
	if (!HasAuthority()) return;

	// Check is character
	if (!Cast<ACharacter>(OtherActor)) return;

	Reset();
}

// Trigger
void AFloorButton::Trigger_Implementation()
{
	// Check has sound
	if (ActivationSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ActivationSound, GetActorLocation());
	}

	PushTimeline.Play();
}

// Reset
void AFloorButton::Reset_Implementation()
{
	PushTimeline.Reverse();
}

// Timeline update
void AFloorButton::TimelineUpdate(float Alpha)
{
	const FVector Start = FVector(0.0f, 0.0f, 0);
	const FVector End = FVector(0.0f, 0.0f, -5.0f);
	ButtonMesh->SetRelativeLocation(FMath::Lerp(Start, End, Alpha));
}

// Timeline finished
void AFloorButton::TimelineFinished()
{
	// Check timeline finished playing forward
	if (!TimelineDirection == ETimelineDirection::Forward) return;

	for (AActor* Element : ActorsToTrigger)
	{
		// Does actor inherit from trigger interface?
		if (ITriggerInterface* TriggerInterface = Cast<ITriggerInterface>(Element)) 
		{
			TriggerInterface->Trigger();
		}
	}
}
