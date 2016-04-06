// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;


private:
	float reach = 100.f;
	FVector PlayerLocation;
	FRotator PlayerRotation;
	void GetPlayerViewpoint();
	FVector GetPlayerReachPoint();
	FHitResult MovableHit;
	FString ActorHit;
	UPhysicsHandleComponent *PhysicsHandle = nullptr;
	UInputComponent *Input = nullptr;
	void Grab();
	void Release();
	void SetupPhysicsHandle();
	void BindInput();
	FHitResult GetFirstPhysicsBody();
	void PlaceGrabbedComponent();
};
