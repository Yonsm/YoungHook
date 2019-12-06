#import <objc/runtime.h>
#import <CoreFoundation/CoreFoundation.h>

#ifdef __cplusplus
extern "C"
{
#endif

static __inline__ __attribute__((always_inline))
void CamoStringDecode(char *dst, const char *src, unsigned length)
{
	for (unsigned i = 0; i < length; i++)
	{
		dst[i] = ((unsigned char)src[i] + i) ^ (unsigned char)length;
	}
	dst[length] = 0;
}

static __inline__ __attribute__((always_inline))
CFStringRef CamoStringDecode2(const char *src, unsigned length)
{
	char dst[2048];
	CamoStringDecode(dst, src, length);
	return CFStringCreateWithCString(NULL, dst, kCFStringEncodingUTF8);
}

#ifdef CamoCall_FunctionPointer
extern void CamoCallInit(void);
extern void *(*Camo_objc_msgSend)(id, SEL, ...);
extern Class (*Camo_objc_getClass)(const char *);
extern Class (*Camo_objc_getMetaClass)(const char *);
extern Method (*Camo_class_getInstanceMethod)(Class, SEL);
extern SEL (*Camo_sel_registerName)(const char *);
extern IMP (*Camo_method_setImplementation)(Method, IMP);
#else
#import <objc/message.h>
#define CamoCallInit()
#define Camo_objc_msgSend						((void * (*)(id, SEL, ...))objc_msgSend)
#define Camo_objc_getClass						objc_getClass
#define Camo_objc_getMetaClass					objc_getMetaClass
#define Camo_class_getInstanceMethod			class_getInstanceMethod
#define Camo_sel_registerName					sel_registerName
#define Camo_method_setImplementation			method_setImplementation
#endif

#define CamoCallSelector(self, sel, ...)		Camo_objc_msgSend(self, sel, __VA_ARGS__)
#define CamoCallMessage(self, name, ...)		Camo_objc_msgSend(self, Camo_sel_registerName(name), __VA_ARGS__)
#define CamoCallClassSelector(cls, sel, ...)	Camo_objc_msgSend(Camo_objc_getClass(cls), sel, __VA_ARGS__)
#define CamoCallClassMessage(cls, name, ...)	Camo_objc_msgSend(Camo_objc_getClass(cls), Camo_sel_registerName(name), __VA_ARGS__)

#ifdef __cplusplus
}
#endif
