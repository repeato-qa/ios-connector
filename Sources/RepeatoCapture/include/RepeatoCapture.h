// RepeatoConnector.h
#import <sys/sysctl.h>
#import <netinet/tcp.h>
#import <sys/socket.h>
#import <arpa/inet.h>
#import <netdb.h>
#import <zlib.h>
#import <WebKit/WebKit.h>
#import <ifaddrs.h>

#import "RepeatoHeaders.h"
#import "Logger.h"
#import "InfoMessages.h"

#ifndef REPEATO_PORT
#define REPEATO_PORT 1313
#endif

#ifndef REPEATO_APPNAME
#define REPEATO_APPNAME RepeatoCapture
#endif

#define REPEATO_MAGIC -141414141

#ifndef REPEATO_OVERSAMPLE
#ifndef REPEATO_HYBRID
#define REPEATO_OVERSAMPLE 1.0
#else
#define REPEATO_OVERSAMPLE *(float *)device.scale
#endif
#endif

#ifndef REPEATO_JPEGQUALITY
#define REPEATO_JPEGQUALITY 0.5
#endif

#ifndef REPEATO_RETRIES
#define REPEATO_RETRIES 3
#endif

#ifdef REPEATO_HYBRID
#ifndef REPEATO_DEFER
#define REPEATO_DEFER 0.5
#endif
#ifndef REPEATO_MAXDEFER
#define REPEATO_MAXDEFER 0.1
#endif
#else
#ifndef REPEATO_DEFER
#define REPEATO_DEFER 0.5
#endif
#ifndef REPEATO_MAXDEFER
#define REPEATO_MAXDEFER 0.1
#endif
#endif

#ifdef DEBUG
#define RMLog NSLog
#else
#define RMLog while(0) NSLog
#endif

#ifdef REPEATO_DEBUG
#define RMDebug NSLog
#else
#define RMDebug while(0) NSLog
#endif

#define REPEATO_NOW [NSDate timeIntervalSinceReferenceDate]
#ifdef REPEATO_BENCHMARK
#define RMBench printf
#else
#define RMBench while(0) printf
#endif

#import <Foundation/Foundation.h>
#import <CoreGraphics/CoreGraphics.h>
#import <os/log.h>

#ifdef REPEATO_LEGACY
static BOOL repeatoLegacy = TRUE;
#else
static BOOL repeatoLegacy = FALSE;
#endif

typedef unsigned rmpixel_t;
typedef unsigned rmencoded_t;

typedef NS_ENUM(int, RMTouchPhase) {
    RMTouchBeganDouble = -1,
    RMTouchBegan = 0,
    RMTouchMoved,
    RMTouchStationary,
    RMTouchEnded,
    RMTouchCancelled,
    RMTouchUseScale,
    RMTouchRegionEntered,
    RMTouchRegionMoved,
    RMTouchRegionExited,
    RMTouchInsertText = 100
};

struct _rmdevice {
    char machine[24];
    char appname[64];
    char appvers[24];
    char hostname[63];
    char scale[4]; // float
    char isIPad[4]; // int
    char protocolVersion[4];// int
    char displaySize[12]; // 9999x9999
    char deviceName[24];
    char systemVersion[4]; // float
    char appFrameWorkType[4]; // int 0: default, 1: flutter, 2: compose
    char expansion[16];
    char magic[4]; // int
    char reserved[32];
};

struct _rmframe {
    NSTimeInterval timestamp;
    union {
        struct { float width, height, imageScale; };
        struct { float x, y; RMTouchPhase phase; };
    };
    int length;
};

#define RMMAX_TOUCHES 2

struct _rmevent {
    NSTimeInterval timestamp;
    RMTouchPhase phase;
    union {
        struct { float x, y; } touches[RMMAX_TOUCHES];
    };
};

@interface REPEATO_APPNAME : NSObject {
@package
    rmpixel_t *buffer, *buffend;
    CGContextRef cg;
}
- (instancetype)initFrame:(const struct _rmframe *)frame;
- (NSData *)subtractAndEncode:(REPEATO_APPNAME *)prevbuff;
- (CGImageRef)cgImage;
@end

@protocol RepeatoDelegate <NSObject>
@required
- (void)remoteConnected:(BOOL)status;
@end


#ifndef __IPHONE_OS_VERSION_MIN_REQUIRED
#import <Cocoa/Cocoa.h>
#else
#import <UIKit/UIKit.h>
#import <objc/runtime.h>

@interface REPEATO_APPNAME(Client)
// Changed startCapture method to no longer require an address list
+ (void)startCaptureWithScaleUpFactor:(float)s port:(long)port;
+ (BOOL)startListeningOnPort:(long)port;
+ (void)shutdown;
@end

static NSTimeInterval timestamp0;
static UITouch *currentTouch;
static NSSet *currentTouches;

@interface RCFakeEvent: UIEvent {
@public
    NSTimeInterval _timestamp;
}
@end

