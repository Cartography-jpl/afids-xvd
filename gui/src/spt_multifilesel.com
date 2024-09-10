$!****************************************************************************
$!
$! Build proc for MIPL module spt_multifilesel
$! VPACK Version 1.8, Friday, May 19, 1995, 11:46:20
$!
$! Execute by entering:		$ @spt_multifilesel
$!
$! The primary option controls how much is to be built.  It must be in
$! the first parameter.  Only the capitalized letters below are necessary.
$!
$! Primary options are:
$!   COMPile     Compile the program modules
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
$!   IMAKE       Only the IMAKE file (used with the VIMAKE program) is created.
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
$! COMPile,ALL:
$!   DEBug      Compile for debug               (/debug/noopt)
$!   PROfile    Compile for PCA                 (/debug)
$!   LISt       Generate a list file            (/list)
$!   LISTALL    Generate a full list            (/show=all)   (implies LIST)
$! CLEAN:
$!   OBJ        Delete object and list files, and purge executable (default)
$!   SRC        Delete source and make files
$!
$!****************************************************************************
$!
$ write sys$output "*** module spt_multifilesel ***"
$!
$ Create_Source = ""
$ Create_Repack =""
$ Create_Imake = ""
$ Do_Make = ""
$!
$! Parse the primary option, which must be in p1.
$ primary = f$edit(p1,"UPCASE,TRIM")
$ if (primary.eqs."") then primary = " "
$ secondary = f$edit(p2,"UPCASE,TRIM")
$!
$ if primary .eqs. "UNPACK" then gosub Set_Unpack_Options
$ if (f$locate("COMP", primary) .eqs. 0) then gosub Set_Exe_Options
$ if (f$locate("ALL", primary) .eqs. 0) then gosub Set_All_Options
$ if (f$locate("STD", primary) .eqs. 0) then gosub Set_Default_Options
$ if (f$locate("SYS", primary) .eqs. 0) then gosub Set_Sys_Options
$ if primary .eqs. " " then gosub Set_Default_Options
$ if primary .eqs. "REPACK" then Create_Repack = "Y"
$ if primary .eqs. "SORC" .or. primary .eqs. "SOURCE" then Create_Source = "Y"
$ if primary .eqs. "IMAKE" then Create_Imake = "Y"
$ if (f$locate("CLEAN", primary) .eqs. 0) then Do_Make = "Y"
$!
$ if (Create_Source .or. Create_Repack .or. Create_Imake .or. Do_Make) -
        then goto Parameter_Okay
$ write sys$output "Invalid argument given to spt_multifilesel.com file -- ", primary
$ write sys$output "For a list of valid arguments, please see the header of"
$ write sys$output "of this .com file."
$ exit
$!
$Parameter_Okay:
$ if Create_Repack then gosub Repack_File
$ if Create_Source then gosub Source_File
$ if Create_Imake then gosub Imake_File
$ if Do_Make then gosub Run_Make_File
$ exit
$!
$ Set_Unpack_Options:
$   Create_Repack = "Y"
$   Create_Source = "Y"
$   Create_Imake = "Y"
$ Return
$!
$ Set_EXE_Options:
$   Create_Source = "Y"
$   Create_Imake = "Y"
$   Do_Make = "Y"
$ Return
$!
$ Set_Default_Options:
$   Create_Source = "Y"
$   Create_Imake = "Y"
$   Do_Make = "Y"
$ Return
$!
$ Set_All_Options:
$   Create_Source = "Y"
$   Create_Imake = "Y"
$   Do_Make = "Y"
$ Return
$!
$ Set_Sys_Options:
$   Create_Source = "Y"
$   Create_Imake = "Y"
$   Do_Make = "Y"
$ Return
$!
$Run_Make_File:
$   if F$SEARCH("spt_multifilesel.imake") .nes. ""
$   then
$      vimake spt_multifilesel
$      purge spt_multifilesel.bld
$   else
$      if F$SEARCH("spt_multifilesel.bld") .eqs. ""
$      then
$         gosub Imake_File
$         vimake spt_multifilesel
$      else
$      endif
$   endif
$   if (primary .eqs. " ")
$   then
$      @spt_multifilesel.bld "STD"
$   else
$      @spt_multifilesel.bld "''primary'" "''secondary'"
$   endif
$ Return
$!#############################################################################
$Repack_File:
$ create spt_multifilesel.repack
$ DECK/DOLLARS="$ VOKAGLEVE"
$ vpack spt_multifilesel.com -mixed -
	-s SptMultiFileSelView.cc SptParamMultiFileSel.cc -
	-i spt_multifilesel.imake
