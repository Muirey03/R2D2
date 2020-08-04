#define kCurrentFunctionChangeNotification @"com.muirey03.r2d2-currentFunctionDidChange"

@interface R2FunctionList : NSObject
@property (nonatomic, strong) NSArray<NSMutableDictionary*>* allFunctions;
@property (nonatomic, assign) NSInteger currentFunction;
+(instancetype)sharedInstance;
-(void)reloadFunctions;
-(NSDictionary*)functionNamed:(NSString*)name;
-(NSDictionary*)functionAtAddress:(uint64_t)addr;
-(NSString*)demangledNameForFunctionAtIndex:(NSUInteger)index;
-(NSString*)signatureForFunctionAtIndex:(NSUInteger)index;
-(void)setSignature:(NSString*)sig forFunctionAtIndex:(NSUInteger)index;
-(void)postCurrentFunctionDidChangeNotification;
@end
