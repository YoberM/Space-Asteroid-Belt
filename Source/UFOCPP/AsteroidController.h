// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Asteroid.h"
#include "GameFramework/Actor.h"
#include "AsteroidController.generated.h"

UCLASS()
class UFOCPP_API AAsteroidController : public AActor
{
	GENERATED_BODY()
	
public:
	// Variables para la generacion
	// 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<AAsteroid*> Asteroids;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* MeshToBuild = nullptr;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UParticleSystem* ParticleTemplate = nullptr;
		float initialImpulse = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float generationDistance = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float generationNumber = 5;


	// Audio
	UPROPERTY(Category = Audio, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		TArray<USoundBase*> AsteroidImpactSounds;
public:	
	// Sets default values for this actor's properties
	AAsteroidController();

public:
	// Funciones para la generacion
	void SpawnAsteroid();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& transform) override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
