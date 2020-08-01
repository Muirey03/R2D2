#import "R2GraphViewController.h"
#import "R2FunctionList.h"
#import "R2Core.h"
#import "R2ArrowPainter.h"

@implementation R2GraphViewController
{
	BOOL _needsRelayout;
}

-(instancetype)init
{
	if ((self = [super init]))
	{
		self.title = @"Graph";
		UIImage* itemImg = [UIImage systemImageNamed:@"rectangle.3.offgrid.fill"];
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

	_scrollView = [[UIScrollView alloc] initWithFrame:CGRectZero];
	_scrollView.delegate = self;
	[self.view addSubview:_scrollView];

	UILayoutGuide* layoutGuide = self.view.safeAreaLayoutGuide;
	_scrollView.translatesAutoresizingMaskIntoConstraints = NO;
	[_scrollView.leadingAnchor constraintEqualToAnchor:layoutGuide.leadingAnchor].active = YES;
	[_scrollView.trailingAnchor constraintEqualToAnchor:layoutGuide.trailingAnchor].active = YES;
	[_scrollView.topAnchor constraintEqualToAnchor:layoutGuide.topAnchor].active = YES;
	[_scrollView.bottomAnchor constraintEqualToAnchor:layoutGuide.bottomAnchor].active = YES;
}

-(void)viewDidAppear:(BOOL)animated
{
	[super viewWillAppear:animated];

	if (_needsRelayout)
	{
		[self layoutGraphForFunctionAtIndex:[R2FunctionList sharedInstance].currentFunction];
		_needsRelayout = NO;
	}
}

-(void)layoutGraphForFunctionAtIndex:(NSInteger)funcIndex
{
	BOOL hasFunc = funcIndex != -1;
	_functionIndex = funcIndex;
	_function = nil;
	_layout = nil;
	_blockViews = nil;
	if (_graphContentView)
		[_graphContentView removeFromSuperview];
	_graphContentView = nil;
	_scrollView.contentSize = CGSizeZero;

	if (hasFunc)
	{
		unsigned long long offset = [[R2FunctionList sharedInstance].allFunctions[funcIndex][@"offset"] unsignedLongLongValue];
		NSArray* fns = [[R2Core sharedInstance] cmdJSON:[NSString stringWithFormat:@"agj 0x%llx", offset]];
		_function = fns.firstObject;
		_layout = [[R2GraphLayout alloc] initWithFunction:_function];

		CGSize graphSize = [_layout contentSize];
		CGSize viewSize = _scrollView.frame.size;
		graphSize.height = MAX(graphSize.height, viewSize.height);

		_scrollView.contentSize = graphSize;
		_scrollView.minimumZoomScale = MAX(viewSize.width / graphSize.width, viewSize.height / graphSize.height);
		_scrollView.maximumZoomScale = 2;
		CGFloat contentOffsetY = MAX(0, MIN([R2GraphLayout contentPadding], (graphSize.height - viewSize.height) / 2));
		_scrollView.contentOffset = CGPointMake((graphSize.width - viewSize.width) / 2, contentOffsetY);
		
		_graphContentView = [[R2GraphContainerView alloc] initWithFrame:CGRectMake(0, 0, graphSize.width, graphSize.height)];
		_graphContentView.backgroundColor = [UIColor systemBackgroundColor];
		[_scrollView addSubview:_graphContentView];

		NSMutableArray<R2GraphBlockView*>* views = [NSMutableArray array];
		for (R2GraphBlock* block in _layout.allBlocks)
		{
			R2GraphBlockView* blockView = [[R2GraphBlockView alloc] initWithGraphBlock:block];
			[views addObject:blockView];
			[_graphContentView addSubview:blockView];
		}
		_blockViews = views;

		R2ArrowPainter* arrowPainter = [[R2ArrowPainter alloc] initWithGraphLayout:_layout];
		_graphContentView.arrows = [arrowPainter drawArrows];
		[_graphContentView setNeedsDisplay];
	}
}

-(void)currentFunctionDidChange:(NSNotification*)note
{
	_needsRelayout = YES;
}

-(UIView*)viewForZoomingInScrollView:(UIScrollView*)scrollView
{
	return _graphContentView;
}

-(void)dealloc
{
	[[NSNotificationCenter defaultCenter] removeObserver:self];
}
@end
