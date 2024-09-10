$!****************************************************************************
$!
$! Build proc for MIPL module dspfile_h
$! VPACK Version 1.8, Monday, April 01, 1996, 21:52:51
$!
$! Execute by entering:		$ @dspfile_h
$!
$! The primary option controls how much is to be built.  It must be in
$! the first parameter.  Only the capitalized letters below are necessary.
$!
$! Primary options are:
$!   ALL         Build a private version, and unpack the PDF and DOC files.
$!   STD         Build a private version, and unpack the PDF file(s).
$!   SYStem      Build the system version with the CLEAN option, and
$!               unpack the PDF and DOC files.
$!   CLEAN       Clean (delete/purge) parts of the code, see secondary options
$!   UNPACK      All files are created.
$!   REPACK      Only the repack file is created.
$!   SOURCE      Only the source files are created.
$!   SORC        Only the source files are created.
$!               (This parameter is left in for backward compatibility).
$!
$!   The default is to use the STD parameter if none is provided.
$!
$!****************************************************************************
$!
$! The secondary options modify how the primary option is performed.
$! Note that secondary options apply to particular primary options,
$! listed below.  If more than one secondary is desired, separate them by
$! commas so the entire list is in a single parameter.
$!
$! Secondary options are:
$! CLEAN:
$!   OBJ        Delete object and list files, and purge executable (default)
$!   SRC        Delete source and make files
$!
$!****************************************************************************
$!
$ write sys$output "*** module dspfile_h ***"
$!
$ Create_Source = ""
$ Create_Repack =""
$!
$! Parse the primary option, which must be in p1.
$ primary = f$edit(p1,"UPCASE,TRIM")
$ if (primary.eqs."") then primary = " "
$ secondary = f$edit(p2,"UPCASE,TRIM")
$!
$ if primary .eqs. "UNPACK" then gosub Set_Unpack_Options
$ if (f$locate("ALL", primary) .eqs. 0) then gosub Set_All_Options
$ if (f$locate("STD", primary) .eqs. 0) then gosub Set_Default_Options
$ if (f$locate("SYS", primary) .eqs. 0) then gosub Set_Sys_Options
$ if primary .eqs. " " then gosub Set_Default_Options
$ if primary .eqs. "REPACK" then Create_Repack = "Y"
$ if primary .eqs. "SORC" .or. primary .eqs. "SOURCE" then Create_Source = "Y"
$!
$ if (Create_Source .or. Create_Repack) -
        then goto Parameter_Okay
$ write sys$output "Invalid argument given to dspfile_h.com file -- ", primary
$ write sys$output "For a list of valid arguments, please see the header of"
$ write sys$output "of this .com file."
$ exit
$!
$Parameter_Okay:
$ if Create_Repack then gosub Repack_File
$ if Create_Source then gosub Source_File
$ exit
$!
$ Set_Unpack_Options:
$   Create_Repack = "Y"
$   Create_Source = "Y"
$ Return
$!
$ Set_Default_Options:
$   Create_Source = "Y"
$   Create_Build = "Y"
$   Do_Make = "Y"
$ Return
$!
$ Set_All_Options:
$   Create_Source = "Y"
$   Create_Build = "Y"
$   Do_Make = "Y"
$ Return
$!
$ Set_Sys_Options:
$   Create_Source = "Y"
$   Create_Build = "Y"
$   Do_Make = "Y"
$ Return
$!
$Run_Make_File:
$   if F$SEARCH("dspfile_h.bld") .eqs. "" then gosub Build_File
$   if (primary .eqs. " ")
$   then
$      @dspfile_h.bld "STD"
$   else
$      @dspfile_h.bld "''primary'" "''secondary'"
$   endif
$ Return
$!#############################################################################
$Repack_File:
$ create dspfile_h.repack
$ DECK/DOLLARS="$ VOKAGLEVE"
$ vpack dspfile_h.com -mixed -
	-s DspFile.h DspFileNameCmd.h
$ Exit
$ VOKAGLEVE
$ Return
$!#############################################################################
$Source_File:
$ create DspFile.h
$ DECK/DOLLARS="$ VOKAGLEVE"
////////////////////////////////////////////////////////////////
// DspFile.h
//	needs a lot more work to be usefull .. quick and dirty
///////////////////////////////////////////////////////////////
#ifndef DSPFILE_H
#define DSPFILE_H
#include "DspDefs.h"
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <Xm/Xm.h>
#include <assert.h>

class DspFile {

    protected:
    
    	int		_bufferSize;
    	char *  	_buffer;
    	FILE *		_file;  
    	char   		_fileName[MAX_NAMESTRINGLENGTH];
    	DSPSTATUSTYPE	_error;  // local error/status 
    	int		_errno;
    	char 		_flags[5];
    	Boolean		_open;
    	Boolean		_eof;
    	int		_bytesReadInLastRead;
    	int		_bytesWrittenInLastWrite;
    	
    	virtual void		deleteBuffer();
        
     	
    public:
    
        DspFile();
        ~DspFile() { deleteBuffer();}
    
        virtual DSPSTATUSTYPE	  createNewBuffer(int bufferSize);
    	virtual DSPSTATUSTYPE 	  readAllBytes(int nbytes, char * buffer);
        virtual DSPSTATUSTYPE	  readOnce(int nbytes, char * buffer) ;
	virtual DSPSTATUSTYPE 	  writeAllBytes(int nbytes, char * buffer);
	virtual DSPSTATUSTYPE	  writeOnce(int nbytes, char * buffer) ;
	virtual void 		  printErrorToScreen(char * msg) ;
	virtual void		  setFlags(Boolean read, Boolean write,
					 Boolean append, Boolean truncate);
	
	virtual DSPSTATUSTYPE	open( char * fileName, int bufferSize = 1024,
					 Boolean read = True, Boolean write = True,
					 Boolean append = True, Boolean truncate = True);
	virtual void		close();
	inline DSPSTATUSTYPE 	isError() 	{ return _error; }
	inline char *		getFileName() 	{ return _fileName;}
	inline  Boolean		isOpened()	{ return _open; }
	virtual Boolean		isEOF();	
	inline  char *		getBufferAddr()	{ return _buffer; }
	inline  int  		getBufferSize() { return _bufferSize;}
	inline  int		getNumbBytesRead() { return _bytesReadInLastRead; }
	inline  int		getNumbBytesWritten() {return _bytesWrittenInLastWrite; }
	//virtual void		  getBehavior() ;
	//virtual void		  setBehavior() ;
};
#endif
$ VOKAGLEVE
$!-----------------------------------------------------------------------------
$ create DspFileNameCmd.h
$ DECK/DOLLARS="$ VOKAGLEVE"
/////////////////////////////////////////////////////////
// DspFileNameCmd: A Command class that loads a file name and 
//	copies it to function pointer.  The Command value
// 	is a dynamically allocated single string.
/////////////////////////////////////////////////////////
#ifndef DSPFILENAMECMD_H
#define DSPFILENAMECMD_H
#include "NoUndoCmd.h"
#include <Xm/Xm.h>


class DspFileNameCmd : public NoUndoCmd {

 protected:

   XtPointer		_obj;
   void 		(*_fp)(XtPointer, char *);
    
 public:

   DspFileNameCmd(char *name, int active, XtPointer obj, void (*fp)(XtPointer, char *));

   virtual void doit();  
    
   virtual void freeValue(CmdValue value) { if (value) delete []value; }

   virtual const char *const className () { return "DspFileNameCmd"; }
};
#endif
$ VOKAGLEVE
$ Return
$!#############################################################################
