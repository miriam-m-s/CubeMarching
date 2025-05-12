#include "Marching.h"

#include "FrameTypes.h"
#include "ProceduralMeshComponent.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Components/InputComponent.h"
	
#include "Field/FieldSystemNoiseAlgo.h"

#include "GameFramework/PlayerController.h"
#include "Operations/EmbedSurfacePath.h"

AMarching::AMarching():MarchingIndex(0)
{
	


	Mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedMesh"));
	RootComponent = Mesh;
}

void AMarching::BeginPlay()
{
	Super::BeginPlay();
	// TerrainMap.SetNum((GridSize.X + 1) * (GridSize.Y + 1) * (GridSize.Z + 1));
	//
	// UE_LOG(LogTemp, Warning, TEXT("Create Terrain"));
	// CreateTerrain();
	// CubeIteration();

}



void AMarching::OnBKeyPressed()
{

	
}

void AMarching::GenerateTerrain()
{
	//si el chunk es mas grande que grid se queda el grid con su tamaño 
	ChunkSize.X = FMath::Min(GridSize.X, ChunkSize.X);
	ChunkSize.Y = FMath::Min(GridSize.Y, ChunkSize.Y);
	ChunkSize.Z = FMath::Min(GridSize.Z, ChunkSize.Z);
	//chunks enteros que hay en el terreno
	
	NumChunks = FIntVector(
		(GridSize.X + ChunkSize.X - 1) / ChunkSize.X,
		(GridSize.Y + ChunkSize.Y - 1) / ChunkSize.Y,
		(GridSize.Z + ChunkSize.Z - 1) / ChunkSize.Z
	);

	// Calcular el tamaño de grid sobrante que no llena un chunk completo (por si lo necesitas)
	Remainder = FIntVector(
		GridSize.X % ChunkSize.X,
		GridSize.Y % ChunkSize.Y,
		GridSize.Z % ChunkSize.Z
	);
	UE_LOG(LogTemp, Warning, TEXT("NumChunks = (%d, %d, %d)"), NumChunks.X, NumChunks.Y, NumChunks.Z);
	UE_LOG(LogTemp, Warning, TEXT("Remainder = (%d, %d, %d)"), Remainder.X, Remainder.Y, Remainder.Z);

	DeleteTerrain();
	
	TerrainMap.SetNum((GridSize.X + 1) * (GridSize.Y + 1) * (GridSize.Z + 1));
	
	UE_LOG(LogTemp, Warning, TEXT("Create Terrain"));
	CreateTerrain();
	CubeIteration();
}

void AMarching::DeleteTerrain()
{
	CleanMeshData();
	
	if (Mesh)
	{
		Mesh->ClearAllMeshSections(); // Esto borra visualmente la malla del viewport
	}

	TerrainMap.Empty();
	UE_LOG(LogTemp, Warning, TEXT("Terrain deleted"));

}

void AMarching::ConvertToStaticMesh()
{
	// if (!Mesh) return;
	//
	// FString PackageName = TEXT("/Game/Generated/MarchingStaticMesh_") + FGuid::NewGuid().ToString();
	// UPackage* Package = CreatePackage(*PackageName);
	//
	// // Crea el Static Mesh
	// UStaticMesh* StaticMesh = NewObject<UStaticMesh>(Package, *FPaths::GetBaseFilename(PackageName), RF_Public | RF_Standalone);
	// StaticMesh->InitResources();
	//
	// UKismetProceduralMeshLibrary::CopyProceduralMeshFromStaticMeshComponent(Mesh, 0, StaticMesh, true);
	//
	// // Guarda el asset en disco
	// FAssetRegistryModule::AssetCreated(StaticMesh);
	// StaticMesh->MarkPackageDirty();
	// FString PackageFileName = FPackageName::LongPackageNameToFilename(PackageName, FPackageName::GetAssetPackageExtension());
	// UPackage::SavePackage(Package, StaticMesh, EObjectFlags::RF_Public | EObjectFlags::RF_Standalone, *PackageFileName);
}

void AMarching::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}



void AMarching::CreateTerrain()
{
	
	UE_LOG(LogTemp, Warning, TEXT("Terrain Map Size: %d"), TerrainMap.Num());
	
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
			
		
				
				float height = FMath::PerlinNoise2D(FVector2D(x / 16.f * 1.5f + 0.001f, y / 16.f * 1.5f + 0.001f));
				height = (height + 1.f) * 0.5f; // Remap [-1,1] to [0,1]
				height *= GridSize.Z; // Escalar a la altura máxima

				for (int z = 0; z < GridSize.Z + 1; z++)
				{
					float density = z - height; // Positivo: aire, Negativo: sólido
					TerrainMap[getTerrainIndex(x, y, z)] = density;
				}

			
		}
	}
}

