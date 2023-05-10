// Fill out your copyright notice in the Description page of Project Settings.


#include "Asteroid.h"

// Sets default values
AAsteroid::AAsteroid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
	AsteroidMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Asteroid"));
	RootComponent = AsteroidMesh;
	AsteroidMesh->BodyInstance.SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	AsteroidMesh->SetRelativeScale3D(FVector(AAsteroid::ScaleFactor, AAsteroid::ScaleFactor, AAsteroid::ScaleFactor));
	AsteroidMesh->SetSimulatePhysics(true);
	AsteroidMesh->OnComponentHit.AddDynamic(this, &AAsteroid::OnHit);
}


// Called when the game starts or when spawned
void AAsteroid::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void AAsteroid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAsteroid::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL))
	{
		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("I Hit: %s"), *OtherActor->GetName()));

			//If asteroid is hit by a projectile
			if (OtherActor->IsA<AUFOCPPProjectile>()) {

				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("projectile: %s"), *OtherActor->GetName()));

				const FVector SpawnLocation = GetActorLocation();


				UWorld* const World = GetWorld();
				
				if (World != nullptr)
				{

					// spawn the projectile

					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Particula Creada")));
					UGameplayStatics::SpawnEmitterAtLocation(World, ParticleTemplate,OtherActor->GetActorLocation());
					
					if (AsteroidImpactSounds.Num() > 0) {

						int RandomIndex = FMath::RandRange(0, AsteroidImpactSounds.Num() - 1);
						UGameplayStatics::PlaySoundAtLocation(this, AsteroidImpactSounds[RandomIndex], GetActorLocation());
					}



					//World->GetTimerManager().SetTimer(TimerHandle_ShotTimerExpired, this, &AAsteroid::ShotTimerExpired, Lifetime);

					//Particle->Activate();
				}


			}
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Emerald, FString::Printf(TEXT("type: %s"), *OtherActor->GetClass()->GetName()));

		}
	}
}

void AAsteroid::SetAsteroidPosition(FVector Position)
{
	RootComponent->SetRelativeLocation(Position);
}

void AAsteroid::ShotTimerExpired()
{
	
}

