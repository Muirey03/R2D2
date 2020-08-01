#import "R2FunctionList.h"

@interface R2FunctionsViewController : UITableViewController <UISearchResultsUpdating>
@property (nonatomic, weak) R2FunctionList* functions;
-(void)resetFilteredResults;
@end
