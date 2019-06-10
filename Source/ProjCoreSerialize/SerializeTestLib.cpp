#include "SerializeTestLib.h"
#include "Util/LogUtilLib.h"
#include "MyObj.h"

#include "Serialization/BufferArchive.h"
#include "Serialization/MemoryReader.h"

#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

namespace 
{
	class FImpl
	{
	public:	
		template<class T>
		static void DumpArray(const TCHAR* InName, const TArray<T>& A)
		{
			UE_LOG(MyLog, Log, TEXT("Dumping array \"%s\"..."), InName);


			for(int32 i = 0; i < A.Num(); i++)
			{	
				TArray<FStringFormatArg> FormatArgs;
				FormatArgs.Add(i);
				FormatArgs.Add(A[i]);
				FString const S = FString::Format(TEXT("[{0}]={1}"), FormatArgs);
				UE_LOG(MyLog, Log, TEXT("%s"), *S);
			}
			UE_LOG(MyLog, Log, TEXT("Dumping array DONE"));
		}
	
		static TArray<FMyStruct> CreateMyStructArray(int32 Count)	
		{
			TArray<FMyStruct> Res;
			for(int32 i = 0; i < Count; i++)
			{
				TArray<FStringFormatArg> Args;
				Args.Add(Count);
				FString S = FString::Format(TEXT("Str_{0}"), Args);
				Res.Add(CreateMyStruct(i, S));
			}
			return Res;
		}

		static FMyStruct CreateMyStruct(int32 V, const FString& S)
		{
			FMyStruct Res;

			{
				Res.V = V;
				Res.S = S;
			}

			{
				Res.V_NoProp = V;
				Res.S_NoProp = S;
			}

			{
				Res.V_Transient = V;
				Res.S_Transient = S;
			}

			{
				Res.V_SaveGame = V;
				Res.S_SaveGame = S;
			}

			return Res;
		}

		template<class Ar>
		static void SerializeMyStruct(Ar& A, FMyStruct& S)
		{	
			// As we defined own operator for serialization:
			A << S;

			/*
			{
				A << S.V;
				A << S.S;
			}

			{
				A << S.V_NoProp;
				A << S.S_NoProp;
			}

			{
				A << S.V_Transient;
				A << S.S_Transient;
			}

			{
				A << S.V_SaveGame;
				A << S.S_SaveGame;
			}
			*/
		}
	};
} // anonymous namespace;

void USerializeTestLib::DoAllTests(UObject* WorldContextObject)
{
	UE_LOG(MyLog, Log, TEXT("USerializeTestLib::DoAllTests..."));
	// @TODO
}

void USerializeTestLib::QuickTest(UObject* WorldContextObject)
{
	UE_LOG(MyLog, Log, TEXT("USerializeTestLib::QuickTest..."));
	// @TODO
}

void USerializeTestLib::ArchiveTest(UObject* WorldContextObject)
{
	UE_LOG(MyLog, Log, TEXT("USerializeTestLib::ArchiveTest..."));
	ArchiveTest_BufferArchive_MemoryReader(WorldContextObject);
	ArchiveTest_ObjectAndNameAsStringProxyArchive(WorldContextObject);
	UE_LOG(MyLog, Log, TEXT("USerializeTestLib::ArchiveTest DONE"));
}

namespace
{
	class FArchiveTest
	{
	public:
		static void DoAllTests(UObject* WorldContextObject, bool bInIsSaveGame, bool bInArNoDelta)	
		{
			UE_LOG
			(
				MyLog, Log, 
				TEXT("ArchiveTest: bSaveGame=%s, bArNoDelta=%s..."), 
				(bInIsSaveGame ? TEXT("YES") : TEXT("no")),
				(bInArNoDelta ? TEXT("YES") : TEXT("no"))
			);

			int32 V = 3;
			FMyStruct MyStruct = FImpl::CreateMyStruct(3, FString("Str_3"));
	
			FBufferArchive Buffer;	
			{
				UE_LOG(MyLog, Log, TEXT("Serializing..."));
				Buffer.ArIsSaveGame = bInIsSaveGame;
				Buffer.ArNoDelta = bInArNoDelta;

				Buffer << V;
				FImpl::SerializeMyStruct(Buffer, MyStruct);

				UE_LOG(MyLog, Log, TEXT("V=%d"), V);
				UE_LOG(MyLog, Log, TEXT("Serializing DONE"));
			}

			{
				FMemoryReader Ar { Buffer, /*bPersistent=true*/ };	

				Buffer.ArIsSaveGame = bInIsSaveGame;
				Buffer.ArNoDelta = bInArNoDelta;

				int32 V = -1;
				FMyStruct MyStruct = FImpl::CreateMyStruct(-1, FString("Str_UNITIALIZED"));

				UE_LOG(MyLog, Log, TEXT("Deserializing..."));
				Ar << V; // Deserializing
				FImpl::SerializeMyStruct(Ar, MyStruct);
				UE_LOG(MyLog, Log, TEXT("V=%d"), V);
				USerializeTestLib::LogStruct(MyStruct);
				UE_LOG(MyLog, Log, TEXT("Deserializing DONE"));
			}
			UE_LOG(MyLog, Log, TEXT("ArchiveTest DONE"));
		}

	};
} // anonymous

