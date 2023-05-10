// Fill out your copyright notice in the Description page of Project Settings.


#include "Maze.h"
#include "Components/InstancedStaticMeshComponent.h"

// Sets default values
AMaze::AMaze()
{
	// Construccion del instanciador
	MazeInstacer = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("MazeInstancer"));
	MazeInstacer->SetCollisionProfileName(TEXT("BlockAll"));
	MazeInstacer->SetStaticMesh(MazeMesh);
	MazeInstacer->SetSimulatePhysics(true);

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	indicador = CreateDefaultSubobject<USpotLightComponent>(TEXT("Linterna"));
	indicador->SetRelativeRotation(FRotator(270, 0, 0));

	GenerateMaze();
}

// Called when the game starts or when spawned
void AMaze::BeginPlay()
{
	Super::BeginPlay();

}


void AMaze::OnConstruction(const FTransform& transform)
{
	Super::OnConstruction(transform);
	// Set de las luces
	indicador->SetLightColor(FColor(255, 217, 182));
	indicador->SetIntensity(15000);

	MazeInstacer->SetStaticMesh(MazeMesh);

	// En caso de que cambien los tamaños, se debe generar un nuevo laberinto
	if (newmazesize != SizexSave || newmazesize != SizeySave) {
		GenerateMaze();
	}
	InstanceMaze();
}

// Called every frame
void AMaze::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMaze::GenerateMaze()
{
	srand((unsigned)time(NULL));

	generateMaze2D();

	// Map se encarga de almacenar y tener la posibilidad de 
	// visualizarlo en el editor
	Map.Empty();
	Map.Init(1, newmazesize * newmazesize);
	for (int i = 0; i < newmazesize * newmazesize; i++)
	{
		Map[i] = newmaze[i/ newmazesize][i% newmazesize];
	}
	// estas variables nos sirven para generar cuando se hagan cambios
	SizexSave = newmazesize;
	SizeySave = newmazesize;
}

void AMaze::InstanceMaze()
{
	MazeInstacer->ClearInstances();

	std::cout << Map.Num() << std::endl;
	for (int i = 0; i < newmazesize; i++)
	{
		for (int j = 0; j < newmazesize; j++)
		{
			if (Map[i * newmazesize + j] == 0) {
				FTransform temp;
				temp.SetLocation(FVector(i * 200, j * 200, 0));
				int32 id = MazeInstacer->AddInstance(temp);
				//UEngine::AddOnScreenDebugMessage(i*Sizex+j, 5.0f, FColor(30, 30, 255), FString("texto"),false,FVector2D(1,1));

			}
			else if (Map[i * newmazesize + j] == 'B') {
				FTransform temp;
				temp.SetLocation(FVector(i * 200, j * 200, 300));
				indicador->SetRelativeLocation(temp.GetLocation());
			}
		}
	}
}

void AMaze::Randomnize()
{
	GenerateMaze();
	InstanceMaze();
}

bool AMaze::checkrelation(int x, int y, cell _cell) {
	if (_cell.parentx == x && _cell.parenty == y) {
		return true;
	}
	return false;
}


