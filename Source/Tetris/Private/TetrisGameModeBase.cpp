#include "TetrisGameModeBase.h"

#include "Engine/World.h"
#include "TetrisGameInstance.h"

void ATetrisGameModeBase::AddToScoreAndLines(int32 count)
{
	Score += count * count;
	Lines += count;
}

void ATetrisGameModeBase::BeginPlay()
{
	UTetrisGameInstance* gameInstance = Cast<UTetrisGameInstance>(GetWorld()->GetGameInstance());
	if (gameInstance != nullptr)
	{
		Level = gameInstance->Level;
		Score = gameInstance->Score;
		LinesToComplete = (Level + 1) * 10;
	}
}

float ATetrisGameModeBase::GetAutoMoveSeconds() const
{
	float moveSeconds = AutomaticDownMoveSeconds - Level * AutomaticDownMoveRemovedPerLevelSeconds;
	if (moveSeconds < AutomaticDownMoveMinimumSeconds) moveSeconds = AutomaticDownMoveMinimumSeconds;

	return moveSeconds;
}
