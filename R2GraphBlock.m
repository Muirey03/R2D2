#import "R2GraphBlock.h"
#import "R2ANSIParser.h"
#import "R2Core.h"

@implementation R2GraphBlock
{
	NSAttributedString* _text;
}

+(CGFloat)textPadding
{
	return 15;
}

-(instancetype)initWithDictionary:(NSDictionary*)dict
{
	if ((self = [super init]))
	{
		_dict = dict;
		_offset = [dict[@"offset"] unsignedLongLongValue];
	}
	return self;
}

-(NSAttributedString*)text
{
	if (_text)
		return _text;
	R2Core* core = [R2Core sharedInstance];
	[core seek:[_dict[@"offset"] unsignedLongLongValue]];
	NSString* disasm = [core cmd:[NSString stringWithFormat:@"pd %lu", (unsigned long)[_dict[@"ops"] count]]];
	disasm = [disasm stringByReplacingOccurrencesOfString:@"\x1b" withString:@"\\033"];

	NSString*(^trimLeadingWhitespace)(NSString*) = ^NSString*(NSString* str)
	{
		NSInteger i = 0;
		while ((i < str.length) && [[NSCharacterSet whitespaceCharacterSet] characterIsMember:[str characterAtIndex:i]])
			i++;
		return [str substringFromIndex:i];
	};
	NSMutableArray* lines = [[disasm componentsSeparatedByString:@"\n"] mutableCopy];
	if (![lines.lastObject length])
		[lines removeLastObject];
	for (NSUInteger i = 0; i < lines.count; i++)
		lines[i] = trimLeadingWhitespace(lines[i]);
	disasm = [lines componentsJoinedByString:@"\n"];

	if (!disasm)
		return nil;

	R2ANSIParser* ansiParser = [R2ANSIParser new];
	_text = [ansiParser attributedStringWithANSIString:disasm];
	return _text;
}

-(CGSize)blockSize
{
	CGFloat padding = [R2GraphBlock textPadding];
	CGSize textSize = [[self text] size];
	return CGSizeMake(textSize.width + 2 * padding, textSize.height + 2 * padding);
}
@end
