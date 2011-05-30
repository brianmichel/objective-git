//
//  GTEnumerator.h
//  ObjectiveGitFramework
//
//  Created by Timothy Clem on 2/21/11.
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


// Options to specify enumeration order when enumerating through a repository.
// These options may be bitwise-OR'd together
enum {
	GTEnumeratorOptionsNone = GIT_SORT_NONE,
	GTEnumeratorOptionsTopologicalSort = GIT_SORT_TOPOLOGICAL, // sort parents before children
	GTEnumeratorOptionsTimeSort = GIT_SORT_TIME, // sort by commit time
	GTEnumeratorOptionsReverse = GIT_SORT_REVERSE, // sort in reverse order
};

typedef unsigned int GTEnumeratorOptions;

@class GTRepository;
@class GTCommit;
@protocol GTObject;

// This object is usually used from within a repository. You generally don't 
// need to instantiate a GTEnumerator. Instead, use the enumerator property on 
// GTRepository
@interface GTEnumerator : NSEnumerator <GTObject> {}

@property (nonatomic, assign) __weak GTRepository *repository;
@property (nonatomic, assign) GTEnumeratorOptions options;

/**
 @param theRepo The GTRepository to initialize the GTEnumerator with.
 @param error The nil initialized address of an NSError object.
 @returns newly initialized GTEnumerator object, or `nil` with initialized `NSError`.
 */
- (id)initWithRepository:(GTRepository *)theRepo error:(NSError **)error;

/**
 Convenience method to get a correctly initialized GTEnumerator.
 @param theRepo The GTRepository to initialize the GTEnumerator with.
 @param error The nil initialized address of an NSError object.
 @returns newly initialized GTEnumerator object, or `nil` with initialized `NSError`.
 */
+ (id)enumeratorWithRepository:(GTRepository *)theRepo error:(NSError **)error;


/**
 @param sha The sha to attempt to push.
 @param error The nil initialized address of an NSError object.
 @return YES if the sha was successfully pushed, or NO with initialized `NSError` if there was a failure.
 */
- (BOOL)push:(NSString *)sha error:(NSError **)error;

// suppress the enumeration of the specified commit and all of its ancestors
/** Suppress a commit.
 Attempts to tell the Enumerator to suppress a commit matching the given sha, as well as all of it's ancestors.
 
 @param sha The sha to attempt to hide.
 @param error The nil initialized address of an NSError object.
 @returns YES if the commit with the given sha was hidden successfully, or NO if it was not with an initialized error.
 */
- (BOOL)skipCommitWithHash:(NSString *)sha error:(NSError **)error;

- (void)reset;
- (NSInteger)countFromSha:(NSString *)sha error:(NSError **)error;

@end
