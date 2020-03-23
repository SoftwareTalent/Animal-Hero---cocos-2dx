#include "ScoreScene.h"
#include "MainScene.h"
#include "GameScene.h"

#if (CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#else
#include "SocialIOS.h"
#endif

//using namespace nextpeer;
CCScene* ScoreScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    ScoreScene *layer = ScoreScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool ScoreScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    int currScore = CCUserDefault::sharedUserDefault()->getIntegerForKey("CurScore", 0);
    int bestScore = CCUserDefault::sharedUserDefault()->getIntegerForKey("BestScore", 0);
    
    // Background
//    newSprite("bg_game0", 50, 50, this, 0, g_fScaleX)->setScaleY(g_fScaleY);
//    CCSprite *bg1 = newSprite("bg_game1", 50, 0, this, 0, g_fScaleX); bg1->setScaleY(g_fScaleY); bg1->setAnchorPoint(ccp(0.5, 0));
    CCLayerColor *backBG = CCLayerColor::create();
    backBG->setColor(ccBLACK);
    backBG->setOpacity(200);
    addChild(backBG, -1);

    CCLabelTTF *lStart = CCLabelTTF::create("GAME OVER!", "Clear Sans", 78);
    lStart->setScale(g_fScaleY);
    lStart->setPosition(ccp(GetX(50), GetY(85)));
    lStart->setColor(ccWHITE);
    addChild(lStart, 5);

    // add background
    newSprite("score_pad", 50, 60, this, 0, g_fScaleX)->setScaleY(g_fScaleY);

    CCLabelTTF *lScore = CCLabelTTF::create("Score", "Clear Sans", 38);
    lScore->setPosition(ccp(GetX(50), GetY(57)));
    lScore->setColor(ccBLACK);
    addChild(lScore, 5);
    // Label
    CCLabelTTF* m_lCurScore = CCLabelTTF::create(CCString::createWithFormat("%d", currScore)->getCString(), "Marker Felt", 34);
    m_lCurScore->setPosition(ccp(GetX(50), GetY(52)));
    m_lCurScore->setColor(ccBLACK);
    addChild(m_lCurScore, 5);

    CCLabelTTF *lBestScore = CCLabelTTF::create("Best Score", "Clear Sans", 38);
    lBestScore->setPosition(ccp(GetX(50), GetY(69)));
    lBestScore->setColor(ccBLACK);
    addChild(lBestScore, 5);

    
    CCLabelTTF* m_lBestScore = CCLabelTTF::create(CCString::createWithFormat("%d", bestScore)->getCString(), "Marker Felt", 34);
    m_lBestScore->setPosition(ccp(GetX(50), GetY(63)));
    m_lBestScore->setColor(ccBLACK);
    addChild(m_lBestScore, 5);
    
    //add Menu
//    CCMenuItemImage *shareMenu = newButton("btn_share", 50, 40, this, 1, menu_selector(ScoreScene::onShare), true, g_fScaleX);
    CCMenuItemImage *backMenu = newButton("btn_home", 25, 35, this, 1, menu_selector(ScoreScene::onBack), true, g_fScaleX);
    CCMenuItemImage *retryMenu = newButton("btn_retry", 75, 35, this, 1, menu_selector(ScoreScene::onRetry), true, g_fScaleX);
    CCMenuItemImage *scoreMenu = newButton("btn_score", 41, 35, this, 1, menu_selector(ScoreScene::onScore), true, g_fScaleX);
    CCMenuItemImage *rateMenu = newButton("btn_rate", 58, 35, this, 1, menu_selector(ScoreScene::onRate), true, g_fScaleX);

    CCMenu* pMenu = CCMenu::create(backMenu, retryMenu, scoreMenu, rateMenu, NULL);
    pMenu->setPosition( CCPointZero );
    this->addChild(pMenu, 2);
    
    g_nGameOver++;
    if (g_nGameOver%5==0) {
#if (CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
        
#else
        SocialIOS::showChartboost();
#endif
    }


//    SocialIOS::showRevmob();

    return true;

}

#pragma mark - Events
void ScoreScene::onBack(CCObject* pSender)
{
    sfx("snd_button");
    CCScene *pScene = MainScene::scene();
    CCDirector::sharedDirector()->replaceScene(CCTransitionSlideInR::create(0.5, pScene));
}

void ScoreScene::onRetry(CCObject* pSender)
{
    g_nBg = rand()%4;
    sfx("snd_button");
    CCScene *pScene = GameScene::scene();
    CCDirector::sharedDirector()->replaceScene(CCTransitionSlideInR::create(0.5, pScene));
}

void ScoreScene::onScore(CCObject* pSender)
{
    sfx("snd_button");
#if (CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, "com/bitmonkey/stunthero/StuntHero"
                                       ,"showLeaderboard"
                                       ,"()V"))
    {
        t.env->CallStaticVoidMethod(t.classID,t.methodID);
    }
#else
    SocialIOS::showLeaderboard();
#endif
}

void ScoreScene::onShare(CCObject* pSender)
{
    sfx("snd_button");
#if (CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
//    shareFacebook_Android();
#else
    SocialIOS::shareFacebook(g_nBestScore);
#endif
}

void ScoreScene::onNoads(CCObject* pSender)
{
    sfx("snd_button");
#if (CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
//    removeAds_Android();
#else
//    SocialIOS::removeAds();
#endif

}

void ScoreScene::onRate(CCObject* pSender)
{
    sfx("snd_button");
#if (CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, "com/bitmonkey/stunthero/StuntHero"
                                       ,"showRateUs"
                                       ,"()V"))
    {
        t.env->CallStaticVoidMethod(t.classID,t.methodID);
    }
#else
    SocialIOS::showRateUs(true);
#endif


}