
#define REPEATO_HYBRID
//#define REMOTE_MINICAP
#define REPEATO_PORT 1313
//#define REMOTE_PNGFORMAT
//#define REPEATO_APPNAME GenericCapture
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
    DebugLog(self, @"Launch arguments", [arguments componentsJoinedByString:@","]);
    //NSString *hostAddress = [[NSUserDefaults standardUserDefaults] stringForKey:@"host-address"];
    NSString *cid = [[NSUserDefaults standardUserDefaults] stringForKey:@"cid"];
    float scaleUpFactor = [[NSUserDefaults standardUserDefaults] floatForKey:@"scale-up-factor"];
    
    //#if TARGET_IPHONE_SIMULATOR
        // on simulators we fall back to localhost, since the DEVELOPER_HOST (Host.current().name) turned out to be slightly unreliable
    //#endif
    [self startCaptureWithScaleUpFactor:scaleUpFactor cid:cid];
  
}
@end