@implementation RCFakeEvent
- (instancetype)init {
    if ((self = [super init])) {
        _timestamp = REPEATO_NOW - timestamp0;
    }
    return self;
}
- (NSTimeInterval)timestamp {
    return _timestamp;
}
- (UITouch *)_firstTouchForView:(UIView *)view {
    RMDebug(@"_firstTouchForView: %@", view);
    return currentTouch;
}
- (NSSet *)touchesForView:(UIView *)view {
    RMDebug(@"touchesForWindow:%@", view);
    return currentTouches;
}
- (NSSet *)touchesForWindow:(UIWindow *)window {
    RMDebug(@"touchesForWindow:%@", window);
    return currentTouches;
}
- (NSSet *)touchesForGestureRecognizer:(UIGestureRecognizer *)rec {
    RMDebug(@"touchesForGestureRecognizer:%@", rec);
    return currentTouches;
}
- (void)_removeTouch:(UITouch *)touch fromGestureRecognizer:(UIGestureRecognizer *)rec {
    RMDebug(@"_removeTouch:%@ fromGestureRecognizer:%@", touch, rec);
}
- (NSSet *)allTouches {
    return currentTouches;
}
- (void)_addWindowAwaitingLatentSystemGestureNotification:(id)a0 deliveredToEventWindow:(id)a1 {
    RMDebug(@"_addWindowAwaitingLatentSystemGestureNotification:%@ deliveredToEventWindow:%@", a0, a1);
}
- (NSUInteger)_buttonMask {
    return 0;
}
- (UIEventType)type {
    return (UIEventType)0;
}
@end

@interface UIView(Description)
- (NSString *)recursiveDescription;
@end

@interface NSObject(ForwardReference)
- (void *)_copyRenderLayer:(void *)a0 layerFlags:(unsigned)a1 commitFlags:(unsigned *)a2;
- (void *)in_copyRenderLayer:(void *)a0 layerFlags:(unsigned)a1 commitFlags:(unsigned *)a2;
- (void)_didCommitLayer:(void *)a0;
- (void)in_didCommitLayer:(void *)a0;
@end

@interface UIApplication(ForwardReference)
- (void)in_sendEvent:(UIEvent *)event;
@end

@implementation UITouch(Identifier)
- (void)_setTouchIdentifier:(unsigned int)ident {
    Ivar ivar = class_getInstanceVariable([self class], "_touchIdentifier");
    ptrdiff_t offset = ivar_getOffset(ivar);
    unsigned *iptr = (unsigned *)((char *)(__bridge void *)self + offset);
    *iptr = ident;
}
@end
#endif

@interface REPEATO_APPNAME()
@property (strong, nonatomic) CADisplayLink *displayLink;
@end

@implementation REPEATO_APPNAME

- (instancetype)initFrame:(const struct _rmframe *)frame {
    if ((self = [super init])) {
        CGSize size = {frame->width * frame->imageScale, frame->height * frame->imageScale};
        int bitsPerComponent = 8;
        int bytesPerRow = (bitsPerComponent / 8) * 4 * (int)size.width;
        int bufferSize = bytesPerRow * (int)size.height;
        cg = CGBitmapContextCreate(NULL, size.width, size.height, bitsPerComponent,
                                   bytesPerRow, CGColorSpaceCreateDeviceRGB(),
                                   (CGBitmapInfo)kCGImageAlphaNoneSkipFirst);
        buffer = (rmpixel_t *)CGBitmapContextGetData(cg);
        buffend = (rmpixel_t *)((char *)buffer + bufferSize);
        if (repeatoLegacy) {
            CGContextTranslateCTM(cg, 0, size.height);
            CGContextScaleCTM(cg, frame->imageScale, -frame->imageScale);
        } else {
            CGContextScaleCTM(cg, frame->imageScale, frame->imageScale);
        }
    }
    return self;
}

- (NSData *)subtractAndEncode:(REPEATO_APPNAME *)prevbuff {
    unsigned tmpsize = 64 * 1024;
    rmencoded_t *tmp = (rmencoded_t *)malloc(tmpsize * sizeof *tmp), *end = tmp + tmpsize;
    rmencoded_t *out = tmp, count = 0, expectedDiff = 0, check = 0;
    *out++ = prevbuff == nil;
    for (const rmpixel_t *curr = buffer, *prev = prevbuff ? prevbuff->buffer : NULL;
         curr < buffend; check += *curr, curr++) {
        rmpixel_t ref = (prev ? *prev++ : 0), diff = *curr - ref - expectedDiff;
        if (!diff && curr != buffer)
            count++;
        else {
            if (count) {
                if (count < 0xff)
                    out[-1] |= count;
                else {
                    out[-1] |= 0xff;
                    *out++ = count;
                }
                count = 0;
            }
            *out++ = diff & 0xffffff00;
            if (out + 4 >= end) {
                size_t ptr = out - tmp;
                tmpsize *= 1.5;
                tmp = (rmencoded_t *)realloc(tmp, tmpsize * sizeof *tmp);
                out = tmp + ptr;
                end = tmp + tmpsize;
            }
        }
        expectedDiff = *curr - ref;
    }
    if (count) {
        if (count < 0xff)
            out[-1] |= count;
        else {
            out[-1] |= 0xff;
            *out++ = count;
        }
    }
    *out++ = check;
    return [NSData dataWithBytesNoCopy:tmp length:(char *)out - (char *)tmp freeWhenDone:YES];
}

- (CGImageRef)cgImage {
    return CGBitmapContextCreateImage(cg);
}

- (void)dealloc {
    CGContextRelease(cg);
}

