//
//  MdnsHandler.m
//  MDNSExample
//
//  Created by Muhammad Ahad on 26/01/2023.
//

#import "MdnsHandler.h"
#import <dns_sd.h>
#import <dns_util.h>
#import <stdlib.h>
#import <os/log.h>

@implementation MdnsHandler
+ (instancetype)shared {
    static id instance;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        instance = [[self alloc] init];
    });
    return instance;
}

- (void)startSearchingForServices {
    NSString *name = @"Connector";
    NSString *type = @"_http._tcp";
    NSString *domain = @"local.";
    DNSServiceRef serviceRef;
    DNSServiceErrorType error = DNSServiceResolve(&serviceRef, 0, 0, [name UTF8String], [type UTF8String], [domain UTF8String], callback, (__bridge void *)(self));
    if (error == kDNSServiceErr_NoError) {
        // Start the service
        os_log(OS_LOG_DEFAULT, "%@: Start MDNS service", self);
        DNSServiceProcessResult(serviceRef);
    } else {
        os_log(OS_LOG_DEFAULT, "%@: Error resolving service: %d", self, error);
    }
}

static void callback(DNSServiceRef serviceRef, DNSServiceFlags flags, uint32_t interfaceIndex, DNSServiceErrorType error, const char *fullname, const char *hosttarget, uint16_t port, uint16_t txtLen, const unsigned char *txtRecord, void *context) {
    if (error == kDNSServiceErr_NoError) {
        // Handle the resolved service
        NSString* hostInfo = [NSString stringWithFormat:@"%s", txtRecord];
        hostInfo = [[hostInfo componentsSeparatedByString:@"="] lastObject];
        os_log(OS_LOG_DEFAULT, "MDNS|Resolved service: Host ip %@", hostInfo);
        NSDictionary *dic = [NSDictionary dictionaryWithObject:hostInfo forKey:@"ip"];
        [[NSNotificationCenter defaultCenter] postNotificationName:@"hostInfo"
                                                            object:NULL userInfo:dic];
    } else {
        os_log(OS_LOG_DEFAULT, "MDNS|Error resolving service: %d", error);
    }
}

@end

