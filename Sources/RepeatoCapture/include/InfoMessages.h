//
//  InfoMessages.m
//  Repeato
//
//  Created by Muhammad Ahad on 18/04/2023.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "Logger.h"

#define REPEATO_INFO_ABORT_DELAY 3
#define REPEATO_INFO_VIEW_TAG 1022
#define REPEATO_INFO_ALERT_PADDING 20
#define REPEATO_INFO_ALERT_INTERNAL_PADDING 8
#define REPEATO_INFO_ALERT_TITLE_FONT 18
#define REPEATO_INFO_ALERT_FONT 16

static BOOL hasCancelledTestOperation = FALSE;

NS_ASSUME_NONNULL_BEGIN
@interface InfoMessages : NSObject <LogAlertDelegate>

@property (nonatomic, strong) UITextView *tv;
@property (nonatomic, strong) UILabel *lblCancel;
@property (nonatomic, strong) UIButton *btnCancel;
@property (nonatomic, strong)NSTimer * timer;

+(instancetype)shared;
-(void) showAlert;
-(void) noLaunchArgumentsPassed;
@end

@implementation InfoMessages
UIView *alertContainer;
int seconds = 0;
NSString *logsHistory = @"";

+ (instancetype)shared {
    static id instance;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        instance = [[self alloc] init];
      
    });
    return instance;
}

-(void) initAlert {
    Logger.shared.delegate = self;
    seconds = 0;
    logsHistory = @"";
    dispatch_async(dispatch_get_main_queue(), ^{
        [self setupAlertUI];
//        [self setupTimer];
    });
}

-(void) showAlert {
    if(alertContainer == nil) {
        [self initAlert];
    }
}

-(void) setupTimer {
    [self stopTimer];
    self.timer = [NSTimer scheduledTimerWithTimeInterval:1
        target:self
        selector:@selector(trackRemainingTime:)
        userInfo:nil
        repeats:YES];
}
-(void) stopTimer {
    if(self.timer != nil) {
        [self.timer invalidate];
    }
}
-(void)trackRemainingTime:(NSTimer *)timer {
    int timeleft = REPEATO_INFO_ABORT_DELAY - seconds;
    DebugLog(self, @"Closing the app in %i...", timeleft);
    [self.lblCancel setText:[NSString stringWithFormat:@"Closing the app in %i...", timeleft]];
    seconds += 1;
    if(seconds > REPEATO_INFO_ABORT_DELAY &&
        hasCancelledTestOperation == FALSE){
        Log(self, @"Closing app on testing operation timeout");
        exit(0);
    }
}

-(void) dismiss {
    [self stopTimer];
    dispatch_async(dispatch_get_main_queue(), ^{
        [alertContainer removeFromSuperview];
        UIViewController *topVC = [[[UIApplication sharedApplication] keyWindow] rootViewController];
        [[topVC.view viewWithTag:REPEATO_INFO_VIEW_TAG] removeFromSuperview];
        alertContainer = Nil;
    });
}

-(void) launchArgumentPassed:(NSString *) hostIP {
//    Log(self,@"Launch arguments given %@", h);
}

-(void) noLaunchArgumentsPassed {
    Log(self,@"No launch arguments given");
    
    dispatch_async(dispatch_get_main_queue(), ^{
        [self.lblCancel setHidden:FALSE];
        [self.btnCancel setHidden:FALSE];
        [self setupTimer];
    });
}