// Static variables shared by the connector
static id<RepeatoDelegate> repeatoDelegate;
static dispatch_queue_t writeQueue;
static struct _rmdevice device;
static int skipEcho; // Was to filter out layer commits during capture
static BOOL capturing; // Am in the middle of capturing
static BOOL isStreamEnabled = true;
static float scaleUpFactor = 0.5; // can be remote controlled
static NSTimeInterval mostRecentScreenUpdate; // last window layer update
static NSTimeInterval lastCaptureTime; // last time capture was forced
static NSArray *buffers; // off-screen buffers use in encoding images
static int frameno; // count of frames captured and transmitted
static NSValue *inhibitEcho; // prevent events from server going back to server
static Class UIWindowLayer; // Use to filter for full window layer updates
static UITouch *realTouch; // An actual UITouch recycled for forging events
static CGSize bufferSize; // current size of off-screen image buffers
static int clientSocket;

// Initialize capture and device header; this is unchanged from previous behavior.
+ (void)initHeaderData {
    writeQueue = dispatch_queue_create("writeQueue", DISPATCH_QUEUE_SERIAL);
    // Initialize device header values:
    *(int *)device.magic = REPEATO_MAGIC;
    size_t size = sizeof device.machine - 1;
    sysctlbyname("hw.machine", device.machine, &size, NULL, 0);
    device.machine[size] = '\0';
    NSDictionary *infoDict = [NSBundle mainBundle].infoDictionary;
    strncpy(device.appname, ([infoDict[@"CFBundleIdentifier"] UTF8String] ?: ""), sizeof(device.appname) - 1);
    strncpy(device.appvers, ([infoDict[@"CFBundleShortVersionString"] UTF8String] ?: ""), sizeof(device.appvers) - 1);
    gethostname(device.hostname, sizeof(device.hostname) - 1);
    *(float *)device.scale = scaleUpFactor;
    *(int *)device.isIPad = ([UIDevice currentDevice].userInterfaceIdiom == UIUserInterfaceIdiomPad);
    *(int *)device.protocolVersion = 180;
    CGRect screenBounds = [self screenBounds];
    CGSize screenSize = screenBounds.size;
    NSString *displaySize = [NSString stringWithFormat:@"%ix%i", (int)screenSize.width, (int)screenSize.height];
    strncpy(device.displaySize, [displaySize UTF8String], sizeof(device.displaySize) - 1);
    
    __block NSString *deviceName;
    __block float systemVersion;
    dispatch_sync(dispatch_get_main_queue(), ^{
        deviceName = [[UIDevice currentDevice] name];
        systemVersion = [[[UIDevice currentDevice] systemVersion] floatValue];
    });
    strncpy(device.deviceName, [deviceName UTF8String], sizeof(device.deviceName) - 1);
    *(float *)device.systemVersion = systemVersion;
    if (NSClassFromString(@"FlutterViewController") != nil) {
        *(int *)device.appFrameWorkType = 1;
        Log(self, @"AppFrameWorkType: Flutter");
    } else if (NSClassFromString(@"ComposeAppBase") != nil || NSClassFromString(@"Applier") != nil) {
        *(int *)device.appFrameWorkType = 2;
        Log(self, @"AppFrameWorkType: Compose");
        dispatch_async(dispatch_get_main_queue(), ^{
            REPEATO_APPNAME *instance = [self sharedInstance];
            instance.displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(displayLinkFired:)];
            instance.displayLink.preferredFramesPerSecond = 20;
            [instance.displayLink addToRunLoop:[NSRunLoop mainRunLoop] forMode:NSRunLoopCommonModes];
        });
    } else {
        Log(self, @"AppFrameWorkType: iOS");
    }
}

+ (void) initFrameScheduler {
    timestamp0 = REPEATO_NOW;
    writeQueue = dispatch_queue_create("writeQueue", DISPATCH_QUEUE_SERIAL);
    UIWindowLayer = objc_getClass("UIWindowLayer");
    
    __block NSArray<UIScreen *> *screens;
    do {
        dispatch_sync(dispatch_get_main_queue(), ^{
            screens = [UIScreen screens];
        });
        if (!screens.count)
            [NSThread sleepForTimeInterval:.5];
    } while (!screens.count);


    method_exchangeImplementations(
        class_getInstanceMethod(CALayer.class, @selector(_copyRenderLayer:layerFlags:commitFlags:)),
        class_getInstanceMethod(CALayer.class, @selector(in_copyRenderLayer:layerFlags:commitFlags:)));

    method_exchangeImplementations(
        class_getInstanceMethod(UIApplication.class, @selector(sendEvent:)),
        class_getInstanceMethod(UIApplication.class, @selector(in_sendEvent:)));

}


+ (CGRect)screenBounds {
    CGRect bounds = CGRectZero;
    while (YES) {
        for (UIWindow *window in [UIApplication sharedApplication].windows) {
            if (window.bounds.size.height > bounds.size.height)
                bounds = window.bounds;
        }
        if (bounds.size.height)
            break;
        else
            [[NSRunLoop mainRunLoop] runUntilDate:[NSDate dateWithTimeIntervalSinceNow:0.2]];
    }
    return bounds;
}

#pragma mark - New Server (Listener) Implementation

// This method replaces the outbound connection logic.
// It creates a listening socket, prints its IP address and port, and then accepts incoming connections.

