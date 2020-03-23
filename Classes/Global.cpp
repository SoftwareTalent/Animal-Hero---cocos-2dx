//
//  Global.cpp
//  Kiddles
//
//  Created by Goldenstar on 7/26/13.
//
//

#include "Global.h"

//boolean
bool g_bSound = true;
bool g_bRated = false;

//size, scale
float g_fWidth;
float g_fHeight;
float g_fScaleX;
float g_fScaleY;

int g_nBestScore = 0;
bool g_bADS = false;
bool g_bTutorial = false;
int g_nBg = 0;
int g_nHero[8] = {2, 0, 0, 0, 0, 0,0,0};
int g_nHeroVal[8] = {0, 50, 100, 200, 300, 400,600,1000};
int g_nHeroJump[8]={0,50,55,55,60,65,75,90};
int g_nCherryCnt = 0;
int g_nHeroIndex = 0;
int g_nGameOver=0;
CCAnimation* g_aBoy[8];
CCAnimation* g_aBoyRun[8];

//global functions
bool init_game()
{
    SimpleAudioEngine::sharedEngine()->playBackgroundMusic("snd_bg.mp3", true);
    //init size, scale
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    g_fWidth = winSize.width;
    g_fHeight = winSize.height;
    g_fScaleX = g_fWidth / 768;
    g_fScaleY = g_fHeight / 1024;
    
    if(CC_CONTENT_SCALE_FACTOR() == 2)
    {
        g_fScaleX *= 2;
        g_fScaleY *= 2;
    }
    
    g_bSound = true;
    SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(1.0f);
    SimpleAudioEngine::sharedEngine()->setEffectsVolume(1.0);
    
    //init all
    loadGameInfo();
    initAnimation();
    
    return true;
}

void loadGameInfo()
{
    g_nBestScore = CCUserDefault::sharedUserDefault()->getIntegerForKey("BestScore", 0);
    g_nCherryCnt =CCUserDefault::sharedUserDefault()->getIntegerForKey("Candy");

    for (int i = 0; i < 8; i++) {
        CCString *str;
        str = CCString::createWithFormat("Hero%d", i);
        g_nHero[i] = CCUserDefault::sharedUserDefault()->getIntegerForKey(str->getCString());

        CCUserDefault::sharedUserDefault()->setIntegerForKey(str->getCString(), g_nHero[i]);
    }

    std::string path = getFilePath();
    FILE *fp = fopen(path.c_str(), "r");
    
    if (!fp)
    {
        CCLOG("can not create file %s", path.c_str());
        return;
    }
    
    fclose(fp);
}

void saveGameInfo()
{
    CCUserDefault::sharedUserDefault()->setIntegerForKey("Candy", g_nCherryCnt);
    for (int i = 0; i < 8; i++) {
        CCString *str;
        str = CCString::createWithFormat("Hero%d", i);
        CCUserDefault::sharedUserDefault()->setIntegerForKey(str->getCString(), g_nHero[i]);
    }
    std::string path = getFilePath();
    FILE *fp = fopen(path.c_str(), "wb");
    
    if (! fp)
    {
        CCLOG("can not create file %s", path.c_str());
        return;
    }
    
    fclose(fp);
}

void initAnimation()
{
    for (int i = 0; i < 8; i++) {
        g_aBoy[i] = new CCAnimation();     g_aBoy[i]->init();     g_aBoy[i]->setDelayPerUnit(0.1f);
        g_aBoyRun[i] = new CCAnimation();     g_aBoyRun[i]->init();     g_aBoyRun[i]->setDelayPerUnit(0.1f);
        for(int num = 1; num < 5; num++)
            g_aBoy[i]->addSpriteFrameWithFileName(CCString::createWithFormat("hero%d_%d.png",i,  num)->getCString());
        for(int num = 1; num < 5; num++)
            g_aBoyRun[i]->addSpriteFrameWithFileName(CCString::createWithFormat("hero%d_run_%d.png",i, num)->getCString());

    }
   
    SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("snd_game.mp3");
    SimpleAudioEngine::sharedEngine()->preloadEffect("snd_fall.mp3");
    SimpleAudioEngine::sharedEngine()->preloadEffect("snd_score.mp3");
    SimpleAudioEngine::sharedEngine()->preloadEffect("snd_button.mp3");


}

std::string getFilePath()
{
    std::string path("");
    
#if IS_ANDROID()
    path.append("/data/data/org.education.ignitednoodle/tmpfile");
    
#endif
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    path.append("D:/tmpfile");
    
#endif
    
    return path;
    
}

float GetX(float x) {
    return g_fWidth * x/100;
}

float GetY(float y) {
    return g_fHeight * y/100;
}


CCSprite *newSprite(const char *sName, float x, float y,CCNode *target, int zOrder, float scale)
{
    CCSprite *sprite = CCSprite::create(CCString::createWithFormat("%s.png", sName)->getCString());
    sprite->setScale(scale);
    sprite->setPosition(ccp(GetX(x), GetY(y)));
    target->addChild(sprite ,zOrder);
    return sprite;
}

