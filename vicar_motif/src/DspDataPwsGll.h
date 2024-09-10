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
#include <iostream>
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