+ (BOOL)startListeningOnPort:(long)port {
    Log(self, @"Start listening...");
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        Log(self, @"Error creating socket: %s", strerror(errno));
        return NO;
    }
    int opt = 1;
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        Log(self, @"Error setting socket options: %s", strerror(errno));
        close(serverSocket);
        return NO;
    }
    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port > 0 ? port : REPEATO_PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        Log(self, @"Error binding socket: %s", strerror(errno));
        close(serverSocket);
        return NO;
    }
    if (listen(serverSocket, 5) < 0) {
        Log(self, @"Error listening on socket: %s", strerror(errno));
        close(serverSocket);
        return NO;
    }
    // Retrieve and print the local IP address and port.
    struct sockaddr_in localAddr;
    socklen_t addrLen = sizeof(localAddr);
    if (getsockname(serverSocket, (struct sockaddr *)&localAddr, &addrLen) == 0) {
        char ipStr[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &localAddr.sin_addr, ipStr, sizeof(ipStr));
        
        // Also print to standard output so Repeato can pick it up.
        // Retrieve the actual local IP address
        char actualIP[INET_ADDRSTRLEN] = "0.0.0.0";
        struct ifaddrs *ifaddr, *ifa;
        if (getifaddrs(&ifaddr) == 0) {
            for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
                if (ifa->ifa_addr && ifa->ifa_addr->sa_family == AF_INET) {
                    struct sockaddr_in *addr = (struct sockaddr_in *)ifa->ifa_addr;
                    if (strcmp(ifa->ifa_name, "en0") == 0) { // en0 is typically the Wi-Fi interface
                        inet_ntop(AF_INET, &addr->sin_addr, actualIP, INET_ADDRSTRLEN);
                        break;
                    }
                }
            }
            freeifaddrs(ifaddr);
        }
        Log(self, @"Server listening on %s:%d", actualIP, ntohs(localAddr.sin_port));
        printf("Server listening on %s:%d\n", actualIP, ntohs(localAddr.sin_port));
    } else {
        Log(self, @"Error getting socket name: %s", strerror(errno));
    }
    // Accept incoming connections asynchronously.
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
        while (YES) {
            struct sockaddr_in clientAddr;
            socklen_t clientLen = sizeof(clientAddr);
            
            int newSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &clientLen);
            if (newSocket < 0) {
                Log(self, @"Error accepting connection: %s", strerror(errno));
                continue;
            }
            if (clientSocket != -1) {
                Log(self, @"Closing existing client socket before accepting a new one");
                close(clientSocket);
                clientSocket = -1;
            }
            clientSocket = newSocket;
            
            char clientIP[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, sizeof(clientIP));
            Log(self, @"Accepted connection from %s:%d", clientIP, ntohs(clientAddr.sin_port));
            // Immediately send the device header to the connected client.
            [self initHeaderData];
            
            long headerSize = sizeof(device);
            long wroteBytes = write(clientSocket, &device, headerSize);
            if (wroteBytes != headerSize) {
                Log(self, @"Could not write device info: %s", strerror(errno));
            }
            // Convert the socket to a FILE* and process events as before.
            FILE *writeFp = fdopen(clientSocket, "w");
            if (writeFp) {
                [self performSelectorInBackground:@selector(processEvents:) withObject:[NSValue valueWithPointer:writeFp]];
            } else {
                Log(self, @"Error converting client socket to FILE*");
                close(clientSocket);
            }
            dispatch_async(writeQueue, ^{
                [self queueCapture];
            });
            [repeatoDelegate remoteConnected:TRUE];

            [self initFrameScheduler];
        }
        close(serverSocket);
    });
    return YES;
}

#pragma mark - Changed Start Capture Method

// Changed startCapture to no longer require a host address.
// It now only sets parameters and calls initHeaderData followed by startListening.
+ (void)startCaptureWithScaleUpFactor:(float)s port:(long)port {
    scaleUpFactor = (s == 0 ? 1 : s);
    [UIApplication.sharedApplication setIdleTimerDisabled:YES];
    Log(self, @"Starting server with scaleUpFactor %.2f", scaleUpFactor);
    if (![self startListeningOnPort:port]) {
        Log(self, @"Failed to start listening");
    } else {
        // Notify delegate that the connector is ready for incoming connections.
        [repeatoDelegate remoteConnected:YES];
    }
}



