#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TriggerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UTriggerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class UNREALENGINE_DEMOS_API ITriggerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void Trigger() {}
};
