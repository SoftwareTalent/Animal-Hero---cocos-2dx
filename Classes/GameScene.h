#ifndef __GameScene_SCENE_H__
#define __GameScene_SCENE_H__

#include "Global.h"

typedef enum StateTypeEnum{
    StateReady = 0,
    StateStretch,
    StateLay,
    StateWalk,
    StateFall,
    State
}StateType;

class GameScene : public CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static CCScene* scene();
    // implement the "static node()" method manually
    CREATE_FUNC(GameScene);
    
    void initWorld();
    void addStick();
    void addBlock();
    void removeSprite(CCSprite* bomb);
    void setWalkState();

    void updateState(float dt);
    
    //touch
    virtual void registerWithTouchDispatcher();
    virtual bool ccTouchBegan(CCTouch *touch,CCEvent *event);
    virtual void ccTouchMoved(CCTouch *touch,CCEvent *event);
    virtual void ccTouchEnded(CCTouch *touch,CCEvent *event);

    void onBack(CCObject* pSender);
    void onJump(CCObject* pSender);
    void onGameOver();
public:
    int m_nScore;
    bool m_bPause;
    bool m_bFlip;
    bool m_jumping;
    int m_jumpNum;
    StateType m_eState;
    CCArray *m_blockArray, *m_stickArray, *m_cherryArray,*m_redArray;
    CCLabelTTF *m_lScore;
    CCLabelTTF *m_lStart;
    CCLabelTTF *m_lblCherryCount;
    CCLabelTTF *m_lblBonus;

//    CCSprite *m_sprtBg[2];

    CCSprite *m_sprtHero;
    CCSprite *m_sprtStick;
    CCSprite *m_sprtBlock;

    CCSprite *scorePad;
    CCSprite *m_sprtHandOn, *m_sprtHandOff;
    
    CCSprite *m_sprCherry;

    float m_fTutorialBarLength;
    
};

#endif // __GameScene_SCENE_H__
