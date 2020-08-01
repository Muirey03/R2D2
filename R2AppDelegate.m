#import "R2AppDelegate.h"
#import "R2FunctionsViewController.h"
#import "R2GraphViewController.h"

@implementation R2AppDelegate
-(void)applicationDidFinishLaunching:(UIApplication*)application
{
	_window = [[UIWindow alloc] initWithFrame:[UIScreen mainScreen].bounds];
	_window.tintColor = [UIColor systemRedColor];

	NSArray* vcClasses = @[
		[R2RootViewController class],
		[R2FunctionsViewController class],
		[R2GraphViewController class]
	];

	_tabController = [UITabBarController new];
	NSMutableArray* viewControllers = [[NSMutableArray alloc] initWithCapacity:vcClasses.count];
	for (Class cls in vcClasses)
	{
		UIViewController* vc = [cls new];
		if ([vc isKindOfClass:[R2RootViewController class]])
			_rootViewController = (R2RootViewController*)vc;
		UINavigationController* navCont = [[UINavigationController alloc] initWithRootViewController:vc];
		navCont.navigationBar.prefersLargeTitles = YES;
		[viewControllers addObject:navCont];
	}
	_tabController.viewControllers = viewControllers;
	_window.rootViewController = _tabController;
	[_window makeKeyAndVisible];
}

-(BOOL)application:(UIApplication*)app openURL:(NSURL*)url options:(NSDictionary<UIApplicationOpenURLOptionsKey, id>*)options
{
	_tabController.selectedViewController = _tabController.viewControllers.firstObject;
	[_rootViewController loadNewProjectFromURL:url];
	return YES;
}
@end
