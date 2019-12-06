//
//  ViewController.m
//  YoungHookDemo
//
//  Created by 郭春杨 on 2019/12/2.
//  Copyright © 2019 Yonsm. All rights reserved.
//

#import "ViewController.h"
#import <WebKit/WebKit.h>
#import "YoungHook.h"

@interface ViewController ()

@end

@interface WKWebView ()
- (id)browsingContextController;
@end

@interface NSObject (WKBrowsingContextControllerMethods)
+ (void)registerSchemeForCustomProtocol:(NSString *)scheme;
@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.

	CamoCallInit();

	WKProcessPool *pool = [[WKProcessPool alloc]  init];
	WKWebViewConfiguration *config = [[WKWebViewConfiguration alloc] init];
	config.processPool = pool;
	WKWebView *webView = [[WKWebView alloc] initWithFrame:self.view.bounds configuration:config];
	webView.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;
	[webView loadHTMLString:@"<html><body>Loading...</body></html>" baseURL:[NSURL URLWithString:@"https://www.apple.com"]];
	//[webView loadRequest:[NSURLRequest requestWithURL:[NSURL URLWithString:@"https://www.apple.com"]]];
	[self.view addSubview:webView];
	
	//id browsingContextController = [webView browsingContextController];
	//[NSClassFromString(@"WKBrowsingContextController") registerSchemeForCustomProtocol:@"https"];
	NSString *a = [objc_getClass("NSString") stringWithCString:"XXX" encoding:NSUTF8StringEncoding];
	
	NSString *b = CFBridgingRelease(
	CamoCallClassMessage("NSString", "stringWithCString:encoding:", "XXXX", NSUTF8StringEncoding)
									);
	
	NSLog(@"%@-%@", a, b);
	//[WKWebView handlesURLScheme:[NSString stringWithCString:CamoDecrypt((__bridge CamoData *) webView) encoding:NSUTF8StringEncoding]];
	//YHMsgSend(WKWebView, handlesURLScheme_, 
	//objc_msgSend(WKWebView);
}

@end

// https://github.com/WebKit/webkit/blob/39a299616172a4d4fe1f7aaf573b41020a1d7358/Source/WebKit/UIProcess/API/Cocoa/WKWebView.mm#L1869
HOOK_MESSAGE(void, WKWebView, __layerTreeCommitComplete)
{
	NSLog(@"%s, %@", __FUNCTION__, self);
	_WKWebView___layerTreeCommitComplete(self, sel);
}

// https://github.com/WebKit/webkit/blob/39a299616172a4d4fe1f7aaf573b41020a1d7358/Source/WebKit/UIProcess/API/Cocoa/WKWebView.mm#L4172
_HOOK_CLASS(BOOL, WKWebView, handlesURLScheme_, NSString *urlScheme)
{
	return _WKWebView_handlesURLScheme_(self, sel, urlScheme);
}


