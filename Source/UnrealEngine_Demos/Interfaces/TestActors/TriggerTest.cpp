#include "TriggerTest.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ATriggerTest::ATriggerTest()
{
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
}

// Trigger
void ATriggerTest::Trigger()
{
	UGameplayStatics::SpawnEmitterAtLocation(this, ParticleSystem, GetActorLocation(), FRotator::ZeroRotator, FVector(3));
	UGameplayStatics::PlaySoundAtLocation(this, TriggerSound, GetActorLocation());
}



