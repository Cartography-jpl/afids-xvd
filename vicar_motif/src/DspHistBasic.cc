///////////////////////////////////////////////////////////////////
//  DspHistBasic.cc
///////////////////////////////////////////////////////////////////
#include "DspHistBasic.h"

///////////////////////////////////////////////////////////////////
//	Resource Defaults  .. widget resources
/////////////////////////////////////////////////////////////////
String DspHistBasic::_defaults [ ] = {
	"*wedgeView.stretchPolicy:              XvicUSE_SW",
	"*wedgeView.colormapPolicy:             XvicALLOC",
	"*wedgeView.lutType:                    XvicRAW",
	"*wedgeView.grayLevels:			128",
	"*wedgeView.scrollBarDisplayPolicy:     XvicNEVER",
	"*wedgeView.shadowThickness:		0",
	"*wedgeView.highlightThickness:		0",	
	"*wedgeView*orientation:          	HORIZONTAL",
	"*wedgeView*nsteps:               	256",
	"*wedgeView*minPixelDN:           	0",
	"*wedgeView*maxPixelDN:           	255",
	"*wedgeFrame.marginWidth:		0"
	"*wedgeFrame.marginHeight:		0"	
	"*wedgeFrame.topShadowColor:		white",
	"*wedgeFrame.bottomShadowColor:		white",				
	"*wedgeFrame.shadowThickness:		1",
	"*titleString.fontList:			-*-*-*-r-normal-*-14-*-*-*-*-*-*-*",
	"*histView.height:			100",
	"*histView.width:			128",
	"*histView.redColor:                	white",
	"*titleString.marginWidth:		0",
	"*axisNorth*longTickLength:		4",
	"*axisSouth*longTickLength:		5",		
	"*axisEast*longTickLength:		4",
	"*axisWest*longTickLength:		4",
	"*histFrame.shadowThickness:		1",
	"*histFrame.topShadowColor:		white",
	"*histFrame.bottomShadowColor:		white",			
 	"*histFrame.marginWidth:	  	0",
	"*histFrame.marginHeight:	  	0",
	"*axisSouth*numbTicks:			9",
	"*axisNorth*numbTicks:			9",
	"*axisEast*numbTicks:			5",
	"*axisWest*numbTicks:			5",
       NULL,
};