void USerializeTestLib::ArchiveTest_ObjectAndNameAsStringProxyArchive(UObject* WorldContextObject)
{
	UE_LOG(MyLog, Log, TEXT("ArchiveTest_ObjectAndNameAsStringProxyArchive..."));

	{
		FBufferArchive Buffer;
		FObjectAndNameAsStringProxyArchive BufferProxy { Buffer, /*bInLoadIfFindFails*/false };

		FMemoryReader Reader { Buffer, /*bPersistent=*/true };
		FObjectAndNameAsStringProxyArchive ReaderProxy { Reader, /*bInLoadIfFindFails*/false };		
	}

	UE_LOG(MyLog, Log, TEXT("ArchiveTest_ObjectAndNameAsStringProxyArchive DONE"));
}

void USerializeTestLib::ArchiveTest_BufferArchive_MemoryReader(UObject* WorldContextObject)
{
	UE_LOG(MyLog, Log, TEXT("ArchiveTest_BufferArchive_MemoryReader..."));

	FArchiveTest::DoAllTests(WorldContextObject, false, false);
	FArchiveTest::DoAllTests(WorldContextObject, true, false);
	FArchiveTest::DoAllTests(WorldContextObject, false, true);
	FArchiveTest::DoAllTests(WorldContextObject, true, true);

	UE_LOG(MyLog, Log, TEXT("ArchiveTest_BufferArchive_MemoryReader DONE"));
}

namespace 
{
	/**
	* Tests of simple types
	*/
	class FSimpleTest
	{
	public:
		template<class DestAr, class SourceAr>
		static void DoAllTests(DestAr& Dest, SourceAr& Src)	
		{
			int32 const REF_Int32 = 3;

			{
				UE_LOG(MyLog, Log, TEXT("Serializing..."));

				{
					int32 var_int32 = REF_Int32;
					Dest << var_int32;
					UE_LOG(MyLog, Log, TEXT("int32: %d"), var_int32);
				}
			}

			{
				UE_LOG(MyLog, Log, TEXT("Deserializing..."));

				{
					int32 var_int32 = -1;
					Src << var_int32;
					UE_LOG(MyLog, Log, TEXT("int32: %d"), var_int32);
				}
			}

			DoArrayTests(Dest, Src);
		}

		template<class DestAr, class SourceAr>
		static void DoArrayTests(DestAr& Dest, SourceAr& Src)	
		{
			UE_LOG(MyLog, Log, TEXT("Array tests..."));
			TArray<int32> var_int32 { 1, 2, 3 };

			{
				UE_LOG(MyLog, Log, TEXT("Serializing..."));

				{
					Dest << var_int32;
					FImpl::DumpArray(TEXT("var_int32 (array)"), var_int32);
				}
			}

			{
				UE_LOG(MyLog, Log, TEXT("Deserializing..."));

				{
					TArray<int32> var_int32 {};
					Src << var_int32;				
					FImpl::DumpArray(TEXT("var_int32 (array)"), var_int32);
				}
			}
			UE_LOG(MyLog, Log, TEXT("Array tests DONE"));
		}
	};
} // anonymous

void USerializeTestLib::SimpleTypeTests(UObject* WorldContextObject)
{
	UE_LOG(MyLog, Log, TEXT("USerializeTestLib::SimpleTypeTest..."));

	{
		UE_LOG(MyLog, Log, TEXT("BufferArchive"));
		FBufferArchive Buffer;
		FMemoryReader Reader { Buffer, /*bPersistent=*/true };
		
		FSimpleTest::DoAllTests(Buffer, Reader);
	}

	UE_LOG(MyLog, Log, TEXT("USerializeTestLib::SimpleTypeTest DONE"));
}

