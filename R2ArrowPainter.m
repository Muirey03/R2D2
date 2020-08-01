#import "R2ArrowPainter.h"

@implementation R2ArrowDescriptor
@end

@implementation R2ArrowPainter
{
	NSMutableArray<R2ArrowDescriptor*>* _arrows;
	NSMutableDictionary<NSNumber*, NSNumber*>* _layerLineCount;
	NSMutableDictionary<NSNumber*, NSNumber*>* _layerTimesVisited;
	NSMutableDictionary<NSValue*, NSNumber*>* _pointCount;
	NSMutableDictionary<NSValue*, NSNumber*>* _pointTimesVisited;
	NSMapTable<R2GraphBlock*, NSNumber*>* _xrefsCount;
	NSMapTable<R2GraphBlock*, NSNumber*>* _xrefsTimesVisited;
}

-(instancetype)initWithGraphLayout:(R2GraphLayout*)layout
{
	if ((self = [self init]))
	{
		_layout = layout;
	}
	return self;
}

-(NSArray*)pointsForPathFromBlock:(R2GraphBlock*)src toBlock:(R2GraphBlock*)dst
{
	NSMutableArray* points = [NSMutableArray new];

	NSUInteger srcLayer = [_layout layerForBlock:src];
	NSUInteger srcIndex = [[_layout blocksInLayer:srcLayer] indexOfObject:src];
	NSUInteger dstLayer = [_layout layerForBlock:dst];
	NSUInteger dstIndex = [[_layout blocksInLayer:dstLayer] indexOfObject:dst];
	if (dstLayer > srcLayer)
	{
		for (NSUInteger layer = srcLayer; layer < dstLayer - 1; layer++)
		{
			NSUInteger layerCount = [_layout blocksInLayer:layer + 1].count;
			NSUInteger index = MIN(dstIndex, layerCount);
			if (layer == srcLayer && srcIndex == layerCount - 1) //go right on first move if far-right block
				index = srcIndex + 1;
			[points addObject:@(CGPointMake(index, layer))];
		}
	}
	else
	{
		for (NSUInteger layer = srcLayer - 1; layer + 2 > dstLayer; layer--)
		{
			NSUInteger layerCount = [_layout blocksInLayer:layer + 1].count;
			NSUInteger index = MIN(dstIndex, layerCount);
			if (layer == srcLayer - 1 && srcIndex == layerCount - 1) //go right on first move if far-right block
				index = srcIndex + 1;
			[points addObject:@(CGPointMake(index, layer))];
		}
	}
	return points;
}

-(CGFloat)xOffsetForPoint:(CGPoint)p
{
	NSUInteger layer = (NSUInteger)p.y;
	NSUInteger index = (NSUInteger)p.x;
	NSArray<R2GraphBlock*>* blocks = [_layout blocksInLayer:layer + 1];

	NSUInteger pointCount = [_pointCount[@(p)] unsignedIntegerValue];
	CGFloat spacing = [R2GraphLayout horizontalPadding] / (pointCount + 1);
	NSUInteger timesVisited = [_pointTimesVisited[@(p)] unsignedIntegerValue];
	_pointTimesVisited[@(p)] = @(++timesVisited);

	CGFloat x;
	if (index == 0)
		x = blocks[0].frame.origin.x - (spacing * timesVisited);
	else
		x = blocks[index - 1].frame.origin.x + blocks[index - 1].frame.size.width + (spacing * timesVisited);
	return x;
}

-(CGFloat)yOffsetForLayer:(NSUInteger)layer
{
	NSUInteger layerCount = [_layerLineCount[@(layer)] unsignedIntegerValue];
	CGFloat spacing = [R2GraphLayout verticalPadding] / (layerCount + 1);
	NSUInteger timesVisited = [_layerTimesVisited[@(layer)] unsignedIntegerValue];
	_layerTimesVisited[@(layer)] = @(++timesVisited);
	NSArray<R2GraphBlock*>* blocks = [_layout blocksInLayer:layer];
	return blocks[0].frame.origin.y + [_layout heightForLayer:layer] + (spacing * timesVisited);
}

-(CGPoint)startPointForArrow:(R2ArrowDescriptor*)arrow
{
	CGRect frame = arrow.src.frame;
	CGFloat y = frame.origin.y + frame.size.height;
	CGFloat centerX = frame.origin.x + frame.size.width / 2;
	const CGFloat spacing = 7;
	if (arrow.type == R2ArrowTypeJump)
	{
		if (!arrow.src.fail)
			return CGPointMake(centerX, y);
		return CGPointMake(centerX + spacing, y);
	}
	if (!arrow.src.jump)
		return CGPointMake(centerX, y);
	return CGPointMake(centerX - spacing, y);
}

-(CGPoint)endPointForArrow:(R2ArrowDescriptor*)arrow
{
	CGRect frame = arrow.dst.frame;
	CGFloat y = frame.origin.y;
	NSUInteger xrefCount = [[_xrefsCount objectForKey:arrow.dst] unsignedIntegerValue];
	NSUInteger timesVisited = [[_xrefsTimesVisited objectForKey:arrow.dst] unsignedIntegerValue];
	[_xrefsTimesVisited setObject:@(++timesVisited) forKey:arrow.dst];
	return CGPointMake(frame.origin.x + (frame.size.width / (xrefCount + 1)) * timesVisited, y);
}

