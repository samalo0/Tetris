#include "StackActor.h"

#include "Classes/Materials/Material.h"
#include "Components/StaticMeshComponent.h"
#include "TetrisGameModeBase.h"
#include "TetrisGameStateBase.h"

AStackActor::AStackActor()
{
	// enable tick but disable it unless needed for animation
	PrimaryActorTick.bCanEverTick = true;
	SetActorTickEnabled(false);

	SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	SetRootComponent(SceneComponent);

	ConstructBlockArray();
}

void AStackActor::AnimateRowRemoval(TArray<int32> rowsToRemove)
{
	// store the rows to remove
	RowsToRemove = rowsToRemove;
	
	// add to the score
	ATetrisGameModeBase* gameMode = Cast<ATetrisGameModeBase>(GetWorld()->GetAuthGameMode());
	if (gameMode != nullptr)
	{
		gameMode->AddToScoreAndLines(rowsToRemove.Num());
		
		// check for level completion
		if (gameMode->Lines >= gameMode->LinesToComplete)
		{
			gameMode->TriggerNextLevel();
			return;
		}
	}

	// Set the material to highlight those being removed
	SetStaticMeshComponentHighlightMaterial(rowsToRemove);

	// Remove them from the stack array for when they are regenerated
	CollapseRows(rowsToRemove);

	// start up highlight wait
	AnimationState = EAnimationState::AS_Highlight;
	SetActorTickEnabled(true);
}

void AStackActor::BeginPlay()
{
	Super::BeginPlay();

	ConstructStaticMeshComponentArray();
}

bool AStackActor::CanFit(FShape shape, int32 locationRow, int32 locationColumn)
{
	for (auto row = 0; row < shape.NumberOfRows; row++)
	{
		for (auto column = 0; column < shape.NumberOfColumns; column++)
		{
			const FBlock shapeBlock = shape.GetBlock(row, column);
			if (shapeBlock.MeshIndex == 0) 
			{
				// if no block in that location in the shape, continue
				continue;
			}
			else {
				// check if the shape is outside the stack grid (unless it's above, that's how the piece starts)
				if ((locationRow + row) >= STACK_ROWS) return false;
				if ((locationColumn + column) < 0) return false;
				if ((locationColumn + column) >= STACK_COLUMNS) return false;
				
				// get the block from the stack - anything there?
				const FBlock stackBlock = GetBlock(locationRow + row, locationColumn + column);
				if (stackBlock.MeshIndex == 0)
				{
					// no, check the rest of the blocks
					continue;
				}
				else {
					// yes, can't fit there
					return false;
				}
			}
		}
	}

	// nothing could not fit
	return true;
}

void AStackActor::CollapseRows(TArray<int32> rowsToRemove)
{
	for (auto rtrIndex = 0; rtrIndex < rowsToRemove.Num(); rtrIndex++)
	{
		// loop over rows from the row to remove back up
		for (auto row = rowsToRemove[rtrIndex]; row > 0; row--)
		{
			for (auto column = 0; column < STACK_COLUMNS; column++)
			{
				// and fill with the row above
				SetBlock(row, column, GetBlock(row - 1, column));
			}
		}

		// fill top row with empties
		for (auto column = 0; column < STACK_COLUMNS; column++)
		{
			SetBlock(0, column, FBlock(0, 0, 0));
		}
	}
}

void AStackActor::ConstructBlockArray()
{
	BlockArray.Empty();

	for (uint32 row = 0; row < STACK_ROWS; row++)
	{
		for (uint32 column = 0; column < STACK_COLUMNS; column++)
		{
			BlockArray.Add(FBlock(0, 0, 0));
		}
	}
}

void AStackActor::ConstructStaticMeshComponentArray()
{
	const ATetrisGameStateBase* gameState = Cast<ATetrisGameStateBase>(GetWorld()->GetGameState());
	if (gameState == nullptr) return;

	// delete old components
	for (auto component = StaticMeshComponentArray.CreateIterator(); component; component++)
	{
		if (*component != nullptr) (*component)->DestroyComponent();
	}
	StaticMeshComponentArray.Empty();

	// update registration to remove them
	RegisterAllComponents();
	
	// create new ones
	uint32 index = 1;
	for (uint32 row = 0; row < STACK_ROWS; row++)
	{
		for (uint32 column = 0; column < STACK_COLUMNS; column++)
		{
			UStaticMeshComponent* newComponent = NewObject<UStaticMeshComponent>(this, FName(*FString::Printf(TEXT("Block%d"), index++)));
			if (newComponent != nullptr)
			{
				StaticMeshComponentArray.Add(newComponent);
				newComponent->CreationMethod = EComponentCreationMethod::UserConstructionScript;
				newComponent->SetRelativeLocation(FVector(column * BLOCK_WIDTH_UNITS, 0.0f, row * BLOCK_WIDTH_UNITS * -1.0f));
				newComponent->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
				newComponent->RegisterComponent();

				const FBlock block = GetBlock(row, column);
				if (block.MeshIndex > 0 && block.MeshIndex <= gameState->StaticMeshArray.Num())
				{
					newComponent->SetStaticMesh(gameState->StaticMeshArray[block.MeshIndex - 1]);
					if (block.MaterialIndex < gameState->MaterialArray.Num())
					{
						newComponent->SetMaterial(0, gameState->MaterialArray[block.MaterialIndex]);
					}
				}
				newComponent->SetRelativeRotation(FRotator(block.RotationMult90Deg * 90.0f, 0.0f, 0.0f));
			}
		}
	}
}

