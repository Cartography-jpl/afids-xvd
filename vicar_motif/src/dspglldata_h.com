$!****************************************************************************
$!
$! Build proc for MIPL module dspglldata_h
$! VPACK Version 1.8, Monday, April 01, 1996, 21:52:51
$!
$! Execute by entering:		$ @dspglldata_h
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
$ write sys$output "*** module dspglldata_h ***"
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
$ write sys$output "Invalid argument given to dspglldata_h.com file -- ", primary
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
$   if F$SEARCH("dspglldata_h.bld") .eqs. "" then gosub Build_File
$   if (primary .eqs. " ")
$   then
$      @dspglldata_h.bld "STD"
$   else
$      @dspglldata_h.bld "''primary'" "''secondary'"
$   endif
$ Return
$!#############################################################################
$Repack_File:
$ create dspglldata_h.repack
$ DECK/DOLLARS="$ VOKAGLEVE"
$ vpack dspglldata_h.com -mixed -
	-s DspDataPwsGll.h DspDataNimBasicGll.h DspDataNimEngRB.h
$ Exit
$ VOKAGLEVE
$ Return
$!#############################################################################
$Source_File:
$ create DspDataPwsGll.h
$ DECK/DOLLARS="$ VOKAGLEVE"
////////////////////////////////////////////////////////////////
// DspDataPwsGll.h
//
//	This is subclassed from Vicar image data for displaying 
//	in GLL pws data.
//
//	
////////////////////////////////////////////////////////////////
#ifndef DSPDATAGLLPWS_H
#define DSPDATAGLLPWS_H
#include "VicarImageData.h"
#include "VicarImageFile.h"
#include "ZoomFactor.h"
#include "zvproto.h"
#include <iostream.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Xm/Xm.h>

class DspDataPwsGll : public VicarImageData { 

   protected:
   
   	int			_numberOfSamplesActual;
   	int			_numberOfLinesActual;
         	  
   public:

	DspDataPwsGll( ) : VicarImageData() { }
	~DspDataPwsGll( ) { }	
	virtual StatusType	open( char * stringOfFilenames );
	virtual StatusType	readTile(int,int,int,int,ImageTile & );
	virtual StatusType	getSuggestedUnzoomedTileSize(int &, int & );
	virtual ZoomFactor      &calcTileZoom( ImageTile * tile,
                                   int Xin=1, int Xout=1, 
                                   int Yin=1, int Yout=1,
                                   int Xsubpan=0, int Ysubpan=0 );
 
        
};
#endif


$ VOKAGLEVE
$!-----------------------------------------------------------------------------
$ create DspDataNimBasicGll.h
$ DECK/DOLLARS="$ VOKAGLEVE"
/////////////////////////////////////////////////////////////////
// DspDataNimBasicGll.h
//
//	This is a subclass of ImageData model.    
////////////////////////////////////////////////////////////////
#ifndef DSPDATAGLLNIMS_H
#define DSPDATAGLLNIMS_H
#include "ImageData.h"

class ZoomFactor;

const int MAX_CHANNELS = 3;	// Max # of channels (e.g. R,G,B, or bw only)

class DspDataNimBasicGll : public ImageData {

   protected:

// 	INFO ABOUT EACH FILE  --- Vicar Specific
	int		_numbChannels;	// # of logical channels, 3 for RGB or 1 for BW

	unsigned char * _pixelBuffers[MAX_CHANNELS];	 // ptrs to unique physical files
	unsigned int *	_rawDNbuffers[MAX_CHANNELS];
	unsigned char * _bufferForChannel[MAX_CHANNELS]; // ptr copies for each chan
	int		_bandForChannel[MAX_CHANNELS];	 // band for each channel
	unsigned char *  getZeroFillBuffer();
	
// 	BUFFER MANAGEMENT (should be called by destructor).
// 		THIS IS A STUB ONLY TO SATISFY THE PURE VIRTUAL FUNCTION
	virtual void	destroyBuffers();
	void            (*_fp_getPixels)(unsigned char*,unsigned char*,unsigned char*); 
					// pntr to fillbuffers routine.   
	void            (*_fp_getRawDN)(unsigned int*,unsigned int*,unsigned int*); 
					// pntr to fillbuffers routine.   
	
// 	TILE MANAGEMENT
	virtual void initTileProperties(ImageTile &tile, ZoomFactor &tileZoom);

   public:

	DspDataNimBasicGll( void (*fp_getPixels)(unsigned char*,unsigned char*,unsigned char*),
			    void (*fp_getRawDN)(unsigned int*,unsigned int*,unsigned int*),
			    int nl, int ns, int nc, char * pixelType);
	~DspDataNimBasicGll();

// 	FILE ROUTINES:
	virtual StatusType open( char * ); 
	virtual StatusType close( );	
 
// 	INFORMATIONAL ONLY,	
	virtual StatusType getSuggestedUnzoomedTileSize(int & height,
							int & width);	

// 	OUTSIDE CONTROLS
// 	set mode to color or b&w - notice that this overrides ImageData
	virtual inline void	setMode( ModeType mode)
				{ _mode = mode; };

  	virtual ZoomFactor &calcTileZoom( ImageTile * tile,
				int Xin=1, int Xout=1, int Yin=1, int Yout=1,
				int Xsubpan = 0, int Ysubpan = 0 );

// 	TILE/BUFFER MANAGEMENT
	virtual ImageTile &createTile(ZoomFactor & tileZoom,
				      Boolean   willCreateBuffers = TRUE ) ;
	virtual inline void destroyTile( ImageTile tile )   {delete &tile; } ;
	inline Boolean	isNewBufferNeeded(ImageTile & tile)
					{return tile.isNewBufferNeeded(); }

// 	READ DATA
	virtual StatusType 	readTile(int unzoomedStartSample,
				    int unzoomedStartLine,
				    int unzoomedWidth, int unzoomedHeight,
				    ImageTile & tile);
				    
	virtual StatusType	readLine( ColorType , int , unsigned char * )
					{return imSUCCESS;}//not implimented here
	virtual StatusType	readPixel(ColorType, int , int, unsigned char * );							    
	virtual char *className() { return ( "DspDataNimBasicGll"); }
};
#endif


$ VOKAGLEVE
$!-----------------------------------------------------------------------------
$ create DspDataNimEngRB.h
$ DECK/DOLLARS="$ VOKAGLEVE"
/////////////////////////////////////////////////////////////////
// DspDataNimEngRB.h    
//
////////////////////////////////////////////////////////////////
#ifndef DSPDATANIMENGRB_H
#define DSPDATANIMENGRB_H
#include "DspDataNimBasicGll.h"

class DspDataNimEngRB : public DspDataNimBasicGll {

   protected:

   public:

	DspDataNimEngRB::DspDataNimEngRB(void (*fp_getPixels)(unsigned char*,unsigned char*,unsigned char*),
					void (*fp_getRawDN)(unsigned int*, unsigned int*,unsigned int *),
					int nl, int ns, 
					int nc, char * pixelType) 
			: DspDataNimBasicGll((*fp_getPixels), (*fp_getRawDN), nl,ns, 
						nc, pixelType) {}
	virtual StatusType open( char * ); 

	virtual char *className() { return ("DspDataNimEngRB"); }
};
#endif 

$ VOKAGLEVE
$ Return
$!#############################################################################
