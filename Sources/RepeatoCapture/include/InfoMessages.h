//
//  InfoMessages.m
//  Repeato
//
//  Created by Muhammad Ahad on 18/04/2023.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKey.h>
#import <OSLog/OSLog.h>

#define REPEATO_INFO_MSG_ABORT_DELAY 3
//#define REPEATO_INFO_MSG_VIEW_TAG 1022

static BOOL hasCancelledTestOperation = FALSE;

NS_ASSUME_NONNULL_BEGIN

@interface InfoMessages : NSObject
//+(void) showToast:(NSString *)title
//          message:(NSString *)message
//          actions:(NSArray *)actions ;
+(void) noLaunchArgumentsPassed;
+(void) launchArgumentPassed:(NSString *) hostIP;
+(void) showToast:(NSString *)title message:(NSString *)message hideAfterDelay:(double)delay;
+(void) dismiss;
@end

NS_ASSUME_NONNULL_END


@implementation InfoMessages

+(void) showToast:(NSString *)title message:(NSString *)message hideAfterDelay:(double)delay {
    [self showToast:title
            message:message
            actions: @[]];
    if(delay > 0) {
        // Delay execution of block for x seconds.
        dispatch_after(dispatch_time(DISPATCH_TIME_NOW,
                                     delay * NSEC_PER_SEC),
                       dispatch_get_main_queue(), ^{
            [self dismiss];
        });
    }
}


+(void) launchArgumentPassed:(NSString *) hostIP {
    NSString *message = [NSString stringWithFormat:@"Trying to connect to %@", hostIP];
    [self showToast:@"Launch arguments given"
            message:message
            hideAfterDelay:3];
}

+(void) noLaunchArgumentsPassed {
    UIAlertAction* cancelAction = [UIAlertAction actionWithTitle:@"Cancel"
                                                           style:UIAlertActionStyleDefault
                                                         handler:^(UIAlertAction * action) {
        hasCancelledTestOperation = TRUE;
    }];
    [self showToast:@"No launch arguments given"
            message:@"Please launch app via Repeato to establish connection.\n\nClosing app.."
            actions: @[cancelAction]];
    
    // Delay execution of block for x seconds
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW,
                                 REPEATO_INFO_MSG_ABORT_DELAY * NSEC_PER_SEC),
                   dispatch_get_main_queue(), ^{
        if(hasCancelledTestOperation == FALSE) {
            os_log(OS_LOG_DEFAULT, "%@: Closing app to relaunch with Repeato", self);
            // abort app
            exit(0);
        }else {
            // do nothing
            os_log(OS_LOG_DEFAULT, "%@: Won't close app becuase user has cancelled testing with Repeato", self);
        }
    });
}

+(void) showToast:(NSString *)title
          message:(NSString *)message
          actions:(NSArray *)actions {
    [self dismiss];
    dispatch_async(dispatch_get_main_queue(), ^{
        UIAlertController* alert = [UIAlertController alertControllerWithTitle:title
                                                                       message: message
                                                                preferredStyle:UIAlertControllerStyleAlert];
        
        for (UIAlertAction *action in actions) {
            [alert addAction: action];
        }
        if(actions.count == 0) {
            UIAlertAction* defaultAction = [UIAlertAction actionWithTitle:@"Ok"
                                                                    style:UIAlertActionStyleDefault
                                                                  handler:^(UIAlertAction * action) {}];
            [alert addAction: defaultAction];
        }
        
        [[[[UIApplication sharedApplication] keyWindow] rootViewController] presentViewController:alert animated:true completion:nil];
    });
}

+(void) dismiss {
    dispatch_async(dispatch_get_main_queue(), ^{
        UIViewController *topVC = [[[[UIApplication sharedApplication] keyWindow] rootViewController] presentedViewController];
        while ([topVC presentedViewController] != nil) {
            topVC = topVC.presentedViewController;
        }
        if([topVC  isKindOfClass:[UIAlertController class]]) {
            [topVC dismissViewControllerAnimated:YES completion:nil];
        }
    });
}
@end
