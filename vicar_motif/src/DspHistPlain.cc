///////////////////////////////////////////////////////////////////
//  DspHistPlain.cc
///////////////////////////////////////////////////////////////////
#include "DspHistPlain.h"
#include "HistLogVerAxis.h"

///////////////////////////////////////////////////////////////////
//	Resource Defaults  .. widget resources
/////////////////////////////////////////////////////////////////
String DspHistPlain::_defaults [ ] = {
	"*titleString.fontList:			-misc-fixed-medium-r-normal-*-13-*-*-*-*-*-*-*",
	"*histView.height:			200",
	"*histView.width:			256",
	"*histView.redColor:                	white",
	"*titleString.marginWidth:		0",
	"*axisNorth*longTickLength:		4",
	"*axisSouth*longTickLength:		5",		
	"*axisEast*longTickLength:		4",
	"*axisWest*longTickLength:		4",
	"*histFrame*shadowThickness:		1",
	"*histFrame*topShadowColor:		white",
	"*histFrame*bottomShadowColor:		white",			
 	"*histFrame*marginWidth:	  	0",
	"*histFrame*marginHeight:	  	0",
	"*axisSouth*numbTicks:			9",
	"*axisNorth*numbTicks:			9",
	"*axisEast*numbTicks:			5",
	"*axisWest*numbTicks:			5",
       NULL,
};

