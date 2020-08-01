#import "R2GraphContainerView.h"

@implementation R2GraphContainerView
-(void)drawRect:(CGRect)rect
{
	[super drawRect:rect];

	for (R2ArrowDescriptor* arrow in _arrows)
	{
		UIBezierPath* path = arrow.path;
		path.lineWidth = 2.5;
		UIColor* col = arrow.type == R2ArrowTypeFail ? [UIColor systemRedColor] : [UIColor systemGreenColor];
		[col setStroke];
		[col setFill];
		[arrow.arrowHeadPath fill];
		[arrow.arrowHeadPath stroke];
		[path stroke];
	}
}

@end
