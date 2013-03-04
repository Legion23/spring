/* This file is part of the Spring engine (GPL v2 or later), see LICENSE.html */

#include "GlobalSynced.h"

#include <assert.h>
#include <cstring>

#include "ExternalAI/SkirmishAIHandler.h"
#include "Game/GameSetup.h"
#include "Sim/Misc/TeamHandler.h"
#include "Sim/Misc/GlobalConstants.h"
#include "System/Util.h"
#include "System/Log/FramePrefixer.h"


/**
 * @brief global synced
 *
 * Global instance of CGlobalSynced
 */
CGlobalSynced* gs = NULL;



CR_BIND(CGlobalSynced, );

CR_REG_METADATA(CGlobalSynced, (
	CR_MEMBER(frameNum),
	CR_MEMBER(speedFactor),
	CR_MEMBER(wantedSpeedFactor),
	CR_MEMBER(paused),
	CR_MEMBER(mapx),
	CR_MEMBER(mapy),
	CR_MEMBER(mapSquares),
	CR_MEMBER(hmapx),
	CR_MEMBER(hmapy),
	CR_MEMBER(pwr2mapx),
	CR_MEMBER(pwr2mapy),
	CR_MEMBER(tempNum),
	CR_MEMBER(godMode),
	CR_MEMBER(globalLOS),
	CR_MEMBER(cheatEnabled),
	CR_MEMBER(noHelperAIs),
	CR_MEMBER(editDefsEnabled),
	CR_MEMBER(useLuaGaia),
	CR_MEMBER(randSeed),
	CR_MEMBER(initRandSeed),
	CR_RESERVED(64)
));


/**
 * Initializes variables in CGlobalSynced
 */
CGlobalSynced::CGlobalSynced()
{
	mapx  = 512;
	mapy  = 512;
	mapxm1 = mapx - 1;
	mapym1 = mapy - 1;
	mapxp1 = mapx + 1;
	mapyp1 = mapy + 1;
	mapSquares = mapx * mapy;
	hmapx = mapx>>1;
	hmapy = mapy>>1;
	pwr2mapx = mapx; //next_power_of_2(mapx);
	pwr2mapy = mapy; //next_power_of_2(mapy);
	randSeed = 18655;
	for (int i = 0; i < MAX_UNITS; ++i)
		randSeeds[i] = (randInt() << 16) | randInt();
	randSeed = 18655; // again
	initRandSeed = randSeed;
	frameNum = 0;
	speedFactor = 1;
	wantedSpeedFactor = 1;
	paused = false;
	godMode = false;
	cheatEnabled = false;
	noHelperAIs = false;
	editDefsEnabled = false;
	tempNum = 2;
	useLuaGaia = true;

	memset(globalLOS, 0, sizeof(globalLOS));
	log_framePrefixer_setFrameNumReference(&frameNum);

	teamHandler = new CTeamHandler();
}


CGlobalSynced::~CGlobalSynced()
{
	SafeDelete(teamHandler);

	log_framePrefixer_setFrameNumReference(NULL);
}


void CGlobalSynced::LoadFromSetup(const CGameSetup* setup)
{
	noHelperAIs = setup->noHelperAIs;
	useLuaGaia  = setup->useLuaGaia;

	skirmishAIHandler.LoadFromSetup(*setup);
	teamHandler->LoadFromSetup(setup);
}

/**
 * @return synced random integer
 *
 * returns a synced random integer
 */
int CGlobalSynced::randInt()
{
	ASSERT_SINGLETHREADED_SIM();
	randSeed = (randSeed * 214013L + 2531011L);
	return (randSeed >> 16) & RANDINT_MAX;
}

int CGlobalSynced::randInt(const CSolidObject *o)
{
	int& rs = randSeeds[o->id];
	rs = (rs * 214013L + 2531011L);
	return (rs >> 16) & RANDINT_MAX;
}

/**
 * @return synced random float
 *
 * returns a synced random float
 */
float CGlobalSynced::randFloat()
{
	ASSERT_SINGLETHREADED_SIM();
	randSeed = (randSeed * 214013L + 2531011L);
	return float((randSeed >> 16) & RANDINT_MAX)/RANDINT_MAX;
}

float CGlobalSynced::randFloat(const CSolidObject *o)
{
	int& rs = randSeeds[o->id];
	rs = (rs * 214013L + 2531011L);
	return float((rs >> 16) & RANDINT_MAX)/RANDINT_MAX;
}

/**
 * @return synced random vector
 *
 * returns a synced random vector
 */
float3 CGlobalSynced::randVector()
{
	ASSERT_SINGLETHREADED_SIM();
	float3 ret;
	do {
		ret.x = randFloat()*2-1;
		ret.y = randFloat()*2-1;
		ret.z = randFloat()*2-1;
	} while(ret.SqLength()>1);

	return ret;
}

float3 CGlobalSynced::randVector(const CSolidObject *o)
{
	float3 ret;
	do {
		ret.x = randFloat(o)*2-1;
		ret.y = randFloat(o)*2-1;
		ret.z = randFloat(o)*2-1;
	} while(ret.SqLength()>1);

	return ret;
}
