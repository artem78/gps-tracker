/*
 ============================================================================
 Name		: AsyncFileMan.h
 Author	  : artem78
 Version	 : 1.0
 Copyright   : 
 Description : CAsyncFileMan declaration
 ============================================================================
 */

#ifndef ASYNCFILEMAN_H
#define ASYNCFILEMAN_H

// Includes

#include <e32base.h>	// For CActive, link against: euser.lib
//#include <e32std.h>
#include <f32file.h>

// Forward declarations

class MAsyncFileManObserver;

// Classes

class CFileManExtended : public CFileMan
	{
public:
	inline TInt ProcessedFiles() { return iNumberOfFilesProcessed; };
	//inline TInt TotalFiles() { return iDirList->Count(); };
	inline TInt TotalFiles() { return iDirList != NULL ? iDirList->Count() : 0; };
	};


class CAsyncFileMan : public CActive, public MFileManObserver
// FixMe: Memory leak when cancel delete operation
	{
public:
	// Cancel and destroy
	~CAsyncFileMan();

	// Two-phased constructor.
	static CAsyncFileMan* NewL(RFs &aFs, MAsyncFileManObserver* aObserver);

	// Two-phased constructor.
	static CAsyncFileMan* NewLC(RFs &aFs, MAsyncFileManObserver* aObserver);

public:
	// New functions
	// Function for making the initial request
//	void StartL();

private:
	// C++ constructor
	CAsyncFileMan(MAsyncFileManObserver* aObserver);

	// Second-phase constructor
	void ConstructL(RFs &aFs);

private:
	// From CActive
	// Handle completion
	void RunL();

	// How to cancel me
	void DoCancel();

	// Override to handle leaves from RunL(). Default implementation causes
	// the active scheduler to panic.
	TInt RunError(TInt aError);
	
// from MFileManObserver
public:
	MFileManObserver::TControl NotifyFileManStarted();
	MFileManObserver::TControl NotifyFileManOperation();
	MFileManObserver::TControl NotifyFileManEnded();

private:
	/*CFileMan**/ CFileManExtended* iFileMan;
	MAsyncFileManObserver* iObserver;
	TBool iCancelOperation; // Used for cancel current operation in file manager
	TInt iProcessedCount/*, iTotalCount*/; // Amount of processed and total files
	
//	TInt GetAmountOfFiles(const TDesC &aPath/*, TBool anIsRecursive=EFalse*/);
	
public:
	// ToDo: Add other operations (rename, copy, etc...)
	TInt Delete(const TDesC& aName, TUint aSwitch=0);
	
	inline TInt ProcessedFiles() { return iFileMan->ProcessedFiles(); };
	//inline TInt TotalFiles() { return iTotalCount; };
	inline TInt TotalFiles() { return iFileMan->TotalFiles(); };
	
	};


class MAsyncFileManObserver
	{
public:
	virtual MFileManObserver::TControl OnFileManStarted();
	virtual MFileManObserver::TControl OnFileManOperation();
	virtual MFileManObserver::TControl OnFileManEnded();
	virtual void OnFileManFinished(TInt aStatus);
	};

#endif // ASYNCFILEMAN_H
