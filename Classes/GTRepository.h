//
//  GTRepository.h
//  ObjectiveGitFramework
//
//  Created by Timothy Clem on 2/17/11.
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
#import "GTEnumerator.h"
#import "GTReference.h"

@class GTObjectDatabase;
@class GTOdbObject;
@class GTCommit;
@class GTIndex;
@class GTBranch;

/** Implements the interface to the `GTObject` for a repository.
 
 This class contains an Objective-C binding to the C library in regards to the git repository.
 
 */
@interface GTRepository : NSObject <GTObject> {}

/**
 Readonly, assigned property for the C-API repo.
 */
@property (nonatomic, assign, readonly) git_repository *repo;

/** Returns the current file url that has been used to initialize the repository.

 @see initializeEmptyRepositoryAtURL:error:
 @see repositoryWithURL:error:
 @see initWithURL:error:
 */
@property (nonatomic, retain, readonly) NSURL *fileUrl;

/** Returns the enumerator for the repository.
 
 This enumerator can be utilized to walk the commit tree within a repository
  and push a commit onto a repository, as well as counting commits from a given sha.
 
 @warning *Note:* This should only be used on the main thread.
 */
@property (nonatomic, retain, readonly) GTEnumerator *enumerator; // should only be used on the main thread

/** Returns the current index for the repository.
 This index can be used to find entris at a given name or index, 
  it can also add entries or files to the index.

 */
@property (nonatomic, retain, readonly) GTIndex *index;

/** Returns the current object database for the repository.
 */
@property (nonatomic, retain, readonly) GTObjectDatabase *objectDatabase;

///----------------------
/// @name Initialization
///----------------------

/** Initialize an empty repository at `localFileURL` and return `YES` if 
  successful, or `NO` with the `error` object initialized with the current
  error. 
 
  @param localFileURL File URL at which to make the repository.
  @param error Error object to be initialized if there is a problem making the repo.
  @return `YES` if successful, or `NO` with an initialized `NSError`.
 */
+ (BOOL)initializeEmptyRepositoryAtURL:(NSURL *)localFileURL error:(NSError **)error;

/** Load a repository at `localFileURL` and return the `GTRepository` object representing the repo if successful, or `nil` with an initialized `NSError`.
 
  @param localFileURL File URL at which to load the repository.
  @param error Error object to be initialized if there is a problem loading the repo.
  @return `GTRepository` object representing the repo, or `nil` with an initialized `NSError`.
 */
+ (id)repositoryWithURL:(NSURL *)localFileURL error:(NSError **)error;
- (id)initWithURL:(NSURL *)localFileURL error:(NSError **)error;

// Helper for getting the sha1 has of a raw object
//
// data - the data to compute a sha1 hash for
// error(out) - will be filled if an error occurs
//
// returns the sha1 for the raw object or nil if there was an error
///----------------------
/// @name Utility
///----------------------

/** Get the SHA1 hash of the input data for a given object type.
  This utility method will return the SHA1 hash of the input string, or return an error if there is an issue computing it.
  
 @param data The string of data that needs to have it's SHA1 value calculated
 @param type The type of object we are computing the SHA1 value for.
 @param error The `nil` initialized error object.
 @return `NSString` initialized with the SHA1 value on success, or `nil` with an initialized `NSError` object.
 */
+ (NSString *)hash:(NSString *)data objectType:(GTObjectType)type error:(NSError **)error;

// Lookup objects in the repo by oid or sha1
///----------------------
/// @name Lookup
///----------------------

/** Look up an object of a given type by oid.
 This method will allow you to look up an object of a given type via Git oid. It will return either the object, or `nil`
   if there is an error.
 
 @param oid The `git_oid` used for lookup.
 @param type The `GTObjectType` used for lookup.
 @param error The `nil` initialized error object.
 @return `GTObject` of a given oid and type, or `nil` with an initialized `NSError` object.
 */
- (GTObject *)lookupObjectByOid:(git_oid *)oid objectType:(GTObjectType)type error:(NSError **)error;

/** Look up an object by oid.
 This method will allow you to look up an object via Git oid. It will return either the object, or `nil`
 if there is an error.
 
 @param oid The `git_oid` used for lookup.
 @param error The `nil` initialized error object.
 @return `GTObject` of a given oid, or `nil` with an initialized `NSError` object.
 */
- (GTObject *)lookupObjectByOid:(git_oid *)oid error:(NSError **)error;

/** Look up an object of a given type by SHA.
 This method will allow you to look up an object of a given type via SHA. It will return either the object, or `nil`
 if there is an error.
 
 @param sha The SHA used for lookup.
 @param type The `GTObjectType` used for lookup.
 @param error The `nil` initialized error object.
 @return `GTObject` of a given SHA and type, or `nil` with an initialized `NSError` object.
 */
- (GTObject *)lookupObjectBySha:(NSString *)sha objectType:(GTObjectType)type error:(NSError **)error;

