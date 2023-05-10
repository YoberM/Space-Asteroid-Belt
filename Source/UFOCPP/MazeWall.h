// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MazeWall.generated.h"

UCLASS()
class UFOCPP_API AMazeWall : public AActor
{
	GENERATED_BODY()

	DECLARE_DELEGATE(FPoroDelegate)


	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* MazeMeshComponent;

	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		TArray<UStaticMeshComponent*> Mazes;


	
public:	
	FPoroDelegate OnPoro;
	// Sets default values for this actor's properties
	AMazeWall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	

public:	
	// Called every frame
	virtual void OnConstruction(const FTransform& transform) override;
	virtual void Tick(float DeltaTime) override;

};
