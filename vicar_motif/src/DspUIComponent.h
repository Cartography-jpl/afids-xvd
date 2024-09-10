///////////////////////////////////////////////////////////////////
//  DspUIComponent.h
///////////////////////////////////////////////////////////////////
#ifndef DSPUICOMPONENT_H
#define DSPUICOMPONENT_H
#include "UIComponent.h"
#include "ViewMacros.h"

class DspUIComponent : public UIComponent {

protected:

//	ATTACHING/DETACHING WIDGETS TO/FROM LIST
	Widget * 		_attachmentList; // widgets attached
	int			_numbAttachments;
	virtual void		attachWidgetToForm(
					Widget w,
					unsigned char  	attachTop = XmATTACH_FORM, 
					Widget		wTop = NULL,
					unsigned char  	attachLeft = XmATTACH_FORM, 
					Widget		wLeft = NULL,
					unsigned char  	attachRight = XmATTACH_FORM, 
					Widget		wRight = NULL,
					unsigned char  	attachBottom = XmATTACH_FORM, 
					Widget		wBottom = NULL);
	virtual void		detachAllWidgetsFromForm();
	virtual void		RemoveFormAttachments( Widget w );

//	ATTACHING/DETACHING VIEWS FOR UPDATING AND LATER DELETING	
	int			_numbViews;
	DspUIComponent **	_views;	 // views to be updated	
	virtual void 		attachView(DspUIComponent * view );
	virtual void 		detachView(DspUIComponent * view );
	virtual void		updateViews();
	virtual void		deleteViews(); // called by this class destructor
	
//	ADDING/REMOVING CALLBACKS
	struct CallbackList{ 
				Widget 		widget; 
				char  		callback_name[500];
				XtCallbackProc 	routine;};
	CallbackList **		_callbackList;
	int			_numbCallbacks;
	virtual void		addCallback( Widget w, String callback_name, XtCallbackProc routine);
	virtual void		removeCallback( Widget w, String callback_name, XtCallbackProc routine);
	virtual void		removeAllCallbacks();
	
//	CONSTRUCTOR .. CALLED BY SUBCLASS
	DspUIComponent(char * name );


public:

	~DspUIComponent();

//	UPDATE .. CALLED BY PARENT USING ABOVE VIEW ATTACHMENTS
	virtual void update() {} // pure virtual .. sort of
	
	
//	CLASSNAME
	virtual const char *const className() { return "DspUIComponent" ; } 

};
#endif
