// Fill out your copyright notice in the Description page of Project Settings.

#include "DoorOpen.h"
#include "Components/AudioComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"

#define OUT
// Sets default values for this component's properties
UDoorOpen::UDoorOpen()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UDoorOpen::BeginPlay()
{
	Super::BeginPlay();
	// float MyFloat = 90.f;
	// FRotator CurrentRotation=GetOwner()->GetActorRotation();
	// CurrentRotation.Yaw=180.f;
	// GetOwner()->SetActorRotation(CurrentRotation);
	// UE_LOG(LogTemp,Warning,TEXT("Actor name: %s"),*GetOwner()->GetName());
	// UE_LOG(LogTemp,Warning,TEXT("Object Position: %s"),*GetOwner()->GetActorLocation().ToString());
	// UE_LOG(LogTemp,Warning,TEXT("Object Rotation: %s"),*GetOwner()->GetActorRotation().ToString());
	// UE_LOG(LogTemp,Warning,TEXT("Object Scale: %s"),*GetOwner()->GetActorScale().ToString());
	
	//2 Code for Rotation from any set point
	InitialYaw=GetOwner()->GetActorRotation().Yaw;
	CurrentYaw=InitialYaw;
	OpenAngle+=InitialYaw;	//OpenAngle=InitialYaw+OpenAngle;
	
	FindPressurePlate();
	FindAudioComponent();

}

void UDoorOpen::FindAudioComponent()
{
	AudioComponent=GetOwner()->FindComponentByClass<UAudioComponent>();
	if(!AudioComponent)
	{
		UE_LOG(LogTemp,Error,TEXT("%s missing audio component!"),*GetOwner()->GetName());

	}
}
void UDoorOpen::FindPressurePlate()
{
	if(!PressurePlate)
	{
		UE_LOG(LogTemp,Error,TEXT("%s has the open door component on it, but no pressur plate set!"),*GetOwner()->GetName());

	}
}
// Called every frame
void UDoorOpen::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
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

void UDoorOpen::OpenDoor(float DeltaTime)
{
	// UE_LOG(LogTemp,Warning,TEXT("%s"),*GetOwner()->GetActorRotation().ToString());
	// UE_LOG(LogTemp,Warning,TEXT("Yaw is: %f"),GetOwner()->GetActorRotation().Yaw);
	
	//1 Code for Rotation and Position change 
	/*float CurrentYaw = GetOwner()->GetActorRotation().Yaw;
	FRotator OpenDoor(0.f,OpenAngle,0.f);
	OpenDoor.Yaw=FMath::Lerp(CurrentYaw,OpenAngle,0.02f);
	GetOwner()->SetActorRotation(OpenDoor);
	
	float CurrentX=GetOwner()->GetActorLocation().X;
	FVector Data1(TargetX,0.f,0.f);
	Data1.X=FMath::Lerp(CurrentX,TargetX,0.02f);
	GetOwner()->SetActorLocation(Data1);*/
	
	//2 Code for Rotation from any set point
	CurrentYaw=FMath::Lerp(CurrentYaw,OpenAngle,DeltaTime * DoorOpenSpeed);//Delta time used for universal frame support
	FRotator DoorRotation=GetOwner()->GetActorRotation();
	DoorRotation.Yaw=CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotation);
	
	CloseDoorSound=false;
	if(!AudioComponent)
	{
		return;
	}
	if(!OpenDoorSound)
	{
		AudioComponent->Play();
		OpenDoorSound=true;

	}

}

void UDoorOpen::DoorClose(float DeltaTime)
{
	CurrentYaw=FMath::Lerp(CurrentYaw,InitialYaw,DeltaTime * DoorCloseSpeed);
	FRotator DoorRotation=GetOwner()->GetActorRotation();
	DoorRotation.Yaw=CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotation);

	OpenDoorSound=false;
	if(!AudioComponent)
	{
		return;
	}
	if(!CloseDoorSound)
	{

		AudioComponent->Play();
		CloseDoorSound=true;

	}
}

float UDoorOpen::TotalMassOfActors() const
{
	float TotalMass=0.f;

	//Find all overlapping actors
	TArray<AActor*> OverlappingActors;
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);
	//Add up their masses
	for(AActor* Actor:OverlappingActors)
	{
		TotalMass+=Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}
	return TotalMass;

}
