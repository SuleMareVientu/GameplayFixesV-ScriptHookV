#pragma once
#include <globals.h>

ULONG_PTR FindPattern(std::string signature);
ULONG_PTR FindPatternGlobal(std::string signature);

#pragma region Game Functions
namespace nGame
{
ULONG_PTR GetScriptEntity(Entity entity);
int GetFragInstNmGtaOffset();
NmMessage CreateNmMessage();
void GivePedNMMessage(NmMessage msgPtr, const Ped ped, const char* message);
void SetNMMessageParam(NmMessagePtr msgPtr, const char* msgParam, int i);
void SetNMMessageParam(NmMessagePtr msgPtr, const char* msgParam, bool b);
void SetNMMessageParam(NmMessagePtr msgPtr, const char* msgParam, float f);
void SetNMMessageParam(NmMessagePtr msgPtr, const char* msgParam, const char* str);
void SetNMMessageParam(NmMessagePtr msgPtr, const char* msgParam, float x, float y, float z);
inline void GivePedNMMessage(NmMessage msgPtr, const Ped ped, eNMStr message) { GivePedNMMessage(std::move(msgPtr), ped, NMStrings[message]); return; }
inline void SetNMMessageParam(NmMessagePtr msgPtr, eNMStr msgParam, int i) { SetNMMessageParam(msgPtr, NMStrings[msgParam], i); return; }
inline void SetNMMessageParam(NmMessagePtr msgPtr, eNMStr msgParam, bool b) { SetNMMessageParam(msgPtr, NMStrings[msgParam], b); return; }
inline void SetNMMessageParam(NmMessagePtr msgPtr, eNMStr msgParam, float f) { SetNMMessageParam(msgPtr, NMStrings[msgParam], f); return; }
inline void SetNMMessageParam(NmMessagePtr msgPtr, eNMStr msgParam, const char* str) { SetNMMessageParam(msgPtr, NMStrings[msgParam], str); return; }
inline void SetNMMessageParam(NmMessagePtr msgPtr, eNMStr msgParam, float x, float y, float z) { SetNMMessageParam(msgPtr, NMStrings[msgParam], x, y, z); return; }
/*
void SetNMMessageInt(ULONG_PTR msgMemPtr, const char* msgParam, int i);
void SetNMMessageBool(ULONG_PTR msgMemPtr, const char* msgParam, bool b);
void SetNMMessageFloat(ULONG_PTR msgMemPtr, const char* msgParam, float f);
void SetNMMessageString(ULONG_PTR msgMemPtr, const char* msgParam, const char* str);
void SetNMMessageVec3(ULONG_PTR msgMemPtr, const char* msgParam, float x, float y, float z);
inline void GivePedNMMessage(ULONG_PTR msgMemPtr, const Ped ped, eNMStr message) { GivePedNMMessage(msgMemPtr, ped, NMStrings[message]); return; }
inline void SetNMMessageInt(ULONG_PTR msgMemPtr, eNMStr msgParam, int i) { SetNMMessageInt(msgMemPtr, NMStrings[msgParam], i); return; }
inline void SetNMMessageBool(ULONG_PTR msgMemPtr, eNMStr msgParam, bool b) { SetNMMessageBool(msgMemPtr, NMStrings[msgParam], b); return; }
inline void SetNMMessageFloat(ULONG_PTR msgMemPtr, eNMStr msgParam, float f) { SetNMMessageFloat(msgMemPtr, NMStrings[msgParam], f); return; }
inline void SetNMMessageString(ULONG_PTR msgMemPtr, eNMStr msgParam, const char* str) { SetNMMessageString(msgMemPtr, NMStrings[msgParam], str); return; }
inline void SetNMMessageVec3(ULONG_PTR msgMemPtr, eNMStr msgParam, float x, float y, float z) { SetNMMessageVec3(msgMemPtr, NMStrings[msgParam], x, y, z); return; }
*/
}
#pragma endregion

void GetGameFunctionsAddresses();
void ApplyExePatches();