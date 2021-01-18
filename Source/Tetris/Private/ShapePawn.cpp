#include "ShapePawn.h"

#include "Components/InputComponent.h"
#include "Classes/Materials/Material.h"
#include "StackActor.h"
#include "Components/StaticMeshComponent.h"
#include "TetrisGameStateBase.h"
#include "TetrisGameModeBase.h"

AShapePawn::AShapePawn()
{
	PrimaryActorTick.bCanEverTick = true;

	ZeroLocatorSceneComponent = CreateDefaultSubobject<USceneComponent>("ZeroLocatorSceneComponent");
	ZeroLocatorSceneComponent->SetMobility(EComponentMobility::Static);
	SetRootComponent(ZeroLocatorSceneComponent);

	ShapeSceneComponent = CreateDefaultSubobject<USceneComponent>("ShapeSceneComponent");
	ShapeSceneComponent->SetRelativeLocation(FVector(LocationColumn * BLOCK_WIDTH_UNITS, 0.0f, LocationRow * BLOCK_WIDTH_UNITS * -1.0f));
	ShapeSceneComponent->SetupAttachment(ZeroLocatorSceneComponent);

	TArray<FBlock> shapeBlocks;
	shapeBlocks.Add(FBlock(0, 0, 0));
	shapeBlocks.Add(FBlock(1, 0, 0));
	shapeBlocks.Add(FBlock(0, 0, 0));
	shapeBlocks.Add(FBlock(0, 0, 0));

	shapeBlocks.Add(FBlock(0, 0, 0));
	shapeBlocks.Add(FBlock(2, 0, 0));
	shapeBlocks.Add(FBlock(0, 0, 0));
	shapeBlocks.Add(FBlock(0, 0, 0));

	shapeBlocks.Add(FBlock(0, 0, 0));
	shapeBlocks.Add(FBlock(2, 0, 0));
	shapeBlocks.Add(FBlock(0, 0, 0));
	shapeBlocks.Add(FBlock(0, 0, 0));

	shapeBlocks.Add(FBlock(0, 0, 0));
	shapeBlocks.Add(FBlock(1, 2, 0));
	shapeBlocks.Add(FBlock(0, 0, 0));
	shapeBlocks.Add(FBlock(0, 0, 0));

	const FShape shape1 = FShape(4, 4, shapeBlocks);
	ShapeArray.Add(shape1);

	shapeBlocks.Empty();
	
	shapeBlocks.Add(FBlock(1, 0, 1));
	shapeBlocks.Add(FBlock(0, 0, 0));
	shapeBlocks.Add(FBlock(0, 0, 0));

	shapeBlocks.Add(FBlock(3, 0, 1));
	shapeBlocks.Add(FBlock(1, 3, 1));
	shapeBlocks.Add(FBlock(0, 0, 0));

	shapeBlocks.Add(FBlock(1, 2, 1));
	shapeBlocks.Add(FBlock(0, 0, 0));
	shapeBlocks.Add(FBlock(0, 0, 0));
	
	const FShape shape2 = FShape(3, 3, shapeBlocks);
	ShapeArray.Add(shape2);

	shapeBlocks.Empty();
	shapeBlocks.Add(FBlock(1, 0, 4));
	shapeBlocks.Add(FBlock(0, 0, 0));
	shapeBlocks.Add(FBlock(0, 0, 0));

	shapeBlocks.Add(FBlock(2, 0, 4));
	shapeBlocks.Add(FBlock(0, 0, 0));
	shapeBlocks.Add(FBlock(0, 0, 0));

	shapeBlocks.Add(FBlock(4, 0, 4));
	shapeBlocks.Add(FBlock(1, 3, 4));
	shapeBlocks.Add(FBlock(0, 0, 0));

	const FShape shape3 = FShape(3, 3, shapeBlocks);
	ShapeArray.Add(shape3);

	shapeBlocks.Empty();

	shapeBlocks.Add(FBlock(0, 0, 0));
	shapeBlocks.Add(FBlock(0, 0, 0));
	shapeBlocks.Add(FBlock(1, 0, 3));

	shapeBlocks.Add(FBlock(0, 0, 0));
	shapeBlocks.Add(FBlock(0, 0, 0));
	shapeBlocks.Add(FBlock(2, 0, 3));

	shapeBlocks.Add(FBlock(0, 0, 0));
	shapeBlocks.Add(FBlock(1, 1, 3));
	shapeBlocks.Add(FBlock(4, 1, 3));

	const FShape shape4 = FShape(3, 3, shapeBlocks);
	ShapeArray.Add(shape4);

	shapeBlocks.Empty();
	shapeBlocks.Add(FBlock(1, 0, 5));
	shapeBlocks.Add(FBlock(0, 0, 0));
	shapeBlocks.Add(FBlock(0, 0, 0));

	shapeBlocks.Add(FBlock(4, 0, 5));
	shapeBlocks.Add(FBlock(4, 2, 5));
	shapeBlocks.Add(FBlock(0, 0, 0));

	shapeBlocks.Add(FBlock(0, 0, 0));
	shapeBlocks.Add(FBlock(1, 2, 5));
	shapeBlocks.Add(FBlock(0, 0, 0));

	const FShape shape5 = FShape(3, 3, shapeBlocks);
	ShapeArray.Add(shape5);

	shapeBlocks.Empty();

	shapeBlocks.Add(FBlock(0, 0, 0));
	shapeBlocks.Add(FBlock(1, 0, 6));
	shapeBlocks.Add(FBlock(0, 0, 0));

	shapeBlocks.Add(FBlock(4, 3, 6));
	shapeBlocks.Add(FBlock(4, 1, 6));
	shapeBlocks.Add(FBlock(0, 0, 0));

	shapeBlocks.Add(FBlock(1, 2, 6));
	shapeBlocks.Add(FBlock(0, 0, 0));
	shapeBlocks.Add(FBlock(0, 0, 0));

	const FShape shape6 = FShape(3, 3, shapeBlocks);
	ShapeArray.Add(shape6);

	shapeBlocks.Empty();
	shapeBlocks.Add(FBlock(4, 3, 2));
	shapeBlocks.Add(FBlock(4, 2, 2));
	shapeBlocks.Add(FBlock(4, 0, 2));
	shapeBlocks.Add(FBlock(4, 1, 2));
	const FShape shape7 = FShape(2, 2, shapeBlocks);
	ShapeArray.Add(shape7);

	ShapeArrayIndex = rand() % ShapeArray.Num();
	NextShapeArrayIndex = rand() % ShapeArray.Num();
}

