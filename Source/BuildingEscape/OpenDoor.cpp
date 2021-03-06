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
	OnTrigger = false;
	if (!PressurePlate) {
		UE_LOG(LogTemp, Error, TEXT("The object %s has no PressurePlate component!"), *GetOwner()->GetName())
	}
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if ((OverLappingActorsMass() >= TriggerMass) && !OnTrigger) {
		OpenDoor();
		OnTrigger = true;
	}
	else if ((OverLappingActorsMass() < TriggerMass) && OnTrigger) {
		CloseDoor();
		OnTrigger = false;
	}
}

void UOpenDoor::OpenDoor()
{
	if (Owner) {
		OnOpenRequest.Broadcast();
		// Owner->SetActorRotation(FRotator(0.0f, OpenAngle, 0.0f));
	}
}

void UOpenDoor::CloseDoor()
{
	if (Owner) {
		OnCloseRequest.Broadcast();
		// Owner->SetActorRotation(FRotator(0.0f, 0.0f, 0.0f));
	}
}

float UOpenDoor::OverLappingActorsMass()
{
	float Mass = 0.0f;
	TArray<AActor*> OverLappingActors;
	if (PressurePlate) {
		PressurePlate->GetOverlappingActors(OverLappingActors);
		for (auto& Actor : OverLappingActors) {
			Mass += Actor->GetRootPrimitiveComponent()->GetMass();
		}
	}
	return Mass;
}

