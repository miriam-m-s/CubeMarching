#pragma once
class UProceduralMeshComponent;
class Chunk
{
	UProceduralMeshComponent* Mesh;
	TArray<float> TerrainMap;
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TMap<FVector, int32> VertexMap;
	FIntPoint chunkLocalSize;

public:
	Chunk();

	void resetMeshData();
	// Métodos para acceder por referencia
	UProceduralMeshComponent*& GetMesh();              // puntero por referencia
	TArray<float>& GetTerrainMap();
	TArray<FVector>& GetVertices();
	FIntPoint& GetChunkLocalSize();
	TArray<int32>& GetTriangles();
	TMap<FVector, int32>& GetVertexMap();

	
};
