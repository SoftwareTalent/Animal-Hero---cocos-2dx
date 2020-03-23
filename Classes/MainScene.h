#ifndef __MAIN_SCENE_H__
#define __MAIN_SCENE_H__

#include "Global.h"

class MainScene: public CCLayer
{
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();

    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static CCScene* scene();
    CCMenu *ShopMenu;
    CCMenu *HeroMenu;
    CCSprite *m_sprHeroBg, *m_sprShopBg;
    CCLabelTTF *m_lblCherryCnt;
    // a selector callback
    void onPlay(CCObject* pSender);
    void onHelp(CCObject* pSender);
    void onSound(CCObject* pSender);
    void onShop(CCObject* pSender);
    void onHeroSelect(CCObject* pSender);

    void onBuy(CCObject* pSender);
    void onUnlockHero(CCObject* pSender);

    virtual void registerWithTouchDispatcher();
    virtual bool ccTouchBegan(CCTouch *touch,CCEvent *event);

    void createHeroMenu();
    void createShopMenu();
    void showHeroMenu();
    void hideHeroMenu();
    void showShopMenu();
    void hideShopMenu();
    void reloadScene();
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(MainScene);
    
    
private:
    CCMenuItemToggle *m_soundItem;
};

#endif // __HELLOWORLD_SCENE_H__
