#include "ExplosiveBarrel.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AExplosiveBarrel::AExplosiveBarrel()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Static mesh
	BarrelMesh = CreateDefaultSubobject<UStaticMeshComponent>("BarrelMesh");
}

void AExplosiveBarrel::Interact()
{
	UGameplayStatics::SpawnEmitterAtLocation(this, ExplosionParticle, GetActorLocation(), FRotator(0.0f, 0.0f, 0.0f), FVector(3.0f, 3.0f, 3.0f));
	UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, GetActorLocation());
}


