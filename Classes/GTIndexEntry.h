//
//  GTIndexEntry.h
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


@interface GTIndexEntry : NSObject {}

@property (nonatomic, assign) git_index_entry *entry;

/**
 Get the path of a given GTIndexEntry.
 @returns An NSString object containing the path of the GTIndexEntry.
 */
@property (nonatomic, assign) NSString *path;

/**
 Get the modification date of a given GTIndexEntry.
 @returns An NSDate containing the modification date of the GTIndexEntry.
 */
@property (nonatomic, assign) NSDate *modificationDate;

/**
 Get the creation date of a given GTIndexEntry.
 @returns An NSDate containing the creation date of the GTIndexEntry.
 */
@property (nonatomic, assign) NSDate *creationDate;
@property (nonatomic, assign) long long fileSize;
@property (nonatomic, assign) NSUInteger dev;
@property (nonatomic, assign) NSUInteger ino;
@property (nonatomic, assign) NSUInteger mode;
@property (nonatomic, assign) NSUInteger uid;
@property (nonatomic, assign) NSUInteger gid;
@property (nonatomic, assign) NSUInteger flags;
@property (nonatomic, assign) NSUInteger stage;
@property (nonatomic, readonly) BOOL isValid;

// Convenience initializers
/**
 Create a new GTIndexEntry with a git_index_entry reference.
 @params theEntry A git_index_entry to initialize the GTIndexEntry object with.
 @returns A newly initialized GTIndexEntry object.
 */
- (id)initWithEntry:(git_index_entry *)theEntry;

/**
 Convenience class method of initWithEntry:.
 @params theEntry A git_index_entry to initialize the GTIndexEntry object with.
 @returns A newly initialized GTIndexEntry object.
 */
+ (id)indexEntryWithEntry:(git_index_entry *)theEntry;

/**
 Get the sha of a given GTIndexEntry.
 @returns An NSString object containing the sha of the GTIndexEntry.
 */
- (NSString *)sha;
- (BOOL)setSha:(NSString *)theSha error:(NSError **)error;

@end
