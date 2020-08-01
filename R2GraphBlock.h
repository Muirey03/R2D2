@class R2GraphBlock;

@interface R2GraphBlock : NSObject
@property (nonatomic, strong) NSDictionary* dict;
@property (nonatomic, readonly) uint64_t offset;
@property (nonatomic, weak) R2GraphBlock* fail;
@property (nonatomic, weak) R2GraphBlock* jump;
@property (nonatomic, assign) BOOL placed;
@property (nonatomic, assign) CGRect frame;
+(CGFloat)textPadding;
-(instancetype)initWithDictionary:(NSDictionary*)dict;
-(NSAttributedString*)text;
-(CGSize)blockSize;
@end
