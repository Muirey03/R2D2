#include <r_core.h>

#define kProjectDidChangeNotification @"com.muirey03.r2d2-projectDidChange"

@interface R2Core : NSObject
@property (nonatomic, strong) NSString* projectName;
@property (nonatomic, readonly) RCore* core;
+(instancetype)sharedInstance;
-(void)reloadCore;
-(void)setupConfig;
-(BOOL)loadFile:(NSString*)filePath;
-(void)saveProjectNamed:(NSString*)name;
-(void)openProjectNamed:(NSString*)name;
-(NSArray*)allProjects;
-(void)analyzeWithCompletion:(void(^)(void))completion;
-(NSString*)cmd:(NSString*)cmdStr;
-(void)cmd:(NSString*)cmdStr completion:(void(^)(NSString*))completion;
-(void)seek:(unsigned long long)addr;
-(id)cmdJSON:(NSString*)cmdStr;
@end
