
#define REMOTE_HYBRID
//#define REMOTE_MINICAP
#define REMOTE_PORT 1313
//#define REMOTE_PNGFORMAT
//#define REMOTE_APPNAME GenericCapture
#define REMOTE_OVERSAMPLE 2
#define REMOTE_BENCHMARK
#define REMOTE_DEFER 0.2
#define REMOTE_MAXDEFER 0.05

#ifndef DEVELOPER_HOST
#define DEVELOPER_HOST @"localhost"
#endif

#define REMOTE_IMPL
#import "RemoteCapture.h"


@implementation RemoteCapture(AutoConnect)
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
        os_log(OS_LOG_DEFAULT, "%@: Host-address launch argument not found -> using fallback %{public}@! Launch arguments: %{public}@", self, DEVELOPER_HOST, arguments);
        [self startCapture:DEVELOPER_HOST scaleUpFactor:scaleUpFactor];
    } else {
        //NSString *hostAddress = arguments[index + 1];
        os_log(OS_LOG_DEFAULT, "%@: Host-address: %{public}@!", self, hostAddress);
        [self startCapture:hostAddress scaleUpFactor:scaleUpFactor];
    }
    //NSLog(@"launch arguments: %@",yourArray);
    
}
@end