#pragma mark Setup Custom Alert View
-(void) setupAlertUI  {
    CGFloat screenHeight = 300;//([UIScreen.mainScreen bounds].size.width / 2); //- 120;
    
    UIViewController *topVC = [[[UIApplication sharedApplication] keyWindow] rootViewController];
    if(topVC == nil) {
        return;
    }
//    [self dismiss];
    float padding = REPEATO_INFO_ALERT_PADDING;
    //container view
    alertContainer = [[UIView alloc] init];
    [alertContainer setTag:REPEATO_INFO_VIEW_TAG];
    [self setupBorder:alertContainer borderWidth:2];
    
    alertContainer.backgroundColor = [UIColor whiteColor];
    [alertContainer.heightAnchor constraintLessThanOrEqualToConstant:screenHeight].active = true;
   
    //Stack View
    UIStackView *stackView = [[UIStackView alloc] init];

    stackView.axis = UILayoutConstraintAxisVertical;
    stackView.distribution = UIStackViewDistributionFillProportionally;
    stackView.alignment = UIStackViewAlignmentFill;
    stackView.spacing = REPEATO_INFO_ALERT_INTERNAL_PADDING;

    // create intermediate components
    [stackView addArrangedSubview:[self setupAlertTitleBar]];
    [stackView addArrangedSubview:[self setupTextView:screenHeight - 120]];
    [stackView addArrangedSubview:[self setupCancelLabel]];
    [stackView addArrangedSubview:[self createCancelOperationButton]];

    [alertContainer addSubview:stackView];
    stackView.translatesAutoresizingMaskIntoConstraints = false;

    //Layout for Stack View
    [stackView.leadingAnchor constraintEqualToAnchor:alertContainer.leadingAnchor
                                            constant:padding].active = true;
    [stackView.trailingAnchor constraintEqualToAnchor:alertContainer.trailingAnchor
                                             constant:-padding].active = true;
    [stackView.topAnchor constraintEqualToAnchor:alertContainer.topAnchor
                                        constant:padding].active = true;
    [stackView.centerYAnchor constraintEqualToAnchor:alertContainer.centerYAnchor].active = true;
    
    //Layout for Container View
    [topVC.view addSubview:alertContainer];
    alertContainer.translatesAutoresizingMaskIntoConstraints = false;
    [alertContainer.leadingAnchor constraintEqualToAnchor:topVC.view.leadingAnchor constant:padding].active = true;
    [alertContainer.trailingAnchor constraintEqualToAnchor:topVC.view.trailingAnchor constant:-padding].active = true;
    [alertContainer.bottomAnchor constraintEqualToAnchor:topVC.view.bottomAnchor constant:-REPEATO_INFO_ALERT_PADDING ].active = true;
    [alertContainer setNeedsUpdateConstraints];
    [self roundView:alertContainer radius:15];
    
    [self.lblCancel setHidden:TRUE];
    [self.btnCancel setHidden:TRUE];
}

#pragma mark Setup UI components
-(UIStackView *) setupAlertTitleBar {
    UILabel *title = [[UILabel alloc] init];
    [title setFont:[UIFont boldSystemFontOfSize:REPEATO_INFO_ALERT_TITLE_FONT]];
    title.translatesAutoresizingMaskIntoConstraints = false;
    [title setText:@"Repeato Connector"];
    [title setTextColor: [UIColor blackColor]];
    
    UIButton *minimizedButton = [[UIButton alloc] init];
    [minimizedButton setFont:[UIFont boldSystemFontOfSize:REPEATO_INFO_ALERT_TITLE_FONT]];
    [minimizedButton setTitleColor:[UIColor blackColor] forState:UIControlStateNormal];
    [minimizedButton setTitle:@"-" forState:UIControlStateNormal];
    [minimizedButton.heightAnchor constraintEqualToConstant:40].active = true;
    [minimizedButton.widthAnchor constraintEqualToConstant:40].active = true;
    minimizedButton.translatesAutoresizingMaskIntoConstraints = false;
    
    UIButton *closeButton = [[UIButton alloc] init];
    [closeButton setTitleColor:[UIColor blackColor] forState:UIControlStateNormal];
    [closeButton setFont:[UIFont boldSystemFontOfSize:REPEATO_INFO_ALERT_TITLE_FONT]];
    [closeButton setTitle:@"X" forState:UIControlStateNormal];
    [closeButton.heightAnchor constraintEqualToConstant:40].active = true;
    [closeButton.widthAnchor constraintEqualToConstant:40].active = true;
    closeButton.translatesAutoresizingMaskIntoConstraints = false;
    [closeButton addTarget:self
                    action:@selector(dismiss)
          forControlEvents:UIControlEventTouchUpInside];
    
    //Stack View
    UIStackView *stackView = [[UIStackView alloc] init];
    stackView.axis = UILayoutConstraintAxisHorizontal;
    stackView.distribution = UIStackViewDistributionFillProportionally;
    stackView.alignment = UIStackViewAlignmentCenter;
    stackView.spacing = 5;
    
    
    [stackView addArrangedSubview:title];
//    [stackView addArrangedSubview:minimizedButton];
    [stackView addArrangedSubview:closeButton];
    stackView.translatesAutoresizingMaskIntoConstraints = false;
    return stackView;
}

