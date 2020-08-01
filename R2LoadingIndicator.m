#import "R2LoadingIndicator.h"

@implementation R2LoadingIndicator
-(instancetype)init
{
	if ((self = [super init]))
	{
		self.modalPresentationStyle = UIModalPresentationOverFullScreen;
		self.modalTransitionStyle = UIModalTransitionStyleCrossDissolve;
	}
	return self;
}

-(void)loadView
{
	[super loadView];

	#pragma clang diagnostic push
	#pragma clang diagnostic ignored "-Wdeprecated-declarations"
	_spinner = [[UIActivityIndicatorView alloc] initWithActivityIndicatorStyle:UIActivityIndicatorViewStyleWhiteLarge];
	#pragma clang diagnostic pop
	CGSize spinnerSize = _spinner.frame.size;
	const CGFloat sizeMult = 2.5;
	CGSize containerSize = CGSizeMake(spinnerSize.width * sizeMult, spinnerSize.height * sizeMult);

	_containerView = [[UIView alloc] initWithFrame:CGRectZero];
	_containerView.clipsToBounds = YES;
	_containerView.layer.cornerRadius = 15;
	_containerView.backgroundColor = [UIColor colorWithWhite:0 alpha:0.5];
	[self.view addSubview:_containerView];

	_containerView.translatesAutoresizingMaskIntoConstraints = NO;
	[_containerView.centerXAnchor constraintEqualToAnchor:self.view.centerXAnchor].active = YES;
	[_containerView.centerYAnchor constraintEqualToAnchor:self.view.centerYAnchor].active = YES;
	[_containerView.widthAnchor constraintEqualToConstant:containerSize.width].active = YES;
	[_containerView.heightAnchor constraintEqualToConstant:containerSize.height].active = YES;

	[_containerView addSubview:_spinner];
	_spinner.translatesAutoresizingMaskIntoConstraints = NO;
	[_spinner.centerXAnchor constraintEqualToAnchor:_containerView.centerXAnchor].active = YES;
	[_spinner.centerYAnchor constraintEqualToAnchor:_containerView.centerYAnchor].active = YES;
	[_spinner.widthAnchor constraintEqualToConstant:spinnerSize.width].active = YES;
	[_spinner.heightAnchor constraintEqualToConstant:spinnerSize.height].active = YES;

	self.view.backgroundColor = [UIColor colorWithWhite:0 alpha:0.2];
}

-(void)viewWillAppear:(BOOL)animated
{
	[super viewWillAppear:animated];
	[_spinner startAnimating];
}

-(void)viewDidDisappear:(BOOL)animated
{
	[super viewDidDisappear:animated];
	[_spinner stopAnimating];
}
@end
