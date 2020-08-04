#import "R2PluginManager.h"
#import "R2Core.h"
#include <dlfcn.h>
#include <unordered_map>
#include <string>

@implementation R2PluginManager
{
	std::unordered_map<std::string, RCorePlugin*> _plugins;
}

+(instancetype)sharedInstance
{
	static R2PluginManager* instance = nil;
	static dispatch_once_t token = 0;
	dispatch_once(&token, ^{
		instance = [R2PluginManager new];
	});
	return instance;
}

-(BOOL)isPluginLoaded:(NSString*)pluginName
{
	return !!_plugins[pluginName.UTF8String];
}

-(BOOL)loadPluginAtPath:(NSString*)pluginPath
{
	const char* file = pluginPath.UTF8String;
	void* handle = r_lib_dl_open(file);
	if (!handle)
		return NO;

	RLibStruct*(*strf)(void) = (RLibStruct*(*)(void))r_lib_dl_sym(handle, "radare_plugin_function");
	RLibStruct* stru = NULL;
	if (strf)
		stru = strf();
	else
		stru = (RLibStruct*)r_lib_dl_sym(handle, "radare_plugin");
	if (!stru)
		return NO;

	//Bit of a hack here:
	if (stru->type == R_LIB_TYPE_CORE)
	{
		RCorePlugin* corePlugin = (RCorePlugin*)stru->data;
		const char* name = corePlugin->name;
		RCmd* rcmd = r_cmd_new();
		rcmd->data = (void*)[R2Core sharedInstance].core;
		corePlugin->init(rcmd, "");
		_plugins[name] = corePlugin;
		r_cmd_free(rcmd);
	}

	if (strf)
		free(stru);
	return YES;
}

-(NSString*)cmd:(NSString*)cmd forPlugin:(NSString*)pluginName
{
	RCorePlugin* plugin = _plugins[pluginName.UTF8String];
	if (!plugin)
		return nil;
	r_cons_push();
	plugin->call((void*)[R2Core sharedInstance].core, cmd.UTF8String);
	const char* retstr = r_cons_get_buffer();
	NSString* ret = retstr ? [NSString stringWithUTF8String:retstr] : nil;
	r_cons_pop();
	return ret;
}

-(void)unloadPluginNamed:(NSString*)pluginName
{
	std::string key = pluginName.UTF8String;
	RCorePlugin* plugin = _plugins[key];
	plugin->fini(NULL, "");
	_plugins.erase(key);
}

-(void)unloadAllPlugins
{
	NSMutableArray* pluginNames = [[NSMutableArray alloc] initWithCapacity:_plugins.size()];
	for (auto iter = _plugins.begin(); iter != _plugins.end(); iter++)
	{
		std::string key = iter->first;
		[pluginNames addObject:[NSString stringWithUTF8String:key.c_str()]];
	}
	for (NSString* pluginName in pluginNames)
		[self unloadPluginNamed:pluginName];
}
@end
