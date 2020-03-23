#import "MKStoreManager.h"
#import "AppDelegate.h"
#import "AppController.h"
#import "SimpleAudioEngine.h"
#import "MainScene.h"
//#import "SHKActivityIndicator.h"
#include "cocos2d.h"
#import "SocialIOS.h"
USING_NS_CC;

@implementation MKStoreManager

@synthesize purchasableObjects;
@synthesize storeObserver;
@synthesize delegate;
@synthesize isPurchasing;
@synthesize alertView;

// all your features should be managed one and only by StoreManager

static NSString *feature1 = @"com.animalhero.extra1_100";
static NSString *feature2 = @"com.animalhero.extra1_300";
static NSString *feature3 = @"com.animalhero.extra1_500";
static NSString *feature4 = @"com.animalhero.extra1_1000";
//static NSString *featureSkier = @"com.pop.tapski.skier";
//static NSString *featureADS = @"com.Kiddles.ads";

BOOL bPurchasedADS = NO;
BOOL bPurchasedSkier = NO;

static MKStoreManager* _sharedStoreManager; // self

- (void)dealloc
{
	[_sharedStoreManager release];
	[storeObserver release];
	[super dealloc];
}


+ (BOOL) isPurchasedSkier{
    [MKStoreManager loadPurchases];
    return bPurchasedSkier;
}

+ (BOOL) isPurchasedADS{
    [MKStoreManager loadPurchases];
    return bPurchasedADS;
}

+ (MKStoreManager*)sharedManager
{
	NSLog(@"pass sharedManager");
	@synchronized(self) {
		
        if (_sharedStoreManager == nil) {
			
            [[self alloc] init]; // assignment not done here
			_sharedStoreManager.purchasableObjects = [[NSMutableArray alloc] init];
            _sharedStoreManager.isPurchasing = NO;
            CCUserDefault::sharedUserDefault()->setBoolForKey("isPurchasing", NO);
			[_sharedStoreManager requestProductData];
			
			[MKStoreManager loadPurchases];
			_sharedStoreManager.storeObserver = [[MKStoreObserver alloc] init];
			[[SKPaymentQueue defaultQueue] addTransactionObserver:_sharedStoreManager.storeObserver];
        }
    }
    
    return _sharedStoreManager;
}

- (UIAlertView*) alertView
{
    if (alertView != nil) {
        return alertView;
    }
    alertView = [[UIAlertView alloc] initWithTitle:nil message:@"Please wait..." delegate:self cancelButtonTitle:nil otherButtonTitles: nil];
    return alertView;
}


#pragma mark Singleton Methods

+ (id)allocWithZone:(NSZone *)zone
{
    @synchronized(self)
    {
        if (_sharedStoreManager == nil)
        {
            _sharedStoreManager = [super allocWithZone:zone];
            return _sharedStoreManager;  // assignment and return on first allocation
        }
    }

    return nil; //on subsequent allocation attempts return nil
}


- (id)copyWithZone:(NSZone *)zone
{
    return self;	
}

- (void) requestProductData
{
	SKProductsRequest *request= [[SKProductsRequest alloc] initWithProductIdentifiers: 
								 [NSSet setWithObjects: feature1, feature2, feature3,feature4, nil]]; // add any other product here
	request.delegate = self;
	[request start];
}


- (void)productsRequest:(SKProductsRequest *)request didReceiveResponse:(SKProductsResponse *)response
{
	[purchasableObjects addObjectsFromArray:response.products];
	// populate your UI Controls here
	for(int i=0;i<[purchasableObjects count];i++)
	{
		SKProduct *product = [purchasableObjects objectAtIndex:i];
		NSLog(@"Feature: %@, Cost: %f, ID: %@",[product localizedTitle],
			  [[product price] doubleValue], [product productIdentifier]);
	}
	
	[request autorelease];
}

- (void) buyFeature:(NSString*) storyID
{
    if (self.isPurchasing) {
        return;
    }
	if ([SKPaymentQueue canMakePayments])
	{
		SKPayment *payment = [SKPayment paymentWithProductIdentifier:storyID];
		[[SKPaymentQueue defaultQueue] addPayment:payment];
        
        self.isPurchasing = YES;
        CCUserDefault::sharedUserDefault()->setBoolForKey("isPurchasing", YES);
//        [[SHKActivityIndicator currentIndicator] displayActivity:@"Processing Request..."];
	}
	else
	{
        [self.alertView dismissWithClickedButtonIndex:0 animated:NO];
        
		UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Smiley Bird Purchase" message:@"You are not authorized to purchase from AppStore"
													   delegate:self cancelButtonTitle:@"OK" otherButtonTitles: nil];
		[alert show];
		[alert release];
	}
}

- (void) buy:(int) index
{
    switch (index) {
        case 0:
            [self buyFeature:feature1];
            break;
        case 1:
            [self buyFeature:feature2];
            break;
        case 2:
            [self buyFeature:feature3];
            break;
        case 3:
            [self buyFeature:feature4];
            break;
            
        default:
            break;
    }
}

- (void) buySkier
{
    if (!([MKStoreManager isPurchasedSkier])) {
//        [self buyFeature:featureSkier];
    }
}

- (void) buyremoveads
{
    if (!([MKStoreManager isPurchasedADS])) {
//        [self buyFeature:featureADS];
    }
}

