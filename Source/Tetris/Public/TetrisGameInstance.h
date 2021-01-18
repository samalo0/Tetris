#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "TetrisGameInstance.generated.h"

/** Tetris game instance, to store variables between levels.
 * 
 */
UCLASS()
class TETRIS_API UTetrisGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	// Current level.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TetrisGameInstance")
	int32 Level = 0;

	// Current score in the game, updated every level.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TetrisGameInstance")
	int32 Score = 0;

};
