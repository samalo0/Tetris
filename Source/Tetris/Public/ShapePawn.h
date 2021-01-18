#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "FShape.h"
#include "ShapePawn.generated.h"

constexpr int32 START_ROW = -4;
constexpr int32 START_COLUMN = 4;

/** The moveable shape that is used to construct the stack.
*/
UCLASS()
class TETRIS_API AShapePawn : public APawn
{

	GENERATED_BODY()

public:

	AShapePawn();

	void ConstructStaticMeshComponents();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Tick(float DeltaTime) override;

protected:

	virtual void BeginPlay() override;

	void MoveDown();
	void MoveDownPressed();
	void MoveDownReleased();
	
	void MoveLeft();
	void MoveLeftPressed();
	void MoveLeftReleased();

	void MoveRight();
	void MoveRightPressed();
	void MoveRightReleased();

	void Rotate();

	// Actively moving shape; copied from ShapeArray and possibly rotated.
	FShape ActiveShape;

	// The amount of time before move down is triggered by itself.
	float AutoMoveSeconds = 1.0f;

	// Desired location for when moving smoothly during tick.
	int32 DesiredColumn = START_COLUMN;
	int32 DesiredRow = START_ROW;
	
	// boolean to store if the keys are being held
	bool bIsDownPressed = false;
	bool bIsLeftPressed = false;
	bool bIsRightPressed = false;

	// Current column location in the game stack.
	int32 LocationColumn = START_COLUMN;
	
	// Current row location in the game stack.
	int32 LocationRow = START_ROW;

	// The speed that the shape travels when moving horizontally.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShapePawn")
	float MoveHorizontalPixelsPerSecond = 100.0f;

	// The speed that the shape travels when moving vertically.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShapePawn")
	float MoveVerticalPixelsPerSecond = 150.0f;

	// Index of the next shape that will be created.
	int32 NextShapeArrayIndex;

	// Array of static mesh components used to visualize the next shape.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ShapePawn")
	TArray<UStaticMeshComponent*> NextStaticMeshComponentArray;

	// Current rotation of the active shape.
	int32 Rotation90Mult = 0;

	// Array of shapes to use during gameplay.
	TArray<FShape> ShapeArray;

	// Index of the currently used shape from the shape array.
	int32 ShapeArrayIndex = 0;

	// Scene component that locates the shape relative to the zero locator scene component.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ShapePawn")
	USceneComponent* ShapeSceneComponent;

	// The stack actor instance; used to add the pawn symbols to the stack and trigger collapse.
	UPROPERTY(EditInstanceOnly, Category = "ShapePawn")
	class AStackActor* StackActorInstance;
	   
	// Array of static mesh components used to visualize shapes.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ShapePawn")
	TArray<UStaticMeshComponent*> StaticMeshComponentArray;

	// Scene component that is placed at the 0 of the stack actor.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ShapePawn")
	USceneComponent* ZeroLocatorSceneComponent;

};