///////////////////////////////////////////////////////////////////
//	Constructor:
///////////////////////////////////////////////////////////////////
DspHistPlain::DspHistPlain(	Widget 		parent, 
						char * 		name, 
						ImageData * dataModel, 
						char *		titleLabel,
					//// int (*fp_getHistData)(),
						DspDisplayPolicy * displayPolicy,
						int		displayLabel)	 
						: DspUIComponent(name)
{
//	SAVE ARGS
	  _titleLabel = titleLabel;
	  _dataModel = dataModel;
	  _displayPolicy = displayPolicy;
	_displayLabel = displayLabel;
	getHistArrayfunc = 0;
	  
//	SET DEFAULT RESOURCES
	  setDefaultResources ( parent, _defaults );
	
// 	CREATE A FORM TO HOLD HIST, FRAME, AXIS, WEDGE	
	  int n = 0;
	  Arg args[20];
	  XtSetArg(args[n], XmNmarginHeight,  0); n++;
	  XtSetArg(args[n], XmNmarginWidth, 0); n++;
	  _w = XtCreateWidget( _name, xmFormWidgetClass, parent, args, n);
       	  installDestroyHandler();
       	  
}
///////////////////////////////////////////////////////////////////
//	create: frame, axis, wedge, hist(in subclass)
///////////////////////////////////////////////////////////////////
void DspHistPlain::createAllViews()
{   
//	CREATE FORM FOR HOLDING HISTOGRAM 
	  _histForm = XtCreateManagedWidget( "histForm", xmFormWidgetClass, _w, NULL, 0);	  
//	DISPLAY LABEL	  
	if(_displayLabel) {
          XmString xmstr = XmStringCreateSimple (_titleLabel);
          _titleString = XtCreateManagedWidget( "titleString", xmLabelWidgetClass, _w, NULL, 0);
	  if(_displayLabel & 1) {		// label to left
          		XtVaSetValues ( _titleString,		
          				XmNlabelString,		xmstr,
            				XmNtopAttachment,	XmATTACH_FORM,		
					XmNleftAttachment, 	XmATTACH_FORM,
          				XmNbottomAttachment,	XmATTACH_FORM,
					XmNalignment,		XmALIGNMENT_BEGINNING,
                			NULL );       
	  		XtVaSetValues( _histForm, 
        				XmNleftAttachment, 	XmATTACH_WIDGET,
					XmNleftWidget,		_titleString,
        				XmNrightAttachment,	XmATTACH_FORM,
               				XmNtopAttachment,    	XmATTACH_FORM,
               				XmNbottomAttachment,    	XmATTACH_FORM,
        				NULL );	 
	  } else if(_displayLabel & 2) {		// label to right
	  		XtVaSetValues( _histForm, 
        				XmNleftAttachment, 	XmATTACH_FORM,
               				XmNtopAttachment,    	XmATTACH_FORM,
               				XmNbottomAttachment,    XmATTACH_FORM,
        				NULL );	 
          		XtVaSetValues ( _titleString,		
          				XmNlabelString,		xmstr,
            				XmNtopAttachment,	XmATTACH_FORM,		
					XmNleftAttachment, 	XmATTACH_WIDGET,
					XmNleftWidget,		_histForm,
          				XmNrightAttachment,	XmATTACH_FORM,
          				XmNbottomAttachment,	XmATTACH_FORM,
//					XmNalignment,		XmALIGNMENT_BEGINNING,
					XmNalignment,		XmALIGNMENT_END,
                 			NULL );       
	  } else if(_displayLabel & 4) {		// label to top
          		XtVaSetValues ( _titleString,		
          				XmNlabelString,		xmstr,
            				XmNtopAttachment,	XmATTACH_FORM,		
					XmNleftAttachment, 	XmATTACH_FORM,
          				XmNrightAttachment,	XmATTACH_FORM,
					XmNheight,		24,
					XmNalignment,		XmALIGNMENT_BEGINNING,
                			NULL );       
	  		XtVaSetValues( _histForm, 
        				XmNleftAttachment, 	XmATTACH_FORM,
        				XmNrightAttachment,	XmATTACH_FORM,
               				XmNtopAttachment,    	XmATTACH_WIDGET,
					XmNtopWidget,		_titleString,
               				XmNbottomAttachment,    XmATTACH_FORM,
                			NULL );       
	  } else if(_displayLabel & 8) {		// label to bottom
	  		XtVaSetValues( _histForm, 
        				XmNleftAttachment, 	XmATTACH_FORM,
        				XmNrightAttachment,	XmATTACH_FORM,
               				XmNtopAttachment,    	XmATTACH_FORM,
        				NULL );	 
          		XtVaSetValues ( _titleString,		
          				XmNlabelString,		xmstr,
            				XmNtopAttachment,	XmATTACH_WIDGET,		
					XmNtopWidget,		_histForm,
					XmNleftAttachment, 	XmATTACH_FORM,
          				XmNrightAttachment,	XmATTACH_FORM,
          				XmNbottomAttachment,	XmATTACH_FORM,
					XmNalignment,		XmALIGNMENT_BEGINNING,
//					XmNalignment,		XmALIGNMENT_END,
                			NULL );       
	  } else if(_displayLabel & 16) {
			// bad idea for now
	}
        XmStringFree (xmstr); 
	} else {
	XtVaSetValues( _histForm, 
        		XmNleftAttachment, 	XmATTACH_FORM,
        		XmNrightAttachment,	XmATTACH_FORM,
               		XmNtopAttachment,    	XmATTACH_FORM,
               		XmNbottomAttachment,    XmATTACH_FORM,
        		NULL );	 
        				  	  	  	  
	}

//	CREATE BASIC FRAMED HISTOGRAM
	_histFrame = XtCreateManagedWidget( "histFrame", 
				xmFrameWidgetClass, _histForm, NULL, 0);
	createHistView(_histFrame);
	   		   	   	   
}
///////////////////////////////////////////////////////////////////
//	createHistView
///////////////////////////////////////////////////////////////////
void DspHistPlain::createHistView(Widget parent)
{
	Widget form = XtCreateManagedWidget("histAndAxisForm",
				xmFormWidgetClass, parent, NULL, 0);
	_histModel = new Histogram(0, 255, 1);
	   
	int * histArray = getHistArray();
	for (int i = 0; i< _histModel->numBins(); i++ ) {
	   	_histModel->setBin( i, histArray[i] );
	}
 	_histView = new HistBox( form,"histView", _histModel);
	_histView->setLogScale(True);
	_histView->showStat(False);
	_histView->setOrientType(VERTICAL);

     if(_displayLabel)
     {	HistAxisView *axisView = new HistLogVerAxis(form, "verAxis", 
				_histModel, NULL, NULL, HORIZONTAL);
	if(_displayLabel & 32)
	{ XtVaSetValues(axisView->baseWidget(), 
			XmNleftAttachment, 	XmATTACH_WIDGET,
			XmNleftWidget, 		_histView->baseWidget(),
        		XmNrightAttachment,	XmATTACH_FORM,
               		XmNtopAttachment,    	XmATTACH_FORM,
               		XmNbottomAttachment,    XmATTACH_FORM,
        		NULL );

	  XtVaSetValues(_histView->baseWidget(), 
			XmNleftAttachment, 	XmATTACH_FORM,
        		//XmNrightAttachment,	XmATTACH_FORM,
               		XmNtopAttachment,    	XmATTACH_FORM,
               		XmNbottomAttachment,    XmATTACH_FORM,
        		NULL );
	} else
	{ XtVaSetValues(_histView->baseWidget(), 
			XmNleftAttachment, 	XmATTACH_WIDGET,
			XmNleftWidget, 		axisView->baseWidget(),
        		XmNrightAttachment,	XmATTACH_FORM,
               		XmNtopAttachment,    	XmATTACH_FORM,
               		XmNbottomAttachment,    XmATTACH_FORM,
        		NULL );
	  XtVaSetValues(axisView->baseWidget(), 
			XmNleftAttachment, 	XmATTACH_FORM,
        		//XmNrightAttachment,	XmATTACH_FORM,
               		XmNtopAttachment,    	XmATTACH_FORM,
               		XmNbottomAttachment,    XmATTACH_FORM,
        		NULL );
	}

	_histView->manage();
        axisView->manage();
     } else
     {	XtVaSetValues(_histView->baseWidget(), 
			XmNleftAttachment, 	XmATTACH_WIDGET,
        		XmNrightAttachment,	XmATTACH_FORM,
               		XmNtopAttachment,    	XmATTACH_FORM,
               		XmNbottomAttachment,    XmATTACH_FORM,
        		NULL );
	_histView->manage();
    }

}
///////////////////////////////////////////////////////////////////
//	Destructor
///////////////////////////////////////////////////////////////////
DspHistPlain::~DspHistPlain()
{
        RemoveFormAttachments( _histView->baseWidget());
        delete _histView;

	XtDestroyWidget( _histForm) ;
	XtDestroyWidget( _titleString);
	delete _histModel;
}
///////////////////////////////////////////////////////////////////
//	updateHistView
///////////////////////////////////////////////////////////////////
void DspHistPlain::update()
{
//	GET NEW HISTOGRAM AND REDISPLAY
	int * histArray = getHistArray();
	 //// int * histArray = (*fp_getHistData)();
	for (int i = 0; i < _histModel->numBins(); i++ ) {  
		_histModel->setBin( i, histArray[i] );
	}	
	_histModel->updateViews();	   	   
}
