#import "R2DecompilerViewController.h"
#import "R2FunctionList.h"
#import "R2PluginManager.h"
#import "R2Core.h"
#import "R2ANSIParser.h"
#import "R2LoadingIndicator.h"

@implementation R2DecompilerViewController
{
	BOOL _needsContentRefresh;
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

	//JS config:
	NSString* js = [self javascriptString];
	WKUserScript* wkUScript = [[WKUserScript alloc] initWithSource:js injectionTime:WKUserScriptInjectionTimeAtDocumentEnd forMainFrameOnly:YES];
	WKUserContentController* wkUController = [WKUserContentController new];
	[wkUController addUserScript:wkUScript];
	WKWebViewConfiguration* config = [WKWebViewConfiguration new];
	config.userContentController = wkUController;

	_webView = [[WKWebView alloc] initWithFrame:CGRectZero configuration:config];
	_webView.opaque = NO;
	_webView.backgroundColor = [UIColor clearColor];
	[self.view addSubview:_webView];

	_webView.translatesAutoresizingMaskIntoConstraints = NO;
	[_webView.leadingAnchor constraintEqualToAnchor:self.view.leadingAnchor].active = YES;
	[_webView.trailingAnchor constraintEqualToAnchor:self.view.trailingAnchor].active = YES;
	[_webView.topAnchor constraintEqualToAnchor:self.view.topAnchor].active = YES;
	[_webView.bottomAnchor constraintEqualToAnchor:self.view.bottomAnchor].active = YES;
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

-(void)refreshContent
{
	R2FunctionList* functions = [R2FunctionList sharedInstance];
	if (functions.currentFunction == -1)
	{
		[_webView loadHTMLString:@"" baseURL:nil];
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
		UIFont* font = [UIFont fontWithName:@"Menlo-Regular" size:15];
		[pseudocode addAttributes:@{NSFontAttributeName : font} range:NSMakeRange(0, pseudocode.string.length)];

		NSDictionary* documentAttributes = @{NSDocumentTypeDocumentAttribute : NSHTMLTextDocumentType};    
		NSData* htmlData = [pseudocode dataFromRange:NSMakeRange(0, pseudocode.length) documentAttributes:documentAttributes error:NULL];
		NSString* htmlString = [[NSString alloc] initWithData:htmlData encoding:NSUTF8StringEncoding];

		dispatch_async(dispatch_get_main_queue(), ^{
			[spinner dismiss];
			[_webView loadHTMLString:htmlString baseURL:nil];
		});
	});
}

-(NSString*)javascriptString
{
	return 	@"var body = document.getElementsByTagName('body')[0];"
			@"body.style.whiteSpace = 'nowrap';"
			@"var meta = document.createElement('meta');"
			@"meta.setAttribute('name', 'viewport');"
			@"meta.setAttribute('content', 'initial-scale=1.0,maximum-scale=3.0, minimum-scale=0.5');"
			@"document.getElementsByTagName('head')[0].appendChild(meta);";
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
