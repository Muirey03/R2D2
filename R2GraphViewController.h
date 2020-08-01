#import "R2GraphLayout.h"
#import "R2GraphBlockView.h"
#import "R2GraphContainerView.h"

@interface R2GraphViewController : UIViewController <UIScrollViewDelegate>
@property (nonatomic, strong) UIScrollView* scrollView;
@property (nonatomic, strong) R2GraphContainerView* graphContentView;
@property (nonatomic, assign) NSInteger functionIndex;
@property (nonatomic, strong) NSDictionary* function;
@property (nonatomic, strong) R2GraphLayout* layout;
@property (nonatomic, strong) NSArray<R2GraphBlockView*>* blockViews;
-(void)layoutGraphForFunctionAtIndex:(NSInteger)funcIndex;
@end
