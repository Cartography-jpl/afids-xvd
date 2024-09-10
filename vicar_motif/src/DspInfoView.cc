///////////////////////////////////////////////////////////////////
//  DspInfoView.cc:
///////////////////////////////////////////////////////////////////
#include "DspInfoView.h"

String DspInfoView::_defaults [ ] = {
       NULL,
};

///////////////////////////////////////////////////////////////////
//	CONSTRUCTOR
///////////////////////////////////////////////////////////////////
DspInfoView::DspInfoView(Widget parent, char *name, void (*fp)(char*)) 
			: DspUIComponent(name)
{
	setDefaultResources( parent, _defaults );
	_fp = fp;
	(*_fp) (_text);
	int n = 0;
	Arg args[20];
	XtSetArg(args[n], XmNeditable, 	False );   n++;
	XtSetArg(args[n], XmNeditMode,	XmMULTI_LINE_EDIT); n++;	
	XtSetArg(args[n], XmNvalue, _text); n++;
	_w = XtCreateWidget( _name, xmTextWidgetClass, parent, args, n);
	installDestroyHandler();
}
///////////////////////////////////////////////////////////////////
//	DESTRUCTOR
///////////////////////////////////////////////////////////////////
DspInfoView::~DspInfoView()
{
	delete _blankString;
	delete _stringBuffer;
}
///////////////////////////////////////////////////////////////////
//	update:  
//		inserts new string
///////////////////////////////////////////////////////////////////
void DspInfoView::update()
{	
	(*_fp) (_text);
	
	// WRITE NEW STRING
        XtVaSetValues (  _w, XmNvalue, _text, NULL);
}
///////////////////////////////////////////////////////////////////
//	
///////////////////////////////////////////////////////////////////
char *	DspInfoView::getStringBuffer()
{
	strcpy( _stringBuffer, _blankString);
	return _stringBuffer;
}
