#include "MainScene.h"
#include "GameScene.h"


#if (CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
    #include <jni.h>
    #include "platform/android/jni/JniHelper.h"
#else
    #include "SocialIOS.h"
#endif

//using namespace nextpeer;
CCScene* MainScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    MainScene *layer = MainScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool MainScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
#if (CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
    
#else
  //SocialIOS::showiAD();
   // SocialIOS::showChartboost();
#endif
  

    this->setTouchEnabled(true);
    loadGameInfo();
    g_nBg = rand()%4;
    if (!(SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying())) {
        SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
        SimpleAudioEngine::sharedEngine()->playBackgroundMusic(CCString::createWithFormat("snd_bg_%d.mp3", g_nBg)->getCString(), true);
    }
    
    // Background
    newSprite(CCString::createWithFormat("bg_%d", g_nBg)->getCString(), 100, 0, this, 0, g_fScaleY)->setAnchorPoint(ccp(1,0));
    newSprite("coin", 90, 88, this, 0, g_fScaleX);

    //->setScaleY(g_fScaleY);
//    CCSprite *bg1 = newSprite("bg_game1", 50, 0, this, 0, g_fScaleX); bg1->setScaleY(g_fScaleY); bg1->setAnchorPoint(ccp(0.5, 0));
//    newSprite("title", 50, 75, this, 0, g_fScaleX);

    CCLabelTTF *lStart = CCLabelTTF::create("Animal Hero", "Marker Felt", 75);
    lStart->setScale(g_fScaleY);
    lStart->setPosition(ccp(GetX(50), GetY(75)));
    lStart->setColor(ccBLACK);
    addChild(lStart, 5);

    m_lblCherryCnt = CCLabelTTF::create(CCString::createWithFormat("%d", g_nCherryCnt)->getCString(), "Clear Sans", 28);
    m_lblCherryCnt->setScale(g_fScaleY);
    m_lblCherryCnt->setPosition(ccp(GetX(80), GetY(88)));
    m_lblCherryCnt->setColor(ccBLACK);
    addChild(m_lblCherryCnt, 5);

    
    //add Menu
    CCMenuItemImage *playItem = newButton("btn_play", 50, 40, this, 1, menu_selector(MainScene::onPlay), true, g_fScaleX);
    CCMenuItemImage *helpItem = newButton("btn_help", 10, 15, this, 1, menu_selector(MainScene::onHelp), true, g_fScaleX);
    
    CCMenuItemImage *shopItem = newButton("btn_shop", 90, 5, this, 1, menu_selector(MainScene::onShop), true, g_fScaleX);
    
    CCMenuItemImage *HeroItem = newButton("btn_selectHero", 90, 15, this, 1, menu_selector(MainScene::onHeroSelect), true, g_fScaleX);

    
    m_soundItem = newToggleButton("btn_snd", 10, 5, this, 1, menu_selector(MainScene::onSound), true, g_fScaleX);
//    m_soundItem->setScale(g_fScaleY*0.8f);
    if (g_bSound) {
        m_soundItem->setSelectedIndex(0);
    }
    else
        m_soundItem->setSelectedIndex(1);
    
    CCMenu* pMenu = CCMenu::create(playItem, helpItem, shopItem, HeroItem, m_soundItem, NULL);
    pMenu->setPosition( CCPointZero );
    this->addChild(pMenu, 2);
    
    createHeroMenu();
    createShopMenu();
  
    return true;
}

