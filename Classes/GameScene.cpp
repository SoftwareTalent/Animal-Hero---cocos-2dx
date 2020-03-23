#include "GameScene.h"
#include "MainScene.h"
#include "ScoreScene.h"

#if (CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
//#include "SocialAndroid.h"
#else
#include "SocialIOS.h"
#endif

#define BLOCK_HEIGHT  30
#define READY_X 25

CCScene* GameScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    GameScene *layer = GameScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
#if (CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
    //#include "SocialAndroid.h"
#else
//    if (!g_bTutorial) {
//        SocialIOS::showiAD();
//    }
//    else{
//        SocialIOS::hideBanner();
//    }
#endif


    m_nScore = 0;
    m_bFlip = false;
    m_bPause = false;
    m_jumping=false;
    m_jumpNum=3;
    m_eState = StateReady;
    g_nBestScore = CCUserDefault::sharedUserDefault()->getIntegerForKey("BestScore", 0);
    m_blockArray = new CCArray();
    m_stickArray = new CCArray();
    m_cherryArray = new CCArray();
    m_redArray = new CCArray();
    
    setTouchEnabled(true);

    if (!(SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying())) {
        SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
        SimpleAudioEngine::sharedEngine()->playBackgroundMusic(CCString::createWithFormat("snd_bg_%d.mp3", g_nBg)->getCString(), true);
    }

    // Background
    newSprite(CCString::createWithFormat("bg_%d", g_nBg)->getCString(), 100, 0, this, 0, g_fScaleY)->setAnchorPoint(ccp(1,0));
     newSprite(CCString::createWithFormat("grass%d", g_nBg)->getCString(), 100, 0, this, 10, g_fScaleY)->setAnchorPoint(ccp(1,0));
    m_sprCherry = newSprite("coin", 90, 85, this, 0, g_fScaleX);
    
    scorePad = newSprite("score", 50, 84.5, this, 0, g_fScaleX);
    m_lScore = CCLabelTTF::create(CCString::createWithFormat("%d", m_nScore)->getCString(), "Marker Felt", 54);
    m_lScore->setScale(g_fScaleY);
    m_lScore->setPosition(ccp(GetX(50), GetY(85)));
    addChild(m_lScore, 5);
    m_lStart = CCLabelTTF::create("Hold your finger on screen\nto stretch out the stick", "Marker Felt", 45);
    m_lStart->setScale(g_fScaleY);
    m_lStart->setPosition(ccp(GetX(50), GetY(75)));
    m_lStart->setColor(ccBLACK);
    addChild(m_lStart, 5);
    
    m_lblCherryCount = CCLabelTTF::create(CCString::createWithFormat("%d", g_nCherryCnt)->getCString(), "Marker Felt", 28);
    m_lblCherryCount->setScale(g_fScaleY);
    m_lblCherryCount->setPosition(ccp(GetX(80), GetY(85)));
    m_lblCherryCount->setColor(ccBLACK);
    addChild(m_lblCherryCount, 5);
    
    m_lblBonus = CCLabelTTF::create("+1", "Clear Sans", 28);
    m_lblBonus->setScale(g_fScaleY);
    m_lblBonus->setPosition(ccp(GetX(50), GetY(35)));
    m_lblBonus->setColor(ccBLACK);
    m_lblBonus->setOpacity(0);
    addChild(m_lblBonus, 5);

    //tutor screen
    if (g_bTutorial) {
        m_lScore->setVisible(false);
        scorePad->setVisible(false);
        m_lStart->setVisible(false);

        newSprite("bg_tutor", 50, 50, this, 1, g_fScaleX)->setScaleY(g_fScaleY);

        m_sprtHandOn = newSprite("hand_tap_on", 50, 25, this, 10, g_fScaleX);
        m_sprtHandOff = newSprite("hand_tap_off", 50, 25, this, 10, g_fScaleX);
        m_sprtHandOn->setVisible(false);
        m_sprtHandOff->setVisible(true);

        CCMenuItemImage *helpItem = newButton("btn_back", 50, 10, this, 10, menu_selector(GameScene::onBack), true, g_fScaleX);
        CCMenu* pMenu = CCMenu::create(helpItem,NULL);
        pMenu->setPosition( CCPointZero );
        this->addChild(pMenu, 10);
    }

    CCMenuItemImage *jumpItem = newButton("btn_jump", 85, 5, this, 1, menu_selector(GameScene::onJump), true, g_fScaleX);
    CCMenu* Menu = CCMenu::create(jumpItem,NULL);
    Menu->setPosition( CCPointZero );
    this->addChild(Menu, 10);
    
    initWorld();
    //schedule
    schedule(schedule_selector(GameScene::updateState), 0.02f);
    scheduleUpdateWithPriority(0.1f);

    return true;
}
void GameScene::onJump(CCObject* pSender)
{
    //m_eState=StateWalk;
    if(m_eState==StateWalk && m_jumping==false&&m_jumpNum>0&&m_bFlip==false)
    {
        m_jumpNum--;
        m_sprtHero->runAction(CCJumpBy::create(0.5, CCPoint(g_nHeroJump[g_nHeroIndex]*g_fScaleX, 0), g_nHeroJump[g_nHeroIndex]*g_fScaleY, 1));
    }    
}
void GameScene::initWorld()
{
    m_sprtHero = newSprite("hero0_1", READY_X-5, BLOCK_HEIGHT, this, 5, g_fScaleX);
    m_sprtHero->setAnchorPoint(ccp(0.5, 0));
    m_sprtHero->setTag(0); // stand state
    m_sprtHero->runAction(CCRepeatForever::create(CCAnimate::create(g_aBoy[g_nHeroIndex])));

    CCSprite* startBlock = newSprite("block", READY_X, BLOCK_HEIGHT, this, 2, g_fScaleX);
    startBlock->setAnchorPoint(ccp(1, 1));
    m_blockArray->addObject(startBlock);
    m_sprtBlock = startBlock;

    addStick();
    addBlock();
}

