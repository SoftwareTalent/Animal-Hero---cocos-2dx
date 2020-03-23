//
//  StickHeroAppController.mm
//  StickHero
//
//  Created by jinsen on 11/8/14.
//  Copyright __MyCompanyName__ 2014. All rights reserved.
//
#import <UIKit/UIKit.h>
#import "AppController.h"
#import "cocos2d.h"
#import "EAGLView.h"
#import "AppDelegate.h"

#import <Social/Social.h>
#import "RootViewController.h"

#import "MKStoreManager.h"

#define LEADERBOARD_ID              @"com.animalhero.highscore"
#define CHARTBOOST_ID               @"55df4ee15b14534bf3885c6a"
#define CHARTBOOST_SIG              @"ef40316118dbd60f3fef9e31e10a203979969ef5"

//// please add your id
#define ADMOB_BANNER_ID    @"ca-app-pub-6556897057775245/9564175019"
#define ADMOB_INTERSTITIAL_ID @"ca-app-pub-1537540977113204/1423416577"

#define REVMOB_ID                   @"545d4f2eef6da0b25eaeef5d"

#define FLURRY_KEY                  @"8NN42DTP3Q6579CRDXRQ"

#define GOOGLE_KEY                  @"UA-51234110-29"

#define APP_ID                      @"1007915217"
#define RATE_MESSAGE                @"Click below to give us a 5-star rating!"
#define RATE_TITLE                  @"Having fun?"

@implementation AppController

@synthesize window;
@synthesize viewController;

@synthesize adBannerView;
@synthesize adBannerViewIsVisible;
@synthesize bannerStart;

#pragma mark -
#pragma mark Application lifecycle

// cocos2d application instance
static AppDelegate s_sharedApplication;

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    
    // Override point for customization after application launch.
    
    //    [self createAdBannerView];
    bannerStart = NO;
    
    // Add the view controller's view to the window and display.
    window = [[UIWindow alloc] initWithFrame: [[UIScreen mainScreen] bounds]];
    EAGLView *__glView = [EAGLView viewWithFrame: [window bounds]
                                     pixelFormat: kEAGLColorFormatRGBA8
                                     depthFormat: GL_DEPTH_COMPONENT16
                              preserveBackbuffer: NO
                                      sharegroup: nil
                                   multiSampling: NO
                                 numberOfSamples:0 ];
    
    // Use RootViewController manage EAGLView
    viewController = [[RootViewController alloc] initWithNibName:nil bundle:nil];
    viewController.wantsFullScreenLayout = YES;
    viewController.view = __glView;
    
    // Set RootViewController to window
    if ( [[UIDevice currentDevice].systemVersion floatValue] < 6.0)
    {
        // warning: addSubView doesn't work on iOS6
        [window addSubview: viewController.view];
    }
    else
    {
        // use this method on ios6
        [window setRootViewController:viewController];
    }
    
    [window makeKeyAndVisible];
    
    [[UIApplication sharedApplication] setStatusBarHidden: YES];
    
    [self initGameCenter];
    
//    [Flurry setCrashReportingEnabled:YES];
//    [Flurry startSession:FLURRY_KEY];
//    [Flurry logEvent:@"Top Color Match was successfully created."];
//    
    [self registerChartboost];
//    [RevMobAds startSessionWithAppID:REVMOB_ID];
    
    //    [self trackGoogle];
    //    [self trackEvent];
    
    cocos2d::CCApplication::sharedApplication()->run();
    return YES;
}


- (void)applicationWillResignActive:(UIApplication *)application {
    /*
     Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
     Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
     */
    cocos2d::CCDirector::sharedDirector()->pause();
}

- (void)applicationDidBecomeActive:(UIApplication *)application {
    /*
     Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
     */
    
    cocos2d::CCDirector::sharedDirector()->resume();
}

- (void)applicationDidEnterBackground:(UIApplication *)application {
    /*
     Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later.
     If your application supports background execution, called instead of applicationWillTerminate: when the user quits.
     */
    cocos2d::CCApplication::sharedApplication()->applicationDidEnterBackground();
}

- (void)applicationWillEnterForeground:(UIApplication *)application {
    /*
     Called as part of  transition from the background to the inactive state: here you can undo many of the changes made on entering the background.
     */
    cocos2d::CCApplication::sharedApplication()->applicationWillEnterForeground();
}

- (void)applicationWillTerminate:(UIApplication *)application {
    /*
     Called when the application is about to terminate.
     See also applicationDidEnterBackground:.
     */
}


