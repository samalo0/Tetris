#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "TetrisGameStateBase.generated.h"

/** Stores the array of static mesh and materials to be used to construct the
 *  game board and pawn.
 */
UCLASS()
class TETRIS_API ATetrisGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:

	// Array of static mesh used to create game board and pawn.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameState")
	TArray<UStaticMesh*> StaticMeshArray;

	// Array of static mesh used to create game board and pawn.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameState")
	TArray<UMaterialInterface*> MaterialArray;

};