void  MainScene::createHeroMenu()
{
    m_sprHeroBg = newSprite("Hero_bg", 50, 50, this, 10, g_fScaleX);
    CCMenuItemImage *heroItem[8];
    for (int i = 0; i < 8; i++) {
        if (g_nHero[0] == 0) {
            g_nHero[0] = 1;
        }
        if (g_nHero[i] >= 1) {
            
            heroItem[i] = newButton(CCString::createWithFormat("btn_Hero%d", i)->getCString(), 34 + (i % 2) * 32, 32.5 + 11 * ((7 - i) / 2), this, 1, menu_selector(MainScene::onUnlockHero), true, g_fScaleX);
        }
        else
        {
            CCString *str = CCString::createWithFormat("btn_lockHero%d", i);
            heroItem[i] = newButton(str->getCString(), 34 + (i % 2) * 32, 32.5 + 11 * ((7 - i) / 2), this, 1, menu_selector(MainScene::onUnlockHero), true, g_fScaleX);
        }
        heroItem[i]->setTag(i);
        heroItem[i]->setScale(g_fScaleX * 0.8);

//        heroItem[i]->setScaleX(g_fScaleX * 0.8);
//        heroItem[i]->setScaleY(g_fScaleY * 0.8);
    }
    HeroMenu = CCMenu::create(heroItem[0], heroItem[1], heroItem[2], heroItem[3], heroItem[4], heroItem[5],heroItem[6],heroItem[7], NULL);
    HeroMenu->setPosition(CCPointZero);
    this->addChild(HeroMenu, 11);
    
    m_sprHeroBg->setVisible(false);
    HeroMenu->setVisible(false);
    HeroMenu->setEnabled(false);
}

void  MainScene::createShopMenu()
{
   m_sprShopBg = newSprite("shop_bg", 50, 50, this, 10, g_fScaleX);
    CCMenuItemImage *cherryItem[4];
//    CCMenuItemToggle *enableCherry;
    for (int i = 0; i < 4; i++) {
        if (g_fWidth == 768) {
            cherryItem[i] = newButton(CCString::createWithFormat("btn_cherry%d", i + 1)->getCString(), 50, 33 + 12 * ((3 - i)), this, 1, menu_selector(MainScene::onBuy), true, g_fScaleX);
        }
        else
        {
            cherryItem[i] = newButton(CCString::createWithFormat("btn_cherry%d", i + 1)->getCString(), 50, 33 + 10 * ((3 - i)), this, 1, menu_selector(MainScene::onBuy), true, g_fScaleX);
        }
        cherryItem[i]->setTag(i);
    }
//    if (g_fWidth == 768) {
//        enableCherry = newToggleButton("btn_enableCherry", 50, 25, this, 1, menu_selector(MainScene::onSound), true, g_fScaleX);
//    }
//    else
//    {
//        enableCherry = newToggleButton("btn_enableCherry", 50, 28, this, 1, menu_selector(MainScene::onSound), true, g_fScaleX);
//    }
    ShopMenu = CCMenu::create(cherryItem[0], cherryItem[1], cherryItem[2], cherryItem[3], NULL);
    ShopMenu->setPosition(CCPointZero);
    this->addChild(ShopMenu, 11);
    
    m_sprShopBg->setVisible(false);
    ShopMenu->setVisible(false);
    ShopMenu->setEnabled(false);
}

void MainScene::showHeroMenu()
{
    m_sprHeroBg->setVisible(true);
    HeroMenu->setVisible(true);
    HeroMenu->setEnabled(true);
}

void MainScene::hideHeroMenu()
{
    m_sprHeroBg->setVisible(false);
    HeroMenu->setVisible(false);
    HeroMenu->setEnabled(false);
}

void MainScene::showShopMenu()
{
    m_sprShopBg->setVisible(true);
    ShopMenu->setVisible(true);
    ShopMenu->setEnabled(true);
}

void MainScene::hideShopMenu()
{
    m_sprShopBg->setVisible(false);
    ShopMenu->setVisible(false);
    ShopMenu->setEnabled(false);
}

#pragma mark - Events
void MainScene::onPlay(CCObject* pSender)
{
    g_bTutorial = false;
    sfx("snd_button");
    CCScene *pScene = GameScene::scene();
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5, pScene));
}

void MainScene::onHelp(CCObject* pSender)
{
    g_bTutorial = true;
    sfx("snd_button");
    CCScene *pScene = GameScene::scene();
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5, pScene));
}

void MainScene::onShop(CCObject* pSender)
{
    showShopMenu();
    hideHeroMenu();
}

