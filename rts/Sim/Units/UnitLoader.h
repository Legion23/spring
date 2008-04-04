#ifndef UNITLOADER_H
#define UNITLOADER_H
// UnitLoader.h: interface for the CUnitLoader class.
//
//////////////////////////////////////////////////////////////////////

class CUnit;
class CWeapon;
#include <string>

#include "UnitDef.h"

using namespace std;
struct GuiSoundSet;

class CUnitLoader
{
public:
	CUnitLoader();
	virtual ~CUnitLoader();

	CUnit* LoadUnit(const string& name,float3 pos, int team,
	                bool build, int facing, const CUnit* builder /* can be NULL */);
	void FlattenGround(const CUnit* unit);

	CWeapon* LoadWeapon(const WeaponDef* weapondef, CUnit* owner, const UnitDef::UnitDefWeapon* udw);
protected:
	void LoadSound(GuiSoundSet &sound);
};

extern CUnitLoader unitLoader;

#endif /* UNITLOADER_H */
