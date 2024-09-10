////////////////////////////////////////////////////////////////
// DspCursorView.cc
//	Contains corrections to CursorDnView in GUI
//	   (1) use of gadget instead of widget
//	   (2) allows matrix of pixels to be displayed
//	   (3) getValueString allows pixel sizes larger 
//		than a byte.
///////////////////////////////////////////////////////////////
#include "DspCursorView.h"
#include "ImageData.h"
#include <Xm/LabelG.h>
#include <Xm/RowColumn.h>
#include <Xm/TextF.h>
#include <Xm/Form.h>
#include <Xm/Xm.h>
#include <string.h>
#include <stdio.h>


XtResource DspCursorView::_resources [ ] = {
  {
    "numbRows",
    "NumbRows",
    XmRInt,
    sizeof(int),
    XtOffset( DspCursorView *, _numbRows),
    XmRString,
    (XtPointer) "1",
 },
   {
    "numbColumns",
    "NumbColumns",
    XmRInt,
    sizeof(int),
    XtOffset( DspCursorView *, _numbColumns),
    XmRString,
    (XtPointer) "1",
 },
  {
    "skipRows",
    "SkipRows",
    XmRInt,
    sizeof(int),
    XtOffset( DspCursorView *, _skipRows),
    XmRString,
    (XtPointer) "1",
 },
   {
    "skipColumns",
    "SkipColumns",
    XmRInt,
    sizeof(int),
    XtOffset( DspCursorView *, _skipColumns),
    XmRString,
    (XtPointer) "1",
 }
};