#pragma mark - logic
void GameScene::addStick()
{
    //new stick
    CCSprite *stick = newSprite("stick", READY_X, BLOCK_HEIGHT, this, 5, g_fScaleX);
    stick->setAnchorPoint(ccp(0.5, 0));
    stick->setScaleY(0);
    //control stick
    m_sprtStick = stick;
    m_sprtStick->setTag(0);   // set stick state by init
    //add stick to array
    m_stickArray->addObject(m_sprtStick);
}

void GameScene::addBlock()
{
    int blockWidth = rand()%20+10;
    int blockPosX = rand()%25+25;

    CCTexture2D *pTexture = CCTextureCache::sharedTextureCache()->addImage("block.png");
    CCSprite *sprtBlock = CCSprite::createWithTexture(pTexture, CCRectMake(0, 0, GetX(blockWidth), GetY(100)));

    sprtBlock->setScaleX(g_fScaleX);sprtBlock->setScaleY(g_fScaleY);
    sprtBlock->setPosition(ccp(GetX(150), GetY(BLOCK_HEIGHT)));
    sprtBlock->setAnchorPoint(ccp(1, 1));
    addChild(sprtBlock ,5);
    
    

    sprtBlock->runAction(CCMoveTo::create(1, ccp(m_sprtBlock->boundingBox().getMaxX()+GetX(blockPosX+blockWidth), GetY(BLOCK_HEIGHT))));
    
//    CCSprite *sprRed = newSprite("sprRed", 150, BLOCK_HEIGHT, this, 5, g_fScaleY);
    pTexture = CCTextureCache::sharedTextureCache()->addImage("sprRed.png");
    CCSprite *sprRed = CCSprite::createWithTexture(pTexture, CCRectMake(0, 0, GetX(2), GetY(100)));
    
    sprRed->setScaleX(g_fScaleX);sprRed->setScaleY(g_fScaleY);
    sprRed->setPosition(ccp(GetX(150) - sprtBlock->boundingBox().size.width / 2, GetY(BLOCK_HEIGHT)));
    sprRed->setAnchorPoint(ccp(0.5, 1));
    addChild(sprRed ,5);
   float dX = m_sprtBlock->boundingBox().getMaxX()+GetX(blockPosX+blockWidth) - GetX(150);
    sprRed->runAction(CCMoveBy::create(1, ccp(dX, 0)));
    
    m_fTutorialBarLength = GetX(blockPosX+blockWidth/2);// Tutorial
    
    m_blockArray->addObject(sprtBlock);
    m_redArray->addObject(sprRed);
    
    int lucky = arc4random() % 4;//create Candy
    if (lucky >= 2) {
        CCSprite *sprCandy = newSprite("coin", 150, 28, this, 5, g_fScaleX);
        
        float moveX = m_sprtBlock->boundingBox().getMaxX()+GetX(blockPosX+blockWidth) - GetX(150);
        float difX = m_sprtBlock->boundingBox().getMaxX()+GetX(blockPosX+blockWidth) - m_sprtBlock->boundingBox().getMaxX() - sprtBlock->boundingBox().size.width / 2 - sprCandy->boundingBox().size.width * 2;
        float ptx;
        
        ptx = sprtBlock->boundingBox().getMinX() - sprCandy->boundingBox().size.width / 2  - (rand() % ((int) (difX)));
        ptx = ptx / g_fWidth * 100;
        sprCandy->setPosition(ccp(GetX(ptx), GetY(28)));
        
        sprCandy->runAction((CCMoveBy::create(1, ccp(moveX, 0))));
        m_cherryArray->addObject(sprCandy);
    }
}

