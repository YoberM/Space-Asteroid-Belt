// Copyright Epic Games, Inc. All Rights Reserved.

#include "UFOCPPPawn.h"
#include "UFOCPPProjectile.h"
#include "TimerManager.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/CollisionProfile.h"
#include "Engine/StaticMesh.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"

const FName AUFOCPPPawn::MoveForwardBinding("MoveForward");
const FName AUFOCPPPawn::MoveRightBinding("MoveRight");
const FName AUFOCPPPawn::FireForwardBinding("FireForward");
const FName AUFOCPPPawn::FireBinding("Fire");

const FName AUFOCPPPawn::Mouse_XBinding("Mouse_X");
const FName AUFOCPPPawn::Mouse_YBinding("Mouse_Y");
const FName AUFOCPPPawn::MouseLeftClickBinding("MouseLeftClick");
const FName AUFOCPPPawn::MouseRightClickBinding("MouseRightClick");



AUFOCPPPawn::AUFOCPPPawn()
{	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ShipMesh(TEXT("/Game/TwinStick/Meshes/TwinStickUFO.TwinStickUFO"));
	// Create the mesh component
	ShipMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
	RootComponent = ShipMeshComponent;
	ShipMeshComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	ShipMeshComponent->SetStaticMesh(ShipMesh.Object);
	
	// Cache our sound effect
	static ConstructorHelpers::FObjectFinder<USoundBase> FireAudio(TEXT("/Game/TwinStick/Audio/TwinStickFire.TwinStickFire"));
	FireSound = FireAudio.Object;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when ship does
	CameraBoom->TargetArmLength = 1200.f;
	CameraBoom->SetRelativeRotation(FRotator(-80.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	CameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;	// Camera does not rotate relative to arm

	// Movement
	MoveSpeed = 1000.0f;
	// Weapon
	GunOffset = FVector(90.f, 0.f, 0.f);
	FireRate = 0.1f;
	bCanFire = true;

	// Creando Linterna
	linterna = CreateDefaultSubobject<USpotLightComponent>(TEXT("Linterna"));
	linterna->SetupAttachment(RootComponent);

	// Creando Linterna
	BoostParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("BoostParticle"));
	BoostParticle->SetupAttachment(RootComponent);
	
	// Creando Sonido
	BoostStarshipAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("BoostStarshipAudio"));
	BoostStarshipAudio->SetupAttachment(RootComponent);

	// Spline settings

	AimSpline = CreateDefaultSubobject<USplineMeshComponent>(TEXT("AimSpline"));
	AimSpline->SetupAttachment(RootComponent);

	
}

void AUFOCPPPawn::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	// set up gameplay key bindings
	PlayerInputComponent->BindAxis(MoveForwardBinding);
	PlayerInputComponent->BindAxis(MoveRightBinding);
	PlayerInputComponent->BindAxis(FireForwardBinding);
	PlayerInputComponent->BindAxis(FireBinding);

	PlayerInputComponent->BindAxis(Mouse_XBinding);
	PlayerInputComponent->BindAxis(Mouse_YBinding);
	PlayerInputComponent->BindAxis(MouseLeftClickBinding);
	PlayerInputComponent->BindAxis(MouseRightClickBinding);
}

