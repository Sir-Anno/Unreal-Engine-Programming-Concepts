#include "InteractTest.h"


// Sets default values
AInteractTest::AInteractTest()
{
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
}

// Interact interface
void AInteractTest::Interact()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Hello, I am a cube :D"));
}


