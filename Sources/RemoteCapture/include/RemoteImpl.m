
#define REMOTE_HYBRID
//#define REMOTE_MINICAP
#define REMOTE_PORT 1313
//#define REMOTE_PNGFORMAT
//#define REMOTE_APPNAME GenericCapture
#define REMOTE_OVERSAMPLE 2
#define REMOTE_BENCHMARK
#define REMOTE_DEFER 0.2
#define REMOTE_MAXDEFER 0.05

#define REMOTE_IMPL
#import "RemoteCapture.h"


@implementation RemoteCapture(AutoConnect)
+ (void)load {
    NSArray *arguments = [[NSProcessInfo processInfo] arguments];
    //os_log(OS_LOG_DEFAULT, "%@: Launch arguments: %{public}@", self, arguments);
    NSUInteger index = [arguments indexOfObject:@"host-address"];
    //NSString *hostAddress = [[NSUserDefaults standardUserDefaults] stringForKey:@"host-address"];
    if (index == NSNotFound) {
        os_log(OS_LOG_DEFAULT, "%@: Host-address launch argument not found, using localhost! Launch arguments: %{public}@", self, arguments);
        [self startCapture:@"localhost"];
    } else {
        NSString *hostAddress = arguments[index + 1];
        os_log(OS_LOG_DEFAULT, "%@: Host-address: %{public}@!", self, hostAddress);
        [self startCapture:hostAddress];
    }
    //NSLog(@"launch arguments: %@",yourArray);
    
}
@end
