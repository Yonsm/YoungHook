
#import "CamoCall.h"
#import <dlfcn.h>

// /usr/lib/libobjc.A.dylib
#define CAMO_usr_lib_libobjc_A_dylib char camo_usr_lib_libobjc_A_dylib[25]; CamoDecryptCString(camo_usr_lib_libobjc_A_dylib, "\x37\x6c\x69\x67\x33\x6f\x6b\x73\x2f\x6b\x67\x6f\x6b\x6d\x64\x6c\x26\x48\x24\x69\x4d\x5f\x5b\x63", 24)
// objc_msgSend
#define CAMO_objc_msgSend char camo_objc_msgSend[13]; CamoDecryptCString(camo_objc_msgSend, "\x63\x6d\x64\x6c\x4f\x5c\x79\x64\x57\x60\x58\x5d", 12)
// objc_getClass
#define CAMO_objc_getClass char camo_objc_getClass[14]; CamoDecryptCString(camo_objc_getClass, "\x62\x6e\x65\x6b\x4e\x65\x62\x72\x46\x58\x62\x73\x72", 13)
// objc_getMetaClass
#define CAMO_objc_getMetaClass char camo_objc_getMetaClass[18]; CamoDecryptCString(camo_objc_getMetaClass, "\x7e\x72\x79\x6f\x4a\x71\x6e\x5e\x54\x6b\x5b\x65\x46\x70\x62\x53\x52", 17)
// class_getInstanceMethod
#define CAMO_class_getInstanceMethod char camo_class_getInstanceMethod[24]; CamoDecryptCString(camo_class_getInstanceMethod, "\x74\x7a\x74\x61\x60\x43\x6a\x6b\x5b\x55\x6f\x59\x57\x69\x6b\x65\x62\x49\x60\x50\x6b\x63\x5d", 23)
// sel_registerName
#define CAMO_sel_registerName char camo_sel_registerName[17]; CamoDecryptCString(camo_sel_registerName, "\x63\x74\x7a\x4c\x5e\x70\x71\x72\x5b\x5b\x6b\x57\x52\x64\x6f\x66", 16)
// method_setImplementation
#define CAMO_method_setImplementation char camo_method_setImplementation[25]; CamoDecryptCString(camo_method_setImplementation, "\x75\x7c\x6a\x6d\x73\x77\x41\x64\x75\x63\x47\x6a\x5c\x67\x6f\x66\x6d\x65\x5a\x66\x58\x5c\x61\x5f", 24)

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
