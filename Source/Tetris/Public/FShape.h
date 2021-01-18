#pragma once

#include "FBlock.h"
#include "FShape.generated.h"

/** Structure that holds all information required for a shape.
*/
USTRUCT(BlueprintType)
struct FShape
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	int32 NumberOfRows = 0;
	
	UPROPERTY(EditAnywhere)
	int32 NumberOfColumns = 0;
	
	UPROPERTY(EditAnywhere)
	TArray<FBlock> Blocks;

	FShape() {}

	FORCEINLINE FShape(int32 inRows, int32 inCols, TArray<FBlock> inBlocks) : NumberOfRows(inRows), NumberOfColumns(inCols), Blocks(inBlocks) {}

	FBlock GetBlock(int32 row, int32 column) const;

	void Rotate(int32 rotationMult90);
};