-(void) paymentCanceled
{
    [self.alertView dismissWithClickedButtonIndex:0 animated:NO];
    
	if([delegate respondsToSelector:@selector(failed)])
		[delegate failed];
    [self performSelector:@selector(didComplete:) withObject:@"Purchase Canceled!" afterDelay:0.4];
}

- (void)completeTransaction:(SKPaymentTransaction *)transaction
{
    NSLog(@"SKPaymentTransactionStatePurchased");
    
    NSLog(@"Transaction Identifier : %@", transaction.transactionIdentifier);
    NSLog(@"Transaction Date : %@", transaction.transactionDate);
    
    [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
    
    [self purchasedProducts:transaction];
    
    [self performSelector:@selector(didComplete:) withObject:@"Purchase Successed!" afterDelay:0.4];
}

- (void) failedTransaction: (SKPaymentTransaction *)transaction
{
    [self.alertView dismissWithClickedButtonIndex:0 animated:NO];
    
	if([delegate respondsToSelector:@selector(failed)])
		[delegate failed];

	NSString *messageToBeShown = [NSString stringWithFormat:@"Reason: %@, You can try: %@", [transaction.error localizedFailureReason], [transaction.error localizedRecoverySuggestion]];
	UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Unable to complete your purchase" message:messageToBeShown
												   delegate:self cancelButtonTitle:@"OK" otherButtonTitles: nil];
	[alert show];
	[alert release];
}

-(void) provideContent: (NSString*) productIdentifier
{
    int totalCoin = CCUserDefault::sharedUserDefault()->getIntegerForKey("totalCoin", 0);
    loadGameInfo();
    //consumable purchase
    if([productIdentifier isEqualToString:feature1])
    {
        SocialIOS::purchased100Coins();
//        CCUserDefault::sharedUserDefault()->setIntegerForKey("Candy", g_nCherryCnt + 100);
//        CCScene *pScene = MainScene::scene();
//        CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5, pScene));

    }
    if([productIdentifier isEqualToString:feature2])
    {
        SocialIOS::purchased300Coins();
//        CCUserDefault::sharedUserDefault()->setIntegerForKey("Candy", g_nCherryCnt + 300);
//        CCScene *pScene = MainScene::scene();
//        CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5, pScene));

    }
    if([productIdentifier isEqualToString:feature3])
    {
        SocialIOS::purchased500Coins();
//        CCUserDefault::sharedUserDefault()->setIntegerForKey("Candy", g_nCherryCnt + 500);
//        CCScene *pScene = MainScene::scene();
//        CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5, pScene));

    }
    if([productIdentifier isEqualToString:feature4])
    {
        SocialIOS::purchased1000Coins();
//        CCUserDefault::sharedUserDefault()->setIntegerForKey("Candy", g_nCherryCnt + 1000);
//        CCScene *pScene = MainScene::scene();
//        CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5, pScene));

    }
    //non-consumable purchase
//    if([productIdentifier isEqualToString:featureSkier])
//    {
//        bPurchasedSkier = YES;
//    }
//    if([productIdentifier isEqualToString:featureADS])
//    {
//		bPurchasedADS = YES;
//    }
//    
    [delegate stPurchased: productIdentifier];
    
	[MKStoreManager updatePurchases];
    
    [self performSelector:@selector(didComplete:) withObject:@"Purchase Successed!" afterDelay:0.4];
    [self.alertView dismissWithClickedButtonIndex:0 animated:NO];
    
}

+(void) loadPurchases 
{
	NSUserDefaults *userDefaults = [NSUserDefaults standardUserDefaults];
//    bPurchasedSkier = [userDefaults boolForKey:featureSkier];
//    bPurchasedADS = [userDefaults boolForKey:featureADS];
}

+(void) updatePurchases
{
	NSUserDefaults *userDefaults = [NSUserDefaults standardUserDefaults];
//    [userDefaults setBool:bPurchasedSkier forKey:featureSkier];
//	[userDefaults setBool:bPurchasedADS forKey:featureADS];
    CCUserDefault::sharedUserDefault()->setBoolForKey("EnableExpert", bPurchasedSkier);
    CCUserDefault::sharedUserDefault()->setBoolForKey("SkierType", bPurchasedSkier);
    CCUserDefault::sharedUserDefault()->setBoolForKey("RemoveAds", bPurchasedADS);
}

+(void) makePaidVersion
{
    [MKStoreManager loadPurchases];
}

-(void)restoreFeature
{
    [[SKPaymentQueue defaultQueue] restoreCompletedTransactions];
    
    CCUserDefault::sharedUserDefault()->setBoolForKey("isPurchasing", YES);
//    [[SHKActivityIndicator currentIndicator] displayActivity:@"Restoring Purchased..."];
}
- (void)restoreFail
{
    self.isPurchasing = NO;
    CCUserDefault::sharedUserDefault()->setBoolForKey("isPurchasing", NO);
//    [[SHKActivityIndicator currentIndicator] hide];
    
    
    [[[UIAlertView alloc] initWithTitle:@"Restore fail" message:@"Can't find purchased item. Please purchase items now." delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil] show];
}

- (void)restoreSuccess
{
    [self performSelector:@selector(didComplete:) withObject:@"Restore Successed!" afterDelay:0.4];
}

- (void)didComplete:(NSString *)completedMessage
{
    self.isPurchasing = NO;
    CCUserDefault::sharedUserDefault()->setBoolForKey("isPurchasing", NO);
//    [[SHKActivityIndicator currentIndicator] displayCompleted:completedMessage];
}

- (void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions
{
    
}

@end
