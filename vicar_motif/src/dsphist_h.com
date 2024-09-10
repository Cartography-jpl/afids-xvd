$!****************************************************************************
$!
$! Build proc for MIPL module dsphist_h
$! VPACK Version 1.8, Tuesday, October 29, 1996, 09:44:10
$!
$! Execute by entering:		$ @dsphist_h
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
$ write sys$output "*** module dsphist_h ***"
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
$ write sys$output "Invalid argument given to dsphist_h.com file -- ", primary
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
$   if F$SEARCH("dsphist_h.bld") .eqs. "" then gosub Build_File
$   if (primary .eqs. " ")
$   then
$      @dsphist_h.bld "STD"
$   else
$      @dsphist_h.bld "''primary'" "''secondary'"
$   endif
$ Return
$!#############################################################################
$Repack_File:
$ create dsphist_h.repack
$ DECK/DOLLARS="$ VOKAGLEVE"
$ vpack dsphist_h.com -mixed -
	-s DspHistBasic.h DspHistInput.h DspHistOutput.h DspHistPower.h -
	   DspHistMean.h DspHistStdDev.h DspHistInputPlain.h DspHistPlain.h
$ Exit
$ VOKAGLEVE
$ Return
$!#############################################################################
$Source_File:
$ create DspHistBasic.h
$ DECK/DOLLARS="$ VOKAGLEVE"
///////////////////////////////////////////////////////////////////
//  DspHistBasic.h
///////////////////////////////////////////////////////////////////
#ifndef DSPHISTBASIC_H
#define DSPHISTBASIC_H
#include "DspUIComponent.h"
#include "BasicImageView.h"
#include "HistGraphView.h"
#include "HistDefs.h"
#include "Histogram.h"
#include "ImageData.h"
#include "DspAxisHorizView.h"
#include "DspAxisVertView.h"
#include "DspBarHorizView.h"
#include "DspBarVertView.h"
#include "DspDisplayPolicy.h"
#include "CollectHist.h"
#include "WedgeView.h"
#include "Application.h"
#include <Xm/Form.h>
#include <Xm/Frame.h>
#include <Xm/Label.h>
#include <Xm/RowColumn.h>
#include <iostream.h>


	Cursor			createBlankCursor ( Widget iw);

class DspHistBasic : public DspUIComponent {

protected:

	 static String		_defaults[];
		
	 char *			_titleLabel;	
	 Histogram *		_histModel;
	 ImageData *		_dataModel;
	 WedgeView *		_wedgeView;
	 HistGraphView	*	_histView;
	 DspAxisVertView *	_axisWest;
	 DspAxisHorizView *	_axisSouth;
	 DspAxisVertView *	_axisEast;
	 DspAxisHorizView *	_axisNorth;
	 DspBarHorizView *	_barNorth;
	 DspDisplayPolicy *	_displayPolicy;
	 Widget			_titleString;
	 Widget			_histForm;
	 Widget			_wedgeFrame;
	 Widget			_histFrame;
	 
	  
	 virtual void		createAllViews();	 	
	 virtual void		createHistView(Widget parent);
	 virtual int *		getHistArray()=0;

public:
	DspHistBasic(	Widget 		parent, 
				char * 		name, 
				ImageData * dataModel, 
				char *		titleLabel,
				DspDisplayPolicy * displayPolicy );
	~DspHistBasic();
	virtual void 		update();
	inline Histogram *	getHistogramModel(){ return ( _histModel ); }
	
};
#endif
$ VOKAGLEVE
$!-----------------------------------------------------------------------------
$ create DspHistInput.h
$ DECK/DOLLARS="$ VOKAGLEVE"
///////////////////////////////////////////////////////////////////
//  DspHistInput.h
///////////////////////////////////////////////////////////////////
#ifndef DSPHISTINPUT_H
#define DSPHISTINPUT_H
#include "DspHistBasic.h"
#include "BasicImageView.h"
#include "ImageData.h"
#include <Xm/Xm.h>

class DspHistInput : public DspHistBasic {

protected:
		
