#include "MyObj.h"
#include "Util/LogUtilLib.h"

namespace
{
	class FImpl
	{
	public:
		template<class T>
		static void LogArray(const TArray<T*>& A)	
		{
			UE_LOG(MyLog, Log, TEXT("Logging array of UMyObj..."));
			UE_LOG(MyLog, Log, TEXT("Num: %d"), A.Num());
			for(int32 i = 0; i < A.Num(); i++)
			{
				UE_LOG(MyLog, Log, TEXT("%d:"), i);
				const T* O = A[i];
				UMyObj::DumpNullable(O);
			}
			UE_LOG(MyLog, Log, TEXT("Logging array of UMyObj DONE"));
		}
	};
} // anynomus

void UMyObj::LogStruct(const FMyStruct& S)
{
	UMyLib::LogStruct(S);
}

UMyObj* UMyObj::NewMyObj(UObject* InOwner)
{
	UE_LOG(MyLog, Log, TEXT("UMyObj::NewMyObj..."));
	UMyObj* Obj = NewObject<UMyObj>(InOwner, UMyObj::StaticClass());
	UE_LOG(MyLog, Log, TEXT("UMyObj::NewMyObj DONE"));
	return Obj;
}

void UMyObj::InitForTest()
{
	V = 3;
	V_Transient = 3;
	V_SaveGame = 3;

	MyStruct.InitForTest();
	MyStruct_Transient.InitForTest();
	MyStruct_SaveGame.InitForTest();
	MyStruct_NoProp.InitForTest();
}

void UMyObj::DumpNullable(const UObject* O)
{
		if(O)
		{
			auto MyObj = Cast<UMyObj>(O);
			if(MyObj)
			{
				MyObj->DumpMe();
			}
			else
			{
				UE_LOG(MyLog, Error, TEXT("*NOT*(!) UMyObj"));
			}
		}
		else
		{
			UE_LOG(MyLog, Warning, TEXT("nullptr"));
		}
}

void UMyObj::DumpMe() const
{
	UE_LOG(MyLog, Log, TEXT("UMyObj::DumpMe..."));
	UE_LOG(MyLog, Log, TEXT("Name: \"%s\"; Class: \"%s\""), *GetName(), *GetClass()->GetName());
	UE_LOG(MyLog, Log, TEXT("V=%d, V_Transient=%d, V_SaveGame=%d; V_NoProp=%d"), V, V_Transient, V_SaveGame, V_NoProp);

	{
		UE_LOG(MyLog, Log, TEXT("MyStruct..."));
		LogStruct(MyStruct);
		UE_LOG(MyLog, Log, TEXT("MyStruct DONE"));
	}

	{		
		UE_LOG(MyLog, Log, TEXT("MyStruct_Transient..."));
		LogStruct(MyStruct_Transient);
		UE_LOG(MyLog, Log, TEXT("MyStruct_Transient DONE"));
	}

	{		
		UE_LOG(MyLog, Log, TEXT("MyStruct_SaveGame..."));
		LogStruct(MyStruct_SaveGame);
		UE_LOG(MyLog, Log, TEXT("MyStruct_SaveGame DONE"));
	}

	{		
		UE_LOG(MyLog, Log, TEXT("MyStruct_NoProp..."));
		LogStruct(MyStruct_NoProp);
		UE_LOG(MyLog, Log, TEXT("MyStruct_NoProp DONE"));
	}

	UE_LOG(MyLog, Log, TEXT("UMyObj::DumpMe DONE"));
}

void UMyObj::LogArray(const TArray<UMyObj*>& InArray)
{
	FImpl::LogArray(InArray);
}

void UMyObj::LogArray(const TArray<const UMyObj*>& InArray)
{
	FImpl::LogArray(InArray);
}

void UMyObj::LogArray(const TArray<UObject*>& InArray)
{
	FImpl::LogArray(InArray);
}

void UMyObj::LogArray(const TArray<const UObject*>& InArray)
{
	FImpl::LogArray(InArray);
}

void UMyObj::PostInitProperties()
{
	UE_LOG(MyLog, Log, TEXT("UMyObj::PostInitProperties..."));
	Super::PostInitProperties();
	DumpMe();
	UE_LOG(MyLog, Log, TEXT("UMyObj::PostInitProperties DONE"));
}

void UMyObj::PostLoad()
{
	UE_LOG(MyLog, Log, TEXT("UMyObj::PostLoad..."));
	Super::PostLoad();
	DumpMe();
	UE_LOG(MyLog, Log, TEXT("UMyObj::PostLoad DONE"));
}

void UMyObj::Serialize(FArchive& Ar)
{
	UE_LOG(MyLog, Log, TEXT("UMyObj::Serialize..."));
	Super::Serialize(Ar);
	DumpMe();
	UE_LOG(MyLog, Log, TEXT("UMyObj::Serialize DONE"));
}

/*
void UMyObj::Serialize(FStructuredArchive::FRecord& Record)
{
	UE_LOG(MyLog, Log, TEXT("UMyObj::Serialize(Record)..."));
	Super::Serialize(Record);
	DumpMe();
	UE_LOG(MyLog, Log, TEXT("UMyObj::Serialize(Record) DONE"));
}
*/