///////////////////////////////////////////////////////////////
//	CONSTRUCTOR
//
///////////////////////////////////////////////////////////////
DspCursorView::DspCursorView(
				Widget parent, 
				char * name,
				CursorModel * cursorModel,
				ImageData * imageData,
				unsigned char bitFlags,
				int pixelSize)
			: CursorBasicView(parent, name, cursorModel, imageData, bitFlags) 
{ 
int i;

// 	INIT VARIABLES
        getResources(_resources, XtNumber( _resources) );        
	_midRow = (_numbRows - 1) / 2;
	_midColumn = (_numbColumns - 1) / 2;
	_numbLines   = imageData->getNumbLines();
	_numbSamples = imageData->getNumbSamples();
	
        
//	SAVE ARGS
	_imageData = imageData;
	_pixelSize = pixelSize;
	
	int len = sizeof( _blankString ) / sizeof(_blankString[0]);
	for ( i=0; i < (len-1); i++ ) {
		_blankString[i]= ' ';
		_pixelBuffer[i]='\0';
	}
	_blankString[i]='\0';	
	_blankString[(_pixelSize+1)] = '\0';
	_pixelBuffer[i] = '\0';
	
//	GET numColumns from Resources for numb columns in row/column widget
	XtVaGetValues ( _w, XmNnumColumns, &_numColumnsResource, NULL );
	if ((_numbColumns != 0) && (_numbRows != 0 ) && (_numColumnsResource != 0)) {
	
	_labelRed = new Widget*[ _numbColumns ];
	_labelGrn = new Widget*[ _numbColumns ];
	_labelBlu = new Widget*[ _numbColumns ];
	_labelBw  = new Widget*[ _numbColumns ];
	for ( i=0; i<_numbColumns; i++) {
		_labelRed[i] = new Widget[ _numbRows ];
		_labelGrn[i] = new Widget[ _numbRows ];
		_labelBlu[i] = new Widget[ _numbRows ];
		_labelBw[i]  = new Widget[ _numbRows ];
	}
	
	_textRed = new Widget*[ _numbColumns ];
	_textGrn = new Widget*[ _numbColumns ];
	_textBlu = new Widget*[ _numbColumns ];
	_textBw  = new Widget*[ _numbColumns ];
	for ( i=0; i<_numbColumns; i++) {
		_textRed[i] = new Widget[ _numbRows ];
		_textGrn[i] = new Widget[ _numbRows ];
		_textBlu[i] = new Widget[ _numbRows ];
		_textBw[i]  = new Widget[ _numbRows ];
	}
	
	_formRed = new Widget*[ _numbColumns ];
	_formGrn = new Widget*[ _numbColumns ];
	_formBlu = new Widget*[ _numbColumns ];
	_formBw  = new Widget*[ _numbColumns ];
	for ( i=0; i<_numbColumns; i++) {
		_formRed[i] = new Widget[ _numbRows ];
		_formGrn[i] = new Widget[ _numbRows ];
		_formBlu[i] = new Widget[ _numbRows ];
		_formBw[i]  = new Widget[ _numbRows ];
	}
	
	createDisplay();
	_glue = new DspCurs2ImageGlue(imageData,this);    
	} 		

}	
///////////////////////////////////////////////////////////////
//	DESTRUCTOR
//
///////////////////////////////////////////////////////////////
DspCursorView::~DspCursorView()
{

	if (_glue)
		delete _glue;
	
	if ((_numbColumns != 0) || (_numbRows != 0 ) || (_numColumnsResource != 0)) 
		removeCursorDisplays();
			
}
///////////////////////////////////////////////////////////////
//      addNewSubView:
//              May be called by "createCursorDisplays"
//              function in subclass to create a single
//              display group consisting of: label and a
//              textfield. The textfield is used to show
//              values that are updated as the cursor
//              moves around in the image. A name string is
//              automatically created as :
//              "label" +  <displayName>,   etc.
///////////////////////////////////////////////////////////////
void DspCursorView::addNewSubView( Widget w, char * displayName,
        Widget *form, Widget * label, Widget * textfield, Boolean enabled )
{
        char name[132];
 
        if (enabled) {
                *form = XtVaCreateManagedWidget(
                			"form", xmFormWidgetClass, w, NULL);
 
//		CREATE VALUE DISPLAY(as a text field)
                strcpy( name, "textfield"); strcat( name, displayName);
                *textfield = XtVaCreateManagedWidget(name, 
                        xmTextFieldWidgetClass,*form,
                        XmNtopAttachment, XmATTACH_FORM,
                        XmNbottomAttachment, XmATTACH_FORM,
                        XmNrightAttachment, XmATTACH_FORM,
                        NULL);
 
//      	CREATE LABEL
                strcpy( name, "label"); strcat( name, displayName);
                *label  = XtVaCreateManagedWidget ( name, 
                        xmLabelGadgetClass, *form,
                        XmNtopAttachment, XmATTACH_FORM,
                        XmNbottomAttachment, XmATTACH_FORM,
                        XmNrightAttachment, XmATTACH_WIDGET,
                        XmNrightWidget, *textfield,
                        NULL );
        }
}
///////////////////////////////////////////////////////////////
//	getValueString()
//		local function for getting a dn value and
//		its string equivalent.  Called once for 
//		each DN.
///////////////////////////////////////////////////////////////
void DspCursorView::getValueString( ColorType color,  
					int x, int y, 
					char * newValueString , 
					Boolean enabled  )
{
	unsigned long int dn;
	StatusType  status;
        int i;

	sprintf ( newValueString, " " );
	if ( enabled) {
		if ((x >= 0 && x <  _numbSamples) && ( y >= 0 && y < _numbLines )) 
			status = _imageData->readPixel( color,  x,  y,  _pixelBuffer );
		else 
			status = imFAILURE;
		if ( status == imSUCCESS ) {
			for (i=0, dn=0; i< _pixelSize; i++) 
				dn = ((unsigned long int)  (dn * (256)) + ((unsigned long int)_pixelBuffer[i]));
			sprintf ( newValueString, "%-*ld", ((int)_columnsResource), dn );			
		}
		else {		   
		    	for (i=0; i < _pixelSize; i++ )
				newValueString[i]= '*';
		    	newValueString[i] = '\0';
		}
	}
}
///////////////////////////////////////////////////////////////
//	cursorMoved() .. implements pure virtual in abstract
//		updates each DN display with new DN
//		values under cursor.
///////////////////////////////////////////////////////////////
void DspCursorView::cursorMoved(XvicImageCallbackStruct * cb )
{
       	char buf[132];
       	int i,j;
       	int x,y;
       	
       	if ((_skipColumns > 1) || (_skipRows > 1)) {
		x = (cb->x) - ((cb->x)%_skipColumns);
		y = (cb->y) - ((cb->y )%_skipRows);
	}
	else {
		x = cb->x - _midRow;
		y = cb->y - _midColumn;
	}

	if ((_numbColumns != 0) && (_numbRows != 0 ) && (_numColumnsResource != 0)) {
	
//	UPDATE RED DN
	if (_redDnEnable) 
	   for (i=0; i<_numbColumns; i++)
           	for (j=0; j<_numbRows; j++) {
		   getValueString( RED,x+i, y+j, buf, _redDnEnable);
		   updateValue( _textRed[i][j], buf, _redDnEnable );
		}

//	UPDATE GREEN DN
	if (_greenDnEnable)
	   for (i=0; i<_numbColumns; i++)
        	for (j=0; j<_numbRows; j++) {
		   getValueString( GREEN,x+i, y+j, buf, _greenDnEnable);
		   updateValue( _textGrn[i][j], buf, _greenDnEnable );
		}

//	UPDATE BLUE DN
	if (_blueDnEnable)
	   for (i=0; i<_numbColumns; i++)
           	for (j=0; j<_numbRows; j++) {
		   getValueString( BLUE, x+i, y+j, buf, _blueDnEnable );
		   updateValue( _textBlu[i][j], buf, _blueDnEnable );
	   	}

//	UPDATE BW DN
	if (_bwDnEnable)
	   for (i=0; i<_numbColumns; i++)
	   	for (j=0; j<_numbRows; j++) {
		   getValueString( BWcolor, x+i, y+j, buf, _bwDnEnable);
		   updateValue( _textBw[i][j], buf, _bwDnEnable);
		}
		
      }
}
///////////////////////////////////////////////////////////////
//	createCursorDisplays()
//		
///////////////////////////////////////////////////////////////
void DspCursorView::createCursorDisplays()
{
     int i,j;

     if ((_numbColumns != 0) || (_numbRows != 0 ) || (_numColumnsResource != 0)) {
	

// 	GET COLOR MODE INFO.  
//		When mode=bw, we disable the rgb displays, and vice versa.
	ModeType mode =  _imageData->getMode(); 

// 	DECIPHER BIT FLAGS
	_redDnEnable = False;
	_greenDnEnable = False;
	_blueDnEnable = False;
	_bwDnEnable = False;
	if ( _bitFlags && (unsigned char) 1  && mode == COLORmode )
		_redDnEnable = True;
	if ( _bitFlags && (unsigned char) 2  && mode == COLORmode )
		_greenDnEnable = True;
	if ( _bitFlags && (unsigned char) 4  && mode == COLORmode )
		_blueDnEnable =   True;
	if ( _bitFlags && (unsigned char) 8  && mode == BWmode )
		_bwDnEnable = True  ;

//	CREATE RED DN DISPLAY
	for (i=0; i<_numbColumns; i++)
           for (j=0; j<_numbRows; j++)
		addNewSubView( _w, "RedDn",
			&_formRed[i][j], &_labelRed[i][j], &_textRed[i][j],
			_redDnEnable );

//	CREATE GREEN DN DISPLAY
	for (i=0; i<_numbColumns; i++)
           for (j=0; j<_numbRows; j++)
		addNewSubView( _w, "GrnDn",
			&_formGrn[i][j], &_labelGrn[i][j], &_textGrn[i][j],
			_greenDnEnable );

//	CREATE BLUE DN DISPLAY
	for (i=0; i<_numbColumns; i++)
           for (j=0; j<_numbRows; j++)
		addNewSubView( _w, "BluDn",
			&_formBlu[i][j], &_labelBlu[i][j], &_textBlu[i][j],
			_blueDnEnable);

//	CREATE BW DN DISPLAY
	for (i=0; i<_numbColumns; i++)
           for (j=0; j<_numbRows; j++)
		if ((_skipColumns == 1) && (_skipRows == 1)
			&& (i == _midColumn) && (j == _midRow) )	
		   addNewSubView( _w, "BwDnCross",
			&_formBw[i][j], &_labelBw[i][j], &_textBw[i][j],
			_bwDnEnable );
		else 
		   addNewSubView( _w, "BwDn",
                        &_formBw[i][j], &_labelBw[i][j], &_textBw[i][j],
                        _bwDnEnable );
                        
//	GET WIDTH OF TEXT WIDGET
	XtVaGetValues ( _textBw[0][0], XmNcolumns, &_columnsResource, NULL );
// _columnsResource = 3; // kluge
	                    
      }
}

