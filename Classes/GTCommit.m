//
//  GTCommit.m
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

#import "GTCommit.h"
#import "GTSignature.h"
#import "GTTree.h"
#import "NSError+Git.h"
#import "GTRepository.h"
#import "NSString+Git.h"


@implementation GTCommit

- (NSString *)description {
	return [NSString stringWithFormat:@"<%@: %p> author: %@, shortMessage: %@, message: %@", NSStringFromClass([self class]), self, self.author, [self shortMessage], [self message]];
}


/**
 @returns git_commit reference for the object.
 */
- (git_commit *)commit {
	
	return (git_commit *)self.obj;
}

#pragma mark -
#pragma mark API

@synthesize author;
@synthesize committer;
@synthesize parents;

/**
 @param theRepo The GTRepository object to create a commit in.
 @param refName The reference name as an NSString.
 @param authorSig The GTSignature object for the author.
 @param committerSig The GTSignature object for the committer.
 @param newMessage The message for the commit as an NSString.
 @param theTree The GTTree object to commit the changes to.
 @param theParents An array of parent GTCommit objects.
 @param error A nil initialized NSError object to return if there is a failure.
 @returns a newly initialized GTCommit object.
 */
+ (GTCommit *)commitInRepository:(GTRepository *)theRepo
			updateRefNamed:(NSString *)refName
					author:(GTSignature *)authorSig
				 committer:(GTSignature *)committerSig
				   message:(NSString *)newMessage
					  tree:(GTTree *)theTree 
				   parents:(NSArray *)theParents 
					 error:(NSError **)error {
	
	NSString *sha = [GTCommit shaByCreatingCommitInRepository:theRepo updateRefNamed:refName author:authorSig committer:committerSig message:newMessage tree:theTree parents:theParents error:error];
	return sha ? (GTCommit *)[theRepo lookupObjectBySha:sha objectType:GTObjectTypeCommit error:error] : nil;
}

/**
 @param theRepo The GTRepository object to create a commit in.
 @param refName The reference name as an NSString.
 @param authorSig The GTSignature object for the author.
 @param committerSig The GTSignature object for the committer.
 @param newMessage The message for the commit as an NSString.
 @param theTree The GTTree object to commit the changes to.
 @param theParents An array of parent GTCommit objects.
 @param error A nil initialized NSError object to return if there is a failure.
 @returns a newly initialized NSString of the commit sha.
 */
+ (NSString *)shaByCreatingCommitInRepository:(GTRepository *)theRepo
				  updateRefNamed:(NSString *)refName
						  author:(GTSignature *)authorSig
					   committer:(GTSignature *)committerSig
						 message:(NSString *)newMessage
							tree:(GTTree *)theTree 
						 parents:(NSArray *)theParents 
						   error:(NSError **)error {
	
	NSUInteger count = theParents ? theParents.count : 0;
	git_commit const *parentCommits[count];
	for (NSUInteger i = 0; i < count; i++){
		parentCommits[i] = ((GTCommit *)[theParents objectAtIndex:i]).commit;
	}
	
	git_oid oid;
	int gitError = git_commit_create_o(
									   &oid, 
									   theRepo.repo, 
									   refName ? [refName UTF8String] : NULL, 
									   authorSig.sig, 
									   committerSig.sig, 
									   newMessage ? [newMessage UTF8String] : "", 
									   theTree.tree, 
									   (int)count, 
									   parentCommits);
	if(gitError != GIT_SUCCESS) {
		if(error != NULL)
			*error = [NSError gitErrorFor:gitError withDescription:@"Failed to create commit in repository"];
		return nil;
	}
	return [NSString git_stringWithOid:&oid];
}

/**
 Get the full message out of the commit.
 @returns An NSString object of the entire message.
 */
- (NSString *)message {
	
	const char *s = git_commit_message(self.commit);
	return [NSString stringWithUTF8String:s];
}

/**
 Get the short message out of the commit.
 @returns An NSString object of the short message.
 */
- (NSString *)shortMessage {
	
	const char *s = git_commit_message_short(self.commit);
	return [NSString stringWithUTF8String:s];
}

/**
 Get the details out of the commit message.
 @returns An NSString object of the message details.
 */
- (NSString *)messageDetails {
	
	NSArray *lines = [self.message componentsSeparatedByString:@"\n"];
	if(lines.count < 2) return @"";
	
	NSMutableString *result = [NSMutableString string];
	NSString *secondLine = [(NSString *)[lines objectAtIndex:1] stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]];
	if(secondLine.length != 0) {
		[result appendFormat:@"%@\n", secondLine];
	}
	for(int i=2; i < lines.count; i++) {
		[result appendFormat:@"%@\n", [lines objectAtIndex:i]];
	}
	
	return result;
}

/**
 Get the commit date.
 @returns An NSDate object initialized with the commit date.
 */
- (NSDate *)commitDate {
	
	time_t t = git_commit_time(self.commit);
	return [NSDate dateWithTimeIntervalSince1970:t];
}

/**
 Get the author of the commit.
 @return The GTSignature object for the author.
 */
- (GTSignature *)author {
	
	if(author == nil) {
		author = [[GTSignature signatureWithSig:(git_signature *)git_commit_author(self.commit)] retain];
	}
	return author;
}

/**
 Get the committer of the commit.
 @return The GTSignature object for the committer.
 */
- (GTSignature *)committer {
	
	if(committer == nil) {
		committer = [[GTSignature signatureWithSig:(git_signature *)git_commit_committer(self.commit)] retain];
	}
	return committer;
}

/**
 Get the GTTree reference that the commit belongs to.
 @returns The GTTree reference that the commit belongs to.
 */
- (GTTree *)tree {
	
	git_tree *t;
	
	int gitError = git_commit_tree(&t, self.commit);
	if(gitError != GIT_SUCCESS) {
		// todo: might want to return this error (and change method signature)
		GTLog("Failed to get tree with error code: %d", gitError);
		return nil;
	}
	return (GTTree *)[GTObject objectWithObj:(git_object *)t inRepository:self.repository];
}

/**
 Get the array of parent commits this commit belongs to.
 @returns An NSArray initialized with the GTCommit objects that are parent(s) to this commit.
 */
- (NSArray *)parents {
	
	if(parents == nil) {
		NSMutableArray *rents = [NSMutableArray array];
		
		// todo: do we care if a call to git_commit_parent fails?
		git_commit *parent;
		for(int i=0; git_commit_parent(&parent, self.commit, i) == GIT_SUCCESS; i++) {
            [rents addObject:(GTCommit *)[GTObject objectWithObj:(git_object *)parent inRepository:self.repository]];
		}
		
		parents = [[NSArray arrayWithArray:rents] retain];
	}
	return parents;
}

- (void)dealloc
{
    [parents release];
    [committer release];
    [author release];
    [super dealloc];
}

@end
