#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TetrisGameModeBase.generated.h"

/** Tetris game mode.
 * 
 */
UCLASS()
class TETRIS_API ATetrisGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	// Adds to the score based on the number of rows removed at once.
	void AddToScoreAndLines(int32 count);

	virtual void BeginPlay() override;

	// Returns the amount of seconds to wait before triggering automatic downward movement.
	float GetAutoMoveSeconds() const;

	// Called when the stack has gone over the top and the game needs to end.
	UFUNCTION(BlueprintImplementableEvent)
	void TriggerGameOver();

	// Called when the number of lines are sufficient to go to the next level.
	UFUNCTION(BlueprintImplementableEvent)
	void TriggerNextLevel();

	// The fastest downward movement can be triggered.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TetrisGameMode")
	float AutomaticDownMoveMinimumSeconds = 0.1f;

	// How fast downward movement is triggered at level 1.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TetrisGameMode")
	float AutomaticDownMoveSeconds = 0.8f;

	// Each level makes the downward movement happen more quickly.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TetrisGameMode")
	float AutomaticDownMoveRemovedPerLevelSeconds = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TetrisGameMode")
	FText ImportantText;

	// Current level.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TetrisGameMode")
	int32 Level = 0;

	// The number of completed lines.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TetrisGameMode")
	int32 Lines = 0;

	// The number of lines to complete to go to the next level.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TetrisGameMode")
	int32 LinesToComplete = 10;

	// Current score in the game.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "TetrisGameMode")
	int32 Score = 0;

};
