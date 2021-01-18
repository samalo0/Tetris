#include "FShape.h"

FBlock FShape::GetBlock(int32 row, int32 column) const
{
	const int32 index = row * NumberOfColumns + column;
	if (index >= 0 && index < Blocks.Num())
	{
		return Blocks[index];
	}

	return FBlock(0, 0, 0);
}

void FShape::Rotate(int32 rotationMult90)
{
	for (auto repeat = 0; repeat < rotationMult90; repeat++)
	{
		TArray<FBlock> rotated;
		const int32 rotatedNumberOfRows = NumberOfColumns;
		const int32 rotatedNumberOfColumns = NumberOfRows;

		for (auto column = NumberOfColumns - 1; column >= 0; column--)
		{
			for (auto row = 0; row < NumberOfRows; row++)
			{
				rotated.Add(GetBlock(row, column));
			}
		}

		NumberOfRows = rotatedNumberOfRows;
		NumberOfColumns = rotatedNumberOfColumns;
		Blocks.Empty();
		Blocks = rotated;
	}
}