#import <objc/runtime.h>

#define MSHookFunction					asdfafq343
#define YHHookMessage					pjmasd
#define YHHookFunctionForProcess		KJlkjsdd
#define YHHookMessageForProcess			oijv349m8f

#define Camo_objc_msgSend				ad5skjfq
#define Camo_objc_getClass				ksasdfkasdf
#define Camo_objc_getMetaClass			iafq348
#define Camo_class_getInstanceMethod	adlncuyJHs3d
#define Camo_sel_registerName			asdlnci2icjia
#define Camo_method_setImplementation	ki28hnd_sldas

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct
{
	unsigned short length:14;
	unsigned char encrypt:2;
	char payload[];
} CamoString;

const char *CamoStringDecrypt(CamoString *string);

extern void CamoCallInit(void);
extern void *(*Camo_objc_msgSend)(id, SEL, ...);
extern Class (*Camo_objc_getClass)(const char *);
extern Class (*Camo_objc_getMetaClass)(const char *);
extern Method (*Camo_class_getInstanceMethod)(Class, SEL);
extern SEL (*Camo_sel_registerName)(const char *);
extern IMP (*Camo_method_setImplementation)(Method, IMP);

#define CamoCallSelector(self, sel, ...)		Camo_objc_msgSend(self, sel, __VA_ARGS__)
#define CamoCallMessage(self, name, ...)		Camo_objc_msgSend(self, Camo_sel_registerName(name), __VA_ARGS__)
#define CamoCallClassSelector(cls, sel, ...)	Camo_objc_msgSend(Camo_objc_getClass(cls), sel, __VA_ARGS__)
#define CamoCallClassMessage(cls, name, ...)	Camo_objc_msgSend(Camo_objc_getClass(cls), Camo_sel_registerName(name), __VA_ARGS__)

#ifdef __cplusplus
}
#endif