	inline int *		getHistArray() { return getHistInputArray(); }

public:

	DspHistInput(Widget parent, 
				char * 		name, 
				ImageData * dataModel, 
				char * 		titleLabel,
				DspDisplayPolicy *  displayPolicy )
			: DspHistBasic(parent,name,dataModel,titleLabel, displayPolicy ) 
				{ createAllViews(); }


};
#endif
$ VOKAGLEVE
$!-----------------------------------------------------------------------------
$ create DspHistOutput.h
$ DECK/DOLLARS="$ VOKAGLEVE"
///////////////////////////////////////////////////////////////////
//  DspHistOutput.h
///////////////////////////////////////////////////////////////////
#ifndef DSPHISTOUTPUT_H
#define DSPHISTOUTPUT_H
#include "DspHistBasic.h"
#include "BasicImageView.h"
#include "ImageData.h"
#include <Xm/Xm.h>

class DspHistOutput : public DspHistBasic {

protected:
		
	inline int *		getHistArray() { return getHistOutputArray(); }
public:

	DspHistOutput(Widget parent, 
				char * 		name, 
				ImageData * dataModel, 
				char * 		titleLabel,
				DspDisplayPolicy *  displayPolicy  )
			: DspHistBasic(parent,name,dataModel,titleLabel, displayPolicy ) 
					{ createAllViews(); }
};
#endif
$ VOKAGLEVE
$!-----------------------------------------------------------------------------
$ create DspHistPower.h
$ DECK/DOLLARS="$ VOKAGLEVE"
///////////////////////////////////////////////////////////////////
//  DspHistPower.h
///////////////////////////////////////////////////////////////////
#ifndef DSPHISTPOWER_H
#define DSPHISTPOWER_H
#include "DspHistBasic.h"
#include "BasicImageView.h"
#include "ImageData.h"
#include <Xm/Xm.h>

class DspHistPower : public DspHistBasic {

protected:
		
	inline int *		getHistArray() { return getHistPwrSpecArray(); }
	
public:

	DspHistPower(Widget parent, 
				char * 		name, 
				ImageData * dataModel, 
				char * 		titleLabel,
				DspDisplayPolicy *  displayPolicy  )
			: DspHistBasic(parent,name,dataModel,titleLabel, displayPolicy ) 
			
					{ createAllViews(); }
};
#endif

$ VOKAGLEVE
$!-----------------------------------------------------------------------------
$ create DspHistMean.h
$ DECK/DOLLARS="$ VOKAGLEVE"
///////////////////////////////////////////////////////////////////
//  DspHistMean.h
///////////////////////////////////////////////////////////////////
#ifndef DSPHISTMEAN_H
#define DSPHISTMEAN_H
#include "DspHistBasic.h"
#include "BasicImageView.h"
#include "ImageData.h"
#include <Xm/Xm.h>

class DspHistMean : public DspHistBasic {

protected:
		
	inline int *		getHistArray() { return getHistMeanArray(); }
	
public:

	DspHistMean(Widget parent, 
				char * 		name, 
				ImageData * dataModel, 
				char * 		titleLabel,
				DspDisplayPolicy *  displayPolicy  )				
			: DspHistBasic(parent,name,dataModel,titleLabel, displayPolicy ) 
				{ createAllViews(); }

};
#endif

$ VOKAGLEVE
$!-----------------------------------------------------------------------------
$ create DspHistStdDev.h
$ DECK/DOLLARS="$ VOKAGLEVE"
///////////////////////////////////////////////////////////////////
//  DspHistStdDev.h
///////////////////////////////////////////////////////////////////
#ifndef DSPHISTSTDDEV_H
#define DSPHISTSTDDEV_H
#include "DspHistBasic.h"
#include "BasicImageView.h"
#include "ImageData.h"
#include <Xm/Xm.h>

class DspHistStdDev : public DspHistBasic {

protected:
		
	inline int *		getHistArray() { return getHistStdDevArray(); }
	
public:

