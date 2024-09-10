$!****************************************************************************
$!
$! Build proc for MIPL module dspcursordump_h
$! VPACK Version 1.8, Monday, April 01, 1996, 21:52:49
$!
$! Execute by entering:		$ @dspcursordump_h
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
$ write sys$output "*** module dspcursordump_h ***"
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
$ write sys$output "Invalid argument given to dspcursordump_h.com file -- ", primary
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
$   if F$SEARCH("dspcursordump_h.bld") .eqs. "" then gosub Build_File
$   if (primary .eqs. " ")
$   then
$      @dspcursordump_h.bld "STD"
$   else
$      @dspcursordump_h.bld "''primary'" "''secondary'"
$   endif
$ Return
$!#############################################################################
$Repack_File:
$ create dspcursordump_h.repack
$ DECK/DOLLARS="$ VOKAGLEVE"
$ vpack dspcursordump_h.com -mixed -
	-s DspCursDump.h DspCursDumpCmd.h
$ Exit
$ VOKAGLEVE
$ Return
$!#############################################################################
$Source_File:
$ create DspCursDump.h
$ DECK/DOLLARS="$ VOKAGLEVE"
////////////////////////////////////////////////////////////////
// DspCursDump.h
///////////////////////////////////////////////////////////////
#ifndef DSPCURSDUMP_H
#define DSPCURSDUMP_H
#include "XvicImage.h"
#include "ImageData.h"
#include "ImageDisplayView.h"
#include "zvproto.h"
// #include "DspFile.h"
#include "DspDefs.h"
#include <Xm/Xm.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <iostream.h>

class CursorModel;
class ImageData;
class DspFile;


class DspCursDump  {

  protected:
  
  	static XtResource 	_resources[];  	
  	int			_numbRows;
  	int			_numbColumns;  	
  	int			_skipRows; // height of pixels to skip
  	int			_skipColumns; // width of pixels to skip
  	ImageData *		_imageData;
  	ImageDisplayView *	_imageView;
  	unsigned char 		_bitFlags;
  	DspFile *		_file;

  	short 			_columnsResource;  // width of text widget
  	int			_midRow;
  	int			_midColumn;
  	int			_titleLines;

	unsigned char  		_pixelBuffer[132];    // buffer for holding pixel value
	char 			_blankString[132];
	char *			_logText;
	char 			_fileName[MAX_NAMESTRINGLENGTH];
	int			_pixelSize;
	int			_lengthOfPixelText;
	int			_numbLines;
	int			_numbSamples;
	Boolean			_redDnEnable,
				_greenDnEnable,
				_blueDnEnable,
				_bwDnEnable;
	
	
//	FUNCTION TO DETERMINE NUMBER OF DIGITS FOR THE MAX PIXEL VALUE OF THIS SIZE PIXEL                                              
        virtual int		maxSizeOfPixelText(int pixelSize);	

//	UPDATE DISPLAYS ON ALL CURSOR SUBVIEWS
	static void     	writeCursorValuesCallback( Widget,
                                                XtPointer clientData,
                                                XtPointer callData);
	virtual void  		writeCursorValues ( int cursorX, int cursorY) ;

//	LOCAL FUNCTION FOR GETTING A DN VALUE AND ITS STRING EQUIV.   
	virtual void 		getValueString( ColorType color,  
				int x, int y, char * newValueString );
				
	virtual void 		updateValue(char * buf, int currentColumn, int lastColumn, char * outBuf);			
		
	virtual void 		getColumnString( int x, char * newValueString, int lastColumn );
	virtual void		printDumpHeader( char * colorText, 
						int lengthOfPixelText,
						char * outBuf,
						int x);
	virtual void		printLineNumber( int lineNumb, char * outBuf);
				
 public:
 
//	CONSTRUCTOR/DESTRUCTOR
	DspCursDump( 		Widget			parent,
				char *			name,
				ImageData * 		imageData,
				ImageDisplayView *	imageView,
				unsigned char 		bitFlags,
				int			pixelSize) ;
			
	~DspCursDump();

	
	virtual void dump(char *);
	virtual const char *const className () { return "DspCursorDump"; }
	
};
#endif
$ VOKAGLEVE
$!-----------------------------------------------------------------------------
$ create DspCursDumpCmd.h
$ DECK/DOLLARS="$ VOKAGLEVE"
/////////////////////////////////////////////////////////////
// DspCursDumpCmd.h: Include file to handle the CursorPos command
//                button from the Browse app.
/////////////////////////////////////////////////////////////
#ifndef DSPCURSDMPCMD_H
#define DSPCURSDMPCMD_H
#include "NoUndoCmd.h"
#include <Xm/Xm.h>
#include "DspCursDump.h"
#include "ImageData.h"
#include "ImageDisplayView.h"

class MainWindow;
class ImageData;
class ImageDisplayView;

class DspCursDumpCmd : public NoUndoCmd {

  private:
     
     DspCursDump *	_cursDump;
     
  protected:

    int                 _created;
    char *		_fileName;
 
    
    virtual void 	doit();
    
  public:

    DspCursDumpCmd ( Widget parent, char*, char*, int,  ImageData*, 
    				ImageDisplayView*, unsigned char, int );
    static  void 	setNameLink(XtPointer obj, char * fileName);
    virtual void	setName(char * fileName);
    virtual const char *const className () { return "DspCursDumpCmd"; }
};
#endif

$ VOKAGLEVE
$ Return
$!#############################################################################
