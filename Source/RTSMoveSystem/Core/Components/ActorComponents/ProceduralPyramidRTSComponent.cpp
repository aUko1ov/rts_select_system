
#include "ProceduralPyramidRTSComponent.h"


UProceduralPyramidRTSComponent::UProceduralPyramidRTSComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	ProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedMesh"));

	if (GetOwner() && GetOwner()->GetRootComponent())
		ProceduralMesh->SetupAttachment(GetOwner()->GetRootComponent());
}


void UProceduralPyramidRTSComponent::CreatePyramidMesh()
{
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UVs;
	TArray<FProcMeshTangent> Tangents;

	ProceduralMesh->SetRelativeLocation(FVector(0, 0, OwnerRelativeHeight));

	GenerateMeshVertices(Vertices, Triangles, Normals, UVs, Tangents);

	ProceduralMesh->CreateMeshSection(0, Vertices, Triangles, Normals, UVs, TArray<FColor>(), Tangents, true);

	if (PyramidMaterial)
	{
		ProceduralMesh->SetMaterial(0, PyramidMaterial);
	}

}

void UProceduralPyramidRTSComponent::DestroyPyramidMesh()
{
	if(ProceduralMesh)
	{
		ProceduralMesh->ClearMeshSection(0);
	}
}

void UProceduralPyramidRTSComponent::GenerateMeshVertices(TArray<FVector>& Vertices, TArray<int32>& Triangles, TArray<FVector>& Normals, TArray<FVector2D>& UVs, TArray<FProcMeshTangent>& Tangents)
{
	// Define vertices
	Vertices.Add(FVector(0, 0, Height)); // Top vertex
	Vertices.Add(FVector(-Width / 2, -Width / 2, 0)); // Base - left front
	Vertices.Add(FVector(Width / 2, -Width / 2, 0)); // Base - right front
	Vertices.Add(FVector(Width / 2, Width / 2, 0)); // Base - right back
	Vertices.Add(FVector(-Width / 2, Width / 2, 0)); // Base - left back

	// Define triangles (order is important for normals)
	// Side 1
	Triangles.Append({0, 1, 2});
	// Side 2
	Triangles.Append({0, 2, 3});
	// Side 3
	Triangles.Append({0, 3, 4});
	// Side 4
	Triangles.Append({0, 4, 1});
	// Base
	Triangles.Append({1, 3, 2});
	Triangles.Append({1, 4, 3});
	
	for (int32 i = 0; i < Vertices.Num(); i++)
	{
		Normals.Add(FVector::UpVector);
	}
	
	UVs.Add(FVector2D(0.5f, 1.0f));
	UVs.Add(FVector2D(0.0f, 0.0f));
	UVs.Add(FVector2D(1.0f, 0.0f));
	UVs.Add(FVector2D(1.0f, 1.0f));
	UVs.Add(FVector2D(0.0f, 1.0f));

	for (int32 i = 0; i < Vertices.Num(); i++)
	{
		Tangents.Add(FProcMeshTangent(0, 1, 0));
	}
}