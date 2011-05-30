//
//  GTTag.h
//  ObjectiveGitFramework
//
//  Created by Timothy Clem on 2/28/11.
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


@class GTSignature;
@class GTRepository;

@interface GTTag : GTObject {}

/**
 Get the tag struct out of the GTTag object.
 @returns A git_tag struct of the GTTag object.
 */
@property (nonatomic, readonly) git_tag *tag;

/**
 Get the signature of who made the tag.
 @returns A GTSignature object containing the signature of who made the tag.
 */
@property (nonatomic, readonly) GTSignature *tagger;

/**
 Convenience method to create a tag in a given repository with a name, target, tagger, and message.
 @param theRepo A GTRepository object that will contain the tag.
 @param tagName An NSString object that will be the name of the tag.
 @param theTarget A GTObject object that be the target of the tag.
 @param theTagger A GTSignature object that represents who is creating the tag.
 @param theMessage An NSString object that will contain a message about the tag.
 @param error A nil initialized NSError object that will remain nil unless there is an issue creating the tag.
 @returns A GTTag object or nil with an initialized NSError object.
 */
+ (GTTag *)tagInRepository:(GTRepository *)theRepo name:(NSString *)tagName target:(GTObject *)theTarget tagger:(GTSignature *)theTagger message:(NSString *)theMessage error:(NSError **)error;

/**
 Convenience method to create a tag and return the sha in a given repository with a name, target, tagger, and message.
 @param theRepo A GTRepository object that will contain the tag.
 @param tagName An NSString object that will be the name of the tag.
 @param theTarget A GTObject object that be the target of the tag.
 @param theTagger A GTSignature object that represents who is creating the tag.
 @param theMessage An NSString object that will contain a message about the tag.
 @param error A nil initialized NSError object that will remain nil unless there is an issue creating the tag.
 @returns An NSString object that contains the sha or nil with an initialized NSError object.
 */
+ (NSString *)shaByCreatingTagInRepository:(GTRepository *)theRepo name:(NSString *)tagName target:(GTObject *)theTarget tagger:(GTSignature *)theTagger message:(NSString *)theMessage error:(NSError **)error;

/**
 Get the message of a given tag.
 @returns An NSString object containing the message of the tag.
 */
- (NSString *)message;

/**
 Get the name of a given tag.
 @returns An NSString object containing the name of the tag.
 */
- (NSString *)name;

/**
 Get the target of a given tag.
 @returns A GTObject object containing the target of the tag.
 */
- (GTObject *)target;

/**
 Get the target type of a given tag.
 @returns An NSString containing the target type of the tag.
 */
- (NSString *)targetType;

@end
