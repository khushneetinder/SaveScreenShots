//
//  ViewController.h
//  MultithreadingApp
//
//  Created by Khushneet Inder on 4/4/2025.
//

#import <Cocoa/Cocoa.h>

@interface ViewController : NSViewController<NSWindowDelegate>

@property (weak) IBOutlet NSTextField *totalScreenshotCountLabel;
@property (weak) IBOutlet NSTextField *remainingCountLabel;
@property (weak) IBOutlet NSTextField *currentRequestLabel;

@property (atomic) int remainingCount;
@property (atomic) int currentRequestNumber;
- (IBAction)takeScreenshotAction:(id)sender;
@end

