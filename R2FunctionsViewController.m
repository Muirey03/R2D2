#import "R2FunctionsViewController.h"
#import "R2Core.h"

@implementation R2FunctionsViewController
{
	NSMutableArray* _filteredIndexes;
	UISearchController* _searchController;
}

-(instancetype)init
{
	if ((self = [super initWithStyle:UITableViewStyleGrouped]))
	{
		self.title = @"Functions";
		UIImage* itemImg = [UIImage systemImageNamed:@"list.bullet"];
		self.tabBarItem = [[UITabBarItem alloc] initWithTitle:self.title image:itemImg tag:0];
		_functions = [R2FunctionList sharedInstance];
		[self resetFilteredResults];

		[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(projectDidChange:) name:kProjectDidChangeNotification object:nil];
	}
	return self;
}

-(void)viewDidLoad
{
	[super viewDidLoad];

	[self.tableView registerClass:[UITableViewCell class] forCellReuseIdentifier:@"FunctionCell"];

	//create search controller:
	UISearchController* searchController = [[UISearchController alloc] initWithSearchResultsController:nil];
	_searchController = searchController;
	searchController.obscuresBackgroundDuringPresentation = NO;
	searchController.searchResultsUpdater = self;
	self.navigationItem.searchController = searchController;
	self.navigationItem.hidesSearchBarWhenScrolling = NO;
}

-(void)projectDidChange:(NSNotification*)note
{
	_searchController.active = NO;
	[_functions reloadFunctions];
	[self resetFilteredResults];
	[self.tableView reloadData];
}

-(void)resetFilteredResults
{
	NSUInteger count = _functions.allFunctions.count;
	if (count)
	{
		_filteredIndexes = [[NSMutableArray alloc] initWithCapacity:count];
		for (NSUInteger i = 0; i < count; i++)
			_filteredIndexes[i] = @(i);
	}
	else
		_filteredIndexes = [NSMutableArray array];
}

#pragma mark Table View Methods

-(NSInteger)numberOfSectionsInTableView:(UITableView*)tableView
{
	return 1;
}

-(NSInteger)tableView:(UITableView*)tableView numberOfRowsInSection:(NSInteger)section
{
	return _filteredIndexes.count;
}

-(UITableViewCell*)tableView:(UITableView*)tableView cellForRowAtIndexPath:(NSIndexPath*)indexPath
{
	UITableViewCell* cell = [tableView dequeueReusableCellWithIdentifier:@"FunctionCell" forIndexPath:indexPath];
	NSUInteger index = [_filteredIndexes[indexPath.row] unsignedIntegerValue];
	cell.textLabel.text = [_functions demangledNameForFunctionAtIndex:index];
	if (_functions.currentFunction == index)
		cell.accessoryType = UITableViewCellAccessoryCheckmark;
	else
		cell.accessoryType = UITableViewCellAccessoryNone;
	return cell;
}

-(void)tableView:(UITableView*)tableView didSelectRowAtIndexPath:(NSIndexPath*)indexPath
{
	NSInteger oldCurrentFunc = _functions.currentFunction;
	_functions.currentFunction = [_filteredIndexes[indexPath.row] integerValue];

	NSIndexPath* oldIndexPath = nil;
	if (oldCurrentFunc != -1)
	{
		NSInteger index = [_filteredIndexes indexOfObject:@(oldCurrentFunc)];
		if (index != NSNotFound)
			oldIndexPath = [NSIndexPath indexPathForRow:index inSection:0];
	}
	NSMutableArray* viewControllers = [@[indexPath] mutableCopy];
	if (oldIndexPath)
		[viewControllers addObject:oldIndexPath];

	[self.tableView reloadRowsAtIndexPaths:viewControllers withRowAnimation:UITableViewRowAnimationFade];
	[tableView deselectRowAtIndexPath:indexPath animated:YES];
}

-(UISwipeActionsConfiguration*)tableView:(UITableView*)tableView leadingSwipeActionsConfigurationForRowAtIndexPath:(NSIndexPath*)indexPath
{
	UIContextualAction* setSigAction = [UIContextualAction contextualActionWithStyle:UIContextualActionStyleNormal title:@"Set Signature" handler:^(UIContextualAction* action, UIView* sourceView, void (^completionHandler)(BOOL)){
		NSUInteger funcIndex = [_filteredIndexes[indexPath.row] unsignedIntegerValue];
		UIAlertController* alert = [UIAlertController alertControllerWithTitle:@"Set Signature" message:nil preferredStyle:UIAlertControllerStyleAlert];
		__block UITextField* alertTextField = nil;
		[alert addTextFieldWithConfigurationHandler:^(UITextField* textField){
			textField.placeholder = @"Enter new function signature";
			textField.text = [_functions signatureForFunctionAtIndex:funcIndex];
			alertTextField = textField;
		}];
		UIAlertAction* doneAction = [UIAlertAction actionWithTitle:@"Done" style:UIAlertActionStyleDefault handler:^(UIAlertAction* alertAction){
			[_functions setSignature:alertTextField.text forFunctionAtIndex:funcIndex];
		}];
		UIAlertAction* cancelAction = [UIAlertAction actionWithTitle:@"Cancel" style:UIAlertActionStyleCancel handler:nil];
		[alert addAction:cancelAction];
		[alert addAction:doneAction];
		[self presentViewController:alert animated:YES completion:^{
			completionHandler(YES);
		}];
	}];
	setSigAction.backgroundColor = [UIColor systemBlueColor];
	NSArray<UIContextualAction*>* actions = @[setSigAction];
	UISwipeActionsConfiguration* config = [UISwipeActionsConfiguration configurationWithActions:actions];
	return config;
}

#pragma mark Search Results Methods

-(void)updateSearchResultsForSearchController:(UISearchController*)searchController
{
	NSString* searchString = [searchController.searchBar.text lowercaseString];
	_filteredIndexes = [NSMutableArray array];
	NSUInteger count = _functions.allFunctions.count;
	for (NSUInteger i = 0; i < count; i++)
	{
		NSString* name = [_functions demangledNameForFunctionAtIndex:i];
		if ([name.lowercaseString containsString:searchString] || !searchString.length)
			[_filteredIndexes addObject:@(i)];
	}
	[self.tableView reloadData];
}

-(void)dealloc
{
	[[NSNotificationCenter defaultCenter] removeObserver:self];
}
@end
