#import "R2Core.h"
#import "R2PluginManager.h"

@implementation R2Core
{
	RCoreFile* _file;
	NSOperationQueue* _queue;
}

+(instancetype)sharedInstance
{
	static R2Core* instance = nil;
	static dispatch_once_t token = 0;
	dispatch_once(&token, ^{
		instance = [R2Core new];
	});
	return instance;
}

-(instancetype)init
{
	if ((self = [super init]))
	{
		[self reloadCore];
		_queue = [NSOperationQueue new];
	}
	return self;
}

-(void)reloadCore
{
	[[R2PluginManager sharedInstance] unloadAllPlugins];
	if (_file)
		r_core_file_close(_core, _file);
	if (_core)
		r_core_free(_core);
	_file = NULL;
	_projectName = nil;
	_core = r_core_new();
	if (!_core)
		abort();
	[self setupConfig];
}

-(void)setupConfig
{
	[self cmd:@"e bin.demangle=false"];
	[self cmd:@"e asm.demangle=true"];
	[self cmd:@"e asm.lines=false"];
	[self cmd:@"e asm.bytes=false"];
	[self cmd:@"e asm.bbline=false"];
	[self cmd:@"e asm.offset=false"];
	[self cmd:@"e asm.xrefs=false"];
	[self cmd:@"e asm.cmt.col=40"];
	[self cmd:@"e asm.noisy=false"];
}

-(BOOL)loadFile:(NSString*)path
{
	[self reloadCore];
	
	ut64 loadAddr = 0;
	_file = r_core_file_open(_core, path.UTF8String, O_RDONLY, loadAddr);
	if (!_file)
		return NO;
	ut64 baseAddr = 0;
	bool success = r_core_bin_load(_core, path.UTF8String, baseAddr);
	return (BOOL)success;
}

-(void)saveProjectNamed:(NSString*)name
{
	[self cmd:[NSString stringWithFormat:@"Ps %@", name]];
	_projectName = name;
}

-(void)openProjectNamed:(NSString*)name
{
	[self reloadCore];

	[self cmd:[NSString stringWithFormat:@"Po %@", name]];
	_projectName = name;
}

-(NSArray*)allProjects
{
	return [self cmdJSON:@"Pj"];
}

-(void)analyzeWithCompletion:(void(^)(void))completion
{
	[self cmd:@"aaa" completion:^(__unused NSString* ret){
		if (completion)
			completion();
	}];
}

-(NSString*)cmd:(NSString*)cmdStr
{
	char* res = r_core_cmd_str(_core, cmdStr.UTF8String);
	NSString* resStr = [NSString stringWithUTF8String:res];
	free(res);
	return resStr;
}

-(void)cmd:(NSString*)cmdStr completion:(void(^)(NSString*))completion
{
	[_queue addOperationWithBlock:^{
		NSString* ret = [self cmd:cmdStr];
		if (completion)
			completion(ret);
	}];
}

-(void)seek:(unsigned long long)addr
{
	r_core_seek(_core, addr, true);
}

-(id)cmdJSON:(NSString*)cmdStr
{
	NSUInteger opcodeLen = [cmdStr rangeOfString:@" "].location == NSNotFound ? cmdStr.length : [cmdStr rangeOfString:@" "].location;
	NSString* opcode = [cmdStr substringWithRange:NSMakeRange(0, opcodeLen)];
	NSString* newOpcode = [[opcode lowercaseString] characterAtIndex:opcode.length - 1] == 'j' ? opcode : [opcode stringByAppendingString:@"j"];
	NSString* newCmdStr = [newOpcode stringByAppendingString:[cmdStr substringWithRange:NSMakeRange(opcodeLen, cmdStr.length - opcodeLen)]];

	NSString* json = [self cmd:newCmdStr];
	if (!json)
		return nil;
	NSData* jsonData = [json dataUsingEncoding:NSUTF8StringEncoding];
	NSError* err = nil;
	id ret = [NSJSONSerialization JSONObjectWithData:jsonData options:kNilOptions error:&err];
	if (err)
		RLog(@"err: %@", err);
	return ret;
}

-(void)dealloc
{
	if (_file)
		r_core_file_close(_core, _file);
	r_core_free(_core);
}
@end