-(UITextView *) setupTextView:(CGFloat) height {
    self.tv = [[UITextView alloc] init];
    [self.tv setFont:[UIFont systemFontOfSize:17]];
    [self.tv setTextColor: [UIColor darkGrayColor]];
    [self.tv setBackgroundColor:[UIColor clearColor]];
    [self.tv setEditable:FALSE];
    [self setupBorder:self.tv borderWidth:2];
    [self roundView:self.tv radius:5];
    [self.tv setText:@"Starting Repeato..."];
//    [self.tv.heightAnchor constraintLessThanOrEqualToConstant:height].active = true;
    [self.tv.heightAnchor constraintEqualToConstant:height].active = true;
    self.tv.translatesAutoresizingMaskIntoConstraints = false;
    
    return self.tv;
}

-(UILabel *) setupCancelLabel {
    self.lblCancel = [[UILabel alloc] init];
    [self.lblCancel setFont:[UIFont boldSystemFontOfSize:REPEATO_INFO_ALERT_FONT]];
    [self.lblCancel.heightAnchor constraintEqualToConstant:20].active = true;
    self.lblCancel.translatesAutoresizingMaskIntoConstraints = false;
    [self.lblCancel setText:@"..."];
    [self.lblCancel setTextColor: [UIColor blackColor]];
    return self.lblCancel;
}

-(UIButton *) createCancelOperationButton {
    self.btnCancel = [[UIButton alloc] init];
    [self.btnCancel setFont:[UIFont boldSystemFontOfSize:REPEATO_INFO_ALERT_TITLE_FONT]];
    [self.btnCancel setTitleColor:[UIColor blackColor] forState:UIControlStateNormal];
    [self setupBorder:self.btnCancel borderWidth:2];
    [self.btnCancel setTitle:@"Cancel" forState:UIControlStateNormal];
    [self.btnCancel.heightAnchor constraintEqualToConstant:45].active = true;
//    [self.btnCancel.widthAnchor constraintEqualToConstant:40].active = true;
    self.btnCancel.translatesAutoresizingMaskIntoConstraints = false;
    [self roundView:self.btnCancel radius:5];
    [self.btnCancel addTarget:self
                    action:@selector(cancelOperation)
          forControlEvents:UIControlEventTouchUpInside];
    return self.btnCancel;
}

#pragma mark Supporting functions
-(void) cancelOperation {
    hasCancelledTestOperation = TRUE;
    Log(self, @"Won't close app becuase user has cancelled testing with Repeato");
    [self dismiss];
}

#pragma mark Round view
-(void) roundView:(UIView *) view radius:(CGFloat) radius{
    view.layer.cornerRadius = radius;
    view.layer.masksToBounds = true;
}

-(void)setupBorder:(UIView *)view borderWidth:(CGFloat)borderWidth{
    [view.layer setBorderColor:[UIColor blackColor].CGColor];
    [view.layer setBorderWidth:1];
}

#pragma mark Logger delegate
- (void)logEvent:(NSString *)message {
    if(self.tv == nil) {
//        NSLog(@"TV nil");
        /// fall back to track logs before tv initializaiton
        logsHistory = [logsHistory stringByAppendingString:[NSString stringWithFormat:@"%@\n", message]];
        return;
    }
    dispatch_async(dispatch_get_main_queue(), ^{
        if(![logsHistory isEqualToString:@""]) {
            self.tv.text = logsHistory; 
            logsHistory = @"";
        }
        [self.tv insertText:[NSString stringWithFormat:@"%@\n", message]];
    });
}

NS_ASSUME_NONNULL_END

@end