$ Exit
$ VOKAGLEVE
$ Return
$!#############################################################################
$Source_File:
$ create SptMultiFileSelView.cc
$ DECK/DOLLARS="$ VOKAGLEVE"
////////////////////////////////////////////////////////////////////////
// SptMultiFileSelView - View object for Multiple File Selection dialog
////////////////////////////////////////////////////////////////////////

#include "SptMultiFileSelView.h"
#include "SptParamBase.h"
#include "SptDirectStringValueCmd.h"

#include "MainWindow.h"
#include "FileSelBox.h"

#include "DD.H"

SptMultiFileSelView::SptMultiFileSelView(Widget parent, char *name,
			SptParamBase *pbase, MainWindow *mainWindow)
	: SptDirectBase(name, pbase)
{

   _stringValueCmd = new SptDirectStringValueCmd(name, TRUE, _paramBase);

   // _w is shared with _ci so we don't have an extra, unnecessary, manager
   // widget in here.  When base widgets are shared, though, each class
   // should install their own destroy handler.

   _fileSelBox = new FileSelBox(parent, name, _stringValueCmd, mainWindow);
   _w = _fileSelBox->baseWidget();
   installDestroyHandler();

   // Add our drop functionality to the standard Motif drop.
   _dropSite = _paramBase->createDropSite(_w, TRUE, TRUE);

//!!!!   displayValue();

//!!!!   _paramBase->createTextDragSite(_w, "process-bdrag()");
}

////////////////////////////////////////////////////////////////////////
// See SptParamBase::createDropSite for why this is needed
////////////////////////////////////////////////////////////////////////

void SptMultiFileSelView::manage()
{
   SptDirectBase::manage();
   if (_dropSite)
      _dropSite->activate();
}

void SptMultiFileSelView::unmanage()
{
   SptDirectBase::unmanage();
   if (_dropSite)
      _dropSite->deactivate();
}

////////////////////////////////////////////////////////////////////////

void SptMultiFileSelView::displayValue()
{
#if 0		//!!!!
   char *value;
   StatusType status;

   status = _paramBase->getParamValue(value);
   if (status == StatusNoCurrentValue)
      displayValue(NULL);
   else if (status != StatusSuccess)
      printf("error getting param value, status=%d!!!!\n", status);
   else
      displayValue(value);
   if (value) delete value;
#endif		//!!!!
}

////////////////////////////////////////////////////////////////////////

void SptMultiFileSelView::displayValue(char *value)
{
   if (value)
      _stringValueCmd->setValueDirect(strdup(value));
   else
      _stringValueCmd->setValueDirect(NULL);
}

////////////////////////////////////////////////////////////////////////

void SptMultiFileSelView::paramValueChanged()
{
   displayValue();
}

////////////////////////////////////////////////////////////////////////
// Since deferred execution makes no sense when the FileSelBox has its
// own OK/Apply/Cancel buttons, we disable it here.  Besides, there is
// no easy way to get at the actual CmdInterface from the FileSelBox.
////////////////////////////////////////////////////////////////////////

void SptMultiFileSelView::setDeferredExec(CmdList *)
{
   // Empty
}

$ VOKAGLEVE
$!-----------------------------------------------------------------------------
$ create SptParamMultiFileSel.cc
$ DECK/DOLLARS="$ VOKAGLEVE"
////////////////////////////////////////////////////////////////////////
// SptParamMultiFileSel - Parameter class for a multiple file selection
// dialog (like on xvd).  Filename may come from user or from dataflow.
// When a new filename arrives, the given command is executed if it is
// non-NULL.  This command receives a dynamically-allocated filename string.
////////////////////////////////////////////////////////////////////////