/// Capture device's screen (and discard it if there is a more recent update to the screen on the way)
/// @param timestamp Time update to the screen was notified
/// @param flush force transmission of screen update reguardless of timestamp
+ (void)capture:(NSTimeInterval)timestamp flush:(BOOL)flush {
    if(!isStreamEnabled){
        Log(self, @"Capture: disabled");
        return;
    }
    Log(self, @"Capture now! %f", scaleUpFactor);
    
    NSTimeInterval start = REPEATO_NOW;
    
    //UIScreen *screen = [UIScreen mainScreen];
    CGRect screenBounds = [self screenBounds];
    CGSize screenSize = screenBounds.size;
    CGFloat imageScale = *(int *)device.isIPad ? 1. : scaleUpFactor;
    __block struct _rmframe frame = {REPEATO_NOW,
        {{(float)screenSize.width, (float)screenSize.height, (float)imageScale}}, 0};

    if (bufferSize.width != frame.width || bufferSize.height != frame.height) {
        buffers = nil;
        buffers = @[[[self alloc] initFrame:&frame],
                    [[self alloc] initFrame:&frame]];
        bufferSize = screenSize;
        frameno = 0;
    }

    REPEATO_APPNAME *buffer = buffers[frameno++&1];
    REPEATO_APPNAME *prevbuff = buffers[frameno&1];
    UIImage *screenshot;
    RMDebug(@"%@, %@ -- %@", buffers, buffer, prevbuff);
    RMBench(" pre Captured #%d(%d), %.1fms %f\n", frameno, flush, (REPEATO_NOW-start)*1000., timestamp);
  
    capturing = TRUE;
    RMDebug(@"CAPTURE0");
    

    CGRect fullBounds = CGRectMake(0, 0,
                                   screenSize.width * scaleUpFactor,
                                   screenSize.height * scaleUpFactor);
    UIGraphicsBeginImageContext(fullBounds.size);
    for (UIWindow *window in [UIApplication sharedApplication].windows)
        if (!window.isHidden){
            [window drawViewHierarchyInRect:fullBounds afterScreenUpdates:NO];
        }

    RMBench(" pre Captured #%d(%d), %.1fms %f\n", frameno, flush, (REPEATO_NOW-start)*1000., timestamp);

    screenshot = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();

    

    skipEcho = 0;

    RMDebug(@"CAPTURE2 %@", [UIApplication sharedApplication].windows.lastObject);
    capturing = FALSE;
    RMBench("Captured #%d(%d), %.1fms %f\n", frameno, flush,
            (REPEATO_NOW-start)*1000., timestamp);
    

    dispatch_async(writeQueue, ^{
        if (timestamp < mostRecentScreenUpdate && !flush) {
            RMBench("Discard 3 %d\n", flush);
            frameno--;
            return;
        }

        NSTimeInterval start = REPEATO_NOW;
        [self encodeAndTransmit:screenshot screenSize:screenSize
                          frame:frame buffer:buffer prevbuff:prevbuff];
        RMBench("Sent #%d(%d), %.1fms %f\n", frameno, flush, ([NSDate
                  timeIntervalSinceReferenceDate]-start)*1000., timestamp);
    });
}

/// Encode image either using Remote's run-length encoded format relaive to previous capture
/// or in minicap format (int32_t length + jpeg image format)
/// @param screenshot UIImage containing screen contents
/// @param screenSize Best extimate of screen size
/// @param frame struct  to be transmitted to server when using Remote's native format
/// @param buffer Buffer to contain most recent screenshot
/// @param prevbuff Buffer containing previous screenshot to relative encode
+ (void)encodeAndTransmit:(UIImage *)screenshot
               screenSize:(CGSize)screenSize frame:(struct _rmframe)frame
       buffer:(REPEATO_APPNAME *)buffer prevbuff:(REPEATO_APPNAME *)prevbuff
{
        NSData *encoded = UIImageJPEGRepresentation(screenshot, REPEATO_JPEGQUALITY);
      
        //Log(self, @" frame size: %lu", encoded.length);

        uint32_t frameSize = (uint32_t)encoded.length;
        int frameHeaderSize = true ? sizeof frameSize : sizeof frame;
        if (write(clientSocket, true ? (void *)&frameSize : (void *)&frame, frameHeaderSize) != frameHeaderSize) {
            Log(self, @"Could not write frame: %s", strerror(errno));
        } else if (write(clientSocket, encoded.bytes, encoded.length) != encoded.length) {
            Log(self, @"Could not write encoded: %s", strerror(errno));
        } else {
            fsync(clientSocket);
        }
}

/// Finds the first descendant view of a given class name in a view hierarchy.
/// @param view View to search through
/// @param className Name of the class to search for
+ (UIView *)findDescendantViewInView:(UIView *)view withClassName:(NSString *)className {
    if ([NSStringFromClass([view class]) isEqualToString:className]) {
        return view;
    }
    for (UIView *subview in view.subviews) {
        UIView *result = [self findDescendantViewInView:subview withClassName:className];
        if (result) {
            return result;
        }
    }
    return nil;
}

/// Injects text into webview's HTML document. Must 'click'/focus on input before executing this function.
/// @param text Text to be injected
/// @param webView Web view to inject text into
+ (void)injectText:(NSString *)text intoWebView:(WKWebView *)webView {
    NSString *js = [NSString stringWithFormat:
            @"document.execCommand('insertText', false, '%@')", text];

    [webView evaluateJavaScript:js completionHandler:^(id result, NSError *error) {
        if (error) {
            NSLog(@"Error injecting text: %@", error.localizedDescription);
        } else {
            NSLog(@"Text injected successfully");
        }
    }];
}

