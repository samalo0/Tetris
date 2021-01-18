#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FBlock.h"
#include "FShape.h"
#include "ShapePawn.h"
#include "StackActor.generated.h"

constexpr uint8 STACK_ROWS = 20;
constexpr uint8 STACK_COLUMNS = 10;
constexpr uint8 BLOCK_WIDTH_UNITS = 10;

UENUM(BlueprintType)
enum class EAnimationState : uint8
{
	AS_Idle UMETA(DisplayName="Idle"),
	AS_Highlight UMETA(DisplayName="Highlight"),
	AS_MovingDown UMETA(DisplayName="Song"),
};

/** The game board for stacking pieces, that keeps track of the state of
	the game.
*/
UCLASS()
class TETRIS_API AStackActor : public AActor
{

	GENERATED_BODY()
	
public:	

	AStackActor();

	// Animate removing the rows from the board.
	void AnimateRowRemoval(TArray<int32> rowsToRemove);

	// Check if a shape can fit into the block stack at the specified location.
	bool CanFit(FShape shape, int32 locationRow, int32 locationColumn);

	// Build the array of static mesh objects to visualize the block array.
	void ConstructStaticMeshComponentArray();

	// Check if there's a full line of blocks to remove; returns array of rows to remove.
	TArray<int32> FindRowsToRemove();

	// Place a shape into the stack
	void SetShape(FShape shape, int32 locationRow, int32 locationColumn, int32 rotation90Mult);

	virtual void Tick(float DeltaTime) override;

protected:

	virtual void BeginPlay() override;

	// Build the block TARRAY that holds the game state.
	void ConstructBlockArray();
	
	// Animation state used during tick.
	EAnimationState AnimationState = EAnimationState::AS_Idle;

	// Collapse the specified rows (move all above down into them).
	void CollapseRows(TArray<int32> rowsToRemove);

	// Get an element from the block array.
	FBlock GetBlock(uint32 row, uint32 column);

	// Set an element of the block array.
	void SetBlock(uint32 row, uint32 column, FBlock block);

	// Set the static mesh components to highlight material.
	void SetStaticMeshComponentHighlightMaterial(TArray<int32> rows);

	// Set the static mesh components to empty mesh.
	void SetStaticMeshComponentMeshToEmpty(TArray<int32> rows);

	// Array of blocks that represent the game board.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StackActor")
	TArray<FBlock> BlockArray;

	// Material used to highlight blocks that are about to be removed.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StackActor")
	UMaterialInterface* HighlightMaterial;

	// The amount of time to highlight blocks before removal.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StackActor")
	float HighlightSeconds = 0.3f;

	// The speed that the blocks travel when moving down during a collapse.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StackActor")
	float MoveSpeedPixelsPerSecond = 75.0f;

	// Array to store rows to remove over time using tick.
	TArray<int32> RowsToRemove;

	// Root component to use for locating the stack actor board.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StackActor")
	USceneComponent* SceneComponent;

	// The pawn instance in the level. Used to re-enable input/tick after a collapse.
	UPROPERTY(EditInstanceOnly, Category = "StackActor")
	AShapePawn* ShapePawnInstance;

	// Array of static mesh components that visualize the game board.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StackActor")
	TArray<UStaticMeshComponent*> StaticMeshComponentArray;
};