void GameScene::removeSprite(CCSprite *sprt)
{
    sprt->stopAllActions();
    sprt->removeFromParentAndCleanup(true);
    if (m_blockArray->containsObject(sprt))
        m_blockArray->removeObject(sprt);
    
    if (m_cherryArray->containsObject(sprt))
        m_cherryArray->removeObject(sprt);
}

void GameScene::setWalkState()
{
    m_eState = StateWalk;
    //SimpleAudioEngine::sharedEngine()->playEffect("snd_stick_fallen.mp3", false);
}

#pragma mark - schedule
void GameScene::updateState(float dt)
{
    CCObject *obj;
    switch (m_eState) {
        case StateReady:
        {
            if (m_sprtHero->getTag() == 0) {
                if (g_bTutorial) { // Tutorial
                    m_eState = StateStretch;
                }
            }
            else
            {
                if (m_sprtHero->getPositionX() > GetX(READY_X)) {
                    
                    CCObject *obj;
                    CCARRAY_FOREACH(m_cherryArray, obj)
                    {
                        CCSprite* sprt = (CCSprite*)obj;
                        
                        sprt->stopAllActions();
                        sprt->removeFromParentAndCleanup(true);
                        if (m_cherryArray->containsObject(sprt))
                            m_cherryArray->removeObject(sprt);
                    }

                    CCARRAY_FOREACH(m_redArray, obj)
                    {
                        CCSprite* sprt = (CCSprite*)obj;
                        
                        sprt->stopAllActions();
                        sprt->removeFromParentAndCleanup(true);
                        if (m_redArray->containsObject(sprt))
                            m_redArray->removeObject(sprt);
                    }

                    m_sprtHero->setPosition(ccpAdd(m_sprtHero->getPosition(), ccp(GetX(-2), 0)));
                    CCARRAY_FOREACH(m_blockArray, obj)
                    {
                        CCSprite* sprtBlock = (CCSprite*)obj;
                        sprtBlock->setPosition(ccpAdd(sprtBlock->getPosition(), ccp(GetX(-2), 0)));
                    }
                    CCARRAY_FOREACH(m_stickArray, obj)
                    {
                        CCSprite* sprtStick = (CCSprite*)obj;
                        sprtStick->setPosition(ccpAdd(sprtStick->getPosition(), ccp(GetX(-2), 0)));
                    }
                }
                else
                {
                    m_sprtHero->setTag(0);  // hero ready state
                    addStick();
                    addBlock();

                    if (g_bTutorial) {

                    }
                    else{
                        //update score visible
                        m_nScore++;
                        sfx("snd_score");
                        m_lScore->setString(CCString::createWithFormat("%d", m_nScore)->getCString());
                        if (m_nScore > g_nBestScore) {
                            g_nBestScore = m_nScore;
                        CCUserDefault::sharedUserDefault()->setIntegerForKey("BestScore", g_nBestScore);
                        #if (CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
                                                    
                        #else
                        SocialIOS::submitScore(g_nBestScore);
                        #endif
                            
                        }
                    }

                }
            }
        }
            break;
        case StateStretch:
        {
            // stretch stick
            if (m_sprtStick->getTag() == 0)  // init state of stick
            {
                m_sprtStick->setScaleY(m_sprtStick->getScaleY()+0.02f*g_fScaleY);
                SimpleAudioEngine::sharedEngine()->playEffect("snd_stick_grow_loop.mp3", false)
                ;
                if (g_bTutorial) {// Tutorial
                    m_sprtHandOn->setVisible(true);
                    m_sprtHandOff->setVisible(false);
                    if (m_sprtStick->boundingBox().size.height > m_fTutorialBarLength) {
                        m_eState = StateLay;
                        SimpleAudioEngine::sharedEngine()->playEffect("snd_kick.mp3", false);
                        m_sprtHandOn->setVisible(false);
                        m_sprtHandOff->setVisible(true);
                    }
                }
            }
        }
            break;
        case StateLay:
        {
            // lay stick
            if (m_sprtStick->getTag() == 0) {
                m_sprtStick->runAction(CCSequence::create(CCRotateBy::create(0.5, 90), CCCallFunc::create(this, callfunc_selector(GameScene::setWalkState)), NULL));
                // set stick state by lay
                m_sprtStick->setTag(1);  // set stick state by lay
            }
        }
            break;
        case StateWalk:
        {
            // update hero
            if (m_sprtHero->getTag() == 0) // if hero stand
            {
                m_sprtHero->runAction(CCRepeatForever::create(CCAnimate::create(g_aBoyRun[g_nHeroIndex])));
                m_sprtHero->setTag(1);   // set hero state by walk
            }

            // move up hero
            m_sprtHero->setPosition(ccpAdd(m_sprtHero->getPosition(), ccp(GetX(2), 0)));
            CCARRAY_FOREACH(m_redArray, obj)
            {
                CCSprite* sprtRed = (CCSprite*)obj;
                float sticX = m_sprtStick->boundingBox().getMaxX();
                if (sticX >= sprtRed->boundingBox().getMinX() && sticX <= sprtRed->boundingBox().getMaxX()) {
                    m_nScore++;
                    m_lblBonus->runAction(CCFadeOut::create(1.5f));
                    m_lScore->setString(CCString::createWithFormat("%d", m_nScore)->getCString());
                    if (m_nScore > g_nBestScore)
                    {
                        g_nBestScore = m_nScore;
                        CCUserDefault::sharedUserDefault()->setIntegerForKey("BestScore", g_nBestScore);
                        
#if (CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
                        
#else
                        SocialIOS::submitScore(g_nBestScore);
#endif
//                        SocialIOS::submitScore(g_nBestScore);
                    }
                    m_redArray->removeObject(sprtRed);
                    this->removeSprite(sprtRed);
                }
            }

            CCARRAY_FOREACH(m_blockArray, obj)
            {
                CCSprite* sprtBlock = (CCSprite*)obj;
                if (m_sprtHero->boundingBox().intersectsRect(sprtBlock->boundingBox()) && m_bFlip == true)
                {
                    CCLog("hero_maxX %f", m_sprtHero->boundingBox().getMaxX());
                    CCLog("block_minX %f", sprtBlock->boundingBox().getMinX());
                    float limitX = sprtBlock->boundingBox().getMinX();
                    float ptx = m_sprtHero->boundingBox().getMaxX() - m_sprtHero->boundingBox().size.width / 5 * 2;

                    if (ptx >= limitX) {
                        m_eState = StateFall;
                    }
                }
            }

            CCARRAY_FOREACH(m_cherryArray, obj)//Candy
            {
                CCSprite* sprtCandy = (CCSprite*)obj;
                if (m_sprtHero->boundingBox().intersectsRect(sprtCandy->boundingBox()) && m_bFlip == true) {
                    float limitX = sprtCandy->boundingBox().getMinX();
                    float ptx = m_sprtHero->boundingBox().getMaxX() - m_sprtHero->boundingBox().size.width / 5 * 2;
                    
                    if (ptx >= limitX) {
                        g_nCherryCnt++;
                        m_lblCherryCount->setString(CCString::createWithFormat("%d", g_nCherryCnt)->getCString());
                        m_cherryArray->removeObject(sprtCandy);
                        this->removeSprite(sprtCandy);
                    }
                }
            }

            // detect end of stick
            
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            
            if(m_sprtHero->getPosition().y>GetY(BLOCK_HEIGHT))
            {
                m_jumping=true;
            }else
            {
                m_jumping=false;
            }
            
            bool isOnBlock = false;
            if(m_jumping==false)
            {
                if (m_sprtHero->getPositionX() > m_sprtStick->boundingBox().getMaxX() )
                {
                    
                    CCARRAY_FOREACH(m_blockArray, obj)
                    {
                        CCSprite* sprtBlock = (CCSprite*)obj;
                        if (m_sprtHero->getPositionX() > sprtBlock->boundingBox().getMinX() && m_sprtHero->getPositionX() < sprtBlock->boundingBox().getMaxX()) {
                            isOnBlock = true;
                            m_sprtBlock = sprtBlock;
                        }
                    }
                    
                    if (isOnBlock == false) {
                        m_eState = StateFall;
                    }
                    else if (m_sprtHero->getPositionX()+GetX(3) > m_sprtBlock->boundingBox().getMaxX()) {  // if hero is on the end of new block
                        m_eState = StateReady;
                        m_sprtHero->stopAllActions();
                        m_sprtHero->runAction(CCRepeatForever::create(CCAnimate::create(g_aBoy[g_nHeroIndex])));
                    }
                }

            }
//            if (m_sprtHero->getPositionX() > m_sprtStick->boundingBox().getMaxX() )
//            {
//
//                CCARRAY_FOREACH(m_blockArray, obj)
//                {
//                    CCSprite* sprtBlock = (CCSprite*)obj;
//                    if (m_sprtHero->getPositionX() > sprtBlock->boundingBox().getMinX() && m_sprtHero->getPositionX() < sprtBlock->boundingBox().getMaxX()) {
//                        isOnBlock = true;
//                        m_sprtBlock = sprtBlock;
//                    }
//                }
//
//                if (isOnBlock == false) {
//                    m_eState = StateFall;
//                }
//                else if (m_sprtHero->getPositionX()+GetX(3) > m_sprtBlock->boundingBox().getMaxX()) {  // if hero is on the end of new block
//                    m_eState = StateReady;
//                    m_sprtHero->stopAllActions();
//                    m_sprtHero->runAction(CCRepeatForever::create(CCAnimate::create(g_aBoy[g_nHeroIndex])));
//                }
//            }
        }
            break;
        case StateFall:
        {
            if (m_sprtStick->getTag() == 2) // if hero stand
            {
                break;

            }
            // lay stick
            if (m_sprtStick->getTag() == 1) {  // laied state of stick
                m_sprtStick->runAction(CCRotateBy::create(0.5, 90));
                // set stick state by fail
                m_sprtStick->setTag(2);
            }

            m_sprtHero->runAction(CCRotateBy::create(0.5, 180));
            m_sprtHero->runAction(CCSequence::create(
                                                     CCMoveTo::create(1, ccpAdd(m_sprtHero->getPosition(), ccp(0, GetY(-50)))),
                                                     CCCallFunc::create(this, callfunc_selector(GameScene::onGameOver)),
                                                     NULL));

            //capture screen
            CCSize size = CCDirector::sharedDirector()->getWinSize();
            CCRenderTexture* texture = CCRenderTexture::create((int)size.width, (int)size.height);
            texture->setPosition(ccp(size.width/2, size.height/2));
            texture->begin();
            CCDirector::sharedDirector()->getRunningScene()->visit();
            texture->end();
            texture->saveToFile("screenshot.png", kCCImageFormatPNG);
            
        }
            break;

        default:
            break;
    }

}