namespace 
{
	/**
	* Tests of object type
	*/
	class FObjTest
	{
	public:
		template<class DestAr, class SourceAr>
		static void DoAllTests(UObject* InOwner, DestAr& Dest, SourceAr& Src)	
		{
			UMyObj* const REF_Obj = UMyObj::NewMyObj(InOwner);
			REF_Obj->InitForTest();

			{
				UE_LOG(MyLog, Log, TEXT("Serializing..."));

				{
					UMyObj* var_obj = REF_Obj;
					UMyObj::DumpNullable(var_obj);

					// WARNING!!! The following line causes ASSERT failure (4.21):
					// [File:C:\UNR_CODE\UE4_21\UE_4.21\Engine\Source\Runtime\Core\Public\Serialization/MemoryArchive.h] [Line: 56]
					// {
					//	Dest << var_obj;
					// }
					
					{
						var_obj->Serialize(Dest);
					}
				}
			}

			{
				UE_LOG(MyLog, Log, TEXT("Deserializing..."));

				{
					// WARNING!!! We must create UObject to deserialize it!!!!
					UMyObj* var_obj = nullptr;
					{
						var_obj = NewObject<UMyObj>(InOwner, UMyObj::StaticClass());
						var_obj->Serialize(Src);
					}

					UE_LOG(MyLog, Log, TEXT("var_obj:"));
					UMyObj::DumpNullable(var_obj);
				}
			}
		}
	};
} // anonymous

void USerializeTestLib::ObjectTests(UObject* WorldContextObject)
{
	UE_LOG(MyLog, Log, TEXT("USerializeTestLib::ObjectTests..."));
	{
		UE_LOG(MyLog, Log, TEXT("--- BufferArchive -----------------"));
		FBufferArchive Buffer;

		// If ArIsSaveGame = true, then ONLY SaveProperties are to be serialized (including withing structs)
		Buffer.ArIsSaveGame = true;
		Buffer.ArNoDelta = true;

		FMemoryReader Reader { Buffer, /*bPersistent=*/true };

		// If ArIsSaveGame = true, then ONLY SaveProperties are to be serialized (including withing structs)
		Reader.ArIsSaveGame = true;
		Reader.ArNoDelta = true;
		
		FObjTest::DoAllTests(WorldContextObject, Buffer, Reader);
	}

	{
		UE_LOG(MyLog, Log, TEXT("--- FObjectAndNameAsStringProxyArchive -----------------"));

		FBufferArchive Buffer;
		FObjectAndNameAsStringProxyArchive BufferProxy { Buffer, /*bInLoadIfFindFails*/false };

		FMemoryReader Reader { Buffer, /*bPersistent=*/true };
		FObjectAndNameAsStringProxyArchive ReaderProxy { Reader, /*bInLoadIfFindFails*/false };

		FObjTest::DoAllTests(WorldContextObject, BufferProxy, ReaderProxy);
	}

	UE_LOG(MyLog, Log, TEXT("USerializeTestLib::ObjectTests DONE"));
}

void USerializeTestLib::ContainerTests(UObject* WorldContextObject)
{
	UE_LOG(MyLog, Log, TEXT("USerializeTestLib::ContainerTests..."));
	
	TArray<FMyStruct> REF_Structs;
	{
		UE_LOG(MyLog, Log, TEXT("REFERENCE Struct Array: "));
		REF_Structs = FImpl::CreateMyStructArray(5);
		LogStructArray(REF_Structs);
	}

	// Serializing
	// @TODO

	// Deserializing	
	// @TODO
}

void USerializeTestLib::StructTests(UObject* WorldContextObject)
{
	UE_LOG(MyLog, Log, TEXT("USerializeTestLib::StructTests..."));

	FMyStruct REF_S;
	FBufferArchive Buffer;

	{	
		UE_LOG(MyLog, Log, TEXT("REFERENCE_Struct:"));
		REF_S = FImpl::CreateMyStruct(1, FString("Str_1"));
		LogStruct(REF_S);
	}

	// Serializing
	{
		// COMPILATION FAILED:
		//Buffer << REF_S;
	}

	// Deserializing	
	{
		FMyStruct LoadedS;
		FMemoryReader Ar { Buffer , /*bPersistent*/ true }; 
		
		// COMPILATION FAILED:
		// Ar << LoadedS;

		UE_LOG(MyLog, Log, TEXT("LoadedS:"));
		LogStruct(LoadedS);
	}
	
}

void USerializeTestLib::LogStructArray(const TArray<FMyStruct>& A)
{
	UE_LOG(MyLog, Log, TEXT("USerializeTestLib::LoggingStructArray..."));
	UE_LOG(MyLog, Log, TEXT("Num=%d"), A.Num());
	for(int32 i = 0; i < A.Num(); i++)
	{
		UE_LOG(MyLog, Log, TEXT("%d:"), i);
		LogStruct(A[i]);
	}
	UE_LOG(MyLog, Log, TEXT("USerializeTestLib::LoggingStructArray DONE"));
}

void USerializeTestLib::LogStruct(const FMyStruct& S)
{
	UMyObj::LogStruct(S);
}