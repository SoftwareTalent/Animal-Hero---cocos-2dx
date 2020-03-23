#import <Foundation/Foundation.h>
#import <StoreKit/StoreKit.h>
#import "MKStoreObserver.h"
@protocol MKStoreKitDelegate <NSObject>
@optional
- (void)stPurchased:(NSString*) purchID;
- (void)failed;
@end

@interface MKStoreManager : NSObject<SKProductsRequestDelegate, SKPaymentTransactionObserver> {
	NSMutableArray *purchasableObjects;
	MKStoreObserver *storeObserver;
    id<MKStoreKitDelegate> delegate;
//    UIAlertView * waitAlert;
//    UIAlertView *parentAskView;
//    //parent
//    int parentRes;
//    int rand1;
//    int rand2;
//    UITextField *textField;
    
//    NSString * purchaseId;
}

@property (atomic) BOOL isPurchasing;
@property (nonatomic, retain) UIAlertView *alertView;

@property (nonatomic, retain) id<MKStoreKitDelegate> delegate;
@property (nonatomic, retain) NSMutableArray *purchasableObjects;
@property (nonatomic, retain) MKStoreObserver *storeObserver;

- (void) requestProductData;

- (void) buy:(int)index;
- (void) buySkier;
- (void) buyremoveads;

- (bool) isParent;

// do not call this directly. This is like a private method
- (void) buyFeature:(NSString*) storyID;

- (void) failedTransaction: (SKPaymentTransaction *)transaction;
- (void) provideContent: (NSString*) productIdentifier;

+ (MKStoreManager*)sharedManager;

+ (BOOL) isPurchasedSkier;
+ (BOOL) isPurchasedADS;

+(void) loadPurchases;
+(void) updatePurchases;

-(void)restoreFeature;
- (void) restoreFail;
-(void)paymentCanceled;

@end
