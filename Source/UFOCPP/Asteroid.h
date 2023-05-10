// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UFOCPPProjectile.h"
#include "GameFramework/Actor.h"
#include "Asteroid.generated.h"

UCLASS(config = Game)
class UFOCPP_API AAsteroid : public AActor
{
	GENERATED_BODY()
	

public:
	// Variables

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float ScaleFactor = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Lifetime = 5.0f;
public:	
	// Sets default values for this actor's properties
	AAsteroid();

public:
	// Variables para la generacion

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* AsteroidMesh = nullptr;

	// Timer necessary for handling particles
	FTimerHandle TimerHandle_ShotTimerExpired;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UParticleSystem* ParticleTemplate;

	UPROPERTY(Category = Audio, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		TArray<USoundBase*> AsteroidImpactSounds;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	//virtual void OnConstruction(const FTransform& transform) override;


	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
		void SetAsteroidPosition(FVector Position);
	// Handler when the particles expires
	void ShotTimerExpired();
};
