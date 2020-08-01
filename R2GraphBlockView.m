#import "R2GraphBlockView.h"

@implementation R2GraphBlockView
-(instancetype)initWithGraphBlock:(R2GraphBlock*)block
{
	if ((self = [self initWithFrame:block.frame]))
	{
		_block = block;
		self.backgroundColor = [UIColor systemFillColor];
		self.clipsToBounds = YES;
		self.layer.cornerRadius = 10;

		_textView = [[UITextView alloc] initWithFrame:CGRectMake(0, 0, block.frame.size.width, block.frame.size.height)];
		_textView.scrollEnabled = NO;
		_textView.attributedText = [block text];
		_textView.editable = NO;
		_textView.backgroundColor = [UIColor clearColor];
		[self addSubview:_textView];

		[_textView sizeToFit];
		_textView.center = CGPointMake(block.frame.size.width / 2, block.frame.size.height / 2);
	}
	return self;
}
@end