#include "SptParamMultiFileSel.h"
#include "SptShowValueCmd.h"
#include "SptMultiFileSelView.h"

#include <Xm/Form.h>

////////////////////////////////////////////////////////////////////////
// Do the initial layout of the parameter area.  The layout consists of
// a mode menu at the top and the views below.  If the server does not
// exist, the mode menu is suppressed, so the view is stuck on Direct, always.
////////////////////////////////////////////////////////////////////////

void SptParamMultiFileSel::doLayout(Widget parent, char *name)
{
   _w = XtVaCreateWidget(name, xmFormWidgetClass, parent, NULL);
   installDestroyHandler();

   if (serverAvail()) {
      createModeMenu(_w, name);
      XtVaSetValues(_modeMenu->baseWidget(),
		XmNleftAttachment, XmATTACH_FORM,
		XmNtopAttachment, XmATTACH_FORM,
		NULL);
      _modeMenu->manage();
   }

   _viewContainer = XtVaCreateManagedWidget("viewContainer",
		xmFormWidgetClass, _w,
		XmNleftAttachment, XmATTACH_FORM,
		XmNrightAttachment, XmATTACH_FORM,
		XmNbottomAttachment, XmATTACH_FORM,
		NULL);
   if (serverAvail())
      XtVaSetValues(_viewContainer,
		XmNtopAttachment, XmATTACH_WIDGET,
		XmNtopWidget, _modeMenu->baseWidget(),
		NULL);
   else
      XtVaSetValues(_viewContainer,
		XmNtopAttachment, XmATTACH_FORM,
		NULL);

   changeModeView(_mode, _showValue);
}

////////////////////////////////////////////////////////////////////////
// Create the Direct view.
////////////////////////////////////////////////////////////////////////

void SptParamMultiFileSel::createDirectView(Widget parent)
{
   _directView = new SptMultiFileSelView(parent, "FileSelBox", this,
				_mainWindow);
   attachViewToContainer(_directView->baseWidget());
}

////////////////////////////////////////////////////////////////////////
// Create the ShowValue command.  It is disabled if there is no server.
////////////////////////////////////////////////////////////////////////

void SptParamMultiFileSel::createShowValueCmd()
{
   if (serverAvail())
      _showValueCmd = new SptShowValueCmd("Show Value", TRUE, _showValue, this);
   else
      _showValueCmd = NULL;
}

////////////////////////////////////////////////////////////////////////
// Create the mode menu.  It differs from SptInputParamBase only in that
// there is no Default mode available.
////////////////////////////////////////////////////////////////////////

void SptParamMultiFileSel::createModeMenu(Widget parent, char *name)
{
   if (serverAvail())
      _modeMenu = new SptModeMenu(parent, name, this, _mode, _showValueCmd,
		True, False, True, True, False);   // Disable None and Default
   else
      _modeMenu = new SptModeMenu(parent, name, this, _mode, _showValueCmd,
		True, False, False, False, False);
}

$ VOKAGLEVE
$ Return
$!#############################################################################
$Imake_File:
$ create spt_multifilesel.imake
#define SUBROUTINE spt_multifilesel
#ifdef ENABLE_SAGE
#define MODULE_LIST SptMultiFileSelView.cc SptParamMultiFileSel.cc
#else
#define MODULE_LIST
#define CLEAN_OTHER_LIST SptMultiFileSelView.cc SptParamMultiFileSel.cc
#endif

/* This module goes in GUI_SUBLIB rather than SAGE_CLIENT_SUBLIB	*/
/* because it needs the FileSelBox classes which are in GUI_SUBLIB.	*/
#define GUI_SUBLIB

#define USES_C_PLUS_PLUS

#ifdef ENABLE_SAGE
#define LIB_DALI
#define LIB_SAGE_BASE
#define LIB_SAGE_CLIENT
#define LIB_DD_PLUS_PLUS
#define CCC_TEMPLATES
#endif

#define LIB_MOTIFAPP
#define LIB_MOTIF

$ Return
$!#############################################################################
