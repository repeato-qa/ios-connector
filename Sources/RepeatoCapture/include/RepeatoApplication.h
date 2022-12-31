//
//  RCAppName.h
//  Repeato
//
//  Created by Muhammad Ahad on 30/12/2022.
//

#ifndef __IPHONE_OS_VERSION_MIN_REQUIRED
#import <Cocoa/Cocoa.h>
#else
#import <UIKit/UIKit.h>
#import <objc/runtime.h>

#ifdef __IPHONE_OS_VERSION_MIN_REQUIRED

static NSMutableArray<NSValue *> *connections;
static char *connectionKey;

static dispatch_queue_t writeQueue; // queue to synchronise outgoing writes
static struct _rmdevice device; // header sent to RemoteUI server on connect
static NSValue *inhibitEcho; // prevent events from server going back to server
static Class UIWindowLayer; // Use to filter for full window layer updates
static UITouch *realTouch; // An actual UITouch recycled for forging events
static CGSize bufferSize; // current size of off-screen image buffers

static int skipEcho; // Was to filter out layer commits during capture
static BOOL capturing; // Am in the middle of capturing
static BOOL isStreamEnabled = true;
static float scaleUpFactor = 0.5; // can be remote controlled
static NSTimeInterval mostRecentScreenUpdate; // last window layer update
static NSTimeInterval lastCaptureTime; // last time capture was forced
static NSArray *buffers; // off-screen buffers use in encoding images
static int frameno; // count of frames captured and transmmitted

#endif

@protocol RepeatoDelegate <NSObject>
@required
- (void)remoteConnected:(BOOL)status;
@end

static id<RepeatoDelegate> repeatoDelegate;

@interface REPEATO_APPNAME: NSObject {
@package
    rmpixel_t *buffer, *buffend;
    CGContextRef cg;
}
- (instancetype)initFrame:(const struct _rmframe *)frame;
- (NSData *)subtractAndEncode:(REPEATO_APPNAME *)prevbuff;
- (CGImageRef)cgImage;
@end

@interface REPEATO_APPNAME(Client)
+ (void)startCapture:(NSString *)addrs scaleUpFactor:(float)s;
+ (void)shutdown;
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

@interface UITouch(Identifier)
- (void)_setTouchIdentifier:(unsigned int)ident;
@end
#endif
