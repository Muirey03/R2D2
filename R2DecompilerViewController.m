#import "R2DecompilerViewController.h"
#import "R2FunctionList.h"
#import "R2PluginManager.h"
#import "R2Core.h"
#import "R2ANSIParser.h"
#import "R2LoadingIndicator.h"

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
	const CGFloat lineNumbersWidth = /*40;*/0; //DEBUG
	const CGFloat topTextInsets = 19;
	const CGFloat leftTextInsets = 7;

	_textView = [[R2DecompilerTextView alloc] initWithFrame:CGRectZero lineNumbersWidth:lineNumbersWidth];
	_textView.editable = NO;
	_textView.textContainerInset = UIEdgeInsetsMake(0, lineNumbersWidth + leftTextInsets, 0, leftTextInsets);
	_textView.contentInset = UIEdgeInsetsMake(topTextInsets, 0, topTextInsets, 0);
	_textView.font = [UIFont fontWithName:@"CourierNewPSMT" size:15];
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
	{
		[self refreshContent];
		_needsContentRefresh = NO;
	}
}

-(void)viewWillTransitionToSize:(CGSize)size withTransitionCoordinator:(id<UIViewControllerTransitionCoordinator>)coordinator
{
	[super viewWillTransitionToSize:size withTransitionCoordinator:coordinator];
	_textView.contentOffset = CGPointZero;
}

-(void)refreshContent
{
	R2FunctionList* functions = [R2FunctionList sharedInstance];
	if (functions.currentFunction == -1)
	{
		_textView.attributedText = nil;
		return;
	}

	R2LoadingIndicator* spinner = [[R2LoadingIndicator alloc] initForPresentationFromController:self];
	dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
		R2PluginManager* pluginManager = [R2PluginManager sharedInstance];
		if (![pluginManager isPluginLoaded:@"r2ghidra"])
			[pluginManager loadPluginAtPath:@"/usr/lib/radare2/4.5.0/core_ghidra.dylib"];
		
		NSDictionary* currentFunc = functions.allFunctions[functions.currentFunction];
		[[R2Core sharedInstance] cmd:[NSString stringWithFormat:@"s 0x%llx", [currentFunc[@"offset"] unsignedLongLongValue]]];
		NSString* ansi = [pluginManager cmd:@"pdg" forPlugin:@"r2ghidra"];
		R2ANSIParser* ansiParser = [R2ANSIParser new];
		NSMutableAttributedString* pseudocode = [[ansiParser attributedStringWithANSIString:ansi] mutableCopy];
		[pseudocode addAttributes:@{NSFontAttributeName : _textView.font} range:NSMakeRange(0, pseudocode.string.length)];

		dispatch_async(dispatch_get_main_queue(), ^{
			[spinner dismiss];
			_textView.attributedText = pseudocode;
		});
	});
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
