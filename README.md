
Hook Util
======

A light weight hook utility for iOS.

#Usage

#import "HookUtil.h"

##Hook a Object C Message

	HOOK_MESSAGE(BOOL, UIApplication, openURL_, NSURL *URL)
	{
		NSLog(@"%s: %@", __FUNCTION__, URL);
		return _UIApplication_openURL_(self, sel, URL);
	}

The above code block will create a `automatically-initialized hook function` (with constructor keyword). Just focus on the hook function body without any other preparation code. The `self` and `sel` are solid naming variables in `HOOK_MESSAGE` macro.

Use `_HOOK_MESSAGE` (instead of `HOOK_MESSAGE`) to create a `manually-initialized hook function`. You must call `_Init_openURL_()` if you want to enable it.

##Hook a C/C++ function

	HOOK_FUNCTION(CFReadStreamRef, /System/Library/Frameworks/CoreFoundation.framework/CoreFoundation, CFReadStreamCreateForHTTPRequest, CFAllocatorRef alloc, CFHTTPMessageRef request)
	{
		NSLog(@"%s: %p", __FUNCTION__, request);
		return _CFReadStreamCreateForHTTPRequest(alloc, request);
	}

Just like _HOOK_MESSAGE, use `_HOOK_FUNCTION` (instead of `HOOK_FUNCTION`) to create a `manually-initialized hook function`. You must call `_Init_CFReadStreamCreateForHTTPRequest()` if you want to enable it.

`HOOK_FUNCTION` and `_HOOK_FUNCTION` can be used only on a jail-broken device with `Cydia Substrate` installed.

#Sample

[HttPeek](https://github.com/Yonsm/HttPeek)
