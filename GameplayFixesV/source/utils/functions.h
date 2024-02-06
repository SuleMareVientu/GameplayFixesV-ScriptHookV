#pragma once

void EnablePedConfigFlag(Ped ped, int flag);
void DisablePedConfigFlag(Ped ped, int flag);
void EnablePedResetFlag(Ped ped, int flag);
void DisablePedResetFlag(Ped ped, int flag);
bool GetWeightedBool(int chance, int startRange = 0, int endRange = 100);
void SetPlayerFlags();