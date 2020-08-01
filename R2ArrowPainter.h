#import "R2GraphBlockView.h"
#import "R2GraphLayout.h"

typedef NS_ENUM(NSInteger, R2ArrowType)
{
	R2ArrowTypeJump,
	R2ArrowTypeFail
};

@interface R2ArrowDescriptor : NSObject
@property (nonatomic, weak) R2GraphBlock* src;
@property (nonatomic, weak) R2GraphBlock* dst;
@property (nonatomic, assign) R2ArrowType type;
@property (nonatomic, strong) NSArray* points;
@property (nonatomic, strong) UIBezierPath* path;
@property (nonatomic, strong) UIBezierPath* arrowHeadPath;
@end

@interface R2ArrowPainter : NSObject
@property (nonatomic, strong) R2GraphLayout* layout;
-(instancetype)initWithGraphLayout:(R2GraphLayout*)layout;
-(NSArray<R2ArrowDescriptor*>*)drawArrows;
@end