/// Run in backgrount to process event structs coming from user interface in order to forge them
/// @param writeFp Connection to RemoteUI server
+ (void)processEvents:(NSValue *)writeFp {
    FILE *readFp = fdopen(fileno((FILE *)writeFp.pointerValue), "r");

    struct _rmevent rpevent;
    while (fread(&rpevent, 1, sizeof rpevent, readFp) == sizeof rpevent) {

        RMLog(@"%@ Event: %d (%f %f)", self, rpevent.phase, rpevent.touches[0].x, rpevent.touches[0].y);

        //if (rpevent.phase == RMTouchMoved && capturing)
        //    continue;

        NSTimeInterval timestamp = rpevent.timestamp;

        NSString *sentText;
        if (rpevent.phase >= RMTouchInsertText) {
            size_t textSize = rpevent.phase - RMTouchInsertText;
            char *buffer = (char *)malloc(textSize + 1);
            textSize = fread(buffer, 1, textSize, readFp);
            buffer[textSize] = '\000';
            sentText = [NSString stringWithUTF8String:buffer];
            free(buffer);
        }

        dispatch_sync(dispatch_get_main_queue(), ^{
            CGPoint location = {rpevent.touches[0].x, rpevent.touches[0].y},
                location2 = {rpevent.touches[1].x, rpevent.touches[1].y};
            static UITextAutocorrectionType saveAuto;
            static BOOL isTextfield, isKeyboard, isButton;
            static UITextField *textField;
            static UITouch *currentTouch2;
            static UIView *currentTarget;
            static unsigned touchIdentifier = 120;
            static WKWebView *webView;
            touchIdentifier++;

            static UITouchesEvent *event;
            if (!event)
                event = [[objc_getClass("UITouchesEvent") alloc] _init];
            RCFakeEvent *fakeEvent = [RCFakeEvent new];
            fakeEvent->_timestamp = timestamp;

            if (sentText) {
                if ([sentText hasPrefix:@"ERROR:"]) {
                    Log(self, @"Error received from remote: %@", sentText);
                    isConnectedWithHost = false;
                    fclose((FILE *)writeFp.pointerValue);
                    close(clientSocket);
                    clientSocket = -1;
                    [InfoMessages.shared onDisconnect];
                    [InfoMessages.shared cancelOperation];
                    return;
                }
                else if([sentText isEqualToString:@"repeato:enter"]){
                    if ([textField isKindOfClass:UITextView.class]) {
                        [textField insertText:@"\n"];
                    }
                    else
                    {
                        if ([textField.delegate respondsToSelector:@selector(textFieldShouldReturn:)]) {
                            [textField.delegate textFieldShouldReturn:textField];
                        }
                        if ( textField.inputViewController != nil && [textField.inputViewController respondsToSelector:@selector(dismissKeyboard)]){
                            [textField.inputViewController dismissKeyboard];
                        }
                        if ([textField respondsToSelector:@selector(resignFirstResponder)]) {
                            [textField resignFirstResponder];
                        }
                        if ([textField respondsToSelector:@selector(endEditing)]) {
                            [textField endEditing:TRUE];
                        }
                    }
                    
                }
                else if ([sentText isEqualToString:@"repeato:clear_text"]){
                    Log(self, @"Remote asked to clear text");
                    [textField setText:@""];
                }
                else if ([sentText isEqualToString:@"repeato:backspace"]) {
                    Log(self, @"Remote asked to remove last letter");
                    [textField deleteBackward];
                }
                else if ([sentText isEqualToString:@"repeato:quit_app"]) {
                    Log(self, @"Remote asked to quit app");
                    exit(0);
                } else if ([sentText isEqualToString:@"repeato:enable_stream"]) {
                    Log(self, @"Enable image stream");
                    isStreamEnabled = TRUE;
                }else if ([sentText isEqualToString:@"repeato:disable_stream"]) {
                    Log(self, @"Disable image stream");
                    isStreamEnabled = FALSE;
                } else if ([sentText hasPrefix:@"repeato:set_scale_up_factor:"]) {
                    NSString *scaleUpParam = [sentText stringByReplacingOccurrencesOfString:@"repeato:set_scale_up_factor:" withString:@""];
                    Log(self, @"Set scale up factor to %{public}@", scaleUpParam);
                    scaleUpFactor = [scaleUpParam floatValue];
                    // send a new frame with the requested resolution right away
                    [self queueCapture];
                } else {
                    if (textField != nil) {
                        Log(self, @"Insert text");
                        [textField insertText:sentText];
                    } else if (webView != nil) {
                        Log(self, @"Insert text into webview");
                        [self injectText:sentText intoWebView:webView];
                    } else {
                        Log(self, @"No textfield or webview found");
                    }
                }

                return;
            }

            inhibitEcho = writeFp;

            switch (rpevent.phase) {

                case RMTouchBeganDouble:

                    currentTarget = nil;
                    for (UIWindow *window in [UIApplication sharedApplication].windows) {
                        UIView *found = [window hitTest:location2 withEvent:nil];
                        if (found)
                            currentTarget = found;
                    }
                    

                    RMDebug(@"Double Target selected: %@", currentTarget);

                    currentTouch2 = [UITouch new];

                    [currentTouch2 setTimestamp:timestamp];
                    [currentTouch2 setInitialTouchTimestamp:timestamp];
                    [currentTouch2 setPhase:(UITouchPhase)rpevent.phase];
                    [currentTouch2 setWindow:currentTarget.window];
                    [currentTouch2 _setTouchIdentifier:touchIdentifier];
                    [currentTouch2 _setPathIndex:1];
                    [currentTouch2 _setPathIdentity:2];
                    [currentTouch2 _setType:0];
                    [currentTouch2 _setSenderID:778835616971358211];
                    [currentTouch2 _setZGradient:0.0];
                    [currentTouch2 _setEdgeType:0];
                    [currentTouch2 _setEdgeAim:0];

                    [currentTouch2 setView:currentTarget];
                    [currentTouch2 setTapCount:1];
                    [currentTouch2 _setLocationInWindow:location resetPrevious:YES];

                case RMTouchBegan:
                    currentTarget = nil;
                    
                    for (UIWindow *window in [UIApplication sharedApplication].windows) {
                        UIView *found = [window hitTest:location withEvent:fakeEvent];
                        if (found)
                            currentTarget = found;
                        
                        /// Find WebView in the window
                        UIView *foundWebView = [self findDescendantViewInView:window withClassName:@"RNCWKWebView"];
                        if (foundWebView) {
                            Log(self, @"Found webview: %@", foundWebView);
                            webView = (WKWebView *)foundWebView;
                        }
                    }

                    isTextfield = [currentTarget respondsToSelector:@selector(setAutocorrectionType:)];
                    isKeyboard = [currentTarget isKindOfClass:objc_getClass("UIKeyboardLayoutStar")];

                    static NSArray *needsTouch;
                    if (!needsTouch) needsTouch = @[
                        objc_getClass("UIButton"),
                        objc_getClass("UITabBarButton"),
                        objc_getClass("_UIButtonBarButton"),
                        objc_getClass("UISegmentedControl"),
                        objc_getClass("UITableViewCellEditControl"),
                        objc_getClass("UISwipeActionStandardButton"),
                    ];

                    isButton = [currentTarget class] == [UIView class];
                    for (Class buttonClass in needsTouch)
                        if ([currentTarget isKindOfClass:buttonClass])
                            isButton = true;

                    if (!currentTouch)
                        currentTouch = realTouch ?: [UITouch new];
                    [currentTouch _setTouchIdentifier:touchIdentifier];

                    RMDebug(@"Target selected: %@ %d %d %d %d %lx\n%@",
                            currentTarget, isTextfield, isKeyboard, isButton,
                            currentTouch._touchIdentifier, currentTouch.hash,
                            [currentTarget recursiveDescription]);

                    if (currentTarget.superview.class ==
                        objc_getClass("UIKeyboardEmojiCollectionViewCell")) {
                        NSString *emoji = [(UILabel *)currentTarget.subviews[0] text];
                        [textField insertText:emoji];
                    }

                    if (isTextfield) {
                        textField = (UITextField *)currentTarget;
                        saveAuto = textField.autocorrectionType;
                        textField.autocorrectionType = UITextAutocorrectionTypeNo;
                    }

                    [currentTouch setTimestamp:timestamp];
                    [currentTouch setInitialTouchTimestamp:timestamp];
                    [currentTouch setPhase:(UITouchPhase)rpevent.phase];
                    [currentTouch setWindow:currentTarget.window];
                    [currentTouch _setPathIndex:1];
                    [currentTouch _setPathIdentity:2];
                    [currentTouch _setType:0];
                    [currentTouch _setSenderID:778835616971358211];
                    [currentTouch _setZGradient:0.0];
                    [currentTouch _setEdgeType:0];
                    [currentTouch _setEdgeAim:0];
                    [currentTouch setView:currentTarget];
                    [currentTouch setTapCount:1];
                    [currentTouch _setLocationInWindow:location resetPrevious:YES];

                    currentTouches = [NSSet setWithObjects:currentTouch, currentTouch2, nil];

                    [event _clearTouches];
                    [event _addTouch:currentTouch forDelayedDelivery:NO];
                    if (currentTouch2)
                        [event _addTouch:currentTouch2 forDelayedDelivery:NO];

                    [[UIApplication sharedApplication] in_sendEvent:event];
                    if (isButton)
                        [currentTarget touchesBegan:currentTouches withEvent:fakeEvent];
                    break;

                case RMTouchMoved:
                case RMTouchStationary:
                    [currentTouch setPhase:(UITouchPhase)rpevent.phase];
                    [currentTouch _setLocationInWindow:location resetPrevious:YES];
                    [currentTouch setTimestamp:timestamp];

                    [currentTouch2 setPhase:(UITouchPhase)rpevent.phase];
                    [currentTouch2 _setLocationInWindow:location2 resetPrevious:YES];
                    [currentTouch2 setTimestamp:timestamp];

                    [[UIApplication sharedApplication] in_sendEvent:event];
                    if (isButton)
                        [currentTarget touchesMoved:currentTouches withEvent:fakeEvent];
                    break;

                case RMTouchEnded:
                case RMTouchCancelled:
                    [currentTouch setPhase:(UITouchPhase)rpevent.phase];
                    [currentTouch _setLocationInWindow:location resetPrevious:YES];
                    [currentTouch setTimestamp:timestamp];

                    [currentTouch2 setPhase:(UITouchPhase)rpevent.phase];
                    [currentTouch2 _setLocationInWindow:location2 resetPrevious:YES];
                    [currentTouch2 setTimestamp:timestamp];

                    [[UIApplication sharedApplication] in_sendEvent:event];
                    if (isButton)
                        [currentTarget touchesEnded:currentTouches withEvent:fakeEvent];

                    if (isTextfield) {
                        UITextField *textField = (UITextField *)currentTarget;
                        textField.autocorrectionType = saveAuto;
                    }

                    currentTouches = nil;
                    currentTarget = nil;
                    currentTouch2 = nil;
                    currentTouch = nil;
                    event = nil;
                    break;

                default:
                    Log(self, @"Invalid Event: %d", rpevent.phase);
            }

            inhibitEcho = nil;
        });
    }

    Log(self, @"processEvents: exits");
    fclose(readFp);

    fclose((FILE *)writeFp.pointerValue);
    [self shutdown];
    Log(self, @"Disconnected");
    isConnectedWithHost = false;
    [InfoMessages.shared onDisconnect];
}

