#include "CubeMarching.h"
#include "ProceduralMeshComponent.h"
#include "Components/InputComponent.h"
#include "Field/FieldSystemNoiseAlgo.h"

#include "GameFramework/PlayerController.h"

ACubeMarching::ACubeMarching():MarchingIndex(0)
{
	// Inicialización de EdgeTable con algunos valores de ejemplo
	EdgeTable = {
		{ FVector(0, 0, 0), FVector(1, 0, 0) },
		{ FVector(1, 0, 0), FVector(1, 1, 0) },
		{ FVector(0, 1, 0), FVector(1, 1, 0) },
		{ FVector(0, 0, 0), FVector(0, 1, 0) },
		{ FVector(0, 0, 1), FVector(1, 0, 1) },
		{ FVector(1, 0, 1), FVector(1, 1, 1) },
		{ FVector(0, 1, 1), FVector(1, 1, 1) },
		{ FVector(0, 0, 1), FVector(0, 1, 1) },
		{ FVector(0, 0, 0), FVector(0, 0, 1) },
		{ FVector(1, 0, 0), FVector(1, 0, 1) },
		{ FVector(1, 1, 0), FVector(1, 1, 1) },
		{ FVector(0, 1, 0), FVector(0, 1, 1) }
	};
	TerrainMap.SetNum((GridSize.X + 1) * (GridSize.Y + 1) * (GridSize.Z + 1));
	// Inicialización de ProceduralMeshComponent
	Mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedMesh"));
	RootComponent = Mesh;
}

void ACubeMarching::BeginPlay()
{
	Super::BeginPlay();
	
	// Habilitar la entrada del jugador
	EnableInput(GetWorld()->GetFirstPlayerController());

	// Vincula la tecla B a la acción para construir la malla
	if (InputComponent)
	{
		InputComponent->BindKey(EKeys::B, IE_Pressed, this, &ACubeMarching::OnBKeyPressed);
	}
}
void ACubeMarching::OnBKeyPressed()
{
	UE_LOG(LogTemp, Warning, TEXT("OnBKeyPressed called"));

	// Vacía los arrays antes de cada nueva construcción
	UE_LOG(LogTemp, Warning, TEXT("Emptying Vertices and Triangles"));
	Vertices.Empty();
	Triangles.Empty();

	// Ejecuta MarchCube con el índice actual
	UE_LOG(LogTemp, Warning, TEXT("Calling MarchCube with index: %d"), MarchingIndex);
	MarchCube(MarchingIndex);

	// Construye la malla
	UE_LOG(LogTemp, Warning, TEXT("Calling BuildMesh"));
	BuildMesh();

	// Incrementa el índice de configuración y reinicia si se excede
	MarchingIndex++;
	UE_LOG(LogTemp, Warning, TEXT("Incremented MarchingIndex to: %d"), MarchingIndex);

	if (MarchingIndex > 255)
	{
		MarchingIndex = 1;
		UE_LOG(LogTemp, Warning, TEXT("MarchingIndex reset to 1"));
	}
}

void ACubeMarching::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}



void ACubeMarching::CreateTerrain()
{
	
	/*
		This loops iterates over a 3D grid of points 
		to assign a noise value or density to each voxel corner.

		Example for a 2D slice (X-Z plane), GridSize = 2:
		
		Grid of points:
		
		o---o---o   --> X (3 points for 2 cells)
		|   |   |
		o---o---o
		|   |   |
		o---o---o
		
		Each "o" is a vertex (corner), and each square formed by 4 "o" points is a cube (in 2D, a cell).
		To get 2 cubes in X, you need 3 points (2+1).
		The same applies in 3D.

		This loop prepares those points for later cube generation and marching cubes logic.
	*/

	for (int x=0;x<GridSize.X+1;x++)
	{
		for (int y=0;y<GridSize.Y+1;y++)
		{
			for (int z=0;z<GridSize.Z+1;z++)
			{
				float noise = FMath::PerlinNoise3D(FVector(x,y,z));
				TerrainMap[getTerrainIndex(x, y, z)] = noise;
			}
		}
	}
}

void ACubeMarching::BuildMesh()
{
	UE_LOG(LogTemp, Warning, TEXT("BuildMesh called"));
	UE_LOG(LogTemp, Warning, TEXT("Vertices count: %d"), Vertices.Num());
	UE_LOG(LogTemp, Warning, TEXT("Triangles count: %d"), Triangles.Num());

	TArray<FVector> normals;
	TArray<FVector2D> uvs;
	TArray<FProcMeshTangent> tangents;
	TArray<FLinearColor> vertexColors;

	// Inicializa las normales, UVs y demás arrays
	UE_LOG(LogTemp, Warning, TEXT("Initializing mesh data arrays"));
	normals.Init(FVector(0, 0, 1), Vertices.Num());
	uvs.Init(FVector2D(0, 0), Vertices.Num());
	tangents.Init(FProcMeshTangent(1, 0, 0), Vertices.Num());
	vertexColors.Init(FLinearColor::White, Vertices.Num());

	UE_LOG(LogTemp, Warning, TEXT("Calling CreateMeshSection_LinearColor"));
	// Crea la malla usando los datos
	Mesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, normals, uvs, vertexColors, tangents, false);
	UE_LOG(LogTemp, Warning, TEXT("Mesh section created successfully"));
}

 int ACubeMarching::GetConfigurationIndex(float* cube)
{
	int configIndex = 0;
	for (int i = 0; i < 8; ++i)
	{
		if ( cube[i] > SurfaceLevel) 
		{
			configIndex |= (1 << i); 
		}
	}

	return configIndex; 


}
void ACubeMarching::MarchCube(int configIndex)
{
	if (configIndex == 0 || configIndex == 255) return;

	int edgeIndex = 0;
	// Genera los vértices y triángulos en base al configIndex
	for (int i = 0; i < 5; i++)
	{

		for (int j = 0; j < 3; j++)
		{
			int indice = TriangleTable[configIndex][edgeIndex];
			if (indice == -1) return;

			FVector vert1 = EdgeTable[indice].Key;
			FVector vert2 = EdgeTable[indice].Value;
			FVector vertice = vert1 + (vert2 - vert1) / 2.0f; // Interpolación correcta
			Vertices.Add(vertice*1000);
			Triangles.Add(Vertices.Num()-1 );
			edgeIndex++;
		}

		// Ahora que añadimos los 3 vértices, añadimos el triángulo
	
		
		
	}
}



void ACubeMarching::CleanMeshData()
{
	Vertices.Empty();
	Triangles.Empty();
}

const int ACubeMarching::getTerrainIndex( int x, int y, int z)
{
	return x + (GridSize.X + 1) * (y + (GridSize.Y + 1) * z);
}
