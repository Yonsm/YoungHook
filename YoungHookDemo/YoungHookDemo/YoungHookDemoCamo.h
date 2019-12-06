// NSString
#define CAMO_NSString char camo_NSString[9]; CamoStringDecode(camo_NSString, "\x46\x5a\x59\x79\x76\x5c\x60\x68", 8)
// stringWithCString:encoding:
#define CAMO_stringWithCString_encoding char camo_stringWithCString_encoding[28]; CamoStringDecode(camo_stringWithCString_encoding, "\x68\x6e\x67\x6f\x71\x77\x46\x6b\x67\x6a\x4e\x3d\x63\x5c\x64\x66\x6c\x10\x6c\x62\x64\x5f\x69\x5b\x5d\x63\x07", 27)
// XXXX
#define CAMO_XXXX char camo_XXXX[5]; CamoStringDecode(camo_XXXX, "\x5c\x5b\x5a\x59", 4)
//@https://www.apple.com
#define CAMO_https_www_apple_com  CFBridgingRelease(CamoStringDecode2("\x7d\x60\x5f\x62\x62\x2a\x34\x33\x5a\x59\x58\x30\x68\x58\x57\x6a\x60\x2a\x64\x67\x64", 21))
