// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <cstdlib>

// Headers generador
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <vector>
#include "Components/SpotLightComponent.h"


#include "EngineUtils.h"
#include "Engine/Engine.h"
#include "Maze.generated.h"


UCLASS()
class UFOCPP_API AMaze : public AActor
{
	GENERATED_BODY()

		// Uso un instanciador para generar todas las paredes.
	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Transient)
	class UInstancedStaticMeshComponent* MazeInstacer; 
	
	UPROPERTY(Category = Mesh, EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMesh* MazeMesh;

	UPROPERTY(Category = "Map", VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		TArray<int32> Map;

	int32 SizexSave = newmazesize;
	int32 SizeySave = newmazesize;
public:	
	// Sets default values for this actor's properties
	AMaze();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Uso onconstruction para que los cambios se faciliten en el editor.
	virtual void OnConstruction(const FTransform& transform) override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void GenerateMaze();
	void InstanceMaze();

	UFUNCTION(CallInEditor, Category = "Generator")
		void Randomnize();
// Generador 
private:
	struct cell {
		int x = -1, y = -1;
		int parentx = -1, parenty = -1;
		bool visited = false;
		int dist = 0;
		cell() {}
		cell(int _x, int _y) {
			x = _x;
			y = _y;
		}
	};

	UPROPERTY(Category = "Map", EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int mazesize = 9;
	std::vector<std::vector<cell>> mazevec;

	int newmazesize = (mazesize - 2) * 2 + 1;

	std::vector<std::vector<int>> newmaze;

	bool checkrelation(int, int, cell);
	void generateMaze2D();

	// Luz que indica el camino mas largo
	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		USpotLightComponent* indicador;
};
