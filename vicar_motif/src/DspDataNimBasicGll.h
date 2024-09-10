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


