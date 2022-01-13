// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenDoor2.h"
#include "Components/AudioComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"

#define OUT
// Sets default values for this component's properties
UOpenDoor2::UOpenDoor2()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor2::BeginPlay()
{
	Super::BeginPlay();

	// ...
	InitialYaw=GetOwner()->GetActorRotation().Yaw;
	CurrentYaw=InitialYaw;
	OpenAngle+=InitialYaw;	//OpenAngle=InitialYaw+OpenAngle;
	
	FindPressurePlate1();
	FindAudioComponent1();
}
void UOpenDoor2::FindAudioComponent1()
{
	AudioComponent1=GetOwner()->FindComponentByClass<UAudioComponent>();
	if(!AudioComponent1)
	{
		UE_LOG(LogTemp,Error,TEXT("%s missing audio component!"),*GetOwner()->GetName());

	}
}
void UOpenDoor2::FindPressurePlate1()
{
	if(!PressurePlate1)
	{
		UE_LOG(LogTemp,Error,TEXT("%s has the open door component on it, but no pressur plate set!"),*GetOwner()->GetName());

	}
}
// Called every frame
void UOpenDoor2::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	if(TotalMassOfActors()>MassToOpenDoors)
	{
		OpenDoor(DeltaTime);
		DoorLastOpened=GetWorld()->GetTimeSeconds();

	}
	else
	{
		if(GetWorld()->GetTimeSeconds()-DoorLastOpened > DoorCloseDelay)
		{
			DoorClose(DeltaTime);
		}
	}
}
void UOpenDoor2::OpenDoor(float DeltaTime)
{
	CurrentYaw=FMath::Lerp(CurrentYaw,OpenAngle,DeltaTime * DoorOpenSpeed);//Delta time used for universal frame support
	FRotator DoorRotation=GetOwner()->GetActorRotation();
	DoorRotation.Yaw=CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotation);
	
	CloseDoorSound=false;
	if(!AudioComponent1)
	{
		return;
	}
	if(!OpenDoorSound)
	{
		AudioComponent1->Play();
		OpenDoorSound=true;

	}

}
void UOpenDoor2::DoorClose(float DeltaTime)
{
	CurrentYaw=FMath::Lerp(CurrentYaw,InitialYaw,DeltaTime * DoorCloseSpeed);
	FRotator DoorRotation=GetOwner()->GetActorRotation();
	DoorRotation.Yaw=CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotation);

	OpenDoorSound=false;
	if(!AudioComponent1)
	{
		return;
	}
	if(!CloseDoorSound)
	{

		AudioComponent1->Play();
		CloseDoorSound=true;

	}
}
float UOpenDoor2::TotalMassOfActors() const
{
	float TotalMass=0.f;

	//Find all overlapping actors
	TArray<AActor*> OverlappingActors;
	PressurePlate1->GetOverlappingActors(OUT OverlappingActors);
	//Add up their masses
	for(AActor* Actor:OverlappingActors)
	{
		TotalMass+=Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}
	return TotalMass;

}
