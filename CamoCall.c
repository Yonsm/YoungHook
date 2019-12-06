
#import "CamoCall.h"
#import <dlfcn.h>

void *(*Camo_objc_msgSend)(id, SEL, ...) = NULL;
Class (*Camo_objc_getClass)(const char *) = NULL;
Class (*Camo_objc_getMetaClass)(const char *) = NULL;
Method (*Camo_class_getInstanceMethod)(Class, SEL) = NULL;
SEL (*Camo_sel_registerName)(const char *) = NULL;
IMP (*Camo_method_setImplementation)(Method, IMP) = NULL;

void CamoCallInit()
{
	if (Camo_objc_msgSend)
		return;

	Camo_objc_msgSend = dlsym(dlopen("/usr/lib/libobjc.A.dylib", RTLD_LAZY), "objc_msgSend");
	Camo_objc_getClass = dlsym(dlopen("/usr/lib/libobjc.A.dylib", RTLD_LAZY), "objc_getClass");
	Camo_objc_getMetaClass = dlsym(dlopen("/usr/lib/libobjc.A.dylib", RTLD_LAZY), "objc_getMetaClass");
	Camo_class_getInstanceMethod = dlsym(dlopen("/usr/lib/libobjc.A.dylib", RTLD_LAZY), "class_getInstanceMethod");
	Camo_sel_registerName = dlsym(dlopen("/usr/lib/libobjc.A.dylib", RTLD_LAZY), "sel_registerName");
	Camo_method_setImplementation = dlsym(dlopen("/usr/lib/libobjc.A.dylib", RTLD_LAZY), "method_setImplementation");
}
