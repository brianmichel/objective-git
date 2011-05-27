//
//  GTTree.m
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

#import "GTTree.h"
#import "GTTreeEntry.h"
#import "NSError+Git.h"


@implementation GTTree

- (NSString *)description {
  return [NSString stringWithFormat:@"<%@: %p> numberOfEntries: %i", NSStringFromClass([self class]), self, [self numberOfEntries]];
}

- (git_tree *)tree {
	
	return (git_tree *)self.obj;
}

#pragma mark -
#pragma mark API

/**
 Get the number of entries in a given tree.
 @returns An NSInteger set to the number of entries in the tree.
 */
- (NSInteger)numberOfEntries {

	return (NSInteger)git_tree_entrycount(self.tree);
}

/**
 Create a new GTTreeEntry with a git_tree_entry reference.
 @param entry A git_tree_entry to initialize a new GTTreeEntry object with.
 @returns A newly initialized GTTreeEntry object.
 */
- (GTTreeEntry *)createEntryWithEntry:(const git_tree_entry *)entry {
	
	return [GTTreeEntry entryWithEntry:entry parentTree:self];
}

/**
 Get the GTTreeEntry at a given index.
 @param index The index at which to get the GTTreeEntry from.
 @returns A GTTreeEntry object at the index.
 */
- (GTTreeEntry *)entryAtIndex:(NSInteger)index {
	
	return [self createEntryWithEntry:git_tree_entry_byindex(self.tree, (int)index)];
}

/**
 Get the GTTreeEntry with a given name.
 @param name The name of the GTTreeEntry object.
 @returns A GTTreeEntry object with the same name as name.
 */
- (GTTreeEntry *)entryWithName:(NSString *)name {
	
	return [self createEntryWithEntry:git_tree_entry_byname(self.tree, [name UTF8String])];
}

/*
- (GTTreeEntry *)addEntryWithSha:(NSString *)sha filename:(NSString *)filename mode:(NSInteger *)mode error:(NSError **)error {
	
	git_tree_entry *newEntry;
	git_oid oid;
 
    BOOL success = [sha git_getOid:&oid error:error];
	if(!success) return nil;
	
	int gitError = git_tree_add_entry(&newEntry, self.tree, &oid, [filename UTF8String], (int)mode);
	if(gitError != GIT_SUCCESS) {
		if(error != NULL)
			*error = [NSError gitErrorForAddTreeEntry:gitError];
		return nil;
	}
	
	return [self createEntryWithEntry:newEntry];
}
*/
@end
