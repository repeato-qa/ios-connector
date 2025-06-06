#define REPEATO_HYBRID
#define REPEATO_DEFAULT_PORT 1313
#define REPEATO_OVERSAMPLE 2
#define REPEATO_BENCHMARK
#define REPEATO_DEFER 0.2
#define REPEATO_MAXDEFER 0.05

#ifndef DEVELOPER_HOST
#define DEVELOPER_HOST "localhost"
#endif

#define REPEATO_IMPL
#import "RepeatoCapture.h"
#import "InfoMessages.h"
#import "Logger.h"


@implementation RepeatoCapture(AutoConnect)
+ (void)load {
    [[InfoMessages shared] showAlert];
    NSArray *arguments = [[NSProcessInfo processInfo] arguments];
    Log(self, @"Launch arguments: ", [arguments componentsJoinedByString:@","]);
    float scaleUpFactor = [[NSUserDefaults standardUserDefaults] floatForKey:@"scale-up-factor"];
    // modified port parsing to include explicit cast
    int port = (int)[[NSUserDefaults standardUserDefaults] integerForKey:@"port"];
    NSString *hostAddress = [[NSUserDefaults standardUserDefaults] stringForKey:@"host-address"];
    
    if(port == 0){
        port = REPEATO_DEFAULT_PORT;
    }
    if([hostAddress length] != 0) {
        // if Repeato passes a host-address launch argument, it is a sign that it is an old Repeato version (<1.8.0)
        Log(self, @"Error: This iOS connector version is too new for your Repeato version. Please upgrade Repeato to 1.8.x or downgrade the iOS connector version to 1.2.x");
    } else {
        [self startCaptureWithScaleUpFactor:scaleUpFactor port:port];
    }
}
@end