#pragma mark -
#pragma mark Memory management

- (void)applicationDidReceiveMemoryWarning:(UIApplication *)application {
    /*
     Free up as much memory as possible by purging cached data objects that can be recreated (or reloaded from disk) later.
     */
    cocos2d::CCDirector::sharedDirector()->purgeCachedData();
}


- (void)dealloc {
    [super dealloc];
}


#pragma mark - get Instance
+(AppController*) get
{
    return (AppController*) [[UIApplication sharedApplication] delegate];
}

#pragma mark -
-(void)showActivityController
{
    NSString *text =NSLocalizedString(@"Bet you can't beat my score!", nil);
    NSURL *url = [NSURL URLWithString:@""];
    UIActivityViewController *activityConroller = [[UIActivityViewController alloc] initWithActivityItems:@[text, url] applicationActivities:nil];
    [self.viewController presentViewController:activityConroller animated:YES completion:nil];
}

#pragma mark - Rate Us
-(void)showRate
{
    NSUserDefaults *userDefaults = [NSUserDefaults standardUserDefaults];
    if([userDefaults boolForKey:@"Rated"])
        return;
    
    UIAlertView *rateAlert = [[UIAlertView alloc]initWithTitle:RATE_TITLE message:RATE_MESSAGE delegate:self cancelButtonTitle:@"Remind me later" otherButtonTitles:@"Rate",nil];
    rateAlert.tag = 1001;
    [rateAlert show];
    [rateAlert release];
}

#pragma mark Save Me
-(void) saveMe
{
    //    CCUserDefault::sharedUserDefault()->setBoolForKey("waitSaveMe", true);
    //
    //    int nExtra = CCUserDefault::sharedUserDefault()->getIntegerForKey("totalCoin", 0);
    //
    //    g_bEnExpert = CCUserDefault::sharedUserDefault()->getBoolForKey("EnableExpert", false);
    //    g_bExpertSkier = CCUserDefault::sharedUserDefault()->getBoolForKey("SkierType", false);
    //
    //    // switch skier condition
    //    NSString *switchSkier;
    //    if (g_bExpertSkier) {
    //        switchSkier = @"Switch to Novice Skier";
    //    }
    //    else
    //        switchSkier = @"Switch to Expert Skier";
    //
    //
    //    UIAlertView *rateAlert = [[UIAlertView alloc]initWithTitle:@"OUCH!" message:[NSString stringWithFormat:@"You can continue from this spot by using an extra life.\n\n%d extra lives remaining.", nExtra] delegate:self cancelButtonTitle:@"End Game" otherButtonTitles:@"Use Extra Life", @"Purchase Extra Lives", switchSkier,nil];
    //    rateAlert.tag = 2001;
    //    [rateAlert show];
    //    [rateAlert release];
    
    
}

#pragma mark Alerts

-(void) alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex {
    if(alertView.tag == 1001)  // rate
    {
        if(buttonIndex == 1)
        {
            NSURL *url = [NSURL URLWithString:[self getRateURL]];
            if (![[UIApplication sharedApplication] openURL:url])
                NSLog(@"%@%@",@"Failed to open url:",[url description]);
            else{
                NSUserDefaults *userDefaults = [NSUserDefaults standardUserDefaults];
                [userDefaults setBool:YES forKey:@"Rated"];
                g_bRated = true;
                //                [g_lMainLayer updateRateButton];
            }
        }
        else
        {
            NSUserDefaults *userDefaults = [NSUserDefaults standardUserDefaults];
            [userDefaults setBool:NO forKey:@"Rated"];
        }
    }
    else if(alertView.tag == 2001)  // game over
    {
        //        if(buttonIndex == 0)  // End Game
        //        {
        //            CCUserDefault::sharedUserDefault()->setBoolForKey("waitSaveMe", false);
        //            CCUserDefault::sharedUserDefault()->setBoolForKey("decideSaveMe", false);
        //        }
        //        else if(buttonIndex == 1) // Extra life
        //        {
        //            int nExtra = CCUserDefault::sharedUserDefault()->getIntegerForKey("totalCoin", 0);
        //            if (nExtra > 0) {
        //                CCUserDefault::sharedUserDefault()->setBoolForKey("waitSaveMe", false);
        //                CCUserDefault::sharedUserDefault()->setBoolForKey("decideSaveMe", true);
        //                int totalCoin = CCUserDefault::sharedUserDefault()->getIntegerForKey("totalCoin", 0);
        //                CCUserDefault::sharedUserDefault()->setIntegerForKey("totalCoin", totalCoin - 1);
        //            }
        //            else
        //            {
        //                [self saveMe];
        //            }
        //        }
        //        else if(buttonIndex == 2)  // Purchase Extra Lives
        //        {
        //            [self saveMe];
        //            [[MKStoreManager sharedManager] buy:0];
        //        }
        //        else  //Switch Skier
        //        {
        //            if (g_bEnExpert) {
        //                g_bExpertSkier = !g_bExpertSkier;
        //                CCUserDefault::sharedUserDefault()->setBoolForKey("SkierType", g_bExpertSkier);
        //            }
        //            else
        //            {
        //                [[MKStoreManager sharedManager] buySkier];
        //            }
        //            [self saveMe];
        //
        //        }
    }
}
-(NSString *)getRateURL
{
    return @"https://itunes.apple.com/us/app/stick-hero-zoro-style/id939297978?ls=1&mt=8";
    NSString *currSysVer = [[UIDevice currentDevice] systemVersion];
    
    if([currSysVer compare:@"7.0" options:NSNumericSearch] != NSOrderedAscending)
        return [NSString stringWithFormat:@"itms-apps://itunes.apple.com/app/id%@", APP_ID];
    else
        return [NSString stringWithFormat:@"itms-apps://ax.itunes.apple.com/WebObjects/MZStore.woa/wa/viewContentsUserReviews?type=Purple+Software&id=%@", APP_ID];
}

