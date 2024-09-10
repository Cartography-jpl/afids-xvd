$!****************************************************************************
$!
$! Build proc for MIPL module dsplut_h
$! VPACK Version 1.8, Monday, April 01, 1996, 21:52:57
$!
$! Execute by entering:		$ @dsplut_h
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
$ write sys$output "*** module dsplut_h ***"
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
$ write sys$output "Invalid argument given to dsplut_h.com file -- ", primary
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
$   if F$SEARCH("dsplut_h.bld") .eqs. "" then gosub Build_File
$   if (primary .eqs. " ")
$   then
$      @dsplut_h.bld "STD"
$   else
$      @dsplut_h.bld "''primary'" "''secondary'"
$   endif
$ Return
$!#############################################################################
$Repack_File:
$ create dsplut_h.repack
$ DECK/DOLLARS="$ VOKAGLEVE"
$ vpack dsplut_h.com -mixed -
	-s DspLut.h
$ Exit
$ VOKAGLEVE
$ Return
$!#############################################################################
$Source_File:
$ create DspLut.h
$ DECK/DOLLARS="$ VOKAGLEVE"
///////////////////////////////////////////////////////////////////
//  DspLut.h
///////////////////////////////////////////////////////////////////
#ifndef DSPLUT_H
#define DSPLUT_H

#include "Lut.h"
#include <iostream.h>
#include <assert.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>


class DspLut : public Lut {


   protected:

	int * 	_defaultLut;		// default values
	int	_pixelSize;		// numb bits	
	int	_lutSize;		

   public:

	DspLut( int pixelSize=8);
	~DspLut() { delete [] _lut; }

	inline  int	getDN(int index) { return(_lut[index]); /*needs test*/}
	virtual void 	setDefAsArray ( int *array );
	virtual void	restoreDefault( int start, int end );
	virtual void	setFlat( int firstIndex, int lastIndex, int value);
	virtual void	setLinear(int firstIndex, int lastIndex, 
					int startValue, int endValue);
	virtual void	setAsArray(int * array); // copy in new array - not just int*array;
	virtual void    resizeLut(int pixelSize);
	inline  int	getNumberOfBitsInPixel()
					{return(_pixelSize);}
	inline  int     getLutSize() { return _lutSize; }
	inline  int     getPixelSize() { return _pixelSize; }

};
#endif
$ VOKAGLEVE
$ Return
$!#############################################################################