///////////////////////////////////////////////////////////////
//	blankSubViews()
///////////////////////////////////////////////////////////////
void DspCursorView::blankSubViews()
{
     int i,j;
    
     if ((_numbColumns != 0) || (_numbRows != 0 ) || (_numColumnsResource != 0)) {
	
//	BLANK EACH VIEW BY DISPLAYING A BLANK STRING IN EACH
	for (i=0; i<_numbColumns; i++) 
	    for (j=0; j<_numbRows; j++) {
           
// 		BLANK OUT RED DN
		updateValue( _textRed[i][j], _blankString, _redDnEnable  );

// 		BLANK OUT  GREEN DN
		updateValue( _textGrn[i][j], _blankString,  _greenDnEnable);

// 		BLANK OUT  BLUE DN
		updateValue( _textBlu[i][j], _blankString, _blueDnEnable );

// 		BLANK OUT B&W DN
		updateValue( _textBw[i][j], _blankString, _bwDnEnable );
	   }
     }	   
}
///////////////////////////////////////////////////////////////
//	removeCursorDisplays()
//		Removes widgets created by createCursorDisplays.
///////////////////////////////////////////////////////////////
void DspCursorView::removeCursorDisplays()
{
        int i,j;
	
	for (j=0; j<_numbRows; j++)
           for (i=0; i<_numbColumns; i++) {
       		removeSubView_(_formRed[i][j], _labelRed[i][j], _textRed[i][j]);
		removeSubView_(_formGrn[i][j], _labelGrn[i][j], _textGrn[i][j]);
  		removeSubView_(_formBlu[i][j], _labelBlu[i][j], _textBlu[i][j]);
		removeSubView_(_formBw[i][j],  _labelBw[i][j], _textBw[i][j]);
	   }
}

///////////////////////////////////////////////////////////////
//      removeSubView_:
//              Removes widgets created by addNewSubView.
///////////////////////////////////////////////////////////////
void DspCursorView::removeSubView_(Widget form, Widget label,
                                Widget textfield)
{
        if (form) {
                XtUnmanageChild(form);
                XtDestroyWidget(form);
        }
        XtDestroyWidget(label);
        XtDestroyWidget(textfield);
        form = NULL;
        label = NULL;
        textfield = NULL;
}