///////////////////////////////////////////////////////////////////
//	Constructor:
///////////////////////////////////////////////////////////////////
DspHistBasic::DspHistBasic(	Widget 		parent, 
						char * 		name, 
						ImageData * dataModel, 
						char *		titleLabel,
					//// int (*fp_getHistData)(),
						DspDisplayPolicy * displayPolicy)	 
						: DspUIComponent(name)
{
//	SAVE ARGS
	  _titleLabel = titleLabel;
	  _dataModel = dataModel;
    ///////_fp_getHistData = fp_getHistData;
	  _displayPolicy = displayPolicy;
	  
//	SET DEFAULT RESOURCES
	  setDefaultResources ( parent, _defaults );
	
// 	CREATE A FORM TO HOLD HIST, FRAME, AXIS, WEDGE	
	  int n = 0;
	  Arg args[20];
	  XtSetArg(args[n], XmNmarginHeight,  0); n++;
	  XtSetArg(args[n], XmNmarginWidth, 0); n++;
	  _w = XtCreateWidget( _name, xmFormWidgetClass, parent, args, n);
       	  installDestroyHandler();
       	  
       	  
     /////createAllViews();
}
///////////////////////////////////////////////////////////////////
//	create: frame, axis, wedge, hist(in subclass)
///////////////////////////////////////////////////////////////////
void DspHistBasic::createAllViews()
{   
//	DISPLAY LABEL	  
          XmString xmstr = XmStringCreateSimple (_titleLabel);
          _titleString = XtCreateManagedWidget( "titleString", xmLabelWidgetClass, _w, NULL, 0);
          XtVaSetValues ( _titleString,		
          		XmNlabelString,		xmstr,
            		XmNtopAttachment,	XmATTACH_FORM,		
			XmNleftAttachment, 	XmATTACH_FORM,
          		XmNrightAttachment,	XmATTACH_FORM,
			XmNalignment,		XmALIGNMENT_CENTER,
                	NULL );       
          XmStringFree (xmstr); 

//	CREATE FORM FOR HOLDING AXIS, BAR, HISTOGRAM 
	  _histForm = XtCreateManagedWidget( "histForm", xmFormWidgetClass, _w, NULL, 0);	  
	  XtVaSetValues( _histForm, 
        		XmNleftAttachment, 	XmATTACH_FORM,
        		XmNrightAttachment,	XmATTACH_FORM,
        		XmNtopWidget,		_titleString,
               		XmNtopAttachment,    	XmATTACH_WIDGET,
        		NULL );	 
        				  	  	  	  
//	CREATE BASIC FRAMED HISTOGRAM
	  _histFrame = XtCreateManagedWidget( "histFrame", xmFrameWidgetClass, _histForm, NULL, 0);
	  createHistView(_histFrame);
	   		   	   	   
//	CREATE AXIS AROUND FRAMED HISTOGRAM 
	   _axisWest  = new DspAxisVertView(  _histForm, "axisWest",_histFrame);
	   _axisSouth = new DspAxisHorizView( _histForm, "axisSouth",_histFrame);
	   _axisEast  = new DspAxisVertView(  _histForm, "axisEast", _histFrame);
	   _axisNorth = new DspAxisHorizView( _histForm, "axisNorth",_histFrame);	
	   _axisWest->setOffset(  _axisNorth->getTickLength()+4 ); 
	   _axisEast->setOffset(  _axisNorth->getTickLength()+4 ); 
	   _axisNorth->setOffset( _axisWest->getTickLength()+1 ); 
	   _axisSouth->setOffset( _axisWest->getTickLength()+1); 	   
	   _axisWest->manage(); 
	   _axisSouth->manage();
	   _axisEast->manage();
	   _axisNorth->manage();	
	   
//	CREATE BAR OVER AXIS)
	  _barNorth = new DspBarHorizView( _histForm, "frameNorth", _axisNorth->baseWidget());	
	  XtVaSetValues (_barNorth->baseWidget(),
	   	XmNtopAttachment,	XmATTACH_FORM,
	  	XmNtopOffset,		2,	// TOP FRAME BAR GETS CUTOFF 
	  	XmNleftAttachment,	XmATTACH_FORM,
	  	XmNleftOffset,		((int) _axisWest->getTickLength()+1 ),
	 	NULL );	   
	  _barNorth->manage(); 	 	 	
	 	  		     	           		                                                	   	   	   
// 	ATTACH AXES 
          XtVaSetValues ( _axisNorth->baseWidget(),	// TOP AXIS
               XmNtopWidget,		_barNorth->baseWidget(),
               XmNtopAttachment,    	XmATTACH_WIDGET,
               NULL );
	   
         XtVaSetValues ( _axisEast->baseWidget(),	// RIGHT AXIS
               XmNrightAttachment,    	XmATTACH_FORM,
	       NULL );
               
         XtVaSetValues ( _axisWest->baseWidget(),	// LEFT AXIS
               XmNleftAttachment,     	XmATTACH_FORM,
               NULL );    
                               
	XtVaSetValues ( _histFrame,
		XmNtopWidget,		_axisNorth->baseWidget(),
		XmNtopAttachment,	XmATTACH_WIDGET,	
		XmNleftWidget,		 _axisWest->baseWidget(),
		XmNleftAttachment,	XmATTACH_WIDGET,	
		XmNrightWidget,          _axisEast->baseWidget() ,   
		XmNrightAttachment,	XmATTACH_WIDGET,
		NULL );	
		
         XtVaSetValues ( _axisSouth->baseWidget(),	// Bottom AXIS
	 	XmNtopWidget,		_histFrame,
                XmNtopAttachment,     	XmATTACH_WIDGET,
                NULL);        		 			
		
//	CREATE WEDGE 
	  int leftspacing = (int) (_axisWest->getTickLength() );
          int n = 0;
	  Arg args[20];
	  XtSetArg(args[n], XmNmarginHeight,  0); n++;
	  XtSetArg(args[n], XmNmarginWidth, 0); n++;
	  _wedgeFrame = XtCreateManagedWidget( "wedgeFrame", xmFrameWidgetClass, _w, args, n);
	  _wedgeView = new WedgeView( _wedgeFrame,  "wedgeView", 6, 128 );
	  _wedgeView->manage();
	  

//	ATTACH WEDGE UNDER HISTOGRAM & AXIS (UNDER _HISTFORM)
	  XtVaSetValues( _wedgeFrame, 
        		XmNleftAttachment, 	XmATTACH_FORM,
        		XmNleftOffset,     	leftspacing + 1,
        	 	XmNbottomAttachment,	XmATTACH_FORM,
        		XmNtopWidget,		_histForm,
               		XmNtopAttachment,    	XmATTACH_WIDGET,
        		NULL );	 		
			 	            
}
///////////////////////////////////////////////////////////////////
//	createHistView
///////////////////////////////////////////////////////////////////
void DspHistBasic::createHistView(Widget parent)
{
	   _histModel = new Histogram(0, 255, 2);
	   
	   int * histArray = getHistArray();
   //// int * histArray = (*fp_getHistData)();
	   for (int i = 0; i< _histModel->numBins(); i++ ) {
	   	_histModel->setBin( i, histArray[i] );
	   }	   
 	   _histView = new HistGraphView( parent,"histView", _histModel,
 	   					NULL,NULL,BLEND,VERTICAL,ASC);
	   _histView->manage();
}
///////////////////////////////////////////////////////////////////
//	Destructor
///////////////////////////////////////////////////////////////////
DspHistBasic::~DspHistBasic()
{
         RemoveFormAttachments( _histView->baseWidget());
	 RemoveFormAttachments( _wedgeView->baseWidget());
	 RemoveFormAttachments( _axisWest->baseWidget());
	 RemoveFormAttachments( _axisSouth->baseWidget());
	 RemoveFormAttachments( _axisEast->baseWidget());
	 RemoveFormAttachments( _axisNorth->baseWidget());
	 RemoveFormAttachments( _barNorth->baseWidget());
	 
        delete _histView;
	delete _wedgeView;
	delete _axisWest;
	delete _axisSouth;
	delete _axisEast;
	delete _axisNorth;
	delete _barNorth;
	XtDestroyWidget( _histForm) ;
	XtDestroyWidget( _titleString);
	delete _histModel;
}
///////////////////////////////////////////////////////////////////
//	updateHistView
///////////////////////////////////////////////////////////////////
void DspHistBasic::update()
{
//	GET NEW HISTOGRAM AND REDISPLAY
	int * histArray = getHistArray();
	 //// int * histArray = (*fp_getHistData)();
	for (int i = 0; i < _histModel->numBins(); i++ ) {  
		_histModel->setBin( i, histArray[i] );
	}	
	_histModel->updateViews();	   	   
}
