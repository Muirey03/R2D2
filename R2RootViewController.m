#import "R2RootViewController.h"
#import "R2Core.h"
#import "R2FunctionList.h"
#import "R2LoadingIndicator.h"

@interface R2RootViewController (Debug)
-(void)debug:(NSString*)str;
@end

@implementation R2RootViewController (Debug)
-(void)debug:(NSString*)str
{
	dispatch_async(dispatch_get_main_queue(), ^{
		UIAlertController* alert = [UIAlertController alertControllerWithTitle:@"Debug" message:(str ?: @"(null)") preferredStyle:UIAlertControllerStyleAlert];
		UIAlertAction* defaultAction = [UIAlertAction actionWithTitle:@"OK" style:UIAlertActionStyleDefault handler:nil];
		[alert addAction:defaultAction];
		[self presentViewController:alert animated:YES completion:nil];
	});
}
@end

@implementation R2RootViewController
-(instancetype)init
{
	if ((self = [super initWithStyle:UITableViewStyleGrouped]))
	{
		self.title = @"Home";
		UIImage* itemImg = [UIImage systemImageNamed:@"house.fill"];
		self.tabBarItem = [[UITabBarItem alloc] initWithTitle:self.title image:itemImg tag:0];

		[self reloadProjects];
	}
	return self;
}

-(void)reloadProjects
{
	_projects = [[R2Core sharedInstance] allProjects];
}

-(void)viewDidLoad
{
	[super viewDidLoad];

	[self.tableView registerClass:[UITableViewCell class] forCellReuseIdentifier:@"ProjectCell"];
	[self.tableView registerClass:[UITableViewCell class] forCellReuseIdentifier:@"NewProjectCell"];
}

-(void)loadNewProject
{
	UIDocumentPickerViewController* picker = [[UIDocumentPickerViewController alloc] initWithDocumentTypes:@[@"public.item"] inMode:UIDocumentPickerModeImport];
	picker.delegate = self;
	[self presentViewController:picker animated:YES completion:nil];
}

-(void)loadNewProjectFromURL:(NSURL*)url
{
	NSString* projName = [url.path lastPathComponent];
	if ([_projects containsObject:projName])
	{
		[self showErrorMessage:[NSString stringWithFormat:@"Project \"%@\" already exists.", projName]];
		return;
	}
	
	R2Core* core = [R2Core sharedInstance];
	NSString* oldSelectedProject = core.projectName;
	[core loadFile:url.path];
	R2LoadingIndicator* spinner = [[R2LoadingIndicator alloc] initForPresentationFromController:self];
	[core analyzeWithCompletion:^{
		[core saveProjectNamed:projName];
		dispatch_async(dispatch_get_main_queue(), ^{
			[spinner dismiss];

			[self reloadProjects];
			if (_projects.count == 1)
				[self.tableView insertSections:[NSIndexSet indexSetWithIndex:1] withRowAnimation:UITableViewRowAnimationFade];
			else
				[self.tableView insertRowsAtIndexPaths:@[[NSIndexPath indexPathForRow:[_projects indexOfObject:projName] inSection:1]] withRowAnimation:UITableViewRowAnimationAutomatic];
			if (oldSelectedProject)
				[self.tableView reloadRowsAtIndexPaths:@[[NSIndexPath indexPathForRow:[_projects indexOfObject:oldSelectedProject] inSection:1]] withRowAnimation:UITableViewRowAnimationFade];
			[self postProjectDidChangeNotification];
		});
	}];
}

-(void)loadProjectAtIndexPath:(NSIndexPath*)indexPath
{
	R2Core* core = [R2Core sharedInstance];

	//don't need to do anything if we're already selected
	NSString* projName = _projects[indexPath.row];
	if ([projName isEqualToString:core.projectName])
		return;

	NSIndexPath* oldIndexPath = nil;
	if (core.projectName)
	{
		NSUInteger oldIndex = [_projects indexOfObject:core.projectName];
		oldIndexPath = [NSIndexPath indexPathForRow:oldIndex inSection:1];
	}
	[core openProjectNamed:projName];
	NSMutableArray* indexPaths = [@[indexPath] mutableCopy];
	if (oldIndexPath)
		[indexPaths addObject:oldIndexPath];
	[self.tableView reloadRowsAtIndexPaths:indexPaths withRowAnimation:UITableViewRowAnimationFade];
	[self postProjectDidChangeNotification];
}