void AMaze::generateMaze2D() {
	// cleaning memory
	if (mazevec.size() != 0) {
		mazevec.clear();
	}

	if (newmaze.size() != 0) {
		newmaze.clear();
	}

	// inicializacion 
	newmazesize = (mazesize - 2) * 2 + 1;
	// newmaze es la generacion final
	for (int i = 0; i < newmazesize; i++)
	{
		newmaze.push_back(std::vector<int>(newmazesize, 0));
	}
	// mazevec es el generador inicial, necesitara de un formateador 
	// para mostrar el resutlado final
	for (int i = 0; i < mazesize; i++)
	{
		std::vector <cell> temp;
		for (int j = 0; j < mazesize; j++)
		{
			temp.push_back(cell(i, j));
		}
		mazevec.push_back(temp);
	}


	// 1 = wall y 0 = camino libre
	// el punto de partida es el punto 1,1
	mazevec[1][1].visited = true;
	// parentx y parenty es usado para indicar a que vecino se ha conectado
	mazevec[1][1].parentx = 1;
	mazevec[1][1].parenty = 1;

	// se limita las celdas
	for (int i = 0; i < mazesize; i++)
	{
		mazevec[0][i].visited = true;
		mazevec[mazesize - 1][i].visited = true;
		mazevec[i][0].visited = true;
		mazevec[i][mazesize - 1].visited = true;
	}


	std::vector<cell*> cellstack;
	cellstack.push_back(&mazevec[1][1]);
	cell* actualcell = &mazevec[1][1];
	// esta celda es usada para obtener la distancia mas larga
	cell max = *cellstack.back();
	while (true)
	{
		// celdas que no han sido visitadas y que puede ser usadas
		std::vector<cell*> celltemp;
		for (int i = -1; i <= 2; i += 2)
		{
			if (mazevec[actualcell->x + i][actualcell->y].visited == false) {
				celltemp.push_back(&mazevec[actualcell->x + i][actualcell->y]);
				// esto verifica si hay celdas horizontales disponibles
			}
			if (mazevec[actualcell->x][actualcell->y + i].visited == false) {
				celltemp.push_back(&mazevec[actualcell->x][actualcell->y + i]);

				// esto verifica si hay celdas verticales disponibles
			}
		}
		// caso en el que no haya ninguna celda util, significa que llego 
		// al final de su recorrido esto lo quita del stack y continua
		if (celltemp.size() == 0) {

			if (actualcell->dist > max.dist) {
				max = *actualcell;
			}
			cellstack.pop_back();
			// si el stack esta vacio, significa que termino.
			if (cellstack.size() == 0) {
				break;
			}
			else {
				actualcell = cellstack.back();
				continue;
			}
		}

		// elijo una celda aleatoria para la generacion
		int pick = rand() % (celltemp.size());
		// a la celda elegida se le vincula con el actual
		celltemp[pick]->parentx = actualcell->x;
		celltemp[pick]->parenty = actualcell->y;
		celltemp[pick]->dist = actualcell->dist + 1;
		cellstack.push_back(celltemp[pick]);
		actualcell = cellstack.back();
		// se marca como visitada a la celda actual
		actualcell->visited = true;



	}
	// Dibujar el maze
	// Se hacen uso de los 2 formateadores
	// Horizontal
	for (int i = 0; i < mazesize - 2; i++)
	{
		for (int j = 0; j < mazesize - 1 - 2; j++)
		{

			if (checkrelation(i + 1, j + 1, mazevec[i + 1][j + 1 + 1]) || checkrelation(i + 1, j + 1 + 1, mazevec[i + 1][j + 1])) {
				int ni = 1 + i * 2;
				int nj = 1 + j * 2;
				// se buscan nuevos indices y se unen por trios.
				newmaze[ni][nj] = 1;
				newmaze[ni][nj + 1] = 1;
				newmaze[ni][nj + 1 + 1] = 1;
			}
		}
	}
	// Vertical
	for (int i = 0; i < mazesize - 1 - 2; i++)
	{
		for (int j = 0; j < mazesize - 2; j++)
		{
			if (checkrelation(i + 1, j + 1, mazevec[i + 1 + 1][j + 1]) || checkrelation(i + 1 + 1, j + 1, mazevec[i + 1][j + 1])) {
				int ni = 1 + i * 2;
				int nj = 1 + j * 2;
				newmaze[ni][nj] = 1;
				newmaze[ni + 1][nj] = 1;
				newmaze[ni + 1 + 1][nj] = 1;
			}
		}
	}
	// Se marcan el sitio inicial y el final.
	newmaze[1][1] = 'A';
	newmaze[(max.x - 1) * 2 + 1][(max.y - 1) * 2 + 1] = 'B';
}

