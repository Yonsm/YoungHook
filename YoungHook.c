

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
#define CAMO_Library_Frameworks_CydiaSubstrate char camo_Library_Frameworks_CydiaSubstrate[60]; CamoDecryptCString(camo_Library_Frameworks_CydiaSubstrate, "\x14\x76\x50\x5a\x45\x57\x47\x4d\x1c\x74\x43\x51\x42\x4b\x5a\x43\x59\x41\x5a\x07\x64\x5f\x41\x4d\x42\x71\x54\x42\x6c\x6a\x6f\x7d\x6f\x7f\x37\x7e\x65\x77\x78\x71\x64\x7d\x63\x7b\x20\x4d\x74\x68\x62\x6b\x5a\x7d\x65\x75\x71\x76\x62\x76\x64", 59)
// MSHookMessageEx
#define CAMO_MSHookMessageEx char camo_MSHookMessageEx[16]; CamoDecryptCString(camo_MSHookMessageEx, "\x42\x5d\x45\x63\x64\x61\x44\x6d\x74\x75\x64\x63\x66\x47\x79", 15)
// MSHookFunction
#define CAMO_MSHookFunction char camo_MSHookFunction[15]; CamoDecryptCString(camo_MSHookFunction, "\x43\x5e\x44\x64\x65\x62\x4e\x72\x68\x66\x70\x6a\x6d\x6f", 14)

//
#ifdef _Support_CydiaSubstrate
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

#if defined(_Support_CydiaSubstrate) || defined(_Support_FishHook)
#ifdef _Support_FishHook
#import "fishhook.h"
#endif
bool YHHookFunction(const char *lib, const char *func, void *hook, void **old)
{
#ifdef _Support_CydiaSubstrate
	if (MSHookFunction(dlsym(dlopen(lib, RTLD_LAZY), func), hook, old))
		return true;
#endif

#ifdef _Support_FishHook
	return rebind_symbols((struct rebinding[1]){{func, hook, old}}, 1);
#else
	return false;
#endif
}
#endif

//
bool YHHookMessage(const char *cls, bool meta, const char *name, IMP hook, IMP *old)
{
#ifdef _Support_CamoCall
	if (_sel_registerName == NULL)
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

#ifdef _Support_CydiaSubstrate
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
#ifdef _Support_MultiProcess
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
#if defined(_Support_CydiaSubstrate) || defined(_Support_FishHook)
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

