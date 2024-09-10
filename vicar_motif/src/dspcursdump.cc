////////////////////////////////////////////////////////////////
// DspCursDump.cc
///////////////////////////////////////////////////////////////
#include "DspCursDump.h"
#include "DspFile.h"


XtResource DspCursDump::_resources [ ] = {
  {
    "numbRows",
    "NumbRows",
    XmRInt,
    sizeof(int),
    XtOffset( DspCursDump *, _numbRows),
    XmRString,
    (XtPointer) "1",
 },
   {
    "numbColumns",
    "NumbColumns",
    XmRInt,
    sizeof(int),
    XtOffset( DspCursDump *, _numbColumns),
    XmRString,
    (XtPointer) "1",
 },
  {
    "skipRows",
    "SkipRows",
    XmRInt,
    sizeof(int),
    XtOffset( DspCursDump *, _skipRows),
    XmRString,
    (XtPointer) "1",
 },
   {
    "skipColumns",
    "SkipColumns",
    XmRInt,
    sizeof(int),
    XtOffset( DspCursDump *, _skipColumns),
    XmRString,
    (XtPointer) "1",
 }
};
    
///////////////////////////////////////////////////////////////
//	CONSTRUCTOR
///////////////////////////////////////////////////////////////
DspCursDump::DspCursDump(	Widget       		parent,
				char *			name,
				ImageData * 		imageData,
				ImageDisplayView *	imageView,
				unsigned char 	 	bitFlags,
				int			pixelSize )
{ 
//	SAVE ARGS
	_imageData = imageData;
	_imageView = imageView;
	_bitFlags = bitFlags;
	_pixelSize = pixelSize;
	
// 	INIT VARIABLES
        XtGetSubresources ( parent, 
                       (XtPointer) this, name,
                       className(),_resources, 
                       XtNumber( _resources), NULL, 
                       0 );
        
        _numbLines   = imageData->getNumbLines();
	_numbSamples = imageData->getNumbSamples();
	_midRow = (_numbRows - 1) / 2;
	_midColumn = (_numbColumns - 1) / 2;
	        
	
//	DETERMINE WHICH BANDS TO DISPLAY
     	if ((_numbColumns != 0) || (_numbRows != 0 )) {
	
// 		GET COLOR MODE INFO.  
		ModeType mode =  _imageData->getMode(); 

// 		DECIPHER BIT FLAGS
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
     	 }
     	 
//	INIT FILENAME
   	strcpy( _fileName, "/tmp/CursorDump");			
}	
///////////////////////////////////////////////////////////////
//	DESTRUCTOR
///////////////////////////////////////////////////////////////
DspCursDump::~DspCursDump()
{
	Widget iw = _imageView->getWidget() ;
	XtRemoveCallback ( iw, 
                XvicNinputCallback,
                &DspCursDump::writeCursorValuesCallback,
                ( XtPointer ) this );		
}
///////////////////////////////////////////////////////////////
//	START THE DUMP PROCEDURE
///////////////////////////////////////////////////////////////
void DspCursDump::dump(char * fileName)
{

// 	GET FILE NAME 
	if (fileName != NULL) 
	  if ((strcmp(fileName,"") != 0) &&(strlen(fileName) < (size_t) MAX_NAMESTRINGLENGTH))
		strcpy( _fileName, fileName);

	unsigned char 	cursorMode;
	int 		x,y;
	Widget 		iw = _imageView->getWidget() ;	

//	IF CURSOR IS PLANTED, GET CURSOR LOCATION AND JUMP TO RTN
	XtVaGetValues( 	iw, 
                        XvicNcursorMode, &cursorMode,
                        XvicNcursorX,	 &x,
                        XvicNcursorY,    &y,
                        NULL);
        if (cursorMode == XvicPLANTED ) {
        	writeCursorValues( x, y );
        }
 

//	ELSE, ADD CALLBACK FOR MOUSE BTN PUSH
	else { 
	    	XtAddCallback ( iw,     
                	XvicNinputCallback,
                	&DspCursDump::writeCursorValuesCallback,
                	( XtPointer ) this );
        }
                
     
}
///////////////////////////////////////////////////////////////
//      writeCursorValuesCallback:             
///////////////////////////////////////////////////////////////
void DspCursDump::writeCursorValuesCallback(Widget, 
                                      XtPointer client_data, 
                                      XtPointer call_data)
{
//	GET ARGS
        DspCursDump * obj = (DspCursDump *) client_data;
        XvicImageCallbackStruct * cb = ( XvicImageCallbackStruct *) call_data;
        
//      GET CURSOR INPUT
	if (cb->reason == XvicCR_INPUT) {
		obj->writeCursorValues( cb->x, cb->y );
        }
        
}
///////////////////////////////////////////////////////////////
//      Get Cursor Input 
///////////////////////////////////////////////////////////////
void DspCursDump::writeCursorValues(int cursorX, int cursorY )
{		

//      REMOVE CALLBACK (We only want to dump once)
	Widget iw = _imageView->getWidget() ;
        XtRemoveCallback ( iw, 
                		XvicNinputCallback,
                		&DspCursDump::writeCursorValuesCallback,
                		( XtPointer ) this );
                
                
//	INIT BUFFER AND STRINGS
	int i;
	int len = sizeof( _blankString ) / sizeof(_blankString[0]);
	for ( i=0; i < len; i++ ) {
		_blankString[i]= ' ';
		_pixelBuffer[i]='\0';
	}

	_blankString[len-1]='\0';	
	_blankString[(_pixelSize+1)] = '\0';
	_lengthOfPixelText = maxSizeOfPixelText(_pixelSize);
	
	_titleLines = 3;
	if (_redDnEnable   == True ) _titleLines += 3;
	if (_greenDnEnable == True ) _titleLines += 3;
	if (_blueDnEnable  == True ) _titleLines += 3;
	if (_bwDnEnable    == True ) _titleLines += 3;
	_logText = new char[((_numbRows * _numbColumns 
				* (_lengthOfPixelText+3)) 
				+ (132*_titleLines))];
                
       	char buf[1000];
       	int j;
       	int x,y;
       	
       	if ((_skipColumns > 1) || (_skipRows > 1)) {
		x = (cursorX) - ((cursorX)%_skipColumns);
		y = (cursorY) - ((cursorY )%_skipRows);
	}
	else {
		x = cursorX - _midColumn;
		y = cursorY - _midRow;
	}
	_logText[0] = '\n';

	if ((_numbColumns != 0) && (_numbRows != 0 )) {
	
//	   UPDATE RED DN
	   if (_redDnEnable) {
	      printDumpHeader( "RED", _lengthOfPixelText, _logText, x );
	      for (i=0; i<_numbRows; i++){
	        printLineNumber( (y+i), _logText);
           	for (j=0; j<_numbColumns; j++) {
           	   getValueString( RED,x+j, y+i, buf);
		   updateValue( buf, j, _numbColumns, _logText );
		}
	      }
	      strcat(_logText, "\n");
	   }

//	   UPDATE GREEN DN
	   if (_greenDnEnable){
	      printDumpHeader( "GREEN", _lengthOfPixelText, _logText, x );
	      strcat(_logText, buf);
	      for (i=0; i<_numbRows; i++) {
	        printLineNumber( (y+i), _logText);
           	for (j=0; j<_numbColumns; j++){
		   getValueString( GREEN, x+j, y+i, buf);
		   updateValue( buf, j, _numbColumns, _logText   );
		}
	      }
	      strcat(_logText, "\n");
	   }

//	   UPDATE BLUE DN
	   if (_blueDnEnable){
	      printDumpHeader( "BLUE", _lengthOfPixelText, _logText, x );
	      strcat(_logText, buf);
	      for (i=0; i<_numbRows; i++) {
	        printLineNumber( (y+i), _logText);
           	for (j=0; j<_numbColumns; j++){
           	   getValueString( BLUE, x+j, y+i, buf);
		   updateValue( buf, j, _numbColumns, _logText   );
	   	}
	      }
	      strcat(_logText, "\n");
	   }

//	   UPDATE BW DN
	   if (_bwDnEnable){
	      printDumpHeader( "BLACK AND WHITE", _lengthOfPixelText, _logText, x );
	      strcat(_logText, buf);
	      for (i=0; i<_numbRows; i++) {
	        printLineNumber( (y+i), _logText);
           	for (j=0; j<_numbColumns; j++){
           	   getValueString( BWcolor, x+j, y+i, buf);
		   updateValue( buf, j, _numbColumns, _logText  );
		}
	      }
	      strcat( _logText, "\n");
	   }
	   
//	   OPEN/CREATE FILE
	   _file = new DspFile();
	   
//	   PRINT TO FILE
	   _file->open( _fileName,strlen(_logText) ); 
	   _file->writeAllBytes(  strlen(_logText), _logText);
	   _file->close( );
	   std::cout << _logText << std::endl;
	
//	   CLEANUP/DELETE
	   delete _logText;
	   delete _file;
	}
}
///////////////////////////////////////////////////////////////
//	getValueString()
//		local function for getting a dn value and
//		its string equivalent.  Called once for 
//		each DN.
///////////////////////////////////////////////////////////////
void DspCursDump::getValueString( ColorType color,  
					int x, int y, 
					char * newValueString )
{
	unsigned long int dn;
	StatusType  status = imFAILURE;
        int i;
	
	sprintf ( newValueString, " " );
	if ((x >= 0 && x <  _numbSamples) && ( y >= 0 && y < _numbLines ))
		status = _imageData->readPixel( color, x, y, _pixelBuffer );
	if ( status == imSUCCESS ) {
		dn = 0;
		for (i=0; i < _pixelSize; i++) 
			dn = ((unsigned long int) (dn * (256)) + ((unsigned long int)_pixelBuffer[i]));
		sprintf ( newValueString, "%*ld", ((int)_lengthOfPixelText), dn );
	}
	else {		   
		for (i=0; i < _pixelSize; i++ )
			newValueString[i]= '*';
		newValueString[i] = '\0';
	}
	
}
///////////////////////////////////////////////////////////////
//	getColumnString()
//		local function for getting the x or location
///////////////////////////////////////////////////////////////
void DspCursDump::getColumnString( int x, char * newValueString, int lastColumn )
{
        sprintf ( newValueString, "%s", "" );
	if ( x <= lastColumn ) 
		sprintf ( newValueString, "%*d", ((int)_lengthOfPixelText), x );	
}
///////////////////////////////////////////////////////////////
//	UPDATE VALUE (ADDs VALUE TO LOG TEXT)	(protected)
///////////////////////////////////////////////////////////////
void DspCursDump::updateValue(char * buf, int currentColumn, int lastColumn, char * outBuf)
{
	strcat( outBuf, buf);
	if ((currentColumn < (lastColumn-1)) && (currentColumn <_numbSamples)) {
		strcat( outBuf, ", ");
	}
	if ((currentColumn >= (lastColumn-1)) )  {
		strcat( outBuf, "\n" );
	}
	if ((currentColumn < (lastColumn-1)) && (currentColumn >=_numbSamples)) {
		strcat( outBuf, "  ");
	}
}
///////////////////////////////////////////////////////////////
//	GET MAX SIZE OF PIXEL TEXT (protected)
///////////////////////////////////////////////////////////////
int DspCursDump::maxSizeOfPixelText(int pixelSize)
{
        int charLength;

	int maxPixelValue = (int) (pow((double)2,(8*pixelSize)));
	for (charLength=0; charLength <132 && maxPixelValue > 0; charLength++) {
		maxPixelValue = maxPixelValue / 10;
	}
	
	return ( charLength );
}	
///////////////////////////////////////////////////////////////
//	PRINT DUMP HEADER (protected)
///////////////////////////////////////////////////////////////
void DspCursDump::printDumpHeader( 	char * 	colorText, 
					int 	lengthOfPixelText, 
					char * 	outBuf,
					int	x)
{
char buf[500];
int i;
char leadingBlanks[500];

//	CREATE LEADING BLANKS 
	strcpy(leadingBlanks, "\0");
	for (i = 0; i < (3 + lengthOfPixelText); i++ ) 
		strcat( leadingBlanks, " ");	

//	START AT BEGINNING OF NEXT LINE
	strcat ( outBuf, "\n");
		
//	PRINT TITLE/TYPE OF DUMP
	strcat( outBuf, leadingBlanks);
	strcat( outBuf, colorText );
	strcat( outBuf, " PIXEL DUMP:  \n");
	
//	CREATE TABLE-COLUMN HEADER
	strcat( outBuf, leadingBlanks);
	for (i=0; i < _numbColumns; i++) { 
		getColumnString( (x+i), buf, _numbSamples);
	      	updateValue( buf, i, _numbColumns, outBuf );   	
	}
	
//	DRAW UNDERLINE UNDER TABLE-COLUMN HEADER
	strcat( outBuf, leadingBlanks);
	for (i=0; i < (_numbColumns * (lengthOfPixelText+2)); i++ )
		strcat(outBuf, "_");
	strcat(outBuf, "\n");
}
///////////////////////////////////////////////////////////////
//	PRINT LINE NUMBER (protected)
///////////////////////////////////////////////////////////////
void DspCursDump::printLineNumber( int lineNumb, char * outBuf)
{
char buf[500];

	getColumnString( lineNumb , buf, _numbLines );
	strcat( outBuf, buf);
	strcat( outBuf, "  |");
}