/// Stop capturing events
+ (void)shutdown {
    [repeatoDelegate remoteConnected:FALSE];
    // Invalidate the display link
    REPEATO_APPNAME *instance = [self sharedInstance];
    [instance.displayLink invalidate];
    instance.displayLink = nil;
    // Close the socket
    if (clientSocket != -1) {
        close(clientSocket);
        clientSocket = -1;
    }
}

/// A delicate peice of code to work out when to request the capture of the screen
/// and transmission of it's representation to the RemoteUI server. Routed through
/// writeQueue to ensure that output does not back up on say, cellular connections.
+ (void)queueCapture {
    if (!clientSocket || clientSocket == -1) {
        return;
    }

    NSTimeInterval timestamp = mostRecentScreenUpdate = REPEATO_NOW;
    // 1. if a minimum time (REPEATO_MAXDEFER) passed since transmitting the last frame -> schedule flush
    BOOL flush = timestamp > lastCaptureTime + REPEATO_MAXDEFER;
    if (flush)
        lastCaptureTime = timestamp;
    
    dispatch_async(writeQueue, ^{
        int64_t delta = 0;
        if (!flush) {
            if (timestamp < mostRecentScreenUpdate) {
                Log(self, @"Discard 1 flush: %d", flush);
                return;
            }
            
            delta = (int64_t)(REPEATO_DEFER * NSEC_PER_SEC);
        }

        // 2. wait another x ms (REPEATO_DEFER) to figure out if maybe a newer frame is coming in. In that case, the prev frame is outdated and can be discarded
        dispatch_after(dispatch_time(DISPATCH_TIME_NOW, delta), dispatch_get_main_queue(), ^{
            if(flush){
                if(timestamp < lastCaptureTime){
                    Log(self, @"timestamp < lastCaptureTime");
                    return;
                }
            }else {
                if(timestamp < mostRecentScreenUpdate){
                    Log(self, @"timestamp < mostRecentScreenUpdate");
                    return;
                }
            }
            
            [self capture:timestamp flush:flush];
//            lastCaptureTime = timestamp;
        });
    });
}


