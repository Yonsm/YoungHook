

#import <stdlib.h>
#import <stdio.h>
#import <dlfcn.h>
#import "YoungHook.h"

#include <CoreFoundation/CoreFoundation.h>
void NSLog(CFStringRef format, ...);

#ifndef _Log
#if defined(DEBUG) || defined(TEST)
#define _Log(s, ...)	NSLog(s, ##__VA_ARGS__)
#else
#define _Log(s, ...)	((void) 0)
#endif
#endif

// /Library/Frameworks/CydiaSubstrate.framework/CydiaSubstrate
#define CAMO_Library_Frameworks_CydiaSubstrate_framework_CydiaSubstrate char camo_Library_Frameworks_CydiaSubstrate_framework_CydiaSubstrate[60]; CamoStringDecode(camo_Library_Frameworks_CydiaSubstrate_framework_CydiaSubstrate, "\x14\x76\x50\x56\x45\x55\x43\x3b\x0c\x74\x3f\x4f\x4a\x51\x3e\x45\x39\x3f\x36\x01\x64\x2d\x49\x3b\x42\x4f\x34\x3e\x2c\x32\x2b\x3b\x2f\x3d\xfffffff3\x3a\x25\x35\x30\x37\x24\x2b\x1f\x25\xffffffe8\x4b\x14\x30\x22\x29\x36\x1b\x25\x13\x19\x12\x22\x16\x24", 59)
// MSHookMessageEx
#define CAMO_MSHookMessageEx char camo_MSHookMessageEx[16]; CamoStringDecode(camo_MSHookMessageEx, "\x42\x5b\x45\x5d\x5c\x5f\x3c\x63\x74\x73\x64\x5d\x5e\x3d\x69", 15)
// MSHookFunction
#define CAMO_MSHookFunction char camo_MSHookFunction[15]; CamoStringDecode(camo_MSHookFunction, "\x43\x5c\x44\x5e\x5d\x60\x42\x74\x58\x64\x70\x5c\x55\x53", 14)

//
#ifdef YoungHook_CydiaSubstrate
bool MSHookFunction(void *symbol, void *hook, void **old)
{
	static void (*_MSHookFunction)(void *symbol, void *hook, void **old) = NULL;
	if (_MSHookFunction == NULL)
	{
		CAMO_Library_Frameworks_CydiaSubstrate;
		CAMO_MSHookFunction;
		_MSHookFunction = dlsym(dlopen(camo_Library_Frameworks_CydiaSubstrate, RTLD_LAZY), camo_MSHookFunction);
		_Log(CFSTR("YoungHook: _MSHookFunction = %p"), _MSHookFunction);
		if (_MSHookFunction == NULL)
		{
			_MSHookFunction = (void *)-1;
		}
	}

	//
	if (_MSHookFunction && (_MSHookFunction != (void *)-1))
	{
		_MSHookFunction(symbol, hook, old);
		return true;
	}
	
	//*old = NULL;
	return false;
}

//
bool MSHookMessage(Class cls, SEL sel, IMP hook, IMP *old)
{
	static void (*_MSHookMessageEx)(Class cls, SEL sel, IMP hook, IMP *old) = NULL;
	if (_MSHookMessageEx == nil)
	{
		CAMO_Library_Frameworks_CydiaSubstrate;
		CAMO_MSHookMessageEx;
		_MSHookMessageEx = dlsym(dlopen(camo_Library_Frameworks_CydiaSubstrate, RTLD_LAZY), camo_MSHookMessageEx);
		_Log(CFSTR("YoungHook: _MSHookMessageEx = %p"), _MSHookMessageEx);
		if (_MSHookMessageEx == NULL)
		{
			_MSHookMessageEx = (void *)-1;
		}
	}
	
	//
	if (_MSHookMessageEx && (_MSHookMessageEx != (void *)-1))
	{
		_MSHookMessageEx(cls, sel, hook, old);
		return true;
	}
	
	//*old = NULL;
	return false;
}
#endif

#if defined(YoungHook_CydiaSubstrate) || defined(YoungHook_FishHook)
#ifdef YoungHook_FishHook
#import "fishhook.h"
#endif
bool YHHookFunction(const char *lib, const char *func, void *hook, void **old)
{
#ifdef YoungHook_CydiaSubstrate
	if (MSHookFunction(dlsym(dlopen(lib, RTLD_LAZY), func), hook, old))
		return true;
#endif

#ifdef YoungHook_FishHook
	return rebind_symbols((struct rebinding[1]){{func, hook, old}}, 1);
#else
	return false;
#endif
}
#endif

//
bool YHHookMessage(const char *cls, bool meta, const char *name, IMP hook, IMP *old)
{
#ifdef YoungHook_AutoInit
	if (Camo_sel_registerName == NULL)
		CamoCallInit();
#endif

	char msg[1024], *p = msg;
	do
	{
		*p++ = (*name == '_') ? ((name[1] == '_') ? *name++ : ':') : *name;
	}
	while (*name++);
	SEL sel = Camo_sel_registerName(msg);
	Class class = meta ? Camo_objc_getMetaClass(cls) : Camo_objc_getClass(cls);

#ifdef YoungHook_CydiaSubstrate
	if (MSHookMessage(class, sel, hook, old))
		return true;
#endif

	//
	Method method = Camo_class_getInstanceMethod(class, sel);
	if (method == NULL)
	{
		_Log(CFSTR("YoungHook: Could not find [%s %s]"), cls, msg);
		return false;
	}

	*old = Camo_method_setImplementation(method, hook);
	return true;
}

//
#ifdef YoungHook_MultiProcess
bool YHIsAnyOneMatched(const char *any, const char *one, char separator)
{
	for (const char *p = one; true; any++)
	{
		if (*p)
		{
			if (*any == *p)
			{
				p++;
				continue;
			}
			else if (*any == 0)
			{
				return false;
			}
			else if (*any == separator)
			{
				p = one;
				continue;
			}
		}
		else if (*any == 0 || *any == separator)
		{
			return true;
		}
		
		for (; *any != separator; any++)
		{
			if (*any == 0)
			{
				return false;
			}
		}
		p = one;
	}
}

//
#if defined(YoungHook_CydiaSubstrate) || defined(YoungHook_FishHook)
bool YHHookFunctionForProcess(const char *proc, const char *lib, const char *func, void *hook, void **old)
{
	if (YHIsAnyOneMatched(proc, getprogname(), '|'))
	{
		return YHHookFunction(lib, func, hook, old);
	}
	return false;
}
#endif

//
bool YHHookMessageForProcess(const char *proc, const char *cls, bool meta, const char *name, IMP hook, IMP *old)
{
	if (YHIsAnyOneMatched(proc, getprogname(), '|'))
	{
		return YHHookMessage(cls, meta, name, hook, old);
	}
	return false;
}
#endif

