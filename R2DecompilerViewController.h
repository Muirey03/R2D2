#import "R2DecompilerTextView.h"

@interface R2DecompilerViewController : UIViewController
@property (nonatomic, strong) R2DecompilerTextView* textView;
-(void)refreshContent;
@end
