
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
#import "MdnsHandler.h"

@implementation RepeatoCapture(AutoConnect)
+ (void)load {
    NSArray *arguments = [[NSProcessInfo processInfo] arguments];
    os_log(OS_LOG_DEFAULT, "%@: Launch arguments: %{public}@", self, arguments);
    NSString *hostAddress = [[NSUserDefaults standardUserDefaults] stringForKey:@"host-address"];
    float scaleUpFactor = [[NSUserDefaults standardUserDefaults] floatForKey:@"scale-up-factor"];

#if TARGET_IPHONE_SIMULATOR
    // on simulators we fall back to localhost, since the DEVELOPER_HOST (Host.current().name) turned out to be slightly unreliable
    if (hostAddress == NULL) {
//        hostAddress = @"localhost";
    }
#endif
    if (hostAddress == NULL) {
        os_log(OS_LOG_DEFAULT, "%@: Host-address launch argument not found. Launch arguments: %{public}@", self, arguments);
        //MSDN discovery
        [[NSNotificationCenter defaultCenter] addObserver:self
                                                 selector:@selector(receiveMDNSNotification:)
                                                     name:@"hostInfo"
                                                   object:nil];
        [[MdnsHandler shared] startSearchingForServices];
//#ifdef DEVELOPER_HOST
//        os_log(OS_LOG_DEFAULT, "%@: Host-address launch argument not found -> using fallback %{public}s! ", self, DEVELOPER_HOST);
//        [self startCapture:@DEVELOPER_HOST scaleUpFactor:scaleUpFactor];
//#endif
    } else {
        os_log(OS_LOG_DEFAULT, "%@: Host-address: %{public}@!", self, hostAddress);
        [self startCapture:hostAddress scaleUpFactor:scaleUpFactor];
    }
}

+ (void) receiveMDNSNotification:(NSNotification *) notification {
    if ([[notification name] isEqualToString:@"hostInfo"]) {
        NSDictionary *userInfo = notification.userInfo;
        NSString *hostAddress = [userInfo objectForKey:@"ip"];
        os_log(OS_LOG_DEFAULT, "%@: Successfully received the host ip Address %@", self, hostAddress);
        os_log(OS_LOG_DEFAULT, "%@: Going to start capture", self);
        float scaleUpFactor = [[NSUserDefaults standardUserDefaults] floatForKey:@"scale-up-factor"];
        [self startCapture:hostAddress scaleUpFactor:scaleUpFactor];
    }
}
@end
