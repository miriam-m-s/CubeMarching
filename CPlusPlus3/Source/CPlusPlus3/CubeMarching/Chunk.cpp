#include "Chunk.h"

#include "ProceduralMeshComponent.h"

Chunk::Chunk():Mesh(nullptr)
{
}




void Chunk::resetMeshData()
{
	Vertices.Empty();
	Triangles.Empty();
	VertexMap.Empty();

	
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

FIntPoint& Chunk::GetChunkLocalSize()
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
