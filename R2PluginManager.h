@interface R2PluginManager : NSObject
+(instancetype)sharedInstance;
-(BOOL)isPluginLoaded:(NSString*)pluginName;
-(BOOL)loadPluginAtPath:(NSString*)pluginPath;
-(NSString*)cmd:(NSString*)cmd forPlugin:(NSString*)pluginName;
-(void)unloadPluginNamed:(NSString*)pluginName;
-(void)unloadAllPlugins;
@end
