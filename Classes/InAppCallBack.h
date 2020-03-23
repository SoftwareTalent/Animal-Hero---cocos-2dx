//
//  InAppCallBack.h
//  StuntHero
//
//  Created by King on 2/4/15.
//
//

#ifndef __StuntHero__InAppCallBack__
#define __StuntHero__InAppCallBack__

//#if (CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
//#include <jni.h>
//#include "platform/android/jni/JniHelper.h"
//#endif
//
////#ifdef __cplusplus
//extern "C" {
//    //#endif
//    
//#if (CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
//    void Java_com_bitmonkey_StuntHero_InAppPurchase(JNIEnv *env, jobject obj, jint responseID);
//#else
    void InAppPurchase(int responseID);
//#endif
//    
//    //#ifdef __cplusplus
//}
#endif /* defined(__RocketBlocker__InAppCallBack__) */