CCMenuItemImage *newButton(const char * btnName, float x, float y,CCNode *target, int zOrder, SEL_MenuHandler selector, bool  isOneImage, float scale)
{
    CCMenuItemImage *item;
    
    if(isOneImage){
        item =CCMenuItemImage::create(
                                      CCString::createWithFormat("%s.png", btnName)->getCString(),
                                      CCString::createWithFormat("%s.png", btnName)->getCString(),
                                      CCString::createWithFormat("%s.png",btnName)->getCString(),target, selector);
        
        CCSprite* normalImage=(CCSprite* )item->getNormalImage();
        CCSprite* selectedImage=(CCSprite*)item->getSelectedImage();
        CCSprite* disabledImage=(CCSprite*)item->getDisabledImage();
        
        selectedImage->setColor(ccc3(normalImage->getColor().r-100, normalImage->getColor().g-100,normalImage->getColor().b-100));
        disabledImage->setColor(ccc3(normalImage->getColor().r-100, normalImage->getColor().g-100,normalImage->getColor().b-100));
        
    }else{
        item = CCMenuItemImage::create(
                                       CCString::createWithFormat("%s_n.png", btnName)->getCString(),
                                       CCString::createWithFormat("%s_s.png", btnName)->getCString(),
                                       target, selector);
    }
    
    item->setScale(scale);
    item->setPosition(GetX(x), GetY(y));
//    target->addChild(item, zOrder);
    return item;
}

CCMenuItemToggle *newToggleButton(const char *btnName, float x, float y,CCNode *target, int zOrder, SEL_MenuHandler selector, bool  isOneImage, float scale)
{
    
    CCMenuItemToggle *item;
    CCMenuItemImage *itemOn, *itemOff;
    
    if(isOneImage)
    {
        itemOn = CCMenuItemImage::create(
                                         CCString::createWithFormat("%s_on.png", btnName)->getCString(),
                                         CCString::createWithFormat("%s_on.png", btnName)->getCString()
                                         );
        itemOff = CCMenuItemImage::create(
                                          CCString::createWithFormat("%s_off.png", btnName)->getCString(),
                                          CCString::createWithFormat("%s_off.png", btnName)->getCString()
                                          );
        
        CCSprite* normalImage=(CCSprite* )itemOn->getNormalImage();
        CCSprite* selectedImage=(CCSprite*)itemOn->getSelectedImage();
        //        CCSprite* disabledImage=(CCSprite*)itemOff->getDisabledImage();
        
        selectedImage->setColor(ccc3(normalImage->getColor().r-200, normalImage->getColor().g-200,normalImage->getColor().b-200));
        //        disabledImage->setColor(ccc3(normalImage->getColor().r-100, normalImage->getColor().g-100,normalImage->getColor().b-100));
        normalImage = (CCSprite*)itemOff->getNormalImage();
        selectedImage=(CCSprite*)itemOff->getSelectedImage();
        selectedImage->setColor(ccc3(normalImage->getColor().r-200, normalImage->getColor().g-200,normalImage->getColor().b-200));
    }
    else
    {
        itemOn = CCMenuItemImage::create(
                                         CCString::createWithFormat("%s_on_n.png", btnName)->getCString(),
                                         CCString::createWithFormat("%s_on_s.png", btnName)->getCString()
                                         );
        itemOff = CCMenuItemImage::create(
                                          CCString::createWithFormat("%s_off_n.png", btnName)->getCString(),
                                          CCString::createWithFormat("%s_off_s.png", btnName)->getCString()
                                          );
    }
    
    item = CCMenuItemToggle::createWithTarget(target, selector, itemOn, itemOff, NULL);
    item->setScale(scale);
    item->setPosition(GetX(x), GetY(y));
//    target->addChild(item, zOrder);
    return item;
    
}

CCLabelAtlas* newLabel(const char* txt, float x, float y, CCNode* target, int zOrder, float scale)
{
    CCLabelAtlas* label=CCLabelAtlas::create(txt, "number.png", 50, 75, '0');
    label->setScale(scale);
    label->setAnchorPoint(ccp(0.5, 0.5));
    label->setPosition(ccp(x, y));
    target->addChild(label, zOrder);
    return label;
}

void showParticle(const char *filename, CCNode *target, float x, float y)
{
    CCParticleSystem *m_particle = CCParticleSystem::create(CCString::createWithFormat("%s.plist", filename)->getCString());
    m_particle->setPosition(GetX(x), GetY(y));
    m_particle->setScale(g_fScaleX);
//    return m_particle;
    
    //    CCParticleSystemQuad *particle =CCParticleSystemQuad::create(filename);
    //    particle->setPosition(pos);
    //    particle->setScale(G_SCALEY);
    //    target->addChild(particle);
}

int sfx(const char* ch)
{
    int sfxnum = SimpleAudioEngine::sharedEngine()->playEffect(CCString::createWithFormat("%s.mp3", ch)->getCString());
    return sfxnum;    
}
