//
//  StickHeroAppController.h
//  StickHero
//
//  Created by jinsen on 11/8/14.
//  Copyright __MyCompanyName__ 2014. All rights reserved.
//
#import <UIKit/UIKit.h>
#import "cocos2d.h"
#import "Global.h"

#import <Social/Social.h>
#import <Foundation/Foundation.h>
#import <MessageUI/MFMailComposeViewController.h>
#import <GameKit/GameKit.h>

#import <iAd/iAd.h>
#import <Chartboost/Chartboost.h>
#import <Chartboost/CBNewsfeed.h>
//#import <RevMobAds/RevMobAds.h>
#import "GameCenterManager.h"

//#import "Flurry.h"

//#import "GADInterstitial.h"
//#import "GADBannerView.h"

//#import "GAI.h"
//#import "GAIFields.h"
//#import "GAITrackedViewController.h"
//#import "GAIDictionaryBuilder.h"

#import "iAd/ADBannerView.h"

@class RootViewController;

@interface AppController : NSObject <UIAccelerometerDelegate, UIAlertViewDelegate, UITextFieldDelegate,UIApplicationDelegate, GKAchievementViewControllerDelegate, GKLeaderboardViewControllerDelegate, ADBannerViewDelegate, GameCenterManagerDelegate,ChartboostDelegate,CBNewsfeedDelegate> {
    UIWindow *window;
    RootViewController    *viewController;
//    GADInterstitial *interstitial_;
//    GADBannerView *bannerView_;
    
    ADBannerView *_adBannerView;
    BOOL _adBannerViewIsVisible;
    UIView *_contentView;
    
    GameCenterManager* gameCenterManager_;
    
    bool m_bInterestitial;
    BOOL _bannerStart;
}

@property (nonatomic, retain) UIWindow *window;
@property (nonatomic, retain) RootViewController *viewController;

@property (nonatomic, retain) GameCenterManager *gameCenterManager;
@property (nonatomic, retain) NSString* currentLeaderBoard;

@property (nonatomic, assign) ADBannerView *adBannerView;
@property (nonatomic) BOOL adBannerViewIsVisible;
@property (nonatomic) BOOL bannerStart;

//get instence
+(AppController*) get;

-(void)showActivityController;

//facebook twitter
-(void) shareFB:(int) score;
-(void) shareTwitter:(int) score;

//gamecenter - leaderboard
-(void) showLeaderboard;
-(void) showAchievements;
-(void) submitScore:(int) score;

//chartboost
-(void) showChartboost;
-(void) showChartboostMoreGame;

//revmob
-(void) showRevmob;

//admob
-(void) showAdmobFullAds;
-(void) openAdmobBannerAds;
-(void) closeAdmobBannerAds;

//iAd
-(void)RemoveiAd ;
-(void)createAdBannerView ;
-(void)showBannerView;
-(void)hideBannerView;

//rate
-(void) showRate;//Us: (BOOL) mode_one;

//save me
-(void) saveMe;

@end