-(void)postProjectDidChangeNotification
{
	[[NSNotificationCenter defaultCenter] postNotificationName:kProjectDidChangeNotification object:nil];
}

-(void)showErrorMessage:(NSString*)errorMsg
{
	UIAlertController* alert = [UIAlertController alertControllerWithTitle:@"Error" message:errorMsg preferredStyle:UIAlertControllerStyleAlert];
	UIAlertAction* defaultAction = [UIAlertAction actionWithTitle:@"OK" style:UIAlertActionStyleDefault handler:nil];
	[alert addAction:defaultAction];
	[self presentViewController:alert animated:YES completion:nil];
}

#pragma mark Document Picker Methods

-(void)documentPicker:(UIDocumentPickerViewController*)picker didPickDocumentsAtURLs:(NSArray<NSURL*>*)urls
{
	NSURL* url = [urls firstObject];
	[self loadNewProjectFromURL:url];
}

#pragma mark Table View Methods

-(NSInteger)numberOfSectionsInTableView:(UITableView*)tableView
{
	return _projects.count ? 2 : 1;
}

-(NSString *)tableView:(UITableView*)tableView titleForHeaderInSection:(NSInteger)section
{
	if (section == 1)
		return @"Projects";
	return nil;
}

-(NSInteger)tableView:(UITableView*)tableView numberOfRowsInSection:(NSInteger)section
{
	return section == 0 ? 1 : _projects.count;
}

-(UITableViewCell*)tableView:(UITableView*)tableView cellForRowAtIndexPath:(NSIndexPath*)indexPath
{
	UITableViewCell* cell;
	if (indexPath.section == 0)
	{
		cell = [tableView dequeueReusableCellWithIdentifier:@"NewProjectCell" forIndexPath:indexPath];
		cell.textLabel.textAlignment = NSTextAlignmentCenter;
		cell.textLabel.text = @"New Project";
		cell.textLabel.textColor = self.view.tintColor;
	}
	else
	{
		cell = [tableView dequeueReusableCellWithIdentifier:@"ProjectCell" forIndexPath:indexPath];
		NSString* projName = _projects[indexPath.row];
		cell.textLabel.text = projName;
		if ([projName isEqualToString:[R2Core sharedInstance].projectName])
			cell.accessoryType = UITableViewCellAccessoryCheckmark;
		else
			cell.accessoryType = UITableViewCellAccessoryNone;
	}
	return cell;
}

-(BOOL)tableView:(UITableView*)tableView shouldHighlightRowAtIndexPath:(NSIndexPath*)indexPath
{
	return YES;
}

-(void)tableView:(UITableView*)tableView didSelectRowAtIndexPath:(NSIndexPath*)indexPath
{
	if (indexPath.section == 0)
		[self loadNewProject];
	else
		[self loadProjectAtIndexPath:indexPath];
	[tableView deselectRowAtIndexPath:indexPath animated:YES];
}

-(BOOL)tableView:(UITableView*)tableView canEditRowAtIndexPath:(NSIndexPath*)indexPath
{
	return indexPath.section == 1;
}

-(void)tableView:(UITableView*)tableView commitEditingStyle:(UITableViewCellEditingStyle)editingStyle forRowAtIndexPath:(NSIndexPath*)indexPath
{
	if (editingStyle != UITableViewCellEditingStyleDelete || indexPath.section != 1)
		return;
	R2Core* core = [R2Core sharedInstance];
	NSString* projName = _projects[indexPath.row];
	if ([core.projectName isEqualToString:projName])
	{
		[core reloadCore];
		[self postProjectDidChangeNotification];
	}
	[core cmd:[NSString stringWithFormat:@"Pd %@", projName]];
	[self reloadProjects];
	if (_projects.count)
		[tableView deleteRowsAtIndexPaths:@[indexPath] withRowAnimation:UITableViewRowAnimationAutomatic];
	else
		[self.tableView deleteSections:[NSIndexSet indexSetWithIndex:1] withRowAnimation:UITableViewRowAnimationFade];
}
@end