#pragma mark - GameCenter
- (BOOL)initGameCenter {
    
    if(self.gameCenterManager != nil)
        return NO;
    self.currentLeaderBoard = LEADERBOARD_ID;
    
    if ([GameCenterManager isGameCenterAvailable])
    {
        self.gameCenterManager = [[[GameCenterManager alloc] init] autorelease];
        [self.gameCenterManager setDelegate:self];
        [self.gameCenterManager authenticateLocalUser];
        return YES;
    }
    else
    {
        NSString *message = @"This IOS version is not available Game Center.";
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Message!" message:message delegate:self cancelButtonTitle:@"YES" otherButtonTitles: nil];
        alert.tag = 1;
        [alert show];
        [alert release];
    }
    
    return NO;
}

- (void) showLeaderboard{
    GKLeaderboardViewController *leaderboardViewController = [[GKLeaderboardViewController alloc] init];
    if (leaderboardViewController != NULL) {
        leaderboardViewController.category = self.currentLeaderBoard;
        leaderboardViewController.timeScope = GKLeaderboardTimeScopeAllTime;
        leaderboardViewController.leaderboardDelegate = self;
        
        [self.viewController presentModalViewController:leaderboardViewController animated:YES];
        
    }
}

- (void) showAchievements{
    GKAchievementViewController *achievements = [[GKAchievementViewController alloc] init];
    if (achievements != NULL){
        achievements.achievementDelegate = self;
        [self.viewController presentModalViewController: achievements animated: YES];
    }
}

- (void)submitScore: (int) uploadScore{
    if( uploadScore > 0){
        
        if ([GameCenterManager isGameCenterAvailable]) {
            [self.gameCenterManager reportScore:uploadScore  forCategory: self.currentLeaderBoard];
            [self.gameCenterManager reloadHighScoresForCategory:self.currentLeaderBoard];
        }
    }
}

#pragma mark GameKit delegate

-(void) achievementViewControllerDidFinish:(GKAchievementViewController *)viewController
{
    [self.viewController dismissModalViewControllerAnimated:YES];
    [viewController release];
}

-(void) leaderboardViewControllerDidFinish:(GKLeaderboardViewController *)viewController
{
    [self.viewController dismissModalViewControllerAnimated:YES];
    [viewController release];
}

#pragma mark GameCenterManager protocol

- (void) scoreReported: (NSError*) error
{
    NSString *message = @"Score submited succesfully to Game Center.";
    //    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Congratulations!" message:message delegate:self cancelButtonTitle:@"YES" otherButtonTitles: nil];
    //    alert.tag = 1;
    //    [alert show];
    //    [alert release];
    NSLog(@"%@", message);
}

- (void) processGameCenterAuth: (NSError*) error
{
    if (error == NULL) {
        NSLog(@"Game Center Auth success!");
    }
    else {
        NSLog(@"Game Center Auth faild!");
    }
}