+ (void)displayLinkFired:(CADisplayLink *)displayLink {
    [self queueCapture];
}

+ (instancetype)sharedInstance {
    static REPEATO_APPNAME *sharedInstance = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        sharedInstance = [[self alloc] init];
    });
    return sharedInstance;
}

@end

@implementation CALayer(REPEATO_APPNAME)

/// Twp methods that can be swizzled in to generate a stream of notifications the screen has been updated
- (void *)in_copyRenderLayer:(void *)a0 layerFlags:(unsigned)a1 commitFlags:(unsigned *)a2 {
    void *out = [self in_copyRenderLayer:a0 layerFlags:a1 commitFlags:a2];
    RMDebug(@"in_copyRenderLayer: %d %d %@ %lu", capturing, skipEcho, self,
            (unsigned long)[UIApplication sharedApplication].windows.count);
    if (self.class == UIWindowLayer)
        [REPEATO_APPNAME queueCapture];
    return out;
}

- (void)in_didCommitLayer:(void *)a0 {
    [self in_didCommitLayer:a0];
    RMDebug(@"in_didCommitLayer: %d %d %@ %lu", capturing, skipEcho, self,
            (unsigned long)[UIApplication sharedApplication].windows.count);
    if (self.class == UIWindowLayer)
        [REPEATO_APPNAME queueCapture];
}

@end

@implementation UIApplication(REPEATO_APPNAME)

/// Swizzled in to capture device events and transmit them to the RemoteUI server
/// so they can be recorded and played back using processEvents: above.
/// Events encoded as a fake frame with negative length rather than image & size.
/// @param anEvent actual UIEvent which contains the UITouches
- (void)in_sendEvent:(UIEvent *)anEvent {
    [self in_sendEvent:anEvent];
    NSSet *touches = anEvent.allTouches;
    NSValue *incomingFp = inhibitEcho;
    realTouch = touches.anyObject;

//    RMLog(@"%@", anEvent);
//    for (UITouch *t in touches)
//        RMLog(@"Gestures: %@", t.gestureRecognizers);


    struct _rmframe header;
    header.timestamp = REPEATO_NOW;
    header.length = -(int)touches.count;

    NSMutableData *out = [NSMutableData new];
    
    [out appendBytes:&header.length length:sizeof header.length];

    for (UITouch *touch in touches) {
        CGPoint loc = [touch locationInView:touch.window];
        header.phase = (RMTouchPhase)touch.phase;
        header.x = loc.x;
        header.y = loc.y;
        [out appendBytes:&header length:sizeof header];
        header.length++;
    }

    dispatch_async(writeQueue, ^{

        Log(self, @"Starting to write event data...");  

        // if (incomingFp) {
        //     Log(self, @"Writing event data to incoming socket");
        //     FILE *writeFp = (FILE *)incomingFp.pointerValue;
        //     if (fwrite(out.bytes, 1, out.length, writeFp) != out.length)
        //         Log(self, @"Could not write event: %s", REPEATO_APPNAME.class, strerror(errno));
        //     else
        //         fflush(writeFp);
        //     return;
        // }
       
    
    });
}
@end