-(void)drawArrow:(R2ArrowDescriptor*)arrow
{
	NSArray<NSValue*>* points = arrow.points;
	NSUInteger srcLayer = [_layout layerForBlock:arrow.src];
	NSUInteger dstLayer = [_layout layerForBlock:arrow.dst];

	CGPoint start = [self startPointForArrow:arrow];
	CGPoint end = [self endPointForArrow:arrow];

	UIBezierPath* path = [UIBezierPath bezierPath];
	[path moveToPoint:start];
	CGFloat startY = [self yOffsetForLayer:srcLayer];
	[path addLineToPoint:CGPointMake(start.x, startY)];

	for (NSValue* val in points)
	{
		CGPoint p = [val CGPointValue];
		CGFloat x = [self xOffsetForPoint:p];
		NSUInteger currentLayer = (NSUInteger)p.y;
		[path addLineToPoint:CGPointMake(x, path.currentPoint.y)];
		CGFloat nextY = [self yOffsetForLayer:currentLayer + (dstLayer > srcLayer ? 1 : 0)];
		CGPoint nextP = CGPointMake(x, nextY);
		[path addLineToPoint:nextP];
	}

	[path addLineToPoint:CGPointMake(end.x, path.currentPoint.y)];
	[path addLineToPoint:end];

	const CGFloat arrowWidth = 7;
	const CGFloat arrowHeight = 4;
	UIBezierPath* arrowHead = [UIBezierPath bezierPath];
	[arrowHead moveToPoint:path.currentPoint];
	[arrowHead addLineToPoint:CGPointMake(path.currentPoint.x - arrowWidth / 2, path.currentPoint.y - arrowHeight)];
	[arrowHead addLineToPoint:CGPointMake(arrowHead.currentPoint.x + arrowWidth, arrowHead.currentPoint.y)];
	[arrowHead closePath];

	arrow.path = path;
	arrow.arrowHeadPath = arrowHead;
}

/*
Plan for preventing overlapping lines:
- Get a count of how many paths go through each layer
- Get a count of how many paths go though each point
- yOffsetForLayer: returns a value each subsequent time it is called on the same layer
- xOffsetForPoint: returns a value each subsequent time it is called on the same point

To prevent overlapping destinations:
- Get a count of numbers of xrefs for each block
- end x adjusted based each subsequent time drawArrowFromBlock is called on the same dst block

To prevent overlapping destinations:
- drawArrowFromBlock: takes a srcPoint
- This is different for jump and fail (sourcePointForJumpFromBlock: / sourcePointForFailFromBlock:)
*/

-(void)doCount
{
	_layerLineCount = [NSMutableDictionary new];
	_layerTimesVisited = [NSMutableDictionary new];
	_pointCount = [NSMutableDictionary new];
	_pointTimesVisited = [NSMutableDictionary new];
	_xrefsCount = [NSMapTable weakToStrongObjectsMapTable];
	_xrefsTimesVisited = [NSMapTable weakToStrongObjectsMapTable];

	#define INCREMENT_LAYER(layer) _layerLineCount[@(layer)] = @([_layerLineCount[@(layer)] unsignedIntegerValue] + 1)
	#define INCREMENT_POINT(p) _pointCount[p] = @([_pointCount[p] unsignedIntegerValue] + 1)
	#define INCREMENT_XREFS(blck) [_xrefsCount setObject:@([[_xrefsCount objectForKey:blck] unsignedIntegerValue] + 1) forKey:blck]

	for (R2ArrowDescriptor* arrow in _arrows)
	{
		INCREMENT_XREFS(arrow.dst);
		NSUInteger srcLayer = [_layout layerForBlock:arrow.src];
		NSUInteger dstLayer = [_layout layerForBlock:arrow.dst];
		if (dstLayer > srcLayer)
			INCREMENT_LAYER(dstLayer - 1);
		else
			INCREMENT_LAYER(srcLayer);
		
		for (NSValue* point in arrow.points)
		{
			INCREMENT_LAYER((NSUInteger)[point CGPointValue].y);
			INCREMENT_POINT(point);
		}
	}
}

-(NSArray<R2ArrowDescriptor*>*)drawArrows
{
	_arrows = [NSMutableArray new];
	for (R2GraphBlock* block in _layout.allBlocks)
	{
		if (block.fail)
		{
			NSArray* points = [self pointsForPathFromBlock:block toBlock:block.fail];
			R2ArrowDescriptor* arrow = [R2ArrowDescriptor new];
			arrow.src = block;
			arrow.dst = block.fail;
			arrow.type = R2ArrowTypeFail;
			arrow.points = points;
			[_arrows addObject:arrow];
		}
		if (block.jump)
		{
			NSArray* points = [self pointsForPathFromBlock:block toBlock:block.jump];
			R2ArrowDescriptor* arrow = [R2ArrowDescriptor new];
			arrow.src = block;
			arrow.dst = block.jump;
			arrow.type = R2ArrowTypeJump;
			arrow.points = points;
			[_arrows addObject:arrow];
		}
	}

	[self doCount];

	for (R2ArrowDescriptor* arrow in _arrows)
	{
		[self drawArrow:arrow];
	}

	return _arrows;
}
@end
