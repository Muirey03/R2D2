#import "R2GraphLayout.h"

#define pVal unsignedLongLongValue

R2GraphBlock* blockForAddress(NSArray<NSDictionary*>* blocks, NSMutableDictionary* blockPool, uint64_t addr)
{
	if (blockPool[@(addr)])
		return blockPool[@(addr)];
	
	for (NSDictionary* blck in blocks)
	{
		if ([blck[@"offset"] pVal] == addr)
		{
			R2GraphBlock* block = [[R2GraphBlock alloc] initWithDictionary:blck];
			blockPool[@(addr)] = block;
			return block;
		}
	}
	return nil;
}

void populateTreeBranch(NSArray<NSDictionary*>* blocks, NSMutableDictionary* blockPool, R2GraphBlock* head)
{
	if (!head.fail && head.dict[@"fail"])
	{
		R2GraphBlock* fail = blockForAddress(blocks, blockPool, [head.dict[@"fail"] pVal]);
		head.fail = fail;
		populateTreeBranch(blocks, blockPool, fail);
	}
	if (!head.jump && head.dict[@"jump"])
	{
		R2GraphBlock* jump = blockForAddress(blocks, blockPool, [head.dict[@"jump"] pVal]);
		head.jump = jump;
		populateTreeBranch(blocks, blockPool, jump);
	}
}

void recurser(NSMutableArray<NSMutableArray*>* layers, R2GraphBlock* block, NSUInteger layer)
{
	if (block.fail && !block.fail.placed)
	{
		//insert block on layer+1
		if (layer + 1 >= layers.count)
			[layers addObject:[NSMutableArray array]];
		[layers[layer+1] addObject:block.fail];
		block.fail.placed = YES;
		recurser(layers, block.fail, layer + 1);
	}
	if (block.jump && !block.jump.placed)
	{
		//insert block on layer+1
		if (layer + 1 >= layers.count)
			[layers addObject:[NSMutableArray array]];
		[layers[layer+1] addObject:block.jump];
		block.jump.placed = YES;
		recurser(layers, block.jump, layer + 1);
	}
}

@implementation R2GraphLayout
{
	NSArray<NSArray<R2GraphBlock*>*>* _layers;
}

+(CGFloat)verticalPadding
{
	return 75;
}

+(CGFloat)horizontalPadding
{
	return 50;
}

+(CGFloat)contentPadding
{
	return 400;
}

-(instancetype)initWithFunction:(NSDictionary*)function
{
	if ((self = [self init]))
	{
		NSArray<NSDictionary*>* blocks = function[@"blocks"];
		if (!blocks.count)
			return self;

		NSMutableDictionary* blockPool = [NSMutableDictionary dictionary];

		//represent the graph as a tree:
		NSDictionary* root = blocks.firstObject;
		R2GraphBlock* rootBlock = blockForAddress(blocks, blockPool, [root[@"offset"] pVal]);
		populateTreeBranch(blocks, blockPool, rootBlock);

		//find layer of each block:
		NSMutableArray<NSMutableArray*>* layers = [NSMutableArray array];
		R2GraphBlock* head = rootBlock;
		[layers addObject:[@[head] mutableCopy]];
		recurser(layers, head, 0);
		_layers = layers;

		[self layoutGraph];

		_allBlocks = blockPool.allValues;
	}
	return self;
}

-(NSUInteger)layerCount
{
	return _layers.count;
}

-(NSArray<R2GraphBlock*>*)blocksInLayer:(NSUInteger)layer
{
	return _layers[layer];
}

-(NSUInteger)layerForBlock:(R2GraphBlock*)block
{
	for (NSUInteger layer = 0; layer < [self layerCount]; layer++)
	{
		if ([[self blocksInLayer:layer] containsObject:block])
			return layer;
	}
	return -1;
}

-(CGFloat)heightForLayer:(NSUInteger)layer
{
	NSArray<R2GraphBlock*>* blocks = [self blocksInLayer:layer];
	CGFloat maxHeight = 0;
	for (R2GraphBlock* block in blocks)
	{
		CGSize sz = [block blockSize];
		if (sz.height > maxHeight)
			maxHeight = sz.height;
	}
	return maxHeight;
}

-(CGFloat)widthForLayer:(NSUInteger)layer
{
	NSArray<R2GraphBlock*>* blocks = [self blocksInLayer:layer];
	CGFloat width = 0;
	for (R2GraphBlock* block in blocks)
	{
		CGSize sz = [block blockSize];
		width += sz.width;
	}
	width += (blocks.count + 1) * [R2GraphLayout horizontalPadding];
	return width;
}

-(CGSize)contentSize
{
	CGSize maxSize = CGSizeZero;
	CGFloat height = 2 * [R2GraphLayout contentPadding] + (self.layerCount - 1) * [R2GraphLayout verticalPadding];
	for (NSUInteger layer = 0; layer < self.layerCount; layer++)
	{
		CGFloat layerWidth = [self widthForLayer:layer];
		CGFloat layerHeight = [self heightForLayer:layer];
		if (layerWidth > maxSize.width)
			maxSize.width = layerWidth;
		height += layerHeight;
	}
	return CGSizeMake(2 * [R2GraphLayout contentPadding] + maxSize.width, height);
}

-(void)layoutGraph
{
	CGFloat minX = CGFLOAT_MAX;
	CGFloat y = [R2GraphLayout verticalPadding];
	for (NSUInteger layer = 0; layer < self.layerCount; layer++)
	{
		NSArray<R2GraphBlock*>* blocks = [self blocksInLayer:layer];
		CGFloat layerWidth = [self widthForLayer:layer];
		CGFloat x = -layerWidth / 2;
		if (x < minX)
			minX = x;
		for (R2GraphBlock* block in blocks)
		{
			CGSize sz = [block blockSize];
			block.frame = CGRectMake(x, y, sz.width, sz.height);
			x += sz.width + [R2GraphLayout horizontalPadding];
		}
		y += [self heightForLayer:layer] + [R2GraphLayout verticalPadding];
	}

	CGFloat xDiff = [R2GraphLayout horizontalPadding] - minX;
	for (NSUInteger layer = 0; layer < self.layerCount; layer++)
	{
		NSArray<R2GraphBlock*>* blocks = [self blocksInLayer:layer];
		for (R2GraphBlock* block in blocks)
		{
			CGRect frame = block.frame;
			frame.origin.x += xDiff + [R2GraphLayout contentPadding];
			frame.origin.y += [R2GraphLayout contentPadding];
			block.frame = frame;
		}
	}
}
@end
