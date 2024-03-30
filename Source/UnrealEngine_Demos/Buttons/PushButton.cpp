#include "PushButton.h"
#include "Kismet/GameplayStatics.h"
#include "UnrealEngine_Demos/Interfaces/InteractInterface.h"


// Sets default values
APushButton::APushButton()
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
}

// Called when the game starts or when spawned
void APushButton::BeginPlay()
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

// Interact interface
void APushButton::Interact()
{
	// Check is server authoritative 
	if(!HasAuthority()) return;

	Trigger();
}

// Called every frame
void APushButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	PushTimeline.TickTimeline(DeltaTime);
}

// Replicated properties
void APushButton::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

// Trigger
void APushButton::Trigger_Implementation()
{
	if (!bCanTrigger) return;

	bCanTrigger = false;

	if (ActivationSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ActivationSound, GetActorLocation());
	}

	PushTimeline.PlayFromStart();
}

// Timeline update
void APushButton::TimelineUpdate(float Alpha)
{
	const FVector Start = FVector(0.0f, 0.0f, 0);
	const FVector End = FVector(0.0f, 0.0f, -5.0f);
	ButtonMesh->SetRelativeLocation(FMath::Lerp(Start, End, Alpha));
}

// Timeline finished
void APushButton::TimelineFinished()
{
	// Check timeline finished playing forward
	if (!TimelineDirection == ETimelineDirection::Forward) return;

	for (AActor* Element : ActorsToTrigger)
	{
		// Does actor inherit from trigger interface?
		if (IInteractInterface* ActorToTrigger = Cast<IInteractInterface>(Element)) 
		{
			ActorToTrigger->Interact();
		}
	}

	bCanTrigger = true;
}
