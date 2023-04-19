//
//  Logger.h
//  Repeato
//
//  Created by Muhammad Ahad on 18/04/2023.
//

#import <OSLog/OSLog.h>

#define Log(_a_,_b_,...) [[Logger shared] log:_a_ format:_b_,##__VA_ARGS__]
#define DebugLog(_a_,_b_,...) [[Logger shared] debugLog:_a_ format:_b_,##__VA_ARGS__]

NS_ASSUME_NONNULL_BEGIN
@protocol LogAlertDelegate
- (void)logEvent:(NSString *)message;
@end

@interface Logger : NSObject

@property (nonatomic, weak) id delegate;
+ (instancetype)shared;
-(void)log:reference format:(NSString *)format, ...;
-(void)debugLog:reference format:(NSString *)format, ...;
@end

@implementation Logger

+ (instancetype)shared {
    static id instance;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        instance = [[self alloc] init];
    });
    return instance;
}

-(void)log:reference format:(NSString *)format, ...{
    va_list ap;
    va_start(ap, format);
    NSString* str = [[NSString alloc] initWithFormat:format arguments:ap];
    va_end(ap);
    os_log(OS_LOG_DEFAULT, "%@: %@", reference, str);
    if(self.delegate){
        [self.delegate logEvent:str];
    }
}

-(void)debugLog:reference format:(NSString *)format, ...{
    va_list ap;
    va_start(ap, format);
    NSString* str = [[NSString alloc] initWithFormat:format arguments:ap];
    va_end(ap);
    os_log(OS_LOG_DEFAULT, "%@: %@", reference, str);
}

@end
NS_ASSUME_NONNULL_END
