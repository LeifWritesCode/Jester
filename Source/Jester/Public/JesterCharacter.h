// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "JesterCharacter.generated.h"

UCLASS()
class JESTER_API AJesterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AJesterCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    // executed whenever a kinect frame is processed that changed the internal state
    UFUNCTION(BlueprintImplementableEvent, Category = "Jester|Events", DisplayName = "Kinect Frame Received")
    void OnKinectFrame();
	
};
