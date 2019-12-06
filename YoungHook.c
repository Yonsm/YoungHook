

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

//
#ifdef _Support_CydiaSubstrate
bool MSHookFunction(void *symbol, void *hook, void **old)
{
	static void (*_MSHookFunction)(void *symbol, void *hook, void **old) = NULL;
	if (_MSHookFunction == NULL)
	{
		_MSHookFunction = dlsym(dlopen("/Library/Frameworks/CydiaSubstrate.framework/CydiaSubstrate", RTLD_LAZY), "MSHookFunction");
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
		_MSHookMessageEx = dlsym(dlopen(("/Library/Frameworks/CydiaSubstrate.framework/CydiaSubstrate", RTLD_LAZY), "MSHookMessageEx");
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
	SEL sel = _sel_registerName(msg);
	Class class = meta ? _objc_getMetaClass(cls) : _objc_getClass(cls);

#ifdef _Support_CydiaSubstrate
	if (MSHookMessage(class, sel, hook, old))
		return true;
#endif

	//
	Method method = _class_getInstanceMethod(class, sel);
	if (method == NULL)
	{
		_Log(CFSTR("YoungHook: Could not find [%s %s]"), cls, msg);
		return false;
	}

	*old = _method_setImplementation(method, hook);
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

