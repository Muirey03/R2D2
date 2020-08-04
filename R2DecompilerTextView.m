#import "R2DecompilerTextView.h"

@implementation R2DecompilerTextView
-(instancetype)initWithFrame:(CGRect)frame lineNumbersWidth:(CGFloat)lineNumbersWidth
{
	if ((self = [self initWithFrame:frame]))
	{
		_lineNumbersWidth = lineNumbersWidth;
	}
	return self;
}

-(void)setupLineNumbers
{
	_lineNumbers = [[UILabel alloc] initWithFrame:CGRectMake(0, 0, _lineNumbersWidth, self.contentSize.height)];
	_lineNumbers.clipsToBounds = NO;
	_lineNumbers.numberOfLines = 0;
	_lineNumbers.font = [UIFont systemFontOfSize:[UIFont labelFontSize]];
	_lineNumbers.textAlignment = NSTextAlignmentCenter;
	_lineNumbers.lineBreakMode = NSLineBreakByClipping;
	_lineNumbers.textColor = [UIColor labelColor];
	_lineNumbers.alpha = 0.5;
	[self addSubview:_lineNumbers];

	const CGFloat marginWidth = 1;
	UIView* marginView = [[UIView alloc] initWithFrame:CGRectZero];
	marginView.backgroundColor = [UIColor labelColor];
	[_lineNumbers addSubview:marginView];

	marginView.translatesAutoresizingMaskIntoConstraints = NO;
	[marginView.widthAnchor constraintEqualToConstant:marginWidth].active = YES;
	[marginView.trailingAnchor constraintEqualToAnchor:_lineNumbers.trailingAnchor].active = YES;
	[marginView.topAnchor constraintEqualToAnchor:_lineNumbers.topAnchor].active = YES;
	[marginView.bottomAnchor constraintEqualToAnchor:_lineNumbers.bottomAnchor].active = YES;
}

-(void)setAttributedText:(NSAttributedString*)attrText
{
	[super setAttributedText:attrText];

	NSString* text = attrText.string;
	[self layoutIfNeeded];
	NSMutableString* lineNoString = [NSMutableString new];
	NSUInteger lineCount = [text componentsSeparatedByString:@"\n"].count;
	for (NSUInteger i = 1; i <= lineCount; i++)
		[lineNoString appendFormat:@"%lu\n", (unsigned long)i];
	_lineNumbers.text = lineNoString;
}

-(void)layoutSubviews
{
	[super layoutSubviews];
	_lineNumbers.frame = CGRectMake(_lineNumbers.frame.origin.x, _lineNumbers.frame.origin.y, _lineNumbersWidth, self.contentSize.height);
}
@end
