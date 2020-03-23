//
//  SocialIOS.m
//  BouncyDoge
//

#import "SocialIOS.h"
#define EMAIL_SUBJECT               @""
#define EMAIL_BODY                  @""

#define GAME_FBTEXT                 @""
#define GAME_TWITTERTEXT            @""

#import "AppController.h"
#import "MKStoreManager.h"
#import "MainScene.h"
#import "InAppCallBack.h"
@class AppController;
void SocialIOS::shareFacebook(int score){
    
    AppController* delegate=(AppController*)[[UIApplication sharedApplication] delegate];
    [delegate shareFB:score];
}

void SocialIOS::shareTwitter(int score){
    AppController* delegate=(AppController*)[[UIApplication sharedApplication] delegate];
    [delegate shareTwitter: score];
}

void SocialIOS::shareEmail(int score){
    //    AppController* delegate=(AppController*)[[UIApplication sharedApplication] delegate];
    //    [delegate sendMail:EMAIL_SUBJECT :[NSString stringWithFormat: @"I got %d points in Bouncy Doge Game", score]];
    
    
}

void SocialIOS::showLeaderboard(){
    AppController* delegate=(AppController*)[[UIApplication sharedApplication] delegate];
    [delegate showLeaderboard];
}
void SocialIOS::showAchievement(){
    AppController* delegate=(AppController*)[[UIApplication sharedApplication] delegate];
    [delegate showAchievements];
}


void SocialIOS::submitScore(int score){
    AppController* delegate=(AppController*)[[UIApplication sharedApplication] delegate];
    [delegate submitScore:score];
    //
}
void SocialIOS::submitAchievement(int index){
    //
    //    AppController* delegate=(AppController*)[[UIApplication sharedApplication] delegate];
    //
    //    switch (index) {
    //        case 0:
    //            [delegate submitAchievement:ACHIEVEMENT_5GAME];
    //            break;
    //        case 1:
    //            [delegate submitAchievement:ACHIEVEMENT_50GAME];
    //            break;
    //        case 2:
    //            [delegate submitAchievement:ACHIEVEMENT_100GAME];
    //            break;
    //        case 3:
    //            [delegate submitAchievement:ACHIEVEMENT_200GAME];
    //            break;
    //        case 4:
    //            [delegate submitAchievement:ACHIEVEMENT_500GAME];
    //            break;
    //        case 5:
    //            [delegate submitAchievement:ACHIEVEMENT_1000GAME];
    //            break;
    //        case 6:
    //            [delegate submitAchievement:ACHIEVEMENT_2500GAME];
    //            break;
    //        case 7:
    //            [delegate submitAchievement:ACHIEVEMENT_5000GAME];
    //            break;
    //        case 8:
    //            [delegate submitAchievement:ACHIEVEMENT_10POINTS];
    //            break;
    //        case 9:
    //            [delegate submitAchievement:ACHIEVEMENT_20POINTS];
    //            break;
    //        case 10:
    //            [delegate submitAchievement:ACHIEVEMENT_30POINTS];
    //            break;
    //        case 11:
    //            [delegate submitAchievement:ACHIEVEMENT_40POINTS];
    //            break;
    //        case 12:
    //            [delegate submitAchievement:ACHIEVEMENT_50POINTS];
    //            break;
    //        case 13:
    //            [delegate submitAchievement:ACHIEVEMENT_60POINTS];
    //            break;
    //        case 14:
    //            [delegate submitAchievement:ACHIEVEMENT_70POINTS];
    //            break;
    //        case 15:
    //            [delegate submitAchievement:ACHIEVEMENT_80POINTS];
    //            break;
    //        case 16:
    //            [delegate submitAchievement:ACHIEVEMENT_90POINTS];
    //            break;
    //        case 17:
    //            [delegate submitAchievement:ACHIEVEMENT_100POINTS];
    //            break;
    //        case 18:
    //            [delegate submitAchievement:ACHIEVEMENT_150POINTS];
    //            break;
    //        case 19:
    //            [delegate submitAchievement:ACHIEVEMENT_200POINTS];
    //            break;
    //
    //        default:
    //            break;
    //    }
    //
}

void SocialIOS::showChartboost()
{
    [[AppController get]showChartboost];
}

void SocialIOS::showChartboostMoreGame()
{
    [[AppController get]showChartboostMoreGame];
}

void SocialIOS::showRevmob()
{
    [[AppController get]showRevmob];
}

void SocialIOS::buyCoin(int index)
{
    switch (index) {
        case 0:
            [[MKStoreManager sharedManager] buy:0];
            break;
        case 1:
            [[MKStoreManager sharedManager] buy:1];
            break;
        case 2:
            [[MKStoreManager sharedManager] buy:2];
            break;
        case 3:
            [[MKStoreManager sharedManager] buy:3];
            break;
            
        default:
            break;
    }
    
}

void SocialIOS::BuySkier()
{
    [[MKStoreManager sharedManager] buySkier];
}

void SocialIOS::removeAds()
{
    [[MKStoreManager sharedManager] buyremoveads];
}

void SocialIOS::restoreIAP()
{
    [[MKStoreManager sharedManager] restoreFeature];
}

void SocialIOS::showRateUs(bool mode_one)
{
    [[AppController get] showRate];
}

void SocialIOS::admobFullAds(){
    [[AppController get] showAdmobFullAds];
}

void SocialIOS::showAdBanner(){
    [[AppController get]openAdmobBannerAds];
}
void SocialIOS::hideBanner(){
    [[AppController get]closeAdmobBannerAds];
}

void SocialIOS::showiAD(){
    [[AppController get] showBannerView];// createAdBannerView];
}
void SocialIOS::hideiAD(){
    [[AppController get] hideBannerView];
}

void SocialIOS::saveMe(){
    [[AppController get] saveMe];

}
void SocialIOS::purchased100Coins()
{
    InAppPurchase(0);
//    CCUserDefault::sharedUserDefault()->setIntegerForKey("Candy", g_nCherryCnt + 100);
//    CCScene *pScene = MainScene::scene();
//    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5, pScene));
}
void SocialIOS::purchased300Coins()
{
    InAppPurchase(1);
//    CCUserDefault::sharedUserDefault()->setIntegerForKey("Candy", g_nCherryCnt + 300);
//    CCScene *pScene = MainScene::scene();
//    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5, pScene));
}
void SocialIOS::purchased500Coins()
{
    InAppPurchase(2);
//    CCUserDefault::sharedUserDefault()->setIntegerForKey("Candy", g_nCherryCnt + 500);
//    CCScene *pScene = MainScene::scene();
//    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5, pScene));
}
void SocialIOS::purchased1000Coins()
{
    InAppPurchase(3);
//    CCUserDefault::sharedUserDefault()->setIntegerForKey("Candy", g_nCherryCnt + 1000);
//    CCScene *pScene = MainScene::scene();
//    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5, pScene));
}