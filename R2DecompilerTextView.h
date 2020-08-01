@interface R2DecompilerTextView : UITextView
@property (nonatomic, assign) CGFloat lineNumbersWidth;
@property (nonatomic, readonly) UILabel* lineNumbers;
-(instancetype)initWithFrame:(CGRect)frame lineNumbersWidth:(CGFloat)lineNumbersWidth;
-(void)setupLineNumbers;
@end
