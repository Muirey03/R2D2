#import "R2ANSIParser.h"

@implementation R2ANSIParser
{
	NSString* _str;
}

-(NSDictionary*)ansiColors
{
	NSDictionary<NSNumber*, NSDictionary*>* ansiColors = @{
		@0 : @{ NSForegroundColorAttributeName : [UIColor labelColor] },
		@30 : @{ NSForegroundColorAttributeName : [UIColor labelColor] },
		@31 : @{ NSForegroundColorAttributeName : [UIColor systemRedColor] },
		@32 : @{ NSForegroundColorAttributeName : [UIColor systemGreenColor] },
		@33 : @{ NSForegroundColorAttributeName : [UIColor systemYellowColor] },
		@34 : @{ NSForegroundColorAttributeName : [UIColor systemBlueColor] },
		@35 : @{ NSForegroundColorAttributeName : [UIColor magentaColor] },
		@36 : @{ NSForegroundColorAttributeName : [UIColor cyanColor] },
		@37 : @{ NSForegroundColorAttributeName : [UIColor labelColor] }
	};
	return ansiColors;
}

-(NSUInteger)indexOfNextANSIEscapeStartingAt:(NSUInteger)index
{
	if (index >= _str.length)
		return NSNotFound;
	NSRange searchRange = NSMakeRange(index, _str.length - index);
	return [_str rangeOfString:@"\\033[" options:kNilOptions range:searchRange].location;
}

-(NSString*)escapeSequenceAtIndex:(NSUInteger)index
{
	NSRange searchRange = NSMakeRange(index, _str.length - index);
	NSUInteger indexOfM = [_str rangeOfString:@"m" options:kNilOptions range:searchRange].location;
	if (indexOfM == NSNotFound)
		return nil;
	return [_str substringWithRange:NSMakeRange(index, indexOfM - index + 1)];
}

-(NSDictionary*)attributesForEscape:(NSString*)escape
{
	int(^ctoi)(char) = ^int(char c)
	{
		return (int)c - (int)'0';
	};

	BOOL bold = 0;
	NSUInteger indexOfSemicolon = [escape rangeOfString:@";"].location;
	if (indexOfSemicolon != NSNotFound)
		bold = (ctoi([escape characterAtIndex:indexOfSemicolon - 1]) == 1);
	NSUInteger startIndex = indexOfSemicolon != NSNotFound ? indexOfSemicolon + 1 : [escape rangeOfString:@"["].location + 1;
	NSUInteger indexOfM = [escape rangeOfString:@"m"].location;
	NSString* colNumberStr = [escape substringWithRange:NSMakeRange(startIndex, indexOfM - startIndex)];
	int colNum = atoi(colNumberStr.UTF8String);
	return [self ansiColors][@(colNum)];
}

-(void)removeANSISequences:(NSMutableAttributedString*)attributedString
{
	NSUInteger index = 0;
	while (index != NSNotFound)
	{
		index = [attributedString.mutableString rangeOfString:@"\\033["].location;
		if (index == NSNotFound)
			break;
		NSRange searchRange = NSMakeRange(index, attributedString.length - index);
		NSUInteger indexOfM = [attributedString.mutableString rangeOfString:@"m" options:kNilOptions range:searchRange].location;
		if (indexOfM == NSNotFound)
			break;
		[attributedString deleteCharactersInRange:NSMakeRange(index, indexOfM - index + 1)];
	}
}

-(NSAttributedString*)attributedStringWithANSIString:(NSString*)str
{
	if (!str)
		return nil;
	str = [str stringByReplacingOccurrencesOfString:@"\033" withString:@"\\033"];
	_str = str;

	NSMutableAttributedString* attributedString = [[NSMutableAttributedString alloc] initWithString:str];
	[attributedString addAttributes:[self ansiColors][@0] range:NSMakeRange(0, attributedString.string.length)];

	NSUInteger index = 0;
	NSDictionary* prevAttrib = nil;
	NSUInteger prevIndex = 0;
	while (index != NSNotFound)
	{
		index = [self indexOfNextANSIEscapeStartingAt:index];
		if (index == NSNotFound)
			break;
		NSString* escape = [self escapeSequenceAtIndex:index];
		NSDictionary* attrib = [self attributesForEscape:escape];
		
		if (prevAttrib)
			[attributedString setAttributes:prevAttrib range:NSMakeRange(prevIndex, index - prevIndex)];
		prevIndex = index;
		prevAttrib = attrib;

		index++;
	}
	if (prevAttrib)
		[attributedString setAttributes:prevAttrib range:NSMakeRange(prevIndex, _str.length - prevIndex)];
	
	[self removeANSISequences:attributedString];

	return attributedString;
}
@end
