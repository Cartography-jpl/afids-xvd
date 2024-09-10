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
#include <iostream>


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
