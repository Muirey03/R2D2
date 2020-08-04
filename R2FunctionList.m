#import "R2FunctionList.h"
#import "R2Core.h"
#include "demangler.h"

@implementation R2FunctionList
+(instancetype)sharedInstance
{
	static R2FunctionList* instance = nil;
	static dispatch_once_t token = 0;
	dispatch_once(&token, ^{
		instance = [R2FunctionList new];
	});
	return instance;
}

-(instancetype)init
{
	if ((self = [super init]))
	{
		[self reloadFunctions];
	}
	return self;
}

-(void)reloadFunctions
{
	_currentFunction = -1;
	NSArray* functions = [R2Core sharedInstance].projectName ? [[R2Core sharedInstance] cmdJSON:@"aflj"] : nil;
	if (!functions)
	{
		_allFunctions = nil;
		[self postCurrentFunctionDidChangeNotification];
		return;
	}
	NSMutableArray* mutableFunctions = [[NSMutableArray alloc] initWithCapacity:functions.count];
	for (NSDictionary* fn in functions)
	{
		if (![fn[@"name"] isEqualToString:@"skip"])
			[mutableFunctions addObject:[fn mutableCopy]];
	}
	_allFunctions = mutableFunctions;
	[self postCurrentFunctionDidChangeNotification];
}

-(NSDictionary*)functionNamed:(NSString*)name
{
	for (NSDictionary* fn in _allFunctions)
		if ([fn[@"name"] isEqualToString:name])
			return fn;
	return nil;
}

-(NSDictionary*)functionAtAddress:(uint64_t)addr
{
	for (NSDictionary* fn in _allFunctions)
	{
		unsigned long long start = [fn[@"offset"] unsignedLongLongValue];
		unsigned long long sz = [fn[@"size"] unsignedLongLongValue];
		if (addr - start < sz)
			return fn;
	}
	return nil;
}

-(NSString*)demangledNameForFunctionAtIndex:(NSUInteger)index
{
	NSMutableDictionary* fn = _allFunctions[index];
	if (fn[@"mryDemangledName"])
		return fn[@"mryDemangledName"];
	NSString* mangledName = fn[@"name"];
	NSMutableArray* components = [[mangledName componentsSeparatedByString:@"."] mutableCopy];
	if (!components.count)
		return mangledName;
	NSString* symbol = components.lastObject;
	NSString* demangled = demangleSymbol(symbol);
	components[components.count - 1] = demangled;
	NSString* demangledName = [components componentsJoinedByString:@"."];
	fn[@"mryDemangledName"] = demangledName;
	return demangledName;
}

-(void)setCurrentFunction:(NSInteger)newFunc
{
	NSInteger oldFunc = _currentFunction;
	_currentFunction = newFunc;
	if (oldFunc != newFunc)
		[self postCurrentFunctionDidChangeNotification];
}

-(NSString*)signatureForFunctionAtIndex:(NSUInteger)index
{
	NSDictionary* fn = _allFunctions[index];
	unsigned long long offset = [fn[@"offset"] unsignedLongLongValue];
	[[R2Core sharedInstance] seek:offset];
	return [[R2Core sharedInstance] cmd:@"afs"];
}

-(void)setSignature:(NSString*)sig forFunctionAtIndex:(NSUInteger)index
{
	NSDictionary* fn = _allFunctions[index];
	unsigned long long offset = [fn[@"offset"] unsignedLongLongValue];
	[[R2Core sharedInstance] seek:offset];
	[[R2Core sharedInstance] cmd:[NSString stringWithFormat:@"afs %@", sig]];
}

-(void)postCurrentFunctionDidChangeNotification
{
	[[NSNotificationCenter defaultCenter] postNotificationName:kCurrentFunctionChangeNotification object:nil];
}
@end