#pragma mark - touch
void GameScene::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
    
}

bool GameScene::ccTouchBegan(CCTouch *touch, CCEvent *event)
{
    CCPoint location = touch->getLocationInView();
    location = CCDirector::sharedDirector()->convertToGL(location);

    m_lStart->setVisible(false);
    if (g_bTutorial)// Tutorial
        return true;

    if (m_eState == StateReady && m_sprtHero->getPositionX() < GetX(READY_X+2)) {
        m_eState = StateStretch;
    }
    
    if (m_eState == StateWalk) {
        if (!m_bFlip && !m_sprtBlock->boundingBox().intersectsRect(m_sprtHero->boundingBox())) {
            m_sprtHero->setFlipY(true);
            m_sprtHero->setAnchorPoint(ccp(0.5, 1));
            m_bFlip = true;
        }
        else
        {
            m_sprtHero->setFlipY(false);
            m_sprtHero->setAnchorPoint(ccp(0.5, 0));
            m_bFlip = false;
        }
    }
    ;
    
    return true;
}

void GameScene::ccTouchEnded(CCTouch *touch,CCEvent *event)
{
    CCPoint touchPoint = touch->getLocationInView();
    touchPoint = CCDirector::sharedDirector()->convertToGL(touchPoint);

    if (g_bTutorial)// Tutorial
        return;

    if (m_eState == StateStretch && m_sprtHero->getPositionX() < GetX(READY_X+2)) {
        m_eState = StateLay;
        SimpleAudioEngine::sharedEngine()->playEffect("snd_kick.mp3", false);
    }


}

void GameScene::ccTouchMoved(CCTouch *touch,CCEvent *event)
{
    CCPoint location = touch->getLocationInView();
    location = CCDirector::sharedDirector()->convertToGL(location);

    
}

#pragma mark - menu
void GameScene::onBack(CCObject* pSender)
{
    sfx("snd_button");
    CCScene *pScene = MainScene::scene();
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5, pScene));
}



void GameScene::onGameOver()
{
    scorePad->setVisible(false);
    m_lScore->setVisible(false);
    
    sfx("snd_dead1");
    CCUserDefault::sharedUserDefault()->setIntegerForKey("CurScore", m_nScore);
    saveGameInfo();
    CCScene *pScene = ScoreScene::scene();
    addChild(pScene, 30);
//    CCDirector::sharedDirector()->replaceScene(CCTransitionSlideInR::create(0.5, pScene));

}