void AUFOCPPPawn::Tick(float DeltaSeconds)
{
	// Find movement direction
	const float ForwardValue = GetInputAxisValue(MoveForwardBinding);
	const float RightValue = GetInputAxisValue(MoveRightBinding);

	const float MouseX = GetInputAxisValue(Mouse_XBinding);

	const float MouseY = GetInputAxisValue(Mouse_YBinding);

	FVector MousePosition(MouseY, MouseX,0.0f);

	
	MousePosition = MousePosition / AUFOCPPPawn::MouseDamplingFactor;



	// Clamp max size so that (X=1, Y=1) doesn't cause faster movement in diagonal directions
	//const FVector MoveDirection = FVector(ForwardValue, RightValue, 0.f).GetClampedToMaxSize(1.0f);
	//const FVector MoveDirection = FVector(ForwardValue, 0.f, 0.f).GetClampedToMaxSize(1.0f);
	const FVector MoveDirection = (ShipMeshComponent->GetForwardVector() * ForwardValue).GetClampedToMaxSize(1.0f);

	// Calculate  movement
	const FVector Movement = MoveDirection * MoveSpeed * DeltaSeconds;

	FRotator NewMouseRotation;
	//Rotacion del mouse
	if (MousePosition.SizeSquared() > 0.0f) {
		NewMouseRotation = (MousePosition  + OldMousePosition).Rotation();
		OldMousePosition = (MousePosition + OldMousePosition).GetClampedToMaxSize(1.0f);
		OldRotation = NewMouseRotation;
	}
	else {

		NewMouseRotation = OldRotation;
	}

	// Movimiento del Spline
	AimSpline->SetWorldRotation(OldRotation);

	FRotator CurrentRotation = GetActorRotation();
	float NewRotation2 = RightValue;

	FRotator NewRotation = FRotator(CurrentRotation.Pitch, CurrentRotation.Yaw + RightValue, CurrentRotation.Roll);

	// If non-zero size, move this actor
	if (Movement.SizeSquared() > 0.0f || RightValue != 0.0f)
	{

		FHitResult Hit(1.f);
		RootComponent->MoveComponent(FVector(0,0,0), NewRotation, true, &Hit);
		
		if (Hit.IsValidBlockingHit())
		{
			const FVector Normal2D = Hit.Normal.GetSafeNormal2D();
			const FVector Deflection = FVector::VectorPlaneProject(Movement, Normal2D) * (1.f - Hit.Time);
			RootComponent->MoveComponent(Deflection, NewRotation, true);
		}
		ShipMeshComponent->AddImpulse(MoveDirection * ShipImpulseFactor *DeltaSeconds);

		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, FString::Printf(TEXT("Ship: %s"), *ShipMeshComponent->GetName()));
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Ship: %s"), *MoveDirection.ToString()));
		}

	}

	//Particle Handler
	if (BoostParticle != nullptr) {
		if (ForwardValue == 1.0f) {
			BoostParticle->Activate();
			if (BoostStarshipAudio != nullptr)
					BoostStarshipAudio->Activate();
		}
		else {
			BoostParticle->Deactivate();
			if (BoostStarshipAudio != nullptr)
					BoostStarshipAudio->Deactivate();
		}
	}

	// Movimiento Set Final

	///////////////////////////  MOUSE ////////////////////////////////////
	

	// Create fire direction vector
	const float MouseLeftClickValue = GetInputAxisValue(MouseLeftClickBinding);
	const float MouseRightClickValue = GetInputAxisValue(MouseRightClickBinding);

	const FVector FireDirection = FVector(MouseLeftClickValue, 0.f, 0.f);



	// Try and fire a shot

	if (MouseRightClickValue != 0)
	{
		GetWorldSettings()->TimeDilation = 0.2;
		//CustomTimeDilation = 0.2;
	}
	else {
		GetWorldSettings()->TimeDilation = 1;
		//CustomTimeDilation = 1;

	}
	FireShot(FireDirection);
	// Light
}

void AUFOCPPPawn::BeginPlay()
{
	Super::BeginPlay();
	/*if (AimSplineMesh != nullptr) {
		AimSpline->SetStaticMesh(AimSplineMesh->GetStaticMesh());
	}*/
}

void AUFOCPPPawn::FireShot(FVector FireDirection)
{
	// If it's ok to fire again
	if (bCanFire == true)
	{
		// If we are pressing fire stick in a direction
		if (FireDirection.SizeSquared() > 0.0f)
		{

			const FRotator FireRotation = FireDirection.Rotation();
			// Spawn projectile at an offset from this pawn
			const FVector SpawnLocation = GetActorLocation() + OldRotation.RotateVector(GunOffset);

			UWorld* const World = GetWorld();
			if (World != nullptr)
			{
				// spawn the projectile
				World->SpawnActor<AUFOCPPProjectile>(SpawnLocation, OldRotation);
			}

			bCanFire = false;
			World->GetTimerManager().SetTimer(TimerHandle_ShotTimerExpired, this, &AUFOCPPPawn::ShotTimerExpired, FireRate);

			// try and play the sound if specified
			if (FireSound != nullptr)
			{
				UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
			}

			bCanFire = false;
		}
	}
}

void AUFOCPPPawn::ShotTimerExpired()
{
	bCanFire = true;
}

void AUFOCPPPawn::OnAimHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) {

	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL))
	{
		//Check other object
	}

}