@interface R2RootViewController : UITableViewController <UIDocumentPickerDelegate>
@property (nonatomic, strong) NSArray* projects;
-(void)reloadProjects;
-(void)loadNewProject;
-(void)loadNewProjectFromURL:(NSURL*)url;
-(void)loadProjectAtIndexPath:(NSIndexPath*)indexPath;
-(void)postProjectDidChangeNotification;
-(void)showErrorMessage:(NSString*)errorMsg;
@end
