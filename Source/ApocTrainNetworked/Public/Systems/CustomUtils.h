// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EngineUtils.h"

/**
 * 
 */
class APOCTRAINNETWORKED_API CustomUtils
{
public:

    template<typename T>
    static T* GetFirstActorOfClass(UWorld* World)
    {
        // Ensure the world is valid
        if (!World)
        {
            return nullptr;
        }

        // Iterate over all actors of the specified class
        for (TActorIterator<T> It(World); It; ++It)
        {
            // Return the first actor found
            return *It;
        }

        // Return nullptr if no actor was found
        return nullptr;
    }
    

	CustomUtils();
	~CustomUtils();
};

