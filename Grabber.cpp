// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

#define OUT // Out Paratermeter telling does nothing

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	//Checking for Physics handle component
	FindPhysicsHandle();
	SetupInputComponent();


}

//Acessing Input binding component i.e keys and mouse etc
void UGrabber::SetupInputComponent()
{
	//Acessing Input binding component i.e keys and mouse etc
	InputComponent=GetOwner()->FindComponentByClass<UInputComponent>();
	if(InputComponent)
	{
		InputComponent->BindAction("Grab",IE_Pressed,this,&UGrabber::Grab);
		InputComponent->BindAction("Grab",IE_Released,this,&UGrabber::Release);
	}

}
//Checking for physics Handle Component
void UGrabber::FindPhysicsHandle()
{
	PhysicsHandle=GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if(PhysicsHandle==nullptr)
	{
		UE_LOG(LogTemp,Error,TEXT("NO physics handle component found on %s!"),*GetOwner()->GetName());
	}
	
}

void UGrabber::Grab()
{
	
	FHitResult HitResult = GetFirstPhysicsBodyInReach();
	UPrimitiveComponent* ComponentToGrab=HitResult.GetComponent();

	//If we hit something then attach the physics handle.
	//TODO attach physics handle.
	if(HitResult.GetActor())
	{
		PhysicsHandle->GrabComponentAtLocation
		(ComponentToGrab,
		NAME_None,
		GetPlayersReach());
	}

}

void UGrabber::Release()
{
	PhysicsHandle->ReleaseComponent();

}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	//If the physic handle is attach
	if(PhysicsHandle->GrabbedComponent)
	{
		//Move the object we are holding.
		PhysicsHandle->SetTargetLocation(GetPlayersReach());

	}
}	

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	
	//Darwing a line from player showing the reach.
	DrawDebugLine(
		GetWorld(),
		GetPlayersWorldPos(),
		GetPlayersReach(),
		FColor(0,255,0),
		false,
		0.f,
		0,
		5.f);
		
	
	FHitResult Hit;
	//Ray-cast out to a certain distance(Reach)
	FCollisionQueryParams TraceParams(FName(TEXT("")),false,GetOwner());
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		GetPlayersWorldPos(),
		GetPlayersReach(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams);


	return Hit;
}

FVector UGrabber::GetPlayersWorldPos() const
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation);
	
	return PlayerViewPointLocation;

}

FVector UGrabber::GetPlayersReach() const
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);
	return PlayerViewPointLocation+PlayerViewPointRotation.Vector()*Reach;

}