
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


@implementation RepeatoCapture(AutoConnect)
+ (void)load {
    NSArray *arguments = [[NSProcessInfo processInfo] arguments];
    os_log(OS_LOG_DEFAULT, "%@: Launch arguments: %{public}@", self, arguments);
    NSString *hostAddress = [[NSUserDefaults standardUserDefaults] stringForKey:@"host-address"];
    float scaleUpFactor = [[NSUserDefaults standardUserDefaults] floatForKey:@"scale-up-factor"];

#if TARGET_IPHONE_SIMULATOR
    // on simulators we fall back to localhost, since the DEVELOPER_HOST (Host.current().name) turned out to be slightly unreliable
    if (hostAddress == NULL) {
        hostAddress = @"localhost";
    }
#endif
    if (hostAddress == NULL) {
        os_log(OS_LOG_DEFAULT, "%@: Host-address launch argument not found. Launch arguments: %{public}@", self, arguments);
#ifdef DEVELOPER_HOST
        os_log(OS_LOG_DEFAULT, "%@: Host-address launch argument not found -> using fallback %{public}s! ", self, DEVELOPER_HOST);
        [self startCapture:@DEVELOPER_HOST scaleUpFactor:scaleUpFactor];
#endif
    } else {
        os_log(OS_LOG_DEFAULT, "%@: Host-address: %{public}@!", self, hostAddress);
        [self startCapture:hostAddress scaleUpFactor:scaleUpFactor];
    }
    
}
@end
