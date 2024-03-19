
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProceduralMeshComponent.h"
#include "ProceduralPyramidRTSComponent.generated.h"


class UProceduralMeshComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RTSMOVESYSTEM_API UProceduralPyramidRTSComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UProceduralPyramidRTSComponent();
	
	void CreatePyramidMesh();

	void DestroyPyramidMesh();

	FORCEINLINE UProceduralMeshComponent* GetProceduralMesh() const {return ProceduralMesh;}

protected:
	UPROPERTY(VisibleAnywhere)
	UProceduralMeshComponent* ProceduralMesh;

	UPROPERTY(EditAnywhere, Category = "Pyramid Properties")
	UMaterialInterface* PyramidMaterial;

	UPROPERTY(EditAnywhere, Category = "Pyramid Properties")
	float OwnerRelativeHeight = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Pyramid Properties")
	float Width = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Pyramid Properties")
	float Height = 200.0f;

	void GenerateMeshVertices(TArray<FVector>& Vertices,
	                          TArray<int32>& Triangles, TArray<FVector>& Normals, TArray<FVector2D>& UVs,
	                          TArray<FProcMeshTangent>& Tangents);
	
};
