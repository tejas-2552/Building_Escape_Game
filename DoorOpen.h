// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "DoorOpen.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UDoorOpen : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDoorOpen();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void OpenDoor(float DeltaTime);
	void DoorClose(float DeltaTime);
	float TotalMassOfActors() const;
	void FindAudioComponent();
	void FindPressurePlate();

	//tracks whether the sound has been played
	bool OpenDoorSound=false;
	bool CloseDoorSound=true;




private:
	//1 Code for roataion change 
	/*float TargetYaw = 180.f;
	float TargetX=200.f;*/

	//2 Code for any change rotation from any set point
	float CurrentYaw;
	float InitialYaw;

	UPROPERTY(EditAnywhere)
	float MassToOpenDoors=50.f;

	UPROPERTY(EditAnywhere)
	float OpenAngle=90;

	float DoorLastOpened=0.f;

	UPROPERTY(EditAnywhere)
	float DoorCloseDelay=0.5f;

	UPROPERTY(EditAnywhere)
	float DoorOpenSpeed=0.8f;

	UPROPERTY(EditAnywhere)
	float DoorCloseSpeed=2.f;

	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate=nullptr;

	UPROPERTY()
	UAudioComponent* AudioComponent=nullptr;
	

};
