@interface R2LoadingIndicator : UIViewController
@property (nonatomic, strong) UIView* containerView;
@property (nonatomic, strong) UIActivityIndicatorView* spinner;
-(instancetype)initForPresentationFromController:(UIViewController*)vc;
-(void)dismiss;
@end