	DspHistStdDev(Widget parent, 
				char * 		name, 
				ImageData * dataModel, 
				char * 		titleLabel,
				DspDisplayPolicy *  displayPolicy  )
			: DspHistBasic(parent,name,dataModel,titleLabel, displayPolicy ) 
					{ createAllViews(); }

};
#endif

$ VOKAGLEVE
$!-----------------------------------------------------------------------------
$ create DspHistInputPlain.h
$ DECK/DOLLARS="$ VOKAGLEVE"
///////////////////////////////////////////////////////////////////
//  DspHistInputPlain.h
///////////////////////////////////////////////////////////////////
#ifndef DSPHISTINPUTPlain_H
#define DSPHISTINPUTPlain_H
#include "DspHistPlain.h"
#include "BasicImageView.h"
#include "ImageData.h"
#include <Xm/Xm.h>

class DspHistInputPlain : public DspHistPlain {

protected:
		
	inline int *		getHistArray() { 
					if(getHistArrayfunc) {
						return getHistArrayfunc();
					} else {
						return getHistInputArray(); 
					}
				}

public:

	DspHistInputPlain(Widget parent, 
				char * 		name, 
				ImageData * dataModel, 
				char * 		titleLabel,
				DspDisplayPolicy *  displayPolicy,
				int		displayLabel=0 )
			: DspHistPlain(parent,name,dataModel,titleLabel, displayPolicy, displayLabel ) 
				{ createAllViews(); }


};
#endif
$ VOKAGLEVE
$!-----------------------------------------------------------------------------
$ create DspHistPlain.h
$ DECK/DOLLARS="$ VOKAGLEVE"
///////////////////////////////////////////////////////////////////
//  DspHistPlain.h
///////////////////////////////////////////////////////////////////
#ifndef DSPHISTPLAIN_H
#define DSPHISTPLAIN_H
#include "DspUIComponent.h"
#include "BasicImageView.h"
#include "HistGraphView.h"
#include "HistBox.h"
#include "HistDefs.h"
#include "Histogram.h"
#include "ImageData.h"
#include "DspAxisHorizView.h"
#include "DspAxisVertView.h"
#include "DspBarHorizView.h"
#include "DspBarVertView.h"
#include "DspDisplayPolicy.h"
#include "CollectHist.h"
#include "WedgeView.h"
#include "rts_inst_display.h"
#include "Application.h"
#include <Xm/Form.h>
#include <Xm/Frame.h>
#include <Xm/Label.h>
#include <Xm/RowColumn.h>
#include <iostream.h>


	Cursor			createBlankCursor ( Widget iw);

class DspHistPlain : public DspUIComponent {

protected:

	 static String		_defaults[];
		
	 char *			_titleLabel;	
	int			_displayLabel;
	 Histogram *		_histModel;
	 ImageData *		_dataModel;
	 WedgeView *		_wedgeView;
//	 HistGraphView	*	_histView;
	 HistBox	*	_histView;
	 DspAxisVertView *	_axisWest;
	 DspAxisHorizView *	_axisSouth;
	 DspAxisVertView *	_axisEast;
	 DspAxisHorizView *	_axisNorth;
	 DspBarHorizView *	_barNorth;
	 DspDisplayPolicy *	_displayPolicy;
	 Widget			_titleString;
	 Widget			_histForm;
	 Widget			_wedgeFrame;
	 Widget			_histFrame;
	
	 virtual void		createAllViews();	 	
	 virtual void		createHistView(Widget parent);
	 virtual int *		getHistArray()=0;

public:
	 int *			(*getHistArrayfunc)(); 
	  
	DspHistPlain(	Widget 		parent, 
				char * 		name, 
				ImageData * dataModel, 
				char *		titleLabel,
				DspDisplayPolicy * displayPolicy,
				int		displayLabel = 0 );
	~DspHistPlain();
	virtual void 		update();
	inline Histogram *	getHistogramModel(){ return ( _histModel ); }
	
};
#endif
$ VOKAGLEVE
$ Return
$!#############################################################################
