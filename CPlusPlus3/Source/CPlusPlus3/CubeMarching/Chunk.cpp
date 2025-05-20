#include "Chunk.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "ProceduralMeshComponent.h"
#include "DynamicMesh/DynamicMesh3.h"

Chunk::Chunk():Mesh(nullptr)
{
}




void Chunk::resetMeshData()
{
	Vertices.Empty();
	Triangles.Empty();
	VertexMap.Empty();
	vertexColors.Empty();

	
}

UProceduralMeshComponent*& Chunk::GetMesh()
{
	return Mesh;
}

UInstancedStaticMeshComponent*& Chunk::GetGrassMesh()
{
	return GrassMesh;
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

TArray<FLinearColor>& Chunk::GetVertexColors()
{
	return vertexColors;
}

TMap<FVector, int32>& Chunk::GetVertexMap()
{
	return VertexMap;
}