void MainScene::onHeroSelect(CCObject* pSender)
{
    showHeroMenu();
    hideShopMenu();
}

void MainScene::onSound(CCObject* pSender)
{
    if (g_bSound) {
        g_bSound = false;
        SimpleAudioEngine::sharedEngine()->setEffectsVolume(0.0f);
        SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0.0f);
    }
    else{
        g_bSound = true;
        SimpleAudioEngine::sharedEngine()->setEffectsVolume(1.0);
        SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(1.0f);
    }
}

void MainScene::onBuy(CCObject *pSender)
{
    CCMenuItemImage *btn = (CCMenuItemImage *) pSender;
    int tag = btn->getTag();
//    
//    #if (CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
//    
//    #else
//        SocialIOS::buyCoin(tag);
//    #endif
    
#if (CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
    JniMethodInfo t;
#endif
    
    switch (tag)
    {
        case 0:
            
#if (CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
           // JniMethodInfo t;
            if (JniHelper::getStaticMethodInfo(t, "com/bitmonkey/stunthero/StuntHero"
                                               ,"buy100Coins"
                                               ,"()V"))
            {
                t.env->CallStaticVoidMethod(t.classID,t.methodID);
                //t.env->DeleteLocalRef(t.classID);
            }
#else
            SocialIOS::buyCoin(tag);
#endif
            break;
        
        case 1:
#if (CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
            //JniMethodInfo t;
            if (JniHelper::getStaticMethodInfo(t, "com/bitmonkey/stunthero/StuntHero"
                                               ,"buy300Coins"
                                               ,"()V"))
            {
                t.env->CallStaticVoidMethod(t.classID,t.methodID);
                //t.env->DeleteLocalRef(t.classID);
            }
#else
            SocialIOS::buyCoin(tag);
#endif
            break;
       
        case 2:
#if (CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
           // JniMethodInfo t;
            if (JniHelper::getStaticMethodInfo(t, "com/bitmonkey/stunthero/StuntHero"
                                               ,"buy500Coins"
                                               ,"()V"))
            {
                t.env->CallStaticVoidMethod(t.classID,t.methodID);
                //t.env->DeleteLocalRef(t.classID);
            }
#else
            SocialIOS::buyCoin(tag);
#endif
            break;
        
        case 3:
#if (CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
           // JniMethodInfo t;
            if (JniHelper::getStaticMethodInfo(t, "com/bitmonkey/stunthero/StuntHero"
                                               ,"buy1000Coins"
                                               ,"()V"))
            {
                t.env->CallStaticVoidMethod(t.classID,t.methodID);
                //t.env->DeleteLocalRef(t.classID);
            }
#else
            SocialIOS::buyCoin(tag);
#endif
            break;
       
        default:
            break;
    }
}

void MainScene::onUnlockHero(CCObject *pSender)
{

    CCMenuItemImage *btn = (CCMenuItemImage *) pSender;
    int tag = btn->getTag();
    if (g_nHero[tag] >= 1) {
        g_nHeroIndex = tag;
        hideHeroMenu();
    }
    else
    {
        if (g_nCherryCnt >= g_nHeroVal[tag]) {
            g_nHero[tag] = 1;
            g_nCherryCnt -= g_nHeroVal[tag];
            saveGameInfo();
            CCScene *pScene = MainScene::scene();
            CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5, pScene));
        }
        else
        {
            showShopMenu();
            hideHeroMenu();
        }
    }
}

void MainScene::reloadScene()
{
    CCScene *pScene = MainScene::scene();
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5, pScene));
}

#pragma mark - touch
void MainScene::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
    
}


bool MainScene::ccTouchBegan(CCTouch *touch, CCEvent *event)
{
    CCPoint location = touch->getLocationInView();
    location = CCDirector::sharedDirector()->convertToGL(location);
    if (!m_sprShopBg->boundingBox().containsPoint(location)) {
        hideShopMenu();
        hideHeroMenu();
    }
    
    return true;
}


