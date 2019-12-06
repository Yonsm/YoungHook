
#import "CamoCall.h"
#import <dlfcn.h>

// /usr/lib/libobjc.A.dylib
#define CAMO_usr_lib_libobjc_A_dylib char camo_usr_lib_libobjc_A_dylib[25]; CamoDecryptCString(camo_usr_lib_libobjc_A_dylib, "\x37\x62\x65\x67\x3b\x7f\x7b\x73\x3f\x63\x67\x6f\x63\x69\x60\x6a\x26\x46\x28\x61\x7d\x6f\x6b\x63", 24)
// objc_msgSend
#define CAMO_objc_msgSend char camo_objc_msgSend[13]; CamoDecryptCString(camo_objc_msgSend, "\x63\x69\x60\x6a\x57\x6a\x75\x62\x57\x66\x6c\x65", 12)
// objc_getClass
#define CAMO_objc_getClass char camo_objc_getClass[14]; CamoDecryptCString(camo_objc_getClass, "\x62\x6e\x61\x69\x56\x6f\x62\x72\x46\x68\x62\x71\x72", 13)
// objc_getMetaClass
#define CAMO_objc_getMetaClass char camo_objc_getMetaClass[18]; CamoDecryptCString(camo_objc_getMetaClass, "\x7e\x72\x65\x6d\x52\x6b\x6e\x7e\x44\x6d\x73\x67\x46\x68\x62\x71\x72", 17)
// class_getInstanceMethod
#define CAMO_class_getInstanceMethod char camo_class_getInstanceMethod[24]; CamoDecryptCString(camo_class_getInstanceMethod, "\x74\x7a\x74\x67\x60\x4d\x76\x75\x7b\x47\x63\x7f\x7f\x6b\x67\x6b\x62\x4b\x60\x70\x6b\x6d\x65", 23)
// sel_registerName
#define CAMO_sel_registerName char camo_sel_registerName[17]; CamoDecryptCString(camo_sel_registerName, "\x63\x6a\x62\x52\x7e\x6e\x6d\x60\x7b\x73\x63\x77\x4a\x62\x6f\x64", 16)
// method_setImplementation
#define CAMO_method_setImplementation char camo_method_setImplementation[25]; CamoDecryptCString(camo_method_setImplementation, "\x75\x72\x62\x7d\x7b\x77\x4d\x62\x75\x7b\x47\x60\x7c\x67\x6f\x64\x6d\x69\x72\x64\x70\x6a\x6d\x6f", 24)

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

	CAMO_usr_lib_libobjc_A_dylib;
	CAMO_objc_msgSend;
	CAMO_objc_getClass;
	CAMO_objc_getMetaClass;
	CAMO_class_getInstanceMethod;
	CAMO_sel_registerName;
	CAMO_method_setImplementation;

	void *lib = dlopen(camo_usr_lib_libobjc_A_dylib, RTLD_LAZY);
	Camo_objc_msgSend = dlsym(lib, camo_objc_msgSend);
	Camo_objc_getClass = dlsym(lib, camo_objc_getClass);
	Camo_objc_getMetaClass = dlsym(lib, camo_objc_getMetaClass);
	Camo_class_getInstanceMethod = dlsym(lib, camo_class_getInstanceMethod);
	Camo_sel_registerName = dlsym(lib, camo_sel_registerName);
	Camo_method_setImplementation = dlsym(lib, camo_method_setImplementation);
	
}
