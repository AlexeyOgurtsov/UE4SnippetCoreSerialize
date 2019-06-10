#pragma once

#include "MyLib.h"
#include "MyObj.generated.h"

UCLASS()
class UMyObj : public UObject
{
	GENERATED_BODY()

public:
	static void LogStruct(const FMyStruct& S);

	UFUNCTION(BlueprintCallable, Category = Creation)
	static UMyObj* NewMyObj(UObject* InOwner);

	UFUNCTION(BlueprintCallable, Category = Test)
	void InitForTest();

	UFUNCTION(BlueprintCallable, Category = Log)
	void DumpMe() const;

	UFUNCTION(BlueprintCallable, Category = Log)
	static void DumpNullable(const UObject* Obj);

	UFUNCTION(BlueprintCallable, Category = Log)
	static void LogArray(const TArray<UMyObj*>& InArray);
	static void LogArray(const TArray<const UMyObj*>& InArray);
	static void LogArray(const TArray<UObject*>& InArray);
	static void LogArray(const TArray<const UObject*>& InArray);

	// ~Properties Begin
	UPROPERTY()
	int32 V = 111;

	UPROPERTY(Transient)
	int32 V_Transient = 111;

	UPROPERTY(SaveGame)
	int32 V_SaveGame = 111;

	int32 V_NoProp = 111;

	UPROPERTY()
	FMyStruct MyStruct;

	UPROPERTY(Transient)
	FMyStruct MyStruct_Transient;

	UPROPERTY(SaveGame)
	FMyStruct MyStruct_SaveGame;

	FMyStruct MyStruct_NoProp;
	// ~Properties End

	// ~ UObject Begin
	virtual void PostInitProperties() override;
	virtual void PostLoad() override;
	// ~ UObject End

	// ~ UObject Serialization Begin
	virtual void Serialize(FArchive& Ar) override;
	// Fails to compile:
	//virtual void Serialize(FStructuredArchive::FRecord& Record) override;
	// ~ UObject Serialization End
};