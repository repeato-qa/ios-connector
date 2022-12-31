//
//  RCConstants.h
//  Repeato
//
//  Created by Muhammad Ahad on 30/12/2022.
//
#import "Foundation/Foundation.h"
#ifndef REPEATO_PORT
#define REPEATO_PORT 31449
#endif

#ifndef REPEATO_APPNAME
#define REPEATO_APPNAME RepeatoCapture
#endif
#define REPEATO_MAGIC -141414141

// Various wire formats used.
//#define REPEATO_NOKEY 3 // Original format
#define REPEATO_VERSION 4 // Sends source file path for security check
//#define MINICAP_VERSION 1 // https://github.com/openstf/minicap#usage
#define HYBRID_VERSION 2 // minicap but starting with "Remote" header

// May be used for security
#define REPEATO_KEY @__FILE__
#define REPEATO_XOR 0xc5

// Times coordinate-resolution to capture.
#ifndef REPEATO_OVERSAMPLE
#ifndef REPEATO_HYBRID
#define REPEATO_OVERSAMPLE 1.0
#else
#define REPEATO_OVERSAMPLE *(float *)device.remote.scale
#endif
#endif

#ifndef REPEATO_JPEGQUALITY
#define REPEATO_JPEGQUALITY 0.5
#endif

#ifndef REPEATO_RETRIES
#define REPEATO_RETRIES 3
#endif

#ifdef REPEATO_HYBRID
// Wait for screen to settle before capture
#ifndef REPEATO_DEFER
#define REPEATO_DEFER 0.5
#endif

// Only wait this long for screen to settle
#ifndef REPEATO_MAXDEFER
#define REPEATO_MAXDEFER 0.1
#endif
#else
// Wait for screen to settle before capture
#ifndef REPEATO_DEFER
#define REPEATO_DEFER 0.5
#endif

// Only wait this long for screen to settle
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

#define RMMAX_TOUCHES 2

//////////

#ifdef REPEATO_LEGACY
static BOOL repeatoLegacy = TRUE;
#else
static BOOL repeatoLegacy = FALSE;
#endif

typedef unsigned rmpixel_t;
typedef unsigned rmencoded_t;

/// Shaows UITouchPhase enum but available to Appkit code in server.
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

/// struct sent from client when it connects to rendering server
/// Can be either original Remote format or "minicap".
struct _rmdevice {
    char version;
    union {
        struct {
            char machine[24];
            char appname[64];
            char appvers[24];
            char hostname[63];
            char scale[4]; // float
            char isIPad[4]; // int
            char protocolVersion[4];// int
            char displaySize[12]; // 9999x9999
            char expansion[48];
            char magic[4]; // int
        } remote;
    };
};

/// struct send before each iage frame sen to render server
struct _rmframe {
    NSTimeInterval timestamp;
    union {
        /// image paramters
        struct { float width, height, imageScale; };
        /// If length < 0, retails of a recorded touch
        struct { float x, y; RMTouchPhase phase; };
    };
    /// length of image data or if < 0 -touch number
    int length;
};

/// Struct sent from UI of server to replay touches in the client
struct _rmevent {
    NSTimeInterval timestamp;
    RMTouchPhase phase;
    union {
        struct { float x, y; } touches[RMMAX_TOUCHES];
    };
    /* int padded; */
};

