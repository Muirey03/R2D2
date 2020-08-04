@import WebKit;

@interface R2DecompilerViewController : UIViewController
@property (nonatomic, strong) WKWebView* webView;
-(void)refreshContent;
-(NSString*)javascriptString;
-(void)reloadWebView;
@end