- (void) reloadScoresComplete: (GKLeaderboard*) leaderBoard error: (NSError*) error
{
    if (error == NULL) {
        NSLog(@"Game Center reload socores success!");
    }
    else {
        NSLog(@"Game Center reload socores faild!");
    }
}

#pragma mark - Share
-(void) shareFB:(int) score
{
    UIImage *img = [self screenshotWithStartNode:@"screenshot.png"];
    NSString *text = [NSString stringWithFormat:@"I passed %d gates in Stick Stick Hero Zoro Style.\n Can you beat my score?\n https://itunes.apple.com/us/app/stick-hero-zoro-style/id939297978?ls=1&mt=8", score];
    [self shareFaceBook:img Text:text];
    
}
-(void) shareTwitter:(int) score
{
    
}

-(void)shareFaceBook:(UIImage *) image Text: (NSString *)text
{
    SLComposeViewController *controller = [SLComposeViewController composeViewControllerForServiceType:SLServiceTypeFacebook];
    
    [controller setInitialText:text];
    //[controller addURL:[NSURL URLWithString:@"http://www.appcoda.com"]];
    [controller addImage:image];
    
    [self.viewController presentViewController:controller animated:YES completion:nil];
}

-(UIImage *) screenshotWithStartNode:(NSString*)filename
{
    //get the path to the Documents directory
    NSArray* paths = NSSearchPathForDirectoriesInDomains
    (NSDocumentDirectory, NSUserDomainMask, YES);
    NSString* documentsDirectory = [paths objectAtIndex:0];
    NSString* screenshotPath = [documentsDirectory stringByAppendingPathComponent:filename];
    
    //get the screenshot as raw data
    NSData *data = [NSData dataWithContentsOfFile:screenshotPath];
    //create an image from the raw data
    UIImage *img = [UIImage imageWithData:data];
    return img;
}

#pragma mark - Chartboost

-(void)registerChartboost
{
//    [Chartboost startWithAppId:CHARTBOOST_ID appSignature:CHARTBOOST_SIG delegate:self];
//
    [Chartboost startWithAppId:CHARTBOOST_ID appSignature:CHARTBOOST_SIG delegate:self];
////        [Chartboost startWithAppId:@"543ba17fc26ee45e85318dd9" appSignature:@"633611aebdc0593b86d2a8838d5a42b9009376f2" delegate:self];
//    
    [Chartboost cacheInterstitial:CBLocationDefault];
////        [Chartboost cacheRewardedVideo:CBLocationMainMenu];
////        [Chartboost cacheMoreApps:CBLocationHomeScreen];
//    
////     Begin Newsfeed initialization.
    [CBNewsfeed startWithDelegate: self];
}


-(void) showChartboost
{
    [Chartboost showInterstitial:CBLocationHomeScreen];
}

-(void) showChartboostMoreGame
{
        [[Chartboost sharedChartboost] showMoreApps];
}

#pragma mark - revmob
-(void) showRevmob
{
//    [[RevMobAds session] showFullscreen];
}

//-(void) removeAds
//{
//    [[MKStoreManager sharedManager] removeAds];
//}
//
//-(void) restoreIAP
//{
//    [[MKStoreManager sharedManager] restoreFunc];
//}
#pragma mark - admob

//- (void) preloadInterstitial
//{
//    interstitial_ = [[GADInterstitial alloc] init];
//    interstitial_.adUnitID = ADMOB_INTERSTITIAL_ID;//@"ca-app-pub-6046034785851961/xxxxxx";
//    interstitial_.delegate = self;
//    //[interstitial_ presentFromRootViewController:navController_];
//    [interstitial_ loadRequest:[GADRequest request]];
//    
//}
//
//- (void) interstitialDidReceiveAd:(GADInterstitial *)ad
//{
//    if (!m_bInterestitial)
//    {
//        //        [interstitial_ presentFromRootViewController:viewController];
//        m_bInterestitial = true;
//    }
//}
//- (void) interstitialDidDismissScreen:(GADInterstitial *)ad
//{
//    [self preloadInterstitial];
//}


//- (void) interstitial:(GADInterstitial *)ad didFailToReceiveAdWithError:(GADRequestError *)error
//{
//    [NSTimer scheduledTimerWithTimeInterval:2.f target:self selector:@selector(preloadInterstitial) userInfo:Nil repeats:NO];
//}

