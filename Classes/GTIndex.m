//
//  GTIndex.m
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

#import "GTIndex.h"
#import "GTIndexEntry.h"
#import "NSError+Git.h"


@implementation GTIndex

- (NSString *)description {
  return [NSString stringWithFormat:@"<%@: %p> path: %@, entryCount: %i", NSStringFromClass([self class]), self, self.path, self.entryCount];
}

- (void)dealloc {
	
	//git_index_free(self.index);
	self.path = nil;
	[super dealloc];
}

#pragma mark -
#pragma mark API

@synthesize index;
@synthesize path;
@synthesize entryCount;

/**
 Create a new GTIndex object initialized with the localFileUrl.
 @param localFileUrl An NSURL containing the local file URL of the file.
 @param error A nil-initialized address of an NSError object to return if there is an error.
 @returns A newly initialized GTIndex object, or nil with an initialized error object if there is an issue.
 */
- (id)initWithPath:(NSURL *)localFileUrl error:(NSError **)error {
	
	if((self = [super init])) {
		self.path = localFileUrl;
		git_index *i;
		int gitError = git_index_open_bare(&i, [[self.path path] UTF8String]);
		if(gitError != GIT_SUCCESS) {
			if(error != NULL)
				*error = [NSError gitErrorForInitIndex:gitError];
            [self release];
			return nil;
		}
		self.index = i;
	}
	return self;
}

/**
 Create a new GTIndex object initialized with the localFileUrl. (Convenience Method)
 @param localFileUrl An NSURL containing the local file URL of the file.
 @param error A nil-initialized address of an NSError object to return if there is an error.
 @returns A newly initialized GTIndex object, or nil with an initialized error object if there is an issue.
 */
+ (id)indexWithPath:(NSURL *)localFileUrl error:(NSError **)error {
	
	return [[[self alloc] initWithPath:localFileUrl error:error] autorelease];
}

/**
 Create a new GTIndex object from a git_index struct.
 @param theIndex A git_index struct representing the index.
 @returns A newly initialized GTIndex object.
 */
- (id)initWithGitIndex:(git_index *)theIndex; {
	
	if((self = [super init])) {
		self.index = theIndex;
	}
	return self;
}

/**
 Create a new GTIndex object from a git_index struct. (Convenience Method)
 @param theIndex A git_index struct representing the index.
 @returns A newly initialized GTIndex object.
 */
+ (id)indexWithGitIndex:(git_index *)theIndex {
	
	return [[[self alloc] initWithGitIndex:theIndex] autorelease];
}

/**
 Get the entry count of a given index.
 @returns An NSInteger containing the number of entries in a given index.
 */
- (NSInteger)entryCount {
	
	return git_index_entrycount(self.index);
}

/**
 Attempt to refresh the GTIndex.
 @param error A nil-initialized NSError object.
 @returns YES if the refresh was successful, or NO with an initialized error object. if there was a problem.
 */
- (BOOL)refreshWithError:(NSError **)error {
	
	int gitError = git_index_read(self.index);
	if(gitError != GIT_SUCCESS) {
		if(error != NULL)
			*error = [NSError gitErrorForReadIndex:gitError];
		return NO;
	}
	return YES;
}

/**
 Clear the given index.
 */
- (void)clear {
	
	git_index_clear(self.index);
}

/**
 Get the GTIndexEntry at a given index.
 @param theIndex An NSInteger of the index at which to find the entry.
 @returns A newly initialized GTIndexEntry object.
 */
- (GTIndexEntry *)entryAtIndex:(NSInteger)theIndex {
	
	return [GTIndexEntry indexEntryWithEntry:git_index_get(self.index, (int)theIndex)];
}

/**
 Get the GTIndexEntry for a given name.
 @param name An NSString of the entry to find.
 @returns A newly initialized GTIndexEntry object.
 */
- (GTIndexEntry *)entryWithName:(NSString *)name {
	
	int i = git_index_find(self.index, [name UTF8String]);
	return [GTIndexEntry indexEntryWithEntry:git_index_get(self.index, i)];
}

/**
 Add a GTIndexEntry object to the current GTIndex.
 @param entry A GTIndexEntry object to add to the GTIndex.
 @param error A nil initialized address of an NSError object.
 @returns YES if successfully added the entry to the index, or NO with an initialized error if there was an issue.
 */
- (BOOL)addEntry:(GTIndexEntry *)entry error:(NSError **)error {
	
	int gitError = git_index_add2(self.index, entry.entry);
	if(gitError != GIT_SUCCESS) {
		if(error != NULL)
			*error = [NSError gitErrorForAddEntryToIndex:gitError];
		return NO;
	}
	return YES;
}

/**
 Add a given file to the current GTIndex.
 @param file An NSString object of the file to add to the GTIndex.
 @param error A nil initialized address of an NSError object.
 @returns YES if successfully added the entry to the index, or NO with an initialized error if there was an issue.
 */
- (BOOL)addFile:(NSString *)file error:(NSError **)error {
	
	int gitError = git_index_add(self.index, [file UTF8String], 0);
	if(gitError != GIT_SUCCESS) {
		if(error != NULL)
			*error = [NSError gitErrorForAddEntryToIndex:gitError];
		return NO;
	}
	return YES;
}

/**
 Write the current GTIndex.
 @param error A nil initialized address of an NSError object.
 @returns YES if the write succeeded, or NO if the write did not succeed with an initialized error.
 */
- (BOOL)writeWithError:(NSError **)error {
	
	int gitError = git_index_write(self.index);
	if(gitError != GIT_SUCCESS) {
		if(error != NULL)
			*error = [NSError gitErrorForWriteIndex:gitError];
		return NO;
	}
	return YES;
}

@end
