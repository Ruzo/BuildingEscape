// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingEscape.h"
#include "OpenDoor.h"


// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	Owner = GetOwner();
	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
	OnTrigger = false;
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if ((OverLappingActorsMass() >= 70.f) && !OnTrigger) {
		OpenDoor();
		OnTrigger = true;
	}
	else if ((OverLappingActorsMass() < 70.f) && OnTrigger) {
		CloseDoor();
		OnTrigger = false;
	}
}

void UOpenDoor::OpenDoor()
{
	// FRotator Rotator(0.0f, OpenAngle, 0.0f);
	Owner->SetActorRotation(FRotator(0.0f, OpenAngle, 0.0f));
}

void UOpenDoor::CloseDoor()
{
	Owner->SetActorRotation(FRotator(0.0f, 0.0f, 0.0f));
}

float UOpenDoor::OverLappingActorsMass()
{
	float Mass = 0.0f;
	TArray<AActor*> OverLappingActors;

	PressurePlate->GetOverlappingActors(OverLappingActors);
	for (AActor *Actor : OverLappingActors) {
		Mass += Actor->GetRootPrimitiveComponent()->GetMass();
	}
	if(Mass > 0.f) {
	UE_LOG(LogTemp, Warning, TEXT("Mass: %f"), Mass)
		}
	return Mass;
}

