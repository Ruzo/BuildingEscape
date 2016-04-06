// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingEscape.h"
#include "Grabber.h"


// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	SetupPhysicsHandle();
	BindInput();
}


// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
	PlaceGrabbedComponent();
}

void UGrabber::GetPlayerViewpoint()
{
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(PlayerLocation, PlayerRotation);
}

// Get player viewpoint
FVector UGrabber::GetPlayerReachPoint()
{
	GetPlayerViewpoint();
	return PlayerLocation + PlayerRotation.Vector() * reach;
}

void UGrabber::Grab()
{
	auto HitResult = GetFirstPhysicsBody();
	auto HitActor = HitResult.GetActor();
	if (HitActor) {
		UE_LOG(LogTemp, Warning, TEXT("%s WAS GRABBED!"), *HitActor->GetName())
		// Attach PhysicsHandle
		auto ComponentToGrab = HitResult.GetComponent();
		PhysicsHandle->GrabComponent(ComponentToGrab, NAME_None, ComponentToGrab->GetOwner()->GetActorLocation(), true);
	}
}

void UGrabber::Release()
{
	if (PhysicsHandle->GrabbedComponent) {
		PhysicsHandle->ReleaseComponent();
	}
}

void UGrabber::SetupPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle) {
		UE_LOG(LogTemp, Error, TEXT("The object %s has no Physics Handle Component!"), *GetOwner()->GetName())
	}
}

void UGrabber::BindInput()
{
	Input = GetOwner()->FindComponentByClass<UInputComponent>();
	if (Input) {
		Input->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		Input->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("The object %s has no Pawn Input Component!"), *GetOwner()->GetName())
	}

}

FHitResult UGrabber::GetFirstPhysicsBody()
{
	FCollisionQueryParams CollisionParams(FName(TEXT("")), false, GetOwner());
	if (GetWorld()->LineTraceSingleByObjectType(
		MovableHit,
		PlayerLocation,
		GetPlayerReachPoint(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		CollisionParams
		))
	{
		return MovableHit;
	}
	return FHitResult();
}

void UGrabber::PlaceGrabbedComponent()
{
	if (PhysicsHandle->GrabbedComponent) {
		GetPlayerReachPoint();
		PhysicsHandle->SetTargetLocationAndRotation(GetPlayerReachPoint(), PlayerRotation);
	}
}

