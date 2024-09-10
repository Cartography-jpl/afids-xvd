$!****************************************************************************
$!
$! Build proc for MIPL module dspimage_h
$! VPACK Version 1.8, Tuesday, October 29, 1996, 09:44:10
$!
$! Execute by entering:		$ @dspimage_h
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
$ write sys$output "*** module dspimage_h ***"
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
$ write sys$output "Invalid argument given to dspimage_h.com file -- ", primary
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
$   if F$SEARCH("dspimage_h.bld") .eqs. "" then gosub Build_File
$   if (primary .eqs. " ")
$   then
$      @dspimage_h.bld "STD"
$   else
$      @dspimage_h.bld "''primary'" "''secondary'"
$   endif
$ Return
$!#############################################################################
$Repack_File:
$ create dspimage_h.repack
$ DECK/DOLLARS="$ VOKAGLEVE"
$ vpack dspimage_h.com -mixed -
	-s DspImageBasic.h DspImageEmpty.h
$ Exit
$ VOKAGLEVE
$ Return
$!#############################################################################
$Source_File:
$ create DspImageBasic.h
$ DECK/DOLLARS="$ VOKAGLEVE"
///////////////////////////////////////////////////////////////////
//  DspImageBasic.h
///////////////////////////////////////////////////////////////////
#ifndef DSPIMAGEBASIC_H
#define DSPIMAGEBASIC_H
#include "DspUIComponent.h"
#include "BasicImageView.h"
#include "WedgeView.h"
#include "ImageDisplayView.h"
#include "ImageData.h"
#include "DspAxisHorizView.h"
#include "DspAxisVertView.h"
#include "DspBarHorizView.h"
#include "DspBarVertView.h"
#include "DspDisplayPolicy.h"
#include "DspUtilMisc.h"
#include "ZoomFactor.h"
#include "rts_inst_display.h"
#include "Histogram.h"
#include <assert.h>
#include <Xm/Xm.h>
#include <Xm/Frame.h>
#include <Xm/Form.h>
#include <Xm/RowColumn.h>
#include <iostream.h>


class DspImageBasic : public DspUIComponent {

   protected:

	static String 		_defaultsBasic[]; // resource defaults for
						 //everything except images.

	DspAxisVertView *	_axisWest;
	DspAxisHorizView *	_axisSouth;
	DspAxisVertView *	_axisEast;
	DspAxisHorizView *	_axisNorth;
	
	Widget			_imageAxisFrame;
	Widget			_imageForm;
	Widget			_imageFrame;
	Widget			_imageAttachForm;
	struct ImageView{	  
		ImageDisplayView *	image;
		ImageData *		imageModel;
		int			(*fp_getPseudoMode)();
		void 			(*fp_getStretch)(int *, int *, int * );
		//Boolean		stretchability;
		void			(*fp_getZoom)(int*,int*,int*,int*,int*);
		DspDisplayPolicy *	displayPolicy;
		int			zoomX;
		int			zoomY;
		int			panX;
		int			panY;
		int			oddEvenFlag;
			
	};
	struct WedgeViewPlus{	  
		WedgeView *		wedge;
		int			(*fp_getPseudoMode)();
		void	 		(*fp_getStretch)(int *, int *, int *);
		//Boolean		stretchability;
		void			(*fp_getZoom)(int*,int*,int*,int*,int*);
		DspDisplayPolicy *	displayPolicy;
	};
	ImageView **		_imageView;	
	WedgeViewPlus *		_wedgeView;
	Widget			_wedgeForm;
	Widget			_wedgeFrame;
	Widget			_parent;
	int			_numbImages;
	int			_viewHeight; // ht of all images 
	int			_viewWidth; // wdth of all images in image area
	int			_wedge_flag; // where is wedge (lrtb?)
	Boolean			_isDialogStretch; 
	
	virtual void createPrimaryImage(char * name, ImageData * dataModel, 
	 		DspDisplayPolicy * displayPolicy,
			int view_height, int view_width, int zoomX, int zoomY,
	 		int panX, int panY, int oddEvenFlag,
			int (*fp_getPseudoMode)(),  // pointer to function
			void (*fp_getStretch)(int *, int *, int *),
			void (*fp_getZoom)(int*,int*,int*,int*,int*) // pntr to getZoom 
	 		) = 0;
	virtual void zoomImage(int zoomX, int zoomY, int panX, int panY, 
				int oddEvenFlag, ImageDisplayView * imageView );
	
   public:

	DspImageBasic(Widget parent, char * name );
	~DspImageBasic();
	inline void setDialogStretch(Boolean b) { _isDialogStretch = b; };
	virtual void createImageWedge();
	Widget get_wedge_widget(void) { return(	_wedgeFrame); }
	