- (void) showAdmobFullAds
{
    //    interstitial_ = [[GADInterstitial alloc] init];
    //    interstitial_.adUnitID = ADMOB_INTERSTITIAL_ID;//@"ca-app-pub-6046034785851961/xxxxxx";
    //    interstitial_.delegate = self;
    //    //[interstitial_ presentFromRootViewController:navController_];
    //    [interstitial_ loadRequest:[GADRequest request]];
    
//    if (m_bInterestitial) [interstitial_ presentFromRootViewController:viewController];
}

-(void) openAdmobBannerAds{
//    if(!bannerView_){
//        
//        //        CGSize size1 = [UIScreen mainScreen].bounds.size;
//        bannerView_ = [[GADBannerView alloc] initWithFrame:CGRectMake(0,  0, viewController.view.frame.size.width, 60)];
//        bannerView_.center = CGPointMake( viewController.view.frame.size.width/2.f, viewController.view.frame.size.height - 30);//viewController.view.frame.size.height-30 );
//        bannerView_.adUnitID = ADMOB_BANNER_ID;
//        
//        bannerView_.rootViewController= viewController;
//        
//        //[UIViewController:addSubview:bannerView_];
//        [viewController.view addSubview:bannerView_];
//        // Initiate a generic request to load it with an ad.
//        [bannerView_ loadRequest:[GADRequest request]];
//    }
}
-(void) closeAdmobBannerAds{
//    if(bannerView_ != nil){
//        [bannerView_ removeFromSuperview];
//        [bannerView_ release];
//        bannerView_ = nil;
//    }
}
#pragma mark - iAd
- (int)getBannerHeight:(UIDeviceOrientation)orientation {
    if (UIInterfaceOrientationIsLandscape(orientation)) {
        return 32;
    } else {
        return 50;
    }
}

- (int)getBannerHeight {
    return [self getBannerHeight:[UIDevice currentDevice].orientation];
}


-(void)RemoveiAd
{
    _adBannerViewIsVisible = false;
    
    [self dismissAdView];
    //[self.adBannerView removeFromSuperview];
}


#pragma mark - ADBannerViewDelegate

- (void)createAdBannerView
{
    //    _adBannerViewIsVisible = true;
    
    // --- WARNING ---
    // If you are planning on creating banner views at runtime in order to support iOS targets that don't support the iAd framework
    // then you will need to modify this method to do runtime checks for the symbols provided by the iAd framework
    // and you will need to weaklink iAd.framework in your project's target settings.
    // See the iPad Programming Guide, Creating a Universal Application for more information.
    // http://developer.apple.com/iphone/library/documentation/general/conceptual/iPadProgrammingGuide/Introduction/Introduction.html
    // --- WARNING ---
    
    // Depending on our orientation when this method is called, we set our initial content size.
    // If you only support portrait or landscape orientations, then you can remove this check and
    // select either ADBannerContentSizeIdentifierPortrait (if portrait only) or ADBannerContentSizeIdentifierLandscape (if landscape only).
    
    UIInterfaceOrientation orientation = [[UIApplication sharedApplication] statusBarOrientation];
    
    NSString *contentSize;
    if (&ADBannerContentSizeIdentifierPortrait != nil)
    {
        contentSize = UIInterfaceOrientationIsPortrait(orientation) ? ADBannerContentSizeIdentifierPortrait : ADBannerContentSizeIdentifierLandscape;
    }
    else
    {
        // user the older sizes
        contentSize = UIInterfaceOrientationIsPortrait(orientation) ? ADBannerContentSizeIdentifier320x50 : ADBannerContentSizeIdentifier480x32;
    }
    
    // Calculate the intial location for the banner.
    // We want this banner to be at the bottom of the view controller, but placed
    // offscreen to ensure that the user won't see the banner until its ready.
    // We'll be informed when we have an ad to show because -bannerViewDidLoadAd: will be called.
    CGRect frame;
    frame.size = [ADBannerView sizeFromBannerContentSizeIdentifier:contentSize];
    frame.origin = CGPointMake(0.0f, -frame.size.height);// CGRectGetMaxY(self.viewController.view.bounds));
    
    // Now to create and configure the banner view
    self.adBannerView = [[ADBannerView alloc] initWithFrame:frame];
    // Set the delegate to self, so that we are notified of ad responses.
    self.adBannerView.delegate = self;
    // Set the autoresizing mask so that the banner is pinned to the bottom
    self.adBannerView.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight | UIViewAutoresizingFlexibleTopMargin;
    // Since we support all orientations in this view controller, support portrait and landscape content sizes.
    // If you only supported landscape or portrait, you could remove the other from this set.
    
    self.adBannerView.requiredContentSizeIdentifiers = (&ADBannerContentSizeIdentifierPortrait != nil) ?
    [NSSet setWithObjects:ADBannerContentSizeIdentifierPortrait, ADBannerContentSizeIdentifierLandscape, nil] :
    [NSSet setWithObjects:ADBannerContentSizeIdentifier320x50, ADBannerContentSizeIdentifier480x32, nil];
    
    // At this point the ad banner is now be visible and looking for an ad.
    
    
}

