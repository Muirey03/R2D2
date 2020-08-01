#import "R2GraphBlock.h"

@interface R2GraphBlockView : UIView
@property (nonatomic, strong) R2GraphBlock* block;
@property (nonatomic, strong) UITextView* textView;
-(instancetype)initWithGraphBlock:(R2GraphBlock*)block;
@end
