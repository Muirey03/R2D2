#import "R2GraphBlock.h"

@interface R2GraphLayout : NSObject
@property (nonatomic, readonly) NSUInteger layerCount;
@property (nonatomic, strong) NSArray<R2GraphBlock*>* allBlocks;
+(CGFloat)verticalPadding;
+(CGFloat)horizontalPadding;
+(CGFloat)contentPadding;
-(instancetype)initWithFunction:(NSDictionary*)function;
-(NSArray<R2GraphBlock*>*)blocksInLayer:(NSUInteger)layer;
-(NSUInteger)layerForBlock:(R2GraphBlock*)block;
-(CGFloat)heightForLayer:(NSUInteger)layer;
-(CGFloat)widthForLayer:(NSUInteger)layer;
-(CGSize)contentSize;
-(void)layoutGraph;
@end
