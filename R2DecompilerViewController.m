#import "R2DecompilerViewController.h"
#import "R2FunctionList.h"

@implementation R2DecompilerViewController
{
	BOOL _needsContentRefresh;
	UILabel* _lineNumbers;
}

-(instancetype)init
{
	if ((self = [super init]))
	{
		self.title = @"Decompiler";
		UIImage* itemImg = [UIImage systemImageNamed:@"textformat.abc"];
		self.tabBarItem = [[UITabBarItem alloc] initWithTitle:self.title image:itemImg tag:0];
		self.navigationItem.largeTitleDisplayMode = UINavigationItemLargeTitleDisplayModeNever;

		[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(currentFunctionDidChange:) name:kCurrentFunctionChangeNotification object:nil];
	}
	return self;
}

-(void)loadView
{
	[super loadView];

	self.view.backgroundColor = [UIColor systemBackgroundColor];
	const CGFloat lineNumbersWidth = 40;
	const CGFloat topTextInsets = 19;
	const CGFloat leftTextInsets = 7;

	_textView = [[R2DecompilerTextView alloc] initWithFrame:CGRectZero lineNumbersWidth:lineNumbersWidth];
	_textView.editable = NO;
	_textView.textContainerInset = UIEdgeInsetsMake(0, lineNumbersWidth + leftTextInsets, 0, leftTextInsets);
	_textView.contentInset = UIEdgeInsetsMake(topTextInsets, 0, topTextInsets, 0);
	_textView.font = [UIFont systemFontOfSize:[UIFont labelFontSize]];
	_textView.backgroundColor = [UIColor clearColor];
	[self.view addSubview:_textView];

	_textView.translatesAutoresizingMaskIntoConstraints = NO;
	[_textView.leadingAnchor constraintEqualToAnchor:self.view.leadingAnchor].active = YES;
	[_textView.trailingAnchor constraintEqualToAnchor:self.view.trailingAnchor].active = YES;
	[_textView.topAnchor constraintEqualToAnchor:self.view.topAnchor].active = YES;
	[_textView.bottomAnchor constraintEqualToAnchor:self.view.bottomAnchor].active = YES;

	[_textView setupLineNumbers];
}

-(void)viewDidAppear:(BOOL)animated
{
	[super viewDidAppear:animated];

	if (_needsContentRefresh)
		[self refreshContent];
}

-(void)viewWillTransitionToSize:(CGSize)size withTransitionCoordinator:(id<UIViewControllerTransitionCoordinator>)coordinator
{
	[super viewWillTransitionToSize:size withTransitionCoordinator:coordinator];
	_textView.contentOffset = CGPointZero;
}

-(void)refreshContent
{
	//DEBUG
	NSMutableString* str = [NSMutableString new];
	for (int i = 0; i < 1000; i++)
		[str appendFormat:@"This is a line %d\n", i];
	[_textView setText:str];
}

-(void)currentFunctionDidChange:(NSNotification*)note
{
	_needsContentRefresh = YES;
}

-(void)dealloc
{
	[[NSNotificationCenter defaultCenter] removeObserver:self];
}
@end