void AShapePawn::BeginPlay()
{
	Super::BeginPlay();

	ATetrisGameModeBase* gameMode = Cast<ATetrisGameModeBase>(GetWorld()->GetAuthGameMode());
	if (gameMode != nullptr)
	{
		AutoMoveSeconds = gameMode->GetAutoMoveSeconds();
	}

	ConstructStaticMeshComponents();
}

void AShapePawn::ConstructStaticMeshComponents()
{
	const ATetrisGameStateBase* gameState = Cast<ATetrisGameStateBase>(GetWorld()->GetGameState());
	if (gameState == nullptr) return;

	// delete old components static mesh components for the shape
	for (auto component = StaticMeshComponentArray.CreateIterator(); component; component++)
	{
		if (*component != nullptr) (*component)->DestroyComponent();
	}
	StaticMeshComponentArray.Empty();

	// delete old components static mesh components for the next shape
	for (auto component = NextStaticMeshComponentArray.CreateIterator(); component; component++)
	{
		if (*component != nullptr) (*component)->DestroyComponent();
	}
	NextStaticMeshComponentArray.Empty();

	// update registration to remove them
	RegisterAllComponents();

	// get the shape and rotate it
	ActiveShape = ShapeArray[ShapeArrayIndex];
	ActiveShape.Rotate(Rotation90Mult);

	// Calculate offsets that center the shape
	const float vertOffset = BLOCK_WIDTH_UNITS / 2.0f * -1;
	const float horizOffset = BLOCK_WIDTH_UNITS / 2.0f;

	// create active shape
	uint32 index = 1;
	for (auto row = 0; row < ActiveShape.NumberOfRows; row++)
	{
		for (auto column = 0; column < ActiveShape.NumberOfColumns; column++)
		{
			UStaticMeshComponent* newComponent = NewObject<UStaticMeshComponent>(this, FName(*FString::Printf(TEXT("Block%d"), index++)));
			if (newComponent != nullptr)
			{
				StaticMeshComponentArray.Add(newComponent);
				newComponent->CreationMethod = EComponentCreationMethod::UserConstructionScript;
				
				newComponent->SetRelativeLocation(FVector(column * BLOCK_WIDTH_UNITS + horizOffset, 0.0f, row * BLOCK_WIDTH_UNITS * -1.0f + vertOffset));
				newComponent->AttachToComponent(ShapeSceneComponent, FAttachmentTransformRules::KeepRelativeTransform);

				newComponent->RegisterComponent();

				const FBlock block = ActiveShape.GetBlock(row, column);
				if (block.MeshIndex > 0 && block.MeshIndex <= gameState->StaticMeshArray.Num())
				{
					newComponent->SetStaticMesh(gameState->StaticMeshArray[block.MeshIndex - 1]);
					if (block.MaterialIndex < gameState->MaterialArray.Num())
					{
						newComponent->SetMaterial(0, gameState->MaterialArray[block.MaterialIndex]);
					}
				}
				newComponent->SetRelativeRotation(FRotator((block.RotationMult90Deg + Rotation90Mult) * 90.0f, 0.0f, 0.0f));
			}
		}
	}

	// create next shape
	const int32 nextHorizOffset = BLOCK_WIDTH_UNITS * -5;
	const int32 nextVertOffset = BLOCK_WIDTH_UNITS * -3;
	const FShape NextShape = ShapeArray[NextShapeArrayIndex];
	index = 1;
	for (auto row = 0; row < NextShape.NumberOfRows; row++)
	{
		for (auto column = 0; column < NextShape.NumberOfColumns; column++)
		{
			UStaticMeshComponent* newComponent = NewObject<UStaticMeshComponent>(this, FName(*FString::Printf(TEXT("NextBlock%d"), index++)));
			if (newComponent != nullptr)
			{
				NextStaticMeshComponentArray.Add(newComponent);
				newComponent->CreationMethod = EComponentCreationMethod::UserConstructionScript;

				newComponent->SetRelativeLocation(FVector(column * BLOCK_WIDTH_UNITS + nextHorizOffset, 0.0f, row * BLOCK_WIDTH_UNITS * -1.0f + nextVertOffset));
				newComponent->AttachToComponent(ZeroLocatorSceneComponent, FAttachmentTransformRules::KeepRelativeTransform);

				newComponent->RegisterComponent();

				const FBlock block = NextShape.GetBlock(row, column);
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

void AShapePawn::MoveDown()
{
	if (StackActorInstance->CanFit(ActiveShape, LocationRow + 1, DesiredColumn))
	{
		DesiredRow++;
	}
	else
	{
		// check for game over
		if (LocationRow < 0)
		{
			ATetrisGameModeBase* gameMode = Cast<ATetrisGameModeBase>(GetWorld()->GetAuthGameMode());
			if (gameMode != nullptr)
			{
				// end game
				DisableInput(GetWorld()->GetFirstPlayerController());
				SetActorTickEnabled(false);
				gameMode->TriggerGameOver();
				return;
			}
		}

		// Place shape in the stack and rebuild stack
		StackActorInstance->SetShape(ActiveShape, DesiredRow, DesiredColumn, Rotation90Mult);
		StackActorInstance->ConstructStaticMeshComponentArray();

		// Create the next shape
		ShapeArrayIndex = NextShapeArrayIndex;
		NextShapeArrayIndex = rand() % ShapeArray.Num();
		Rotation90Mult = 0;
		ConstructStaticMeshComponents();

		// reset location
		LocationRow = START_ROW;
		LocationColumn = START_COLUMN;
		DesiredRow = START_ROW;
		DesiredColumn = START_COLUMN;
		ShapeSceneComponent->SetRelativeLocation(FVector(LocationColumn * BLOCK_WIDTH_UNITS, 0.0f, LocationRow * BLOCK_WIDTH_UNITS * -1.0f));

		// force pressing again for the next shape
		bIsDownPressed = false;
		bIsLeftPressed = false;
		bIsRightPressed = false;

		// trigger a collapse of full lines
		TArray<int32> rowsToRemove = StackActorInstance->FindRowsToRemove();
		if (rowsToRemove.Num() > 0)
		{
			// if a collapse needs to happen, disable tick and input while waiting
			DisableInput(GetWorld()->GetFirstPlayerController());
			SetActorTickEnabled(false);

			// trigger animation of the removable rows
			StackActorInstance->AnimateRowRemoval(rowsToRemove);
		}
	}
}

void AShapePawn::MoveDownPressed()
{
	bIsDownPressed = true;
}

void AShapePawn::MoveDownReleased() 
{
	bIsDownPressed = false;
}

void AShapePawn::MoveLeft()
{
	//if (StackActorInstance->CanFit(ActiveShape, LocationRow, LocationColumn - 1))
	if (StackActorInstance->CanFit(ActiveShape, DesiredRow, LocationColumn - 1))
	{
		DesiredColumn--;
	}
}

void AShapePawn::MoveLeftPressed()
{
	bIsLeftPressed = true;
}

void AShapePawn::MoveLeftReleased()
{
	bIsLeftPressed = false;
}

void AShapePawn::MoveRight()
{
	if (StackActorInstance->CanFit(ActiveShape, DesiredRow, LocationColumn + 1))
	{
		DesiredColumn++;
	}
}

void AShapePawn::MoveRightPressed()
{
	bIsRightPressed = true;
}

void AShapePawn::MoveRightReleased()
{
	bIsRightPressed = false;
}

void AShapePawn::Rotate()
{
	// check if it can fit once it rotates
	FShape shapeRotated = ShapeArray[ShapeArrayIndex];
	shapeRotated.Rotate(Rotation90Mult + 1);
	
	if (StackActorInstance->CanFit(shapeRotated, DesiredRow, DesiredColumn))
	{
		Rotation90Mult++;
		if (Rotation90Mult >= 4) Rotation90Mult = 0;

		ConstructStaticMeshComponents();
	}
}

void AShapePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (StackActorInstance != nullptr)
	{
		InputComponent->BindAction("MoveDown", EInputEvent::IE_Pressed, this, &AShapePawn::MoveDownPressed);
		InputComponent->BindAction("MoveDown", EInputEvent::IE_Released, this, &AShapePawn::MoveDownReleased);

		InputComponent->BindAction("MoveLeft", EInputEvent::IE_Pressed, this, &AShapePawn::MoveLeftPressed);
		InputComponent->BindAction("MoveLeft", EInputEvent::IE_Released, this, &AShapePawn::MoveLeftReleased);

		InputComponent->BindAction("MoveRight", EInputEvent::IE_Pressed, this, &AShapePawn::MoveRightPressed);
		InputComponent->BindAction("MoveRight", EInputEvent::IE_Released, this, &AShapePawn::MoveRightReleased);

		InputComponent->BindAction("Rotate", EInputEvent::IE_Pressed, this, &AShapePawn::Rotate);
	}
}

void AShapePawn::Tick(float DeltaTime)
{
	static float autoMoveDownCounter = 0.0f;
	
	Super::Tick(DeltaTime);

	if (bIsLeftPressed && DesiredColumn == LocationColumn)
	{
		MoveLeft();
	}

	if (bIsRightPressed && DesiredColumn == LocationColumn)
	{
		MoveRight();
	}

	// check for having pressed left or right to move the shape
	if (DesiredColumn < LocationColumn)
	{
		ShapeSceneComponent->AddRelativeLocation(FVector(MoveHorizontalPixelsPerSecond * DeltaTime * -1.0f, 0.0f, 0.0f));
		const FVector currentLocation = ShapeSceneComponent->GetRelativeTransform().GetLocation();
		if (currentLocation.X <= DesiredColumn * BLOCK_WIDTH_UNITS)
		{
			LocationColumn = DesiredColumn;
			ShapeSceneComponent->SetRelativeLocation(FVector(LocationColumn * BLOCK_WIDTH_UNITS, 0.0f, currentLocation.Z));
		}
	}
	else if (DesiredColumn > LocationColumn)
	{
		ShapeSceneComponent->AddRelativeLocation(FVector(MoveHorizontalPixelsPerSecond * DeltaTime, 0.0f, 0.0f));
		const FVector currentLocation = ShapeSceneComponent->GetRelativeTransform().GetLocation();
		if (currentLocation.X >= DesiredColumn * BLOCK_WIDTH_UNITS)
		{
			LocationColumn = DesiredColumn;
			ShapeSceneComponent->SetRelativeLocation(FVector(LocationColumn * BLOCK_WIDTH_UNITS, 0.0f, currentLocation.Z));
		}
	}

	// check if down is being held
	if (bIsDownPressed)
	{
		autoMoveDownCounter = 0;
		
		// if not already moving down, trigger a move down
		if (DesiredRow == LocationRow)
		{
			MoveDown();
		}
	}
	else
	{
		autoMoveDownCounter += DeltaTime;
		if (autoMoveDownCounter >= AutoMoveSeconds)
		{
			autoMoveDownCounter = 0;
			MoveDown();
		}
	}
	
	// check for moving down
	if (DesiredRow > LocationRow)
	{
		ShapeSceneComponent->AddRelativeLocation(FVector(0.0f, 0.0f, MoveVerticalPixelsPerSecond * DeltaTime * -1.0f));
		const FVector currentLocation = ShapeSceneComponent->GetRelativeTransform().GetLocation();
		if (currentLocation.Z <= DesiredRow * BLOCK_WIDTH_UNITS * -1.0f)
		{
			LocationRow = DesiredRow;
			ShapeSceneComponent->SetRelativeLocation(FVector(currentLocation.X, 0.0f, LocationRow * BLOCK_WIDTH_UNITS * -1.0f));
		}
	}
}

