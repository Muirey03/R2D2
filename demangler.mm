#include "demangler.h"

extern "C" char* __cxa_demangle(const char* sym, char* buf, size_t* len, int* status);

bool isDigit(char c)
{
	return (c >= '0' && c <= '9');
}

extern "C" NSString* demangleSymbol(NSString* mangledSymbol)
{
	NSString* demangledStr;
	if (mangledSymbol.length < 2)
		return mangledSymbol;
	int status = 0;
	char* demangled = __cxa_demangle(&mangledSymbol.UTF8String[1], NULL, NULL, &status);
	if (!demangled || status != 0)
		goto fail;
	demangledStr = [NSString stringWithUTF8String:demangled];
	free((void*)demangled);
	return demangledStr;

fail:
	if (demangled)
		free((void*)demangled);
	
	//radare2 skips the last '_', try adding it back in:
	if (isDigit([mangledSymbol characterAtIndex:mangledSymbol.length - 1]))
	{
		NSString* corrected = [mangledSymbol stringByAppendingString:@"_"];
		NSString* attempt2 = demangleSymbol(corrected);
		if (![corrected isEqualToString:attempt2])
			return attempt2;
	}
	return mangledSymbol;
}
