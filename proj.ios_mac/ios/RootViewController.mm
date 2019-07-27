/****************************************************************************
 Copyright (c) 2013      cocos2d-x.org
 Copyright (c) 2013-2014 Chukong Technologies Inc.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#import "RootViewController.h"
#import "cocos2d.h"
#import "CCEAGLView.h"
#import "Manager.h"
#import "PublicLayer.h"

@implementation RootViewController

/*
 // The designated initializer.  Override if you create the controller programmatically and want to perform customization that is not appropriate for viewDidLoad.
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    if ((self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil])) {
        // Custom initialization
    }
    return self;
}
*/

/*
// Implement loadView to create a view hierarchy programmatically, without using a nib.
- (void)loadView {
}
*/

/*
// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];
}

*/
// Override to allow orientations other than the default portrait orientation.
// This method is deprecated on ios6
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    return UIInterfaceOrientationIsLandscape( interfaceOrientation );
}

// For ios6, use supportedInterfaceOrientations & shouldAutorotate instead
- (NSUInteger) supportedInterfaceOrientations{
#ifdef __IPHONE_6_0
    return UIInterfaceOrientationMaskAllButUpsideDown;
#endif
}

- (BOOL) shouldAutorotate {
    return YES;
}

- (void)didRotateFromInterfaceOrientation:(UIInterfaceOrientation)fromInterfaceOrientation {
    [super didRotateFromInterfaceOrientation:fromInterfaceOrientation];

    cocos2d::GLView *glview = cocos2d::Director::getInstance()->getOpenGLView();

    if (glview)
    {
        CCEAGLView *eaglview = (CCEAGLView*) glview->getEAGLView();

        if (eaglview)
        {
            CGSize s = CGSizeMake([eaglview getWidth], [eaglview getHeight]);
            cocos2d::Application::getInstance()->applicationScreenSizeChanged((int) s.width, (int) s.height);
        }
    }
}

//fix not hide status on ios7
- (BOOL)prefersStatusBarHidden
{
    return YES;
}

- (void)didReceiveMemoryWarning {
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];

    // Release any cached data, images, etc that aren't in use.
}

- (void)viewDidLoad {
    [super viewDidLoad];
    // 监听购买结果
    //[[SKPaymentQueue defaultQueue] addTransactionObserver:self];
    NSLog(@"------------------viewDidLoad");
}
- (void)viewDidUnload {
    [super viewDidUnload];
    //[[SKPaymentQueue defaultQueue] removeTransactionObserver:self];
    NSLog(@"------------------viewDidUnload");
}


- (void)dealloc {
    [super dealloc];
}


/*
 *  IOS内付费
 *
 */
//+ (void)iosPay:(NSString *)type{
//    NSLog(@"------------------iosPay");
//    if([SKPaymentQueue canMakePayments]){
//        NSLog(type);
//        [self requestProductData: type];
//    }else{
//        NSLog(@"不允许程序内付费");
//    }
//
//}
//// 下面的ProductId应该是事先在itunesConnect中添加好的，已存在的付费项目。否则查询会失败。
//+ (void)requestProductData:(NSString *)type{
//    NSLog(@"-------------请求对应的产品信息----------------");
//    NSArray *product = [[NSArray alloc] initWithObjects:type, nil];
//    
//    NSSet *nsset = [NSSet setWithArray:product];
//    SKProductsRequest *request = [[SKProductsRequest alloc] initWithProductIdentifiers:nsset];
//    request.delegate = self;
//    [request start];
//    
//}
//// 以上查询的回调函数
//+ (void)productsRequest:(SKProductsRequest *)request didReceiveResponse:(SKProductsResponse *)response {
//    NSArray *myProduct = response.products;
//    NSLog(@"111111111111111111");
//    if (myProduct.count == 0) {
//        NSLog(@"无法获取产品信息，购买失败。");
//        return;
//    }
//    NSLog(@"222222222222222222");
//    SKPayment * payment = [SKPayment paymentWithProduct:myProduct[0]];
//    [[SKPaymentQueue defaultQueue] addPayment:payment];
//}
//+ (void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions {
//    for (SKPaymentTransaction *transaction in transactions)
//    {
//        switch (transaction.transactionState)
//        {
//            case SKPaymentTransactionStatePurchased://交易完成
//                NSLog(@"transactionIdentifier = %@", transaction.transactionIdentifier);
//                [self completeTransaction:transaction];
//                break;
//            case SKPaymentTransactionStateFailed://交易失败
//                NSLog(@"交易失败");
//                [self failedTransaction:transaction];
//                break;
//            case SKPaymentTransactionStateRestored://已经购买过该商品
//                NSLog(@"已经购买过该商品");
//                [self restoreTransaction:transaction];
//                break;
//            case SKPaymentTransactionStatePurchasing://商品添加进列表
//                NSLog(@"商品添加进列表");
//                break;
//            default:
//                break;
//        }
//    }
//}
//+ (void)completeTransaction:(SKPaymentTransaction *)transaction {
//    Manager::getInstance()->getPublicLayer()->callbackIOSPaySuccess(transaction.transactionIdentifier);
//    // Your application should implement these two methods.
////    NSString * productIdentifier = transaction.payment.productIdentifier;
////    NSString * receipt = [transaction.transactionReceipt base64EncodedString];
////    if ([productIdentifier length] > 0) {
////        // 向自己的服务器验证购买凭证
////    }
////    // Remove the transaction from the payment queue.
////    [[SKPaymentQueue defaultQueue] finishTransaction: transaction];
//    
//}
//+ (void)failedTransaction:(SKPaymentTransaction *)transaction {
//    if(transaction.error.code != SKErrorPaymentCancelled) {
//        NSLog(@"购买失败");
//    } else {
//        NSLog(@"用户取消交易");
//    }
//    [[SKPaymentQueue defaultQueue] finishTransaction: transaction];
//}
//+ (void)restoreTransaction:(SKPaymentTransaction *)transaction {
//    // 对于已购商品，处理恢复购买的逻辑
//    [[SKPaymentQueue defaultQueue] finishTransaction: transaction];
//}


@end
