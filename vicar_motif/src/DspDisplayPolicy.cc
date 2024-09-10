///////////////////////////////////////////////////////////////////
//       DspDisplayPolicy.cc: Contains Pseudo color Luts.
//
//      FUTURE:  
//		(1) may need more pixel size code in the future
//
//		(2) change stretchImage to 3 stretch lutarrays
//		for handling color. Then correct getStretchLut()
//
//		(3) change in GUI design caused change in this
//		module.  Now it doesn't need to be created for 
//		every image.  It can be shared umong images/wedges.
//
///////////////////////////////////////////////////////////////////
#include "DspDisplayPolicy.h"
#include "DspInstrBasic.h"
#include "UIComponent.h"
#include <iostream>



///////////////////////////////////////////////////////////////////
//	CONSTRUCTOR 
///////////////////////////////////////////////////////////////////
DspDisplayPolicy::DspDisplayPolicy(DspInstrBasic * , char *   )
{
//	INIT VARIABLES
	_pixelSize = 8;  // default

//	CREATE 3 LUTS FOR PSEUDO COLOR (init in dspinstrument)
	_lutPseudoColor[__red]   = new DspLut();
	_lutPseudoColor[__green] = new DspLut();
	_lutPseudoColor[__blue]  = new DspLut();
	
//	CREATE 3 LUTS FOR STRETCHING
	_lutStretch[__red]   = new DspLut();
	_lutStretch[__green] = new DspLut();		// temp
	_lutStretch[__blue]  = new DspLut();		// temp
	
//	INIT STRETCH LUTS
	_lutStretch[__red]->ramp();
	_lutStretch[__green]->ramp();
	_lutStretch[__blue]->ramp();			
}

///////////////////////////////////////////////////////////////////
//	UPDATE (or change) MODE IN IW (wedge or image)
//		called internally by addWidget()
//		and may be called externally for updating widget
///////////////////////////////////////////////////////////////////
void DspDisplayPolicy::updateMode(Widget w, 
					void (*fp_getStretch)(int *, int *, int *),
					Boolean pseudoColored,
					Boolean isDialogStretch)
{
//	GET COLOR MODE (IE BLACK-WHITE OR COLOR)
	unsigned char	colorMode;
	XtVaGetValues(w, XvicNimageMode, &colorMode, NULL );
	
//	SET PseudoColor LUTS IN IMAGE
        if (pseudoColored && colorMode == XvicBW) 
	     XvicImageSetColorLUT(w, _lutPseudoColor[__red]->getAsArray(),
				_lutPseudoColor[__green]->getAsArray(),
				_lutPseudoColor[__blue]->getAsArray());
						
//	First STRETCH IMAGE (if stretch dialog not enabled)
	stretchImage(w,fp_getStretch,isDialogStretch);

//	SET TO PSEUDO MODE OR NON PSEUDOMODE (but always stretched-may be raw)			
	if ((colorMode == XvicBW ) && ( pseudoColored )) 
       		XtVaSetValues (w, XvicNlutType, XvicPSEUDO, NULL );
	else 
		XtVaSetValues (w, XvicNlutType, XvicSTRETCH, NULL );
}
 
///////////////////////////////////////////////////////////////////
//	stretchImage (private) called by updateMode
//		Stretch table from *Browse user i/f*   (BW only at this time)
///////////////////////////////////////////////////////////////////
void DspDisplayPolicy::stretchImage(Widget w, 
					void (*fp_getStretch)(int *, int *, int *),
					Boolean isDialogStretch)
{
//	IF STRETCH TABLE FROM BROWSE i/f OR R/T CONTROL i/f (IE NOT STRETCH DIALOG)
	if ( !isDialogStretch) {
			
//		GET STRETCH FROM DSP CNTRL (IE BROWSE or r/t control) 
//		(NOTICE THIS HANDLES BW ONLY AT THIS TIME - NA FOR COLOR YET)			
		(*fp_getStretch)( _lutStretch[0]->getAsArray(),
					 _lutStretch[1]->getAsArray(),
					 _lutStretch[2]->getAsArray());
	}	
	 
//	SET STRETCH LUT 
	unsigned char	colorMode;
	XtVaGetValues(w, XvicNimageMode, &colorMode, NULL );
	if (colorMode != XvicCOLOR)
		XvicImageSetMonoLUT(w, 	_lutStretch[0]->getAsArray());
	else 
		XvicImageSetColorLUT(w, _lutStretch[__red]->getAsArray(),
						_lutStretch[__green]->getAsArray(),
						_lutStretch[__blue]->getAsArray());		
}