/** Look up an object by SHA.
 This method will allow you to look up an object via SHA. It will return either the object, or `nil`
 if there is an error.
 
 @param sha The SHA used for lookup.
 @param error The `nil` initialized error object.
 @return `GTObject` of a given SHA, or `nil` with an initialized `NSError` object.
 */
- (GTObject *)lookupObjectBySha:(NSString *)sha error:(NSError **)error;

///----------------------
/// @name Enumeration
///----------------------

/** Enumerate commits starting at a given SHA with sorting options.
 This method will enumerate all of the commits starting at `sha` and continuing to HEAD.
 
 @param sha The SHA at which to start the enumeration.
 @param options The `GTEnumeratorOptions` mask to be set for enumeration.
 @param error The `nil` initialized error object.
 @param block The `(void (^)(GTCommit *, BOOL*))` block used for enumeration.
 */
- (void)enumerateCommitsBeginningAtSha:(NSString *)sha sortOptions:(GTEnumeratorOptions)options error:(NSError **)error usingBlock:(void (^)(GTCommit *, BOOL*))block;

/** Enumerate commits starting at a given SHA.
 This method will enumerate all of the commits starting at `sha` and continuing to HEAD.
 
 @param sha The SHA at which to start the enumeration.
 @param error The `nil` initialized error object.
 @param block The `(void (^)(GTCommit *, BOOL*))` block used for enumeration.
 */
- (void)enumerateCommitsBeginningAtSha:(NSString *)sha error:(NSError **)error usingBlock:(void (^)(GTCommit *, BOOL*))block;

/** Select all commits beginning at a specified SHA.
 This method will return an array of all the commit starting from a specificed SHA.
 
 @param sha The SHA at which to start the selection.
 @param error The `nil` initialized error object.
 @param block The `(void (^)(GTCommit *, BOOL*))` block used for enumeration.
 */
- (NSArray *)selectCommitsBeginningAtSha:(NSString *)sha error:(NSError **)error block:(BOOL (^)(GTCommit *commit, BOOL *stop))block;

///-------------
/// @name Setup
///-------------

/** Setup the `index` property.
  This method will setup the `index` property or return an error if there is an issue.
 
 @param error The `nil` initialized error object.
 @return `YES` if the index was setup successfully, or `NO` with an initialized `NSError` if there was a problem.
 */
- (BOOL)setupIndexWithError:(NSError **)error;

///------------------
/// @name References
///------------------

/** Get the current HEAD references of the repository.
  This will return the current reference to HEAD in the repository.
 
 @param error The `nil` initialized error object.
 @return A `GTReference` object representing HEAD, or `nil` with an initialized `NSError`.
 */
- (GTReference *)headReferenceWithError:(NSError **)error;

// Convenience methods to return references in this repository (see GTReference.h)
/** Get an array of all the references of a given type.
  This method will return an array of `GTReference` objects of a given type, or nil if there is an error.
 
 @param types A mask of `GTReferenceTypes` used to match against the references.
 @param error The `nil` initialized error object.
 @return An array of `GTReference` objects, or `nil` with an initialized `NSError`.
 */
- (NSArray *)allReferenceNamesOfTypes:(GTReferenceTypes)types error:(NSError **)error;

/** Get an array of all the references.
 This method will return an array of `GTReference` objects, or nil if there is an error.
 
 @param error The `nil` initialized error object.
 @return An array of `GTReference` objects, or `nil` with an initialized `NSError`.
 */
- (NSArray *)allReferenceNamesWithError:(NSError **)error;

// Convenience methods to return branches in the repository
- (NSArray *)allBranchesWithError:(NSError **)error;

// Count all commits in the current branch (HEAD)
//
// error(out) - will be filled if an error occurs
//
// returns number of commits in the current branch or NSNotFound if an error occurred
- (NSInteger)numberOfCommitsInCurrentBranch:(NSError **)error;

// Create a new branch with this name and based off this reference.
//
// name - the name for the new branch
// ref - the reference to create the new branch off
// error(out) - will be filled if an error occurs
//
// returns the new branch or nil if an error occurred.
- (GTBranch *)createBranchNamed:(NSString *)name fromReference:(GTReference *)ref error:(NSError **)error;

// Get the current branch.
//
// error(out) - will be filled if an error occurs
//
// returns the current branch or nil if an error occurred.
- (GTBranch *)currentBranchWithError:(NSError **)error;

// Is this repository empty? This will only be the case in a freshly `git init`'d repository.
//
// returns whether this repository is empty
- (BOOL)isEmpty;

// Find the commits that are on our local branch but not on the remote branch.
//
// error(out) - will be filled if an error occurs
//
// returns the local commits, an empty array if there is no remote branch, or nil if an error occurred
- (NSArray *)localCommitsWithError:(NSError **)error;
	
@end
