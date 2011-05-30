//
//  GTBlob.h
//  ObjectiveGitFramework
//
//  Created by Timothy Clem on 2/25/11.
//
//  The MIT License
//
//  Copyright (c) 2011 Tim Clem
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
//

#import <git2.h>
#import "GTObject.h"


@interface GTBlob : GTObject {}

@property (nonatomic, readonly) git_blob *blob;

/**
 Create a new blob object initialized with a string and repostory.
 @param string An NSString to create the blob with.
 @param repository A GTRepository object to create the blob in.
 @param error A nil initialized address of an error object.
 @returns A newly initialized GTBlob object, or `nil` with an initialized `NSError` if initialization fails.
 */
+ (id)blobWithString:(NSString *)string inRepository:(GTRepository *)repository error:(NSError **)error;

/**
 Create a new blob object initialized with data and repostory. (Convenience Method)
 @param data An NSData to create the blob with.
 @param repository A GTRepository object to create the blob in.
 @param error A nil initialized address of an error object.
 @returns A newly initialized GTBlob object, or `nil` with an initialized `NSError` if initialization fails.
 */
+ (id)blobWithData:(NSData *)data inRepository:(GTRepository *)repository error:(NSError **)error;

/**
 Create a new blob object initialized with a file and repostory. (Convenience Method)
 @param file An NSURL to create the blob with.
 @param repository A GTRepository object to create the blob in.
 @param error A nil initialized address of an error object.
 @returns A newly initialized GTBlob object, or `nil` with an initialized `NSError` if initialization fails.
 */
+ (id)blobWithFile:(NSURL *)file inRepository:(GTRepository *)repository error:(NSError **)error;

/**
 Create a new blob object initialized with a string and repostory.
 @param string An NSString to create the blob with.
 @param repository A GTRepository object to create the blob in.
 @param error A nil initialized address of an error object.
 @returns A newly initialized GTBlob object, or nil with an initialized error if initialization fails.
 */
- (id)initWithString:(NSString *)string inRepository:(GTRepository *)repository error:(NSError **)error;

/**
 Create a new blob object initialized with data and repostory.
 @param data An NSData to create the blob with.
 @param repository A GTRepository object to create the blob in.
 @param error A nil initialized address of an error object.
 @returns A newly initialized GTBlob object, or nil with an initialized error if initialization fails.
 */
- (id)initWithData:(NSData *)data inRepository:(GTRepository *)repository error:(NSError **)error;

/**
 Create a new blob object initialized with a file and repostory. (Convenience Method)
 @param file An NSURL to create the blob with.
 @param repository A GTRepository object to create the blob in.
 @param error A nil initialized address of an error object.
 @returns A newly initialized GTBlob object, or nil with an initialized error if initialization fails.
 */
- (id)initWithFile:(NSURL *)file inRepository:(GTRepository *)repository error:(NSError **)error;

/**
 Get the size of the GTBlob object.
 @returns An NSInteger with the size of the GTBlob.
 */
- (NSInteger)size;

/**
 Get the content of a GTBlob object.
 @returns An NSString containing the contents of the GTBlob.
 */
- (NSString *)content;

/**
 Get the data of the GTBlob object.
 @returns An NSData object containing the data of the GTBlob object.
 */
- (NSData *)data;

@end
