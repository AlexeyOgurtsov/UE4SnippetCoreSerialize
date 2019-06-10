#include "MyLib.h"
#include "Util/LogUtilLib.h"

void FMyStruct::InitForTest()
{
	// ~Props Begin
	V = 333;
	S = FString(TEXT("333"));
	// ~Props End

	// ~Non_Props Begin
	V_NoProp = 333;
	S_NoProp = FString(TEXT("333"));
	// ~Non_Props End

	// ~Transient Props Begin
	V_Transient = 333;
	S_Transient = FString(TEXT("333"));
	// ~Transient Props End

	// ~SaveGame Props Begin
	V_SaveGame = 333;
	S_SaveGame = FString(TEXT("333"));
	// ~SaveGame Props End
}

void FMyStruct::Serialize(FArchive& Ar)
{
	UE_LOG(MyLog, Log, TEXT("FMyStruct::Serialize..."));
	UE_LOG(MyLog, Log, TEXT("FMyStruct::Serialize DONE"));
}

void FMyStruct::PostSerialize()
{
	UE_LOG(MyLog, Log, TEXT("FMyStruct::PostSerialize..."));
	UE_LOG(MyLog, Log, TEXT("FMyStruct::PostSerialize DONE"));
}

void UMyLib::LogStruct(const FMyStruct& S)
{
	UE_LOG(MyLog, Log, TEXT("UMyLib::LoggingStruct..."));

	{
		UE_LOG(MyLog, Log, TEXT("UPROPERTYs..."));
		UE_LOG(MyLog, Log, TEXT("V=%d; S=\"%s\""), S.V, *S.S);
		UE_LOG(MyLog, Log, TEXT("UPROPERTYs DONE"));
	}

	{
		UE_LOG(MyLog, Log, TEXT("NonProps..."));
		UE_LOG(MyLog, Log, TEXT("V_NoProp=%d; S_NoProp=\"%s\""), S.V_NoProp, *S.S_NoProp);
		UE_LOG(MyLog, Log, TEXT("NonProps DONE"));
	}

	{
		UE_LOG(MyLog, Log, TEXT("Transient props..."));
		UE_LOG(MyLog, Log, TEXT("V_Transient=%d; S_Transient=\"%s\""), S.V_Transient, *S.S_Transient);
		UE_LOG(MyLog, Log, TEXT("Transient props DONE"));
	}

	{
		UE_LOG(MyLog, Log, TEXT("Savegame props..."));
		UE_LOG(MyLog, Log, TEXT("V_SaveGame=%d; S_SaveGame=\"%s\""), S.V_SaveGame, *S.S_SaveGame);
		UE_LOG(MyLog, Log, TEXT("Savegame props DONE"));
	}

	UE_LOG(MyLog, Log, TEXT("UMyLib::LoggingStruct DONE"));
}