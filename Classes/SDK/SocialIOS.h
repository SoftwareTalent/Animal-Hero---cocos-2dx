//
//  SocialIOS.h
//  AllGas
//
class SocialIOS {
    
public:
    static void shareFacebook(int score);
    static void shareTwitter(int score);
    static void shareEmail(int score);

    static void showLeaderboard();
    static void showAchievement();
    static void submitScore(int score);
    static void submitAchievement(int index);
    
    static void showChartboost();
    static void showChartboostMoreGame();
    static void showRevmob();
    
    static void buyCoin(int index);
    static void BuySkier();
    static void removeAds();
    static void restoreIAP();
    
    static void showRateUs(bool mode_one);
    
    static void admobFullAds();
    static void showAdBanner();
    static void hideBanner();
    
    static void showiAD();
    static void hideiAD();

    static void saveMe();
    
    static void purchased100Coins();
    static void purchased300Coins();
    static void purchased500Coins();
    static void purchased1000Coins();
    
};