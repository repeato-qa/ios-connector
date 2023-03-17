//
//  MdnsHandler.h
//  MDNSExample
//
//  Created by Muhammad Ahad on 26/01/2023.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface MdnsHandler : NSObject

+ (id)shared;
- (void)startSearchingForServices;

@end

NS_ASSUME_NONNULL_END