void AMarching::BuildMesh()
{
	// UE_LOG(LogTemp, Warning, TEXT("BuildMesh called"));
	 UE_LOG(LogTemp, Warning, TEXT("Vertices count: %d"), Vertices.Num());
	// UE_LOG(LogTemp, Warning, TEXT("Triangles count: %d"), Triangles.Num());

	TArray<FVector> normals;
	TArray<FVector2D> uvs;
	TArray<FProcMeshTangent> tangents;
	TArray<FLinearColor> vertexColors;

	// Inicializa las normales, UVs y demás arrays
	//UE_LOG(LogTemp, Warning, TEXT("Initializing mesh data arrays"));
	//normals.Init(FVector(0, 0, 1), Vertices.Num());
	normals.SetNumZeroed(Vertices.Num());
	for (int i = 0; i < Triangles.Num(); i += 3)
	{
		int i0 = Triangles[i];
		int i1 = Triangles[i + 1];
		int i2 = Triangles[i + 2];
	
		const FVector& v0 = Vertices[i0];
		const FVector& v1 = Vertices[i1];
		const FVector& v2 = Vertices[i2];
	
		FVector faceNormal = FVector::CrossProduct(v1 - v0, v2 - v0).GetSafeNormal();
	
		normals[i0] -= faceNormal;
		normals[i1] -= faceNormal;
		normals[i2] -= faceNormal;
	}
	
	// Paso 3: normalizar cada normal de vértice
	for (int i = 0; i < normals.Num(); ++i)
	{
	
		normals[i].Normalize();
	}
	uvs.Init(FVector2D(0, 0), Vertices.Num());
	tangents.Init(FProcMeshTangent(1, 0, 0), Vertices.Num());
	vertexColors.Init(FLinearColor::Green, Vertices.Num());

	// UE_LOG(LogTemp, Warning, TEXT("Calling CreateMeshSection_LinearColor"));
	// Crea la malla usando los datos
	Mesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, normals, uvs, vertexColors, tangents, true);
	// UE_LOG(LogTemp, Warning, TEXT("Mesh section created successfully"));
}

 int AMarching::GetConfigurationIndex(float* cube)
{
	int configIndex = 0;
	for (int i = 0; i < 8; ++i)
	{
		if ( cube[i] > SurfaceLevel) 
		{
			//desplazar bits a la izquierda si i es igual 3 0000 1000 y el or "suma" los bits con el anterior
			configIndex |= (1 << i);
			
		}
	}

	return configIndex; 


}
void AMarching::MarchCube(FVector pos,float* cube)
{
	int configIndex = GetConfigurationIndex(cube);
	if (configIndex == 0 || configIndex == 255) return;

	int edgeIndex = 0;
	// maximo numero de trinagulos que puede haber en una intersecion de un cubo con estas 256 posibles combinociones 
	for (int i = 0; i < 5; i++)
	{
		FVector verts[3];
		int vertIndices[3];

		// Recoge los 3 vértices del triángulo primero
		for (int j = 0; j < 3; j++)
		{
			int indice = TriangleTable[configIndex][edgeIndex];
			if (indice == -1) return;

			FVector vert1 = pos + EdgeTable[indice][0];
			FVector vert2 = pos + EdgeTable[indice][1];
			float noise1 = TerrainMap[getTerrainIndex(vert1.X, vert1.Y, vert1.Z)];
			float noise2 = TerrainMap[getTerrainIndex(vert2.X, vert2.Y, vert2.Z)];
			float t = (SurfaceLevel - noise1) / (noise2 - noise1);
			FVector vertice = FMath::Lerp(vert1, vert2, t);
			FVector snapped = vertice.GridSnap(0.01f);

			int* valor = VertexMap.Find(snapped);
			if (valor)
			{
				vertIndices[j] = *valor;
			}
			else
			{
				Vertices.Add(vertice * 100);
				int newIndex = Vertices.Num() - 1;
				VertexMap.Add(snapped, newIndex);
				vertIndices[j] = newIndex;
			}

			edgeIndex++;
		}

		
		Triangles.Add(vertIndices[0]);
		Triangles.Add(vertIndices[2]);
		Triangles.Add(vertIndices[1]);
	}

}



void AMarching::CleanMeshData()
{
	Vertices.Empty();
	Triangles.Empty();
	VertexMap.Empty(); 
}

const int AMarching::getTerrainIndex( int x, int y, int z)
{
	return x + y * (GridSize.X + 1) + z * (GridSize.X + 1) * (GridSize.Y + 1);

}

void AMarching::CubeIteration()
{
	
	//iterates for each cube of the grid
	
	for (int x=0;x<GridSize.X;x++)
	{
		for (int y=0;y<GridSize.Y;y++)
		{
			for (int z=0;z<GridSize.Z;z++)
			{
				float* cube=new float[8];
				for (int i = 0; i < 8; i++)
				{
					FVector corner = FVector(x, y, z)+ CornerTable[i] ;
					cube[i] = TerrainMap[getTerrainIndex(corner.X, corner.Y, corner.Z)];
				}
			
				MarchCube(FVector(x,y,z), cube);
			}
		}
	}
	BuildMesh();
}