- (void)bannerViewDidLoadAd:(ADBannerView *)banner
{
    return;
    if (!_adBannerViewIsVisible) {
        _adBannerViewIsVisible = true;
        [self.viewController.view addSubview:self.adBannerView];
        if (bannerStart == YES) {
            [self hideBannerView];
        }
        bannerStart = YES;
        
    }
    
}

- (void)bannerView:(ADBannerView *)banner didFailToReceiveAdWithError:(NSError *)error
{
    if (_adBannerViewIsVisible)
    {
        _adBannerViewIsVisible = NO;
        
        [UIView beginAnimations:@"animateAdBannerOff" context:NULL];
        CGRect s = [[UIScreen mainScreen] bounds];
        [UIView beginAnimations:@"animateAdBannerOff" context:NULL];
        self.adBannerView.frame = CGRectOffset(self.adBannerView.frame, 0, s.size.height );
        [UIView commitAnimations];
    }
}


-(void)showBannerView
{
    CGSize size = [UIScreen mainScreen].bounds.size;
    float bannerheight = 50;
    if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad)
        bannerheight = 65;
    adBannerView = [[ADBannerView alloc] initWithFrame:CGRectMake(0, 0, size.width,bannerheight)];
    adBannerView.delegate = self;
    [viewController.view addSubview:adBannerView];
    CGRect adFrame = adBannerView.frame;
    adBannerView.frame = adFrame;
}


-(void)hideBannerView
{
    if (self.adBannerView)
    {
        CGSize s = self.viewController.view.frame.size;
        
        CGRect frame = self.adBannerView.frame;
        frame.origin.y = - frame.size.height;// 0.0f;
        frame.origin.x = (s.width/2.0f - frame.size.width/2.0f);
        
        self.adBannerView.frame = frame;
        
        [UIView animateWithDuration:0.5
                              delay:0.1
                            options: UIViewAnimationCurveEaseOut
                         animations:^
         {
             CGSize s = self.viewController.view.frame.size;
             
             CGRect frame = self.adBannerView.frame;
             frame.origin.y = - frame.size.height;// frame.origin.y +  frame.size.height;
             frame.origin.x = (s.width/2.0f - frame.size.width/2.0f);
         }
                         completion:^(BOOL finished)
         {
         }];
    }
}


-(void)dismissAdView
{
    if (self.adBannerView)
    {
        [UIView animateWithDuration:0.5
                              delay:0.1
                            options: UIViewAnimationCurveEaseOut
                         animations:^
         {
             CGSize s = self.viewController.view.frame.size;
             
             CGRect frame = self.adBannerView.frame;
             frame.origin.y = -frame.size.height; //frame.origin.y + frame.size.height ;
             frame.origin.x = (s.width/2.0f - frame.size.width/2.0f);
             self.adBannerView.frame = frame;
         }
                         completion:^(BOOL finished)
         {
             [self.adBannerView removeFromSuperview];
             self.adBannerView.delegate = nil;
             self.adBannerView = nil;
             
         }];
    }
    
    
}


#pragma mark - Analysis

/*
 -(void)trackGoogle
 {
 [GAI sharedInstance].trackUncaughtExceptions = YES;
 
 // Optional: set Google Analytics dispatch interval to e.g. 20 seconds.
 [GAI sharedInstance].dispatchInterval = 20;
 
 // Optional: set Logger to VERBOSE for debug information.
 [[[GAI sharedInstance] logger] setLogLevel:kGAILogLevelVerbose];
 
 // Initialize tracker. Replace with your tracking ID.
 [[GAI sharedInstance] trackerWithTrackingId:GOOGLE_KEY];
 }
 
 -(void)trackEvent
 {
 id<GAITracker> tracker = [[GAI sharedInstance] defaultTracker];
 [tracker set:kGAIScreenName value:@"Amazing Rush was successfully created"];
 [tracker send:[[GAIDictionaryBuilder createAppView] build]];
 }
 
 */

@end

