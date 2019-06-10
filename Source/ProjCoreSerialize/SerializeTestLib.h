#pragma once

#include "MyLib.h"
#include "SerializeTestLib.generated.h"

UCLASS()
class USerializeTestLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// ~Tests Begin
	UFUNCTION(BlueprintCallable, Category = SerializeTest, Meta=(WorldContext=WorldContextObject))
	static void DoAllTests(UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = SerializeTest, Meta=(WorldContext=WorldContextObject))
	static void QuickTest(UObject* WorldContextObject);

	// ~Archive Begin
	UFUNCTION(BlueprintCallable, Category = SerializeTest, Meta=(WorldContext=WorldContextObject))
	static void ArchiveTest(UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = SerializeTest, Meta=(WorldContext=WorldContextObject))
	static void ArchiveTest_ObjectAndNameAsStringProxyArchive(UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = SerializeTest, Meta=(WorldContext=WorldContextObject))
	static void ArchiveTest_BufferArchive_MemoryReader(UObject* WorldContextObject);	
	// ~Archive End

	UFUNCTION(BlueprintCallable, Category = SerializeTest, Meta=(WorldContext=WorldContextObject))
	static void SimpleTypeTests(UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = SerializeTest, Meta=(WorldContext=WorldContextObject))
	static void ObjectTests(UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = SerializeTest, Meta=(WorldContext=WorldContextObject))
	static void ContainerTests(UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = SerializeTest, Meta=(WorldContext=WorldContextObject))
	static void StructTests(UObject* WorldContextObject);
	// ~Tests end

	// ~Helpers Begin
	UFUNCTION(BlueprintCallable, Category = Helper)
	static void LogStruct(const FMyStruct& S);

	UFUNCTION(BlueprintCallable, Category = Helper)
	static void LogStructArray(const TArray<FMyStruct>& A);
	// ~Helpers End
};