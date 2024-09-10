$!****************************************************************************
$!
$! Build proc for MIPL module dspinstrument_h
$! VPACK Version 1.8, Tuesday, October 29, 1996, 09:44:11
$!
$! Execute by entering:		$ @dspinstrument_h
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
$ write sys$output "*** module dspinstrument_h ***"
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
$ write sys$output "Invalid argument given to dspinstrument_h.com file -- ", primary
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
$   if F$SEARCH("dspinstrument_h.bld") .eqs. "" then gosub Build_File
$   if (primary .eqs. " ")
$   then
$      @dspinstrument_h.bld "STD"
$   else
$      @dspinstrument_h.bld "''primary'" "''secondary'"
$   endif
$ Return
$!#############################################################################
$Repack_File:
$ create dspinstrument_h.repack
$ DECK/DOLLARS="$ VOKAGLEVE"
$ vpack dspinstrument_h.com -mixed -
	-s DspInstrBasic.h
$ Exit
$ VOKAGLEVE
$ Return
$!#############################################################################
$Source_File:
$ create DspInstrBasic.h
$ DECK/DOLLARS="$ VOKAGLEVE"
///////////////////////////////////////////////////////////////////
//  DspInstrBasic.h
///////////////////////////////////////////////////////////////////
#ifndef DSPINSTRBASIC_H
#define DSPINSTRBASIC_H
#include "DspDefs.h"
#include "DspUIComponent.h"
#include "DspImageBasic.h"
#include "DspImageEmpty.h"
#include "DspHistBasic.h"
#include "DspInfoView.h"
#include "DspDisplayPolicy.h"
#include "DspCursCmd.h"
#include "DspCursDumpCmd.h"
// #include "DspStretchCmd.h"
//  #include "DspStretchCmdIF.h"
// #include "DspStretchCmdDialog.h"
//#include "DspPostDialogCmd.h"
#include "DspUtilMisc.h"
#include "Lut.h"
#include "Histogram.h"
#include "ImageData.h"
#include "ImageDisplayView.h"
#include "MenuCmdList.h"
#include "PrintWidgetTreeCmd.h"
#include "Histogram.h"
#include "PopupMenu.h"
#include "CollectHist.h"
#include <Xm/Xm.h>
#include <iostream.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
//  #include "PostStretchCmd.h"

class DspFileNameCmd;
class PostSingleFileDialogCmd;
class DspWindowBasic;

class DspInstrBasic : public DspUIComponent {
 
   protected:
   
   	/*static XtResource 	_resources[];  */	
	static String 		_defaults[]; // resource defaults.
	DspWindowBasic *	_window; 
   	ImageData **		_dataModel;
   	DspImageBasic *  	_imageDisplayer;
   	DspHistBasic *		_histDisplayer1;
   	DspHistBasic *		_histDisplayer2;
   	Histogram *		_rawHistModel;
   	Histogram *		_stretchHistModel;
   	DspInfoView *		_titleBar;
	DspInfoView *		_imageText;
	DspInfoView *		_stretchText;
	DspInfoView *		_picnoErt;
	char 			_filename[MAX_NAMESTRINGLENGTH];
	Widget			_parent;
	Widget			_imageArea;
   	Widget			_histArea;	
  	DspDisplayPolicy *	_displayPolicy;
  	int			_numbModels;
  	MenuCmdList *		_cmdList;
//	DspStretchCmd *		_stretchCmd;
//	CmdInterface * 		_stretchCmdInterface;
//	DspPostDialogCmd *	_postStretchCmd;
	PopupMenu *		_popup;
	Histogram *		_emptyHistogram; // blank unmanaged histogram used to inherit 
 						 // stretchcmd, etc from gui code. 
//	DspStretchCmdDialog *	_stretchDialog;
	ImageDisplayView *	_imageView;
	
	MenuCmdList *		_dumpMenuCmdList;
	DspCursCmd *		_cursorDnCmd;
	DspCursCmd *		_cursorMatrixCmd; 	
	DspCursDumpCmd *	_cursDumpCmd;
	DspFileNameCmd *	_fileNameCmd;
	Cmd *			_printWidgetTreeCmd;
	PostSingleFileDialogCmd * _postFileDialogCmd;	
	int			_pixelSize; // used in popup
   				
	virtual void		createDisplay();
   	virtual void		createImageModel()=0;
   	virtual void 		createImageDisplay(Widget)=0;
   	virtual void		createHistDisplay(Widget)=0;
   	virtual void		createTextDisplay()=0;
   	virtual void		setAttachments();
   	virtual DspDisplayPolicy * createDisplayPolicy();
   	virtual void		updateImageModel();
   	virtual void		createPopup()=0; // calls addpopup
   	virtual void		addPopup( // called by createPopup in subclass
   					ImageDisplayView * imageView,
   					ImageData * dataModel,
   					DspImageBasic * imageDisplayer,
   					int	i,
   					Histogram * rawHistModel,
   					Histogram * stretchHistModel,
					Boolean posSen=TRUE, 
					Boolean dnSen=TRUE,
					Boolean dumpSen=TRUE,
					Boolean strSen=TRUE, 
					Boolean treeSen=TRUE);
	virtual void		createPulloutDumpMenu(MenuCmdList * menuCmdList, 
								Boolean dumpSen,
								ImageData * dataModel,
								ImageDisplayView * imageView);
   public:  
   				DspInstrBasic(Widget, char *, DspWindowBasic * ,int);
				~DspInstrBasic();
	virtual void		manage();
	virtual void 		unmanage();  	  	  	
   	virtual void		update();
	virtual const char *const className() { return "DspInstrBasic"; }
	
};
#endif
$ VOKAGLEVE
$ Return
$!#############################################################################
