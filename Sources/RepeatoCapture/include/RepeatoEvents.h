//
//  Repeato
//
//  Created by Muhammad Ahad on 30/12/2022.
//

static NSTimeInterval timestamp0;
static UITouch *currentTouch;
static NSSet *currentTouches;
static BOOL lateJoiners;

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
