
#include "myProceduralMesh.h"
void myProceduralMesh::DrawStaticElements(FStaticPrimitiveDrawInterface* PDI)
{
	if( RuntimeVirtualTextures.Num() == 0 )
	{
		return;
	}
	const FRYCProcMeshProxySection* Section = Sections[0];
	UMaterialInterface* SectionMat = Section->Material;
	check(SectionMat);

	FMeshBatch MeshBatch;
	MeshBatch.LODIndex = 0;
	MeshBatch.SegmentIndex = 0;

	MeshBatch.VertexFactory = &MeshVertexBuffersSection->VertexFactory;
	MeshBatch.Type = PT_TriangleList;

	MeshBatch.LODIndex = 0;
	MeshBatch.SegmentIndex = 0;

	MeshBatch.bDitheredLODTransition = !IsMovable() && Section->Material->GetRenderProxy()->GetMaterialInterface()->IsDitheredLODTransition();

	MeshBatch.bRenderToVirtualTexture = true;
	MeshBatch.MaterialRenderProxy = Section->Material->GetRenderProxy();
	MeshBatch.RuntimeVirtualTextureMaterialType = (int32)ERuntimeVirtualTextureMaterialType::BaseColor_Normal_Specular;
	MeshBatch.ReverseCulling = IsLocalToWorldDeterminantNegative();

	MeshBatch.DepthPriorityGroup = SDPG_World;
	MeshBatch.bCanApplyViewModeOverrides = false;

	MeshBatch.Elements.Empty(1);
	FMeshBatchElement BatchElement;
	BatchElement.IndexBuffer = &MeshIndexBuffersSection->IndexBuffer;
	BatchElement.FirstIndex = 0;
	BatchElement.NumPrimitives = MeshIndexBuffersSection->IndexBuffer.Indices.Num() / 3;
	BatchElement.MinVertexIndex = 0;
	BatchElement.MaxVertexIndex = MeshVertexBuffersSection->VertexBuffers.PositionVertexBuffer.GetNumVertices() - 1;		
	MeshBatch.Elements.Add(BatchElement);
	
	PDI->DrawMesh(MeshBatch, FLT_MAX);
}
