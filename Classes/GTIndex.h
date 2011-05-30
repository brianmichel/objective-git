//
//  GTIndex.h
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


@class GTIndexEntry;

@interface GTIndex : NSObject {}

@property (nonatomic, assign) git_index *index;
@property (nonatomic, copy) NSURL *path;

/**
 Get the entry count of a given index.
 @returns An NSInteger containing the number of entries in a given index.
 */
@property (nonatomic, assign) NSInteger entryCount;

// Convenience initializers
/**
 Create a new GTIndex object initialized with the localFileUrl.
 @param localFileUrl An NSURL containing the local file URL of the file.
 @param error A nil-initialized address of an NSError object to return if there is an error.
 @returns A newly initialized GTIndex object, or nil with an initialized error object if there is an issue.
 */
- (id)initWithPath:(NSURL *)localFileUrl error:(NSError **)error;

/**
 Create a new GTIndex object initialized with the localFileUrl. (Convenience Method)
 @param localFileUrl An NSURL containing the local file URL of the file.
 @param error A nil-initialized address of an NSError object to return if there is an error.
 @returns A newly initialized GTIndex object, or nil with an initialized error object if there is an issue.
 */
+ (id)indexWithPath:(NSURL *)localFileUrl error:(NSError **)error;

/**
 Create a new GTIndex object from a git_index struct.
 @param theIndex A git_index struct representing the index.
 @returns A newly initialized GTIndex object.
 */
- (id)initWithGitIndex:(git_index *)theIndex;

/**
 Create a new GTIndex object from a git_index struct. (Convenience Method)
 @param theIndex A git_index struct representing the index.
 @returns A newly initialized GTIndex object.
 */
+ (id)indexWithGitIndex:(git_index *)theIndex;

// Refresh the index from the datastore
//
// error(out) - will be filled if an error occurs
//
// returns YES if refresh was successful
/**
 Attempt to refresh the GTIndex.
 @param error A nil-initialized NSError object.
 @returns YES if the refresh was successful, or NO with an initialized error object. if there was a problem.
 */
- (BOOL)refreshWithError:(NSError **)error;

// Clear the contents (all entry objects) of the index. This happens in memory.
// Changes can be written to the datastore by calling writeAndReturnError:
/**
 Clear the given index.
 */
- (void)clear;

// Get entries from the index
/**
 Get the GTIndexEntry at a given index.
 @param theIndex An NSInteger of the index at which to find the entry.
 @returns A newly initialized GTIndexEntry object.
 */
- (GTIndexEntry *)entryAtIndex:(NSInteger)theIndex;

/**
 Get the GTIndexEntry for a given name.
 @param name An NSString of the entry to find.
 @returns A newly initialized GTIndexEntry object.
 */
- (GTIndexEntry *)entryWithName:(NSString *)name;

// Add entries to the index
/**
 Add a GTIndexEntry object to the current GTIndex.
 @param entry A GTIndexEntry object to add to the GTIndex.
 @param error A nil initialized address of an NSError object.
 @returns YES if successfully added the entry to the index, or NO with an initialized error if there was an issue.
 */
- (BOOL)addEntry:(GTIndexEntry *)entry error:(NSError **)error;

/**
 Add a given file to the current GTIndex.
 @param file An NSString object of the file to add to the GTIndex.
 @param error A nil initialized address of an NSError object.
 @returns YES if successfully added the entry to the index, or NO with an initialized error if there was an issue.
 */
- (BOOL)addFile:(NSString *)file error:(NSError **)error;

// Write the index to the datastore
//
// error(out) - will be filled if an error occurs
//
// returns YES if the write was successful.
/**
 Write the current GTIndex.
 @param error A nil initialized address of an NSError object.
 @returns YES if the write succeeded, or NO if the write did not succeed with an initialized error.
 */
- (BOOL)writeWithError:(NSError **)error;

@end
