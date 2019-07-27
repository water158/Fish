#import <UIKit/UIKit.h>

@class RootViewController;

@interface AppController : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    RootViewController* _viewController;
}

//@property(nonatomic, readonly) RootViewController* viewController;

@end

