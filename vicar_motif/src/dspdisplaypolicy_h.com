$!****************************************************************************
$!
$! Build proc for MIPL module dspdisplaypolicy_h
$! VPACK Version 1.9, Monday, October 05, 1998, 17:17:58
$!
$! Execute by entering:		$ @dspdisplaypolicy_h
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
$ write sys$output "*** module dspdisplaypolicy_h ***"
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
$ write sys$output "Invalid argument given to dspdisplaypolicy_h.com file -- ", primary
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
$   if F$SEARCH("dspdisplaypolicy_h.bld") .eqs. "" then gosub Build_File
$   if (primary .eqs. " ")
$   then
$      @dspdisplaypolicy_h.bld "STD"
$   else
$      @dspdisplaypolicy_h.bld "''primary'" "''secondary'"
$   endif
$ Return
$!#############################################################################
$Repack_File:
$ create dspdisplaypolicy_h.repack
$ DECK/DOLLARS="$ VOKAGLEVE"
$ vpack dspdisplaypolicy_h.com -mixed -
	-s DspDisplayPolicy.h
$ Exit
$ VOKAGLEVE
$ Return
$!#############################################################################
$Source_File:
$ create DspDisplayPolicy.h
$ DECK/DOLLARS="$ VOKAGLEVE"
///////////////////////////////////////////////////////////////////
//  DspDisplayPolicy.h
//
///////////////////////////////////////////////////////////////////
#ifndef DSPDISPLAYPOLICY_H
#define DSPDISPLAYPOLICY_H

#include "DspLut.h"
#include "XvicImage.h"
#include "DspDefs.h"
//   #include "DspInstrBasic.h"
#include <Xm/Xm.h>
#include <iostream.h>
#include <assert.h>
#include <stdio.h>
#include <ctype.h>

class DspInstrBasic;

			
class DspDisplayPolicy {

   protected:
   
	int			_pixelSize;
        			
	DspLut *		_lutPseudoColor[3];  // 0=red, 1=green, 2=blue
	DspLut *		_lutStretch[3]; //stretch made on bw image, before pseudoclring
	
   public:
   
   	DspDisplayPolicy(DspInstrBasic * instrDisplay,
   				char *   instrDisplayWidgetName);
			
	~DspDisplayPolicy(){ delete [] _lutPseudoColor; delete [] _lutStretch; }
	inline void	setLinearInLutPseudoColor(RGBTYPE i, 
				int startIndex, int endIndex,
				int startValue, int endValue)
			{	_lutPseudoColor[i]->setLinear(
					startIndex, endIndex, 
					startValue,endValue);
			}
	inline void	setFlatInLutPseudoColor(RGBTYPE i, 
				int firstIndex, int lastIndex, int value)
			{	_lutPseudoColor[i]->setFlat(
					firstIndex, lastIndex, 
					value );
			}
	inline  DspLut* getPseudoColorLut(RGBTYPE i) 
					{  return( _lutPseudoColor[((int)i)]); }
	inline  DspLut* getStretchLut(RGBTYPE i) 
					{ return( _lutStretch[((int)i)]); }
					
	virtual void	stretchImage(Widget w, 
					void (*fp_getStretch)(int *, int *, int *),
					Boolean isDialogStretch);
	virtual void  	updateMode( Widget w, 
					void (*fp_getStretch)(int *, int *, int *),
					Boolean pseudoColored,
					Boolean isDialogStretch );					
        inline int      getPixelSize()         
                                        { return _pixelSize; }
	inline void     setPixelSize(int pixelSize)
					{ _pixelSize = pixelSize; }					
};
#endif
$ VOKAGLEVE
$ Return
$!#############################################################################
