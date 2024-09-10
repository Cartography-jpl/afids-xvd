$!****************************************************************************
$!
$! Build proc for MIPL module dspuicomponent_h
$! VPACK Version 1.8, Tuesday, October 29, 1996, 09:44:12
$!
$! Execute by entering:		$ @dspuicomponent_h
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
$ write sys$output "*** module dspuicomponent_h ***"
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
$ write sys$output "Invalid argument given to dspuicomponent_h.com file -- ", primary
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
$   if F$SEARCH("dspuicomponent_h.bld") .eqs. "" then gosub Build_File
$   if (primary .eqs. " ")
$   then
$      @dspuicomponent_h.bld "STD"
$   else
$      @dspuicomponent_h.bld "''primary'" "''secondary'"
$   endif
$ Return
$!#############################################################################
$Repack_File:
$ create dspuicomponent_h.repack
$ DECK/DOLLARS="$ VOKAGLEVE"
$ vpack dspuicomponent_h.com -mixed -
	-s DspUIComponent.h
$ Exit
$ VOKAGLEVE
$ Return
$!#############################################################################
$Source_File:
$ create DspUIComponent.h
$ DECK/DOLLARS="$ VOKAGLEVE"
///////////////////////////////////////////////////////////////////
//  DspUIComponent.h
///////////////////////////////////////////////////////////////////
#ifndef DSPUICOMPONENT_H
#define DSPUICOMPONENT_H
#include "UIComponent.h"
#include "ViewMacros.h"

class DspUIComponent : public UIComponent {

protected:

//	ATTACHING/DETACHING WIDGETS TO/FROM LIST
	Widget * 		_attachmentList; // widgets attached
	int			_numbAttachments;
	virtual void		attachWidgetToForm(
					Widget w,
					unsigned char  	attachTop = XmATTACH_FORM, 
					Widget		wTop = NULL,
					unsigned char  	attachLeft = XmATTACH_FORM, 
					Widget		wLeft = NULL,
					unsigned char  	attachRight = XmATTACH_FORM, 
					Widget		wRight = NULL,
					unsigned char  	attachBottom = XmATTACH_FORM, 
					Widget		wBottom = NULL);
	virtual void		detachAllWidgetsFromForm();
	virtual void		RemoveFormAttachments( Widget w );

//	ATTACHING/DETACHING VIEWS FOR UPDATING AND LATER DELETING	
	int			_numbViews;
	DspUIComponent **	_views;	 // views to be updated	
	virtual void 		attachView(DspUIComponent * view );
	virtual void 		detachView(DspUIComponent * view );
	virtual void		updateViews();
	virtual void		deleteViews(); // called by this class destructor
	
//	ADDING/REMOVING CALLBACKS
	typedef struct CallbackList{ 
				Widget 		widget; 
				char  		callback_name[500];
				XtCallbackProc 	routine;};
	CallbackList **		_callbackList;
	int			_numbCallbacks;
	virtual void		addCallback( Widget w, String callback_name, XtCallbackProc routine);
	virtual void		removeCallback( Widget w, String callback_name, XtCallbackProc routine);
	virtual void		removeAllCallbacks();
	
//	CONSTRUCTOR .. CALLED BY SUBCLASS
	DspUIComponent(char * name );


public:

	~DspUIComponent();

//	UPDATE .. CALLED BY PARENT USING ABOVE VIEW ATTACHMENTS
	virtual void update() {} // pure virtual .. sort of
	
	
//	CLASSNAME
	virtual const char *const className() { return "DspUIComponent" ; } 

};
#endif
$ VOKAGLEVE
$ Return
$!#############################################################################
