// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractTestActor.h"


// Sets default values
AInteractTestActor::AInteractTestActor()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(GetRootComponent());
}

void AInteractTestActor::Interact()
{
	// Check is server authoritative
	if (!HasAuthority()) return
	
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Hello I am a cube :D"));
}

