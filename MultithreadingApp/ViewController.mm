//
//  ViewController.m
//  MultithreadingApp
//
//  Created by Khushneet Inder on 4/4/2025.
//

#import "ViewController.h"
#include "ScreenshotMgr.hpp"

@implementation ViewController {
	ScreenshotMgr *mMgr;
	int mRequestID;
}

static void notifyAll(int current, int remainingCount, void* objectToNotify) {
	dispatch_async(dispatch_get_main_queue(), ^{
		ViewController* vc = CFBridgingRelease(objectToNotify);
		vc.remainingCount = remainingCount;
		vc.currentRequestNumber = current;
		vc.remainingCountLabel.stringValue = [NSString stringWithFormat:@"%@", [NSNumber numberWithInt:remainingCount]];
		vc.currentRequestLabel.stringValue = [NSString stringWithFormat:@"%@", [NSNumber numberWithInt:current]];
	});
}

- (void)viewDidLoad {
	[super viewDidLoad];

	mMgr = new ScreenshotMgr();
	mRequestID = 0;
}

-(void)viewDidAppear {
	[super viewDidAppear];
	
	self.view.window.delegate = self;
}

- (IBAction)takeScreenshotAction:(id)sender {
	void* data = (void*)CFBridgingRetain(self);
	
	mMgr->Take(++mRequestID, notifyAll, data);
	self.totalScreenshotCountLabel.stringValue = [NSString stringWithFormat:@"%@", [NSNumber numberWithInt:mRequestID]];
	
	self.remainingCount = mRequestID - self.currentRequestNumber;
}

- (BOOL)windowShouldClose:(NSWindow *)sender {
	bool shouldClose = (self.remainingCount == 0);
	
	if (!shouldClose) {
		NSAlert* alert = [NSAlert alertWithMessageText:@"Processing..." defaultButton:@"OK" alternateButton:nil otherButton:nil informativeTextWithFormat:@"Screenshots are still saving on the disk"];
		[alert beginSheetModalForWindow:sender completionHandler:nil];
	}
	
	return shouldClose;
}

@end
