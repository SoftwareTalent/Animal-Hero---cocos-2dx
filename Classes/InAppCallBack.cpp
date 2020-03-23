//
//  InAppCallBack.cpp
//  StuntHero
//
//  Created by King on 2/4/15.
//
//
//
#include "InAppCallBack.h"
#include "GameScene.h"
#include "Global.h"
#include "MainScene.h"

//extern "C" {
//    
//#if (CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
//    
//    void Java_com_bitmonkey_stunthero_StuntHero_InAppPurchase(JNIEnv *env, jobject obj, jint responseID)
//    {
//        CCScene *pScene;
//        switch (responseID) {
//            case 0:
//                CCUserDefault::sharedUserDefault()->setIntegerForKey("Candy", g_nCherryCnt + 100);
//                pScene = MainScene::scene();
//                CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5, pScene));
//                break;
//            case 1:
//                CCUserDefault::sharedUserDefault()->setIntegerForKey("Candy", g_nCherryCnt + 300);
//                pScene = MainScene::scene();
//                CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5, pScene));
//            case 2:
//                CCUserDefault::sharedUserDefault()->setIntegerForKey("Candy", g_nCherryCnt + 500);
//                pScene = MainScene::scene();
//                CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5, pScene));
//                break;
//            case 3:
//                CCUserDefault::sharedUserDefault()->setIntegerForKey("Candy", g_nCherryCnt + 1000);
//                pScene = MainScene::scene();
//                CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5, pScene));
//                break;
//                
//            default:
//                break;
//        }
//    }
//#else
#include "SocialIOS.h"
    
    void InAppPurchase(int responseID)
    {
        CCScene *pScene;
        switch (responseID) {
            case 0:
                CCUserDefault::sharedUserDefault()->setIntegerForKey("Candy", g_nCherryCnt + 100);
                pScene = MainScene::scene();
                CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5, pScene));
                break;
            case 1:
                CCUserDefault::sharedUserDefault()->setIntegerForKey("Candy", g_nCherryCnt + 100);
                pScene = MainScene::scene();
                CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5, pScene));
                break;
            case 2:
                CCUserDefault::sharedUserDefault()->setIntegerForKey("Candy", g_nCherryCnt + 100);
                pScene = MainScene::scene();
                CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5, pScene));
                break;
            case 3:
                CCUserDefault::sharedUserDefault()->setIntegerForKey("Candy", g_nCherryCnt + 100);
                pScene = MainScene::scene();
                CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5, pScene));
                break;
            default:
                break;
        }
    }
//#endif

//}