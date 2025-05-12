#include "Chunk.h"

#include "ProceduralMeshComponent.h"

Chunk::Chunk()
{
}

Chunk::~Chunk()
{
	if (Mesh)
	{
		delete Mesh;
		Mesh=nullptr;
		
	}
}

void Chunk::resetMeshData()
{
	Vertices.Empty();
	Triangles.Empty();
	VertexMap.Empty();
	if (Mesh)Mesh->ClearAllMeshSections();
	
}

UProceduralMeshComponent*& Chunk::GetMesh()
{
	return Mesh;
}

TArray<float>& Chunk::GetTerrainMap()
{
	return TerrainMap;
}

TArray<FVector>& Chunk::GetVertices()
{
	return Vertices;
}

FIntVector& Chunk::GetChunkLocalSize()
{
	return chunkLocalSize;
}

TArray<int32>& Chunk::GetTriangles()
{
	return Triangles;
}

TMap<FVector, int32>& Chunk::GetVertexMap()
{
	return VertexMap;
}