	virtual void createImageArea( ImageData *dataModel, 
			DspDisplayPolicy * displayPolicy,
			int view_height, int view_width, int zoomX, int zoomY,
			int panX, int panY, int oddEvenFlag,
			int (*fp_getPseudoMode)(),  // pointer to function
			void (*fp_getStretch)(int *, int *, int *),
			void (*fp_getZoom)(int*,int*,int*,int*,int*), // pntr to getZoom 
			int wedge_flag = 1				//add wedge to left??
			);

	virtual ImageDisplayView *addFirstImage( char * name, ImageData * dataModel, 
	 		DspDisplayPolicy * displayPolicy,
			int view_height=400, int view_width=400, 
	 		int zoomX=1, int zoomY=1,
	 		int panX=0, int panY=0,
	 		int oddEvenFlag=0,
	 		int (*fp_getPseudoMode)()=setPseudoModeOff,  // pointer to function
			void (*fp_getStretch)(int *, int *, int *)=fillRGBImageStretchLut,
			void (*fp_getZoom)(int*,int*,int*,int*,int*)=setZoomParams2Off // pntr to getZoom 
	 		) = 0;

	virtual ImageDisplayView *addImage(char * name, ImageData *dataModel, 
	 		DspDisplayPolicy * displayPolicy,
			int view_height=400, int view_width=400, 
	 		int zoomX=1, int zoomY=1,
	 		int panX=0, int panY=0,
	 		int oddEvenFlag=0,
	 		int (*fp_getPseudoMode)()=setPseudoModeOff,  // pointer to function
			void (*fp_getStretch)(int *, int *, int *)=fillRGBImageStretchLutWithRaw,
			void (*fp_getZoom)(int*,int*,int*,int*,int*)=setZoomParams2Off // pntr to getZoom 
	 		);

	virtual Widget  		getImageWidget(int i = 0);  
	virtual DspDisplayPolicy * 	getDisplayPolicy(int i = 0);
	inline	int			getNumbImages() { return _numbImages; }
	inline  ImageData *		getImageModel(int i = 0) { return (_imageView[i]->imageModel); }
	virtual void			update();
	virtual void			updateImageZoom ();
};
#endif
$ VOKAGLEVE
$!-----------------------------------------------------------------------------
$ create DspImageEmpty.h
$ DECK/DOLLARS="$ VOKAGLEVE"
///////////////////////////////////////////////////////////////////
//  DspImageEmpty.h
///////////////////////////////////////////////////////////////////
#ifndef DSPIMAGEEMPTY_H
#define DSPIMAGEEMPTY_H
#include "DspImageBasic.h"

class DspImageEmpty : public DspImageBasic {

   protected:

	static String _defaultsImageEmpty[]; // resource defaults for ImageEmpty only
	virtual void  createPrimaryImage(char *, ImageData *, DspDisplayPolicy * ,
						int, int, int, 
						int, int, int, int,
						int (*)(),
						void (*)(int *, int *, int *),			
						void (*)(int*,int*,int*,int*,int*))
						{ }

   public:

	DspImageEmpty(Widget parent, char * name, 
			ImageData * dataModel=0, DspDisplayPolicy * displayPolicy=0,
			int view_height = 400, int view_width = 400, int zoomX=1, int zoomY=1,
			int panX=0, int panY=0, int oddEvenFlag=0, int wedge_flag = 1) 
	      : DspImageBasic( parent, name ) 
		{ 
			int (*fp_getPseudoMode)() = getPseudoMode;
			void (*fp_getStretch)(int *, int *, int * ) = fillRGBImageStretchLut;
			void (*fp_getZoom)(int*,int*,int*,int*,int*) = fillZoomParams;
			createImageArea(dataModel, displayPolicy, view_height,view_width, zoomX, zoomY,
					panX, panY, oddEvenFlag,fp_getPseudoMode,fp_getStretch,
					fp_getZoom, wedge_flag); 
		}

        virtual ImageDisplayView * addFirstImage( 
        		char *, ImageData *,
        		DspDisplayPolicy * displayPolicy,
			int image_height, int image_width, int zoomX, int zoomY,
			int panX, int panY, int oddEvenFlag,
			int (*fp_getPseudoMode)(),  // pointer to function
			void (*fp_getStretch)(int *, int *, int *),			
			void (*fp_getZoom)(int*,int*,int*,int*,int*)); // pntr to getZoom );

};
#endif
$ VOKAGLEVE
$ Return
$!#############################################################################
