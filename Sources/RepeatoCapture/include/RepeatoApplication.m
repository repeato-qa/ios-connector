//
//  RCApplication.m
//  Repeato
//
//  Created by Muhammad Ahad on 30/12/2022.
//
#import "RepeatoConstants.h"
#import "RepeatoApplication.h"
#import <os/log.h>

#ifdef __IPHONE_OS_VERSION_MIN_REQUIRED
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
    if (device.version <= HYBRID_VERSION)
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
        for (NSValue *fp in connections) {
            if (fp == incomingFp)
                continue;
            FILE *writeFp = (FILE *)fp.pointerValue;
            if (fwrite(out.bytes, 1, out.length, writeFp) != out.length)
                os_log(OS_LOG_DEFAULT, "%@: Could not write event: %s", REPEATO_APPNAME.class, strerror(errno));
            else
                fflush(writeFp);
        }
    });
}

//#endif
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
