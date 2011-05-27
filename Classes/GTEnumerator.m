//
//  GTEnumerator.m
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

#import "GTEnumerator.h"
#import "GTCommit.h"
#import "NSError+Git.h"
#import "NSString+Git.h"
#import "GTRepository.h"


@interface GTEnumerator()
@property (nonatomic, assign) git_revwalk *walk;
@end

@implementation GTEnumerator

- (NSString *)description {
  return [NSString stringWithFormat:@"<%@: %p> repository: %@", NSStringFromClass([self class]), self, self.repository];
}

- (void)dealloc {
	
	git_revwalk_free(self.walk);
	self.repository = nil;
	[super dealloc];
}

- (void)finalize {
	
	git_revwalk_free(self.walk);
	[super finalize];
}

#pragma mark -
#pragma mark API

@synthesize repository;
@synthesize walk;
@synthesize options;

/**
 @param theRepo The GTRepository to initialize the GTEnumerator with.
 @param error The nil initialized address of an NSError object.
 @returns newly initialized GTEnumerator object, or nil with initialized error.
 */
- (id)initWithRepository:(GTRepository *)theRepo error:(NSError **)error {
	
	if((self = [super init])) {
		self.repository = theRepo;
		git_revwalk *w;
		int gitError = git_revwalk_new(&w, self.repository.repo);
		if(gitError != GIT_SUCCESS) {
			if (error != NULL)
				*error = [NSError gitErrorForInitRevWalker:gitError];
            [self release];
			return nil;
		}
		self.walk = w;
	}
	return self;
}

/**
 Convenience method to get a correctly initialized GTEnumerator.
 @param theRepo The GTRepository to initialize the GTEnumerator with.
 @param error The nil initialized address of an NSError object.
 @returns newly initialized GTEnumerator object, or nil with initialized error.
 */
+ (id)enumeratorWithRepository:(GTRepository *)theRepo error:(NSError **)error {
	
	return [[[self alloc] initWithRepository:theRepo error:error] autorelease];
}

/**
 @param sha The sha to attempt to push.
 @param error The nil initialized address of an NSError object.
 @return YES if the sha was successfully pushed, or NO with initialized error if there was a failure.
 */
- (BOOL)push:(NSString *)sha error:(NSError **)error {
	
	git_oid oid;
	BOOL success = [sha git_getOid:&oid error:error];
	if(!success)return NO;
	
	[self reset];
	
	int gitError = git_revwalk_push(self.walk, &oid);
	if(gitError != GIT_SUCCESS) {
		if (error != NULL)
			*error = [NSError gitErrorForPushRevWalker:gitError];
		return NO;
	}
	
	return YES;
}

/**
 Attempts to tell the Enumerator to hide a commit matching the given sha.
 @param sha The sha to attempt to hide.
 @param error The nil initialized address of an NSError object.
 @returns YES if the commit with the given sha was hidden successfully, or NO if it was not with an initialized error.
 */
- (BOOL)skipCommitWithHash:(NSString *)sha error:(NSError **)error {
	
	git_oid oid;
	BOOL success = [sha git_getOid:&oid error:error];
	if(!success)return NO;
	
	int gitError = git_revwalk_hide(self.walk, &oid);
	if(gitError != GIT_SUCCESS) {
		if (error != NULL)
			*error = [NSError gitErrorForHideRevWalker:gitError];
		return NO;
	}
	return YES;
}

/**
 Resets the revwalker.
 */
- (void)reset {
	
	git_revwalk_reset(self.walk);
}


/**
 Set the options on the GTEnumerator.
 @param newOptions A mask of GTEnumerationOptions to configure the enumerator.
 */
- (void)setOptions:(GTEnumeratorOptions)newOptions {
    options = newOptions;
	git_revwalk_sorting(self.walk, (unsigned int)options);
}

/**
 Get the next object in the repo.
 @return Commit object matching next sha.
 */
- (id)nextObject {
	
	git_oid oid;
	int gitError = git_revwalk_next(&oid, self.walk);
	if(gitError == GIT_EREVWALKOVER)
		return nil;
	
	// ignore error if we can't lookup object and just return nil
	return [self.repository lookupObjectBySha:[NSString git_stringWithOid:&oid] objectType:GTObjectTypeCommit error:nil];
}

- (NSArray *)allObjects {

    NSMutableArray *array = [NSMutableArray array];
    id object = nil;
    while ((object = [self nextObject]) != nil) {
        [array addObject:object];
    }
    return array;
}

/**
 @param sha The sha string to start counting from.
 @param error The nil initialized address of an NSError object.
 @returns initialized NSInteger with the count.
 */
- (NSInteger)countFromSha:(NSString *)sha error:(NSError **)error {
	
	[self setOptions:GTEnumeratorOptionsNone];
	
	BOOL success = [self push:sha error:error];
	if(!success) return NSNotFound;
	
	git_oid oid;
	NSUInteger count = 0;
	while(git_revwalk_next(&oid, self.walk) == GIT_SUCCESS) {
		count++;
	}
	return count;
}

@end
