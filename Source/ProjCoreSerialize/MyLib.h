#pragma once

#include "MyLib.generated.h"

USTRUCT(BlueprintType) 
struct FMyStruct
{
	GENERATED_BODY()

	// ~Props Begin
	UPROPERTY()
	int32 V = 111;

	UPROPERTY()
	FString S = FString(TEXT("111"));
	// ~Props End

	// ~Non_Props Begin
	int32 V_NoProp = 111;
	FString S_NoProp = FString(TEXT("111"));
	// ~Non_Props End

	// ~Transient Props Begin
	UPROPERTY(Transient)
	int32 V_Transient = 111;

	UPROPERTY(Transient)
	FString S_Transient = FString(TEXT("111"));
	// ~Transient Props End

	// ~SaveGame Props Begin
	UPROPERTY(SaveGame)
	int32 V_SaveGame = 111;

	UPROPERTY(SaveGame)
	FString S_SaveGame = FString(TEXT("111"));
	// ~SaveGame Props End
	
	void InitForTest();

	// ~Serialization Begin
	void Serialize(FArchive& Ar);
	void PostSerialize();
	// ~Serialization End
};

// Declaring own serialization operator
template<class Ar>
Ar& operator<<(Ar& A, FMyStruct& S)
{
	A << S.V << S.S;
	A << S.V_NoProp << S.S_NoProp;
	A << S.V_Transient << S.S_Transient;
	A << S.V_SaveGame << S.S_SaveGame;
	return A;
}

/*
template<>
class TStructOpsTypeTraits<FMyStruct> : public TStructOpsTypeTraitsBase2<FMyStruct>
{
	enum
	{
		WithSerialize = true,
		WithPostSerialize = true
	};
};
*/



/**
* Structs WITHOUT USTRUCT
*/
struct FMyPlainStruct
{
	int32 V;
	FString S;
};

// Declaring own serialization operator
template<class Ar>
Ar& operator<<(Ar& A, FMyPlainStruct& S)
{
	A << S.V << S.S;
	return A;
}

/**
* Struct with provided traits
*/
USTRUCT()
struct FMyTraitedStruct
{	
	GENERATED_BODY()

	// ~Props Begin
	UPROPERTY()
	int32 V;

	UPROPERTY()
	FString S;
	// ~Props End

	// @TODO
	//void Serialize(FArchive& Ar);
	//void PostSerialize(FArchive& Ar);
};
template<>
struct TStructOpsTypeTraits<FMyTraitedStruct> : public TStructOpsTypeTraitsBase2<FMyTraitedStruct>
{
	enum
	{
		WithNetSerializer = false,
		WithSerializer = false, // TODO
		WithPostSerialize = false // TODO
	};
};

USTRUCT(BlueprintType, Atomic)
struct FMyAtomicStruct
{
	GENERATED_BODY()

	// ~Props Begin
	UPROPERTY()
	int32 V;

	UPROPERTY()
	FString S;
	// ~Props End
};

UCLASS()
class UMyLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// ~FMyStruct Begin
	UFUNCTION(BlueprintCallable, Category = Helper)
	static void LogStruct(const FMyStruct& S);
	// ~FMyStruct End
};