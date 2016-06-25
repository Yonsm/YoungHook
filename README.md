
Hook Util
======

A light weight hook utility for iOS.

#Usage

	#import "HookUtil.h"

###Hook a Object C Message

	HOOK_MESSAGE(BOOL, UIApplication, openURL_, NSURL *URL)
	{
		NSLog(@"%s: %@", __FUNCTION__, URL);
		return _UIApplication_openURL_(self, sel, URL);
	}

The above code block will create a `automatically-initialized hook function` (with constructor keyword). Just focus on the hook function body without any other preparation code. The `self` and `sel` are solid naming variables in `HOOK_MESSAGE` macro.

Use `_HOOK_MESSAGE` (instead of `HOOK_MESSAGE`) to create a `manually-initialized hook function`. You must call `_Init_openURL_()` if you want to enable it.

###Hook a C/C++ function

	HOOK_FUNCTION(OSStatus, /System/Library/Frameworks/Security.framework/Security, SSLHandshake, SSLContextRef context)
	{
		OSStatus ret = _SSLHandshake(context);
	    return (ret == errSSLServerAuthCompleted) ? _SSLHandshake(context) : ret;
	}

Just like _HOOK_MESSAGE, use `_HOOK_FUNCTION` (instead of `HOOK_FUNCTION`) to create a `manually-initialized hook function`. You must call `_Init_SSLHandshake()` if you want to enable it.

`HOOK_FUNCTION` and `_HOOK_FUNCTION` can be used only on a jail-broken device with `Cydia Substrate` installed.

#Sample

See [HttPeek](https://github.com/Yonsm/HttPeek)
