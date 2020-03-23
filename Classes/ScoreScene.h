#ifndef __ScoreScene_SCENE_H__
#define __ScoreScene_SCENE_H__

#include "Global.h"

class ScoreScene : public cocos2d::CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void onBack(CCObject* pSender);
    void onRetry(CCObject* pSender);
    void onScore(CCObject* pSender);
    void onShare(CCObject* pSender);
    void onNoads(CCObject* pSender);
    void onRate(CCObject* pSender);
    
    // implement the "static node()" method manually
    CREATE_FUNC(ScoreScene);
};

#endif // __ScoreScene_SCENE_H__
