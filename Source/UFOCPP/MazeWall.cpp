#include "MazeWall.h"

// Sets default values
AMazeWall::AMazeWall()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ShipMesh(TEXT("/Game/Maze/Meshes/MazeWallMesh.MazeWallMesh"));
	// Create the mesh component
	MazeMeshComponent =  CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RootMazeWallMesh"));
	MazeMeshComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	MazeMeshComponent->SetStaticMesh(ShipMesh.Object);
	RootComponent = MazeMeshComponent;

	UStaticMeshComponent* temp;

	//OnPoro.BindUObject(this, &AMazeWall::Funcionporotocon);
	//OnPoro.Execute();

	for (int i = 0; i < 3; i++)
	{
		temp = CreateDefaultSubobject<UStaticMeshComponent>(*FString::Printf( TEXT("MazeWallMesh%d"),i));
		temp->SetupAttachment(RootComponent);
		Mazes.Add(temp);
		Mazes[i]->SetCollisionProfileName(TEXT("BlockAll"));
		Mazes[i]->SetStaticMesh(ShipMesh.Object);

	}
	// ADD
	// 



	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMazeWall::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMazeWall::OnConstruction(const FTransform& transform)
{
	Super::OnConstruction(transform);
	/*RootComponent->SetWorldLocation(transform.GetLocation());*/
	for (int i = 0; i < 3; i++)
	{
		// Usar las posiciones de la matriz
		Mazes[i]->SetRelativeLocation(FVector(100, 100, i * 100));
	}
}

// Called every frame
void AMazeWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

