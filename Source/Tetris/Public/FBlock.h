#pragma once
#include "FBlock.generated.h"

/** Structure that holds a singular block data.
*/
USTRUCT(BlueprintType)
struct FBlock
{
	GENERATED_BODY()

	// Index into the static mesh array stored in the game state.
	UPROPERTY(EditAnywhere, Category = "Block")
	uint8 MeshIndex = 0;
	
	// Rotation of the static mesh.
	UPROPERTY(EditAnywhere, Category = "Block")
	uint8 RotationMult90Deg = 0;
	
	// Index into the material array stored in the game state.
	UPROPERTY(EditAnywhere, Category = "Block")
	uint8 MaterialIndex = 0;
	
	FBlock() {}

	FORCEINLINE FBlock(uint8 inShapeIndex, uint8 inRotationMult90Deg, uint8 inColorIndex) : MeshIndex(inShapeIndex), RotationMult90Deg(inRotationMult90Deg), MaterialIndex(inColorIndex) {}
};
