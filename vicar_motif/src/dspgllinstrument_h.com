$!****************************************************************************
$!
$! Build proc for MIPL module dspgllinstrument_h
$! VPACK Version 1.8, Tuesday, October 29, 1996, 09:44:09
$!
$! Execute by entering:		$ @dspgllinstrument_h
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
$ write sys$output "*** module dspgllinstrument_h ***"
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
$ write sys$output "Invalid argument given to dspgllinstrument_h.com file -- ", primary
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
$   if F$SEARCH("dspgllinstrument_h.bld") .eqs. "" then gosub Build_File
$   if (primary .eqs. " ")
$   then
$      @dspgllinstrument_h.bld "STD"
$   else
$      @dspgllinstrument_h.bld "''primary'" "''secondary'"
$   endif
$ Return
$!#############################################################################
$Repack_File:
$ create dspgllinstrument_h.repack
$ DECK/DOLLARS="$ VOKAGLEVE"
$ vpack dspgllinstrument_h.com -mixed -
	-s DspInstrPws.h DspInstrSsi.h DspInstrNimSci.h DspInstrNimEng.h
$ Exit
$ VOKAGLEVE
$ Return
$!#############################################################################
$Source_File:
$ create DspInstrPws.h
$ DECK/DOLLARS="$ VOKAGLEVE"
///////////////////////////////////////////////////////////////////
//  DspInstrPws.h
///////////////////////////////////////////////////////////////////
#ifndef DSPINSTRUMENTPWS_H
#define DSPINSTRUMENTPWS_H
#include "DspInstrBasic.h"
#include "DspHistPower.h"
#include "DspDataPwsGll.h"
#include <Xm/Xm.h>
#include <iostream.h>

class DspDataPwsGll;

class DspInstrPws : public DspInstrBasic {
   protected:

	virtual void	createImageDisplay( Widget );
	virtual void	createHistDisplay( Widget );
	virtual void	createImageModel();
	virtual void	createPopup();
	virtual void	createTextDisplay();
   	
   	
	
  public:

	DspInstrPws(Widget parent, char * name, DspWindowBasic * window) 
		: DspInstrBasic(parent, name, window, 1) 
		{ createDisplay(); }
	~DspInstrPws(){}
	virtual const char *const className() { return "DspInstrPws"; }
	
};
#endif
$ VOKAGLEVE
$!-----------------------------------------------------------------------------
$ create DspInstrSsi.h
$ DECK/DOLLARS="$ VOKAGLEVE"
///////////////////////////////////////////////////////////////////
//  DspInstrSsi.h
///////////////////////////////////////////////////////////////////
#ifndef DSPINSTRSSI_H
#define DSPINSTRSSI_H
#include "DspInstrBasic.h"
#include <iostream.h>

class DspInstrSsi : public DspInstrBasic {

   protected:

	virtual void	createImageDisplay( Widget );
	virtual void	createHistDisplay( Widget );
	virtual void	createImageModel();
	virtual void	createPopup();
	virtual void	createTextDisplay();
   	
   

  public:

	DspInstrSsi(Widget parent, char * name, 
			DspWindowBasic * window);
	virtual const char *const className() { return "DspInstrSsi"; }
};

#endif
$ VOKAGLEVE
$!-----------------------------------------------------------------------------
$ create DspInstrNimSci.h
$ DECK/DOLLARS="$ VOKAGLEVE"
///////////////////////////////////////////////////////////////////
//  DspInstrNimSci.h
///////////////////////////////////////////////////////////////////
#ifndef DSPINSTRNIMSCI_H
#define DSPINSTRNIMSCI_H
#include "DspInstrBasic.h"
#include "DspHistMean.h"
#include "DspHistStdDev.h"
#include "DspDataNimBasicGll.h"
#include "ImageDisplayView.h"

enum SciModelType {primary=0,D2=1, Wh=2, Av=3} ;

//	EXTERNAL	
	void    setZoomParams2Off( int * zoomX, int *zoomY, int *panY, int * panX ,int*oddEvenFlag);	 
	int     setPseudoModeOff();
	int     setPseudoModeOn();
	void	fillImageStretchLutWithRaw(int * lut1);


class DspInstrNimSci : public DspInstrBasic {
   protected:

	ImageDisplayView *	_imageViewNimsAv;
	ImageDisplayView *	_imageViewNimsWh;	
	ImageDisplayView *	_imageViewNims2D;	
	//ImageDisplayView *	_imageView;	
		
	virtual void		createHistDisplay( Widget );
	virtual void		createImageDisplay( Widget );
	virtual void		createImageModel();
	virtual void		createPopup();
	virtual void		createTextDisplay();
   	   	
		
  public:

	DspInstrNimSci(Widget parent, char * name, DspWindowBasic * window) ;
	~DspInstrNimSci(){}
	virtual const char *const className() { return "DspInstrNimSci"; }
	
};
#endif
$ VOKAGLEVE
$!-----------------------------------------------------------------------------
$ create DspInstrNimEng.h
$ DECK/DOLLARS="$ VOKAGLEVE"
///////////////////////////////////////////////////////////////////
//  DspInstrNimEng.h
///////////////////////////////////////////////////////////////////
#ifndef DSPINSTRNIMSENG_H
#define DSPINSTRNIMSENG_H
#include "DspInstrBasic.h"
#include "DspHistMean.h"
#include "DspHistStdDev.h"
#include "DspDataNimBasicGll.h"
#include "DspDataNimEngRB.h"
#include "ImageDisplayView.h"

enum EngModelType {RB, BW};

class DspInstrNimEng : public DspInstrBasic {

   protected:

	ImageData *             _dataModelBW;
	//ImageDisplayView *	_imageView;
	ImageDisplayView *	_imageViewNimsBW;
	
	virtual void		createHistDisplay( Widget );
	virtual void		createImageDisplay( Widget );
	virtual void		createImageModel();
	virtual void		createPopup();
	virtual void 		updateImageZoom() {}
	virtual void		createTextDisplay();
   	
  public:

	DspInstrNimEng(Widget parent, char * name, DspWindowBasic * window);
	virtual const char *const className() { return "DspInstrNimEng"; }
};
#endif
$ VOKAGLEVE
$ Return
$!#############################################################################
