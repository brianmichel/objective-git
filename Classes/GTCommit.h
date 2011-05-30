//
//  GTCommit.h
//  ObjectiveGitFramework
//
//  Created by Timothy Clem on 2/22/11.
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
@class GTTree;

@interface GTCommit : GTObject {}

@property (nonatomic, readonly) git_commit *commit;
@property (nonatomic, readonly) GTSignature *author;
@property (nonatomic, readonly) GTSignature *committer;
@property (nonatomic, readonly) NSArray *parents;

/** Create and return a new commit in a given repository.
 @param theRepo The `GTRepository` object in which to add the commit.
 @param refName The string representing the reference name to update.
 @param authorSig The `GTSignature` object of the author of the commit.
 @param committerSig The `GTSignature` object of the committer.
 @param newMessage The string representing the commit message.
 @param theTree The `GTTree` object to add the commit to.
 @param theParents An array of `GTCommit` object which are parents of the new commit.
 @param error The `nil` initialized error.
 @return A newly initialized `GTCommit` object, or `nil` with an initialized `NSError` object.
 */
+ (GTCommit *)commitInRepository:(GTRepository *)theRepo
                  updateRefNamed:(NSString *)refName
                          author:(GTSignature *)authorSig
                       committer:(GTSignature *)committerSig
                         message:(NSString *)newMessage
                            tree:(GTTree *)theTree 
                         parents:(NSArray *)theParents 
                           error:(NSError **)error;

/** Create and return a new commit sha in a given repository.
 @param theRepo The `GTRepository` object in which to add the commit.
 @param refName The string representing the reference name to update.
 @param authorSig The `GTSignature` object of the author of the commit.
 @param committerSig The `GTSignature` object of the committer.
 @param newMessage The string representing the commit message.
 @param theTree The `GTTree` object to add the commit to.
 @param theParents An array of `GTCommit` object which are parents of the new commit.
 @param error The `nil` initialized error.
 @return A newly initialized string containing the sha of the commit, or `nil` with an initialized `NSError` object.
 */
+ (NSString *)shaByCreatingCommitInRepository:(GTRepository *)theRepo
                               updateRefNamed:(NSString *)refName
                                       author:(GTSignature *)authorSig
                                    committer:(GTSignature *)committerSig
                                      message:(NSString *)newMessage
                                         tree:(GTTree *)theTree 
                                      parents:(NSArray *)theParents 
                                        error:(NSError **)error;

/** Get the commit message.
 @return An `NSString` object containing the commit message.
 */
- (NSString *)message;

/** Get the shortened version of the commit message.
 @return An `NSString` object containing the shortened commit message.
 */
- (NSString *)shortMessage;

/** Get the message details for the given commit.
 @return An `NSString` object containing the message details for the commit.
 */
- (NSString *)messageDetails;

/** Get the date of the commit.
 @return An `NSDate` object initialized to the commit date.
 */
- (NSDate *)commitDate;

/** Get the `GTTree` object that the commit belongs to.
 @return A `GTTree` object which contains the commit.
 */
- (GTTree *)tree;

@end
