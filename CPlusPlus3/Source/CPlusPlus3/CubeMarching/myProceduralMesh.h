#pragma once
#include "ProceduralMeshComponent.h"
class myProceduralMesh:public UProceduralMeshComponent
{
public:
	virtual void DrawStaticElements(FStaticPrimitiveDrawInterface* PDI);
	
};