TArray<int32> AStackActor::FindRowsToRemove()
{
	TArray<int32> rowsToRemove;

	for (auto row = 0; row < STACK_ROWS; row++)
	{
		// loop over the columns and see if all in a row are blocks
		int32 column;
		for (column = 0; column < STACK_COLUMNS; column++)
		{
			if (GetBlock(row, column).MeshIndex == 0) break;
		}
		if (column == STACK_COLUMNS)
		{
			// the entire row has blocks
			rowsToRemove.Add(row);
		}
	}

	return rowsToRemove;
}

FBlock AStackActor::GetBlock(uint32 row, uint32 column)
{
	const int32 index = row * STACK_COLUMNS + column;

	if (index >= 0 && index < BlockArray.Num()) return BlockArray[index];

	return FBlock(0, 0, 0);
}

void AStackActor::SetBlock(uint32 row, uint32 column, FBlock block)
{
	const int32 index = row * STACK_COLUMNS + column;

	if (index < BlockArray.Num()) BlockArray[index] = block;
}

void AStackActor::SetShape(FShape shape, int32 locationRow, int32 locationColumn, int32 rotation90Mult)
{
	for (auto row = 0; row < shape.NumberOfRows; row++)
	{
		for (auto column = 0; column < shape.NumberOfColumns; column++)
		{
			FBlock block = shape.GetBlock(row, column);
			if (block.MeshIndex > 0)
			{
				block.RotationMult90Deg += rotation90Mult;
				SetBlock(locationRow + row, locationColumn + column, block);
			}
		}
	}
}

void AStackActor::SetStaticMeshComponentHighlightMaterial(TArray<int32> rows)
{
	// Change the material on the static mesh components to highlight
	for (auto index = 0; index < rows.Num(); index++)
	{
		for (auto column = 0; column < STACK_COLUMNS; column++)
		{
			const int32 meshIndex = rows[index] * STACK_COLUMNS + column;
			if (meshIndex >= 0 && meshIndex < StaticMeshComponentArray.Num())
			{
				StaticMeshComponentArray[meshIndex]->SetMaterial(0, HighlightMaterial);
			}
		}
	}
}

void AStackActor::SetStaticMeshComponentMeshToEmpty(TArray<int32> rows)
{
	// Change the material on the static mesh components to highlight
	for (auto index = 0; index < rows.Num(); index++)
	{
		for (auto column = 0; column < STACK_COLUMNS; column++)
		{
			const int32 meshIndex = rows[index] * STACK_COLUMNS + column;
			if (meshIndex >= 0 && meshIndex < StaticMeshComponentArray.Num())
			{
				StaticMeshComponentArray[meshIndex]->SetStaticMesh(nullptr);
			}
		}
	}
}

void AStackActor::Tick(float DeltaTime)
{
	// timer for how long to display highlight material
	static float highlightCounter = 0.0f;
	
	// counter for how many times rows have been moved down
	static int32 iterations;

	Super::Tick(DeltaTime);

	switch (AnimationState)
	{
	case EAnimationState::AS_Idle:
		break;
	case EAnimationState::AS_Highlight:
		// wait for hightlight time to end
		highlightCounter += DeltaTime;

		if (highlightCounter >= HighlightSeconds)
		{
			// clear mesh and trigger rows to move down
			highlightCounter = 0.0f;
			iterations = 1;
			AnimationState = EAnimationState::AS_MovingDown;
			SetStaticMeshComponentMeshToEmpty(RowsToRemove);
		}
		break;
	case EAnimationState::AS_MovingDown:
		if (RowsToRemove.Num() > 0)
		{
			bool finishedWithRow = false;
			
			// find all static mesh components above the row to remove, and move them down
			for (auto row = 0; row < RowsToRemove[0]; row++)
			{
				for (auto column = 0; column < STACK_COLUMNS; column++)
				{
					const int32 index = row * STACK_COLUMNS + column;
					if (index >= 0 && index < StaticMeshComponentArray.Num())
					{
						StaticMeshComponentArray[index]->AddRelativeLocation(FVector(0.0f, 0.0f, MoveSpeedPixelsPerSecond * DeltaTime * -1));
						
						const FVector currentLocation = StaticMeshComponentArray[index]->GetRelativeTransform().GetLocation();
						if (currentLocation.Z <= (row + iterations) * BLOCK_WIDTH_UNITS * -1)
						{
							// set them into position perfectly
							StaticMeshComponentArray[index]->SetRelativeLocation(FVector(currentLocation.X, currentLocation.Y, (row + iterations) * BLOCK_WIDTH_UNITS * -1));
							finishedWithRow = true;
						}
					}
				}
			}

			if (finishedWithRow)
			{
				RowsToRemove.RemoveAt(0);
				iterations++;
			}
		}
		else
		{
			// Reconstruct static mesh components
			ConstructStaticMeshComponentArray();

			// disable tick for the stack and set back to idle
			AnimationState = EAnimationState::AS_Idle;
			SetActorTickEnabled(false);

			// re-enable input/movement on the pawn
			if (ShapePawnInstance != nullptr)
			{
				ShapePawnInstance->EnableInput(GetWorld()->GetFirstPlayerController());
				ShapePawnInstance->SetActorTickEnabled(true);
			}
		}
		break;
	}
}

