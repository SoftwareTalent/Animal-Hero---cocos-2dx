//
//  Global.h
//  Kiddles
//
//  Created by Goldenstar on 7/26/13.
//
//

#ifndef __GLOBAL__
#define __GLOBAL__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "cocos-ext.h"

USING_NS_CC;
using namespace CocosDenshion;
USING_NS_CC_EXT;

#ifdef UI_USER_INTERFACE_IDIOM//()
#define IS_IPAD() (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad)
#endif

#define IS_ANDROID() (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#define IS_IOS() (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#define FLOOR_HEIGHT 61

#define SFX_BUTTON SimpleAudioEngine::sharedEngine()->playEffect("snd_button.mp3");


//boolean
extern bool g_bSound;
extern bool g_bRated;

//size, scale
extern float g_fWidth;
extern float g_fHeight;
extern float g_fScaleX;
extern float g_fScaleY;

extern int g_nBestScore;
extern bool g_bADS;

extern bool g_bTutorial;

extern CCAnimation* g_aBoy[8];
extern CCAnimation* g_aBoyRun[8];

extern int g_nBg;
extern int g_nHero[8];
extern int g_nHeroVal[8];
extern int g_nCherryCnt;
extern int g_nHeroIndex;
extern int g_nHeroJump[8];
extern int g_nGameOver;
//init functions
bool init_game();
void loadGameInfo();
void saveGameInfo();
std::string getFilePath();
void initAnimation();

//utility functions
float GetX(float x);
float GetY(float y);

CCSprite *newSprite(const char *sName, float x, float y, CCNode* target, int zOrder, float scale);
CCMenuItemImage *newButton(const char* btnName, float x, float y, CCNode* target, int zOrder, SEL_MenuHandler selector, bool isOneImage, float scale);

CCMenuItemToggle *newToggleButton(const char *btnName, float x, float y, CCNode* target, int zOrder, SEL_MenuHandler selector, bool isOneImage, float scale);

CCLabelAtlas* newLabel(const char* txt, float x, float y, CCNode* target, int zOrder, float scale);

void showParticle(const char *filename, CCNode *target, float x, float y);

int sfx(const char* sfxName);

#endif /* defined(__Kiddles__Global__) */
