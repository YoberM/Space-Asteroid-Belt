// Fill out your copyright notice in the Description page of Project Settings.


#include "AsteroidController.h"

// Sets default values
AAsteroidController::AAsteroidController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	
}

void AAsteroidController::SpawnAsteroid()
{
	FVector PlayerLocation = GetActorLocation();
	UE_LOG(LogTemp, Display, TEXT("%s"), *GetActorLocation().ToString());

	// Generar un número aleatorio para determinar la distancia y dirección del asteroide desde el jugador
	float Distance = FMath::RandRange(-generationDistance, generationDistance);
	float Angle = FMath::RandRange(0.0f, 360.0f);
	FVector Offset = FVector(Distance * FMath::Cos(Angle), Distance * FMath::Sin(Angle), 0.0f);

	// Instanciar un nuevo asteroide en la posición aleatoria cerca del jugador
	FVector AsteroidLocation = PlayerLocation + Offset;
	FRotator AsteroidRotation = FRotator::ZeroRotator;
	AAsteroid* NewAsteroid = GetWorld()->SpawnActor<AAsteroid>(AAsteroid::StaticClass(), AsteroidLocation, AsteroidRotation);
	
	NewAsteroid->AsteroidMesh->SetStaticMesh(MeshToBuild->GetStaticMesh());
	NewAsteroid->ParticleTemplate = ParticleTemplate;
	NewAsteroid->AsteroidImpactSounds = AsteroidImpactSounds;
	Asteroids.Add(NewAsteroid);

}

// Called when the game starts or when spawned
void AAsteroidController::BeginPlay()
{
	Super::BeginPlay();
	MeshToBuild->SetVisibility(false);
	for (int i = 0; i < generationNumber; i++)
	{
		SpawnAsteroid();
	}

}

void AAsteroidController::OnConstruction(const FTransform& transform)
{

}

// Called every frame
void AAsteroidController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

