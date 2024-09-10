///////////////////////////////////////////////////////////////////
//  DspUIComponent.cc  .. a superclass to view type subclass.
///////////////////////////////////////////////////////////////////
#include "DspUIComponent.h"


////////////////////////////////////////////////////////////////
//      Constructor
////////////////////////////////////////////////////////////////
DspUIComponent::DspUIComponent(char * name) : UIComponent(name)
{
	_numbViews = 0;
	_views = 0;
	_numbAttachments = 0;
	_numbCallbacks = 0;
	_attachmentList = 0;						     
}

////////////////////////////////////////////////////////////////
//     Destructor 
////////////////////////////////////////////////////////////////
DspUIComponent::~DspUIComponent()
{
	removeAllCallbacks();
	detachAllWidgetsFromForm();
      	if ( _views != 0 )
      		deleteViews();
}

////////////////////////////////////////////////////////////////
//      attachView
//		Unlike GUI, this is called by the parent object
//		on the child object 
////////////////////////////////////////////////////////////////
void DspUIComponent::attachView(DspUIComponent * view)
{
	AttachViewMacro( DspUIComponent, _views, _numbViews, view);
//      view->update ( );
}
////////////////////////////////////////////////////////////////
//      detachView
////////////////////////////////////////////////////////////////
void DspUIComponent::detachView (DspUIComponent *view)
{
 	DetachViewMacro( DspUIComponent, _views, _numbViews, view )
}
 
////////////////////////////////////////////////////////////////
//      updateViews
//		Calls update on all attached views
////////////////////////////////////////////////////////////////
void  DspUIComponent::updateViews()
{
      for (int i=0; i<_numbViews; i++)
              _views[i]->update( );
}

////////////////////////////////////////////////////////////////
//      deleteViews  
//		Deletes all views on the list
////////////////////////////////////////////////////////////////
void  DspUIComponent::deleteViews()
{
      	for (int i=(_numbViews-1); i>=0; i--) {
		detachView ((DspUIComponent *) _views[i]);
                delete _views[i];
                _views[i] = 0;
	}
}

////////////////////////////////////////////////////////////////
//      attachWidgetToForm
////////////////////////////////////////////////////////////////
void DspUIComponent::attachWidgetToForm(
				Widget w,
				unsigned char  	attachTop , 
				Widget		wTop ,
				unsigned char  	attachLeft , 
				Widget		wLeft ,
				unsigned char  	attachRight , 
				Widget		wRight ,
				unsigned char  	attachBottom , 
				Widget		wBottom 
			)
{
//	FIRST ATTACH WIDGET ACCORDING TO ARGS
        XtVaSetValues   ( w,
        	     XmNtopWidget,	   wTop,
                     XmNtopAttachment,     attachTop,
                     XmNleftWidget,	   wLeft,
                     XmNleftAttachment,    attachLeft,
                     XmNrightWidget,	   wRight,
                     XmNrightAttachment,   attachRight,
                     XmNbottomWidget,	   wBottom,
                     XmNbottomAttachment,  attachBottom,                     
                     NULL );	

//	ADD ATTACHED WIDGET TO LIST (TO REMOVE LATER)
        Widget * newAttachmentList = new Widget[(_numbAttachments+1)];
        for (int i = 0; i< _numbAttachments; i++) 
        	newAttachmentList[i] = _attachmentList[i];
        if (_numbAttachments > 0) delete [] _attachmentList;
        _attachmentList = newAttachmentList;
        _attachmentList[_numbAttachments] = w;
        _numbAttachments++;
}

////////////////////////////////////////////////////////////////
//      detachAllWidgetsFromForm .. called by subclass destructor
////////////////////////////////////////////////////////////////
void DspUIComponent::detachAllWidgetsFromForm()
{
	if (_attachmentList != 0) {
		for (int i=(_numbAttachments-1); i>=0; i--)
			RemoveFormAttachments( _attachmentList[i] );
		delete [] _attachmentList;
		_attachmentList = 0;		
	}
	_numbAttachments = 0;
}

////////////////////////////////////////////////////////////////
// 	RemoveFormAttachments     
//			detaches any widget from parent form
///////////////////////////////////////////////////////////////
void DspUIComponent::RemoveFormAttachments( Widget w )  
{
    if ( w != 0 ) 
	XtVaSetValues ( w,	
		XmNtopWidget,		XmATTACH_NONE,	
          	XmNbottomAttachment, 	XmATTACH_NONE,	
		XmNleftAttachment, 	XmATTACH_NONE,	
          	XmNrightAttachment,	XmATTACH_NONE,	
		NULL );   
}

////////////////////////////////////////////////////////////////
// 	addCallback
///////////////////////////////////////////////////////////////
void DspUIComponent::addCallback( Widget w, String callback_name, 
					XtCallbackProc routine )
{
//	ADD CALLBACK TO LIST
	CallbackList **	newCallbackList;
	newCallbackList = new CallbackList*[(_numbCallbacks+1)];
	for ( int i = 0; i < _numbCallbacks; i++ ) 
		newCallbackList[i] = _callbackList[i];
	if (_numbCallbacks > 0) 
		delete []_callbackList;
	_callbackList = newCallbackList;

// 	CREATE NEW IMAGE (AT END OF LIST)
	_callbackList[_numbCallbacks] = new CallbackList;
	_callbackList[_numbCallbacks]->widget = w;
	strcpy(_callbackList[_numbCallbacks]->callback_name,callback_name);
	_callbackList[_numbCallbacks]->routine = routine;	
		
//	DO CALLBACK
	XtAddCallback( w, callback_name, routine, (XtPointer) this);

//	INC COUNTER
	_numbCallbacks++;
}

////////////////////////////////////////////////////////////////
// 	removeCallback .. doesn't need to be called except in this class
///////////////////////////////////////////////////////////////
void DspUIComponent::removeCallback( Widget w, String callback_name, 
					XtCallbackProc routine )

{	
	CallbackList **	newCallbackList;
	newCallbackList = new CallbackList*[(_numbCallbacks-1)];
	for (int i=0, j=0; i< _numbCallbacks; i++ ) {
		if ((_callbackList[j]->widget != w) &&
				(strcmp(_callbackList[j]->callback_name, callback_name)!= 0) &&
				(_callbackList[j]->routine != routine)) {
			newCallbackList[i] = _callbackList[j];
			j++;
		}
		else {
			XtRemoveCallback(w, callback_name, routine, (XtPointer)this);
		}
	}
	if (_numbCallbacks > 0)
		delete []_callbackList;
	_callbackList = newCallbackList;
	_numbCallbacks--;
}
////////////////////////////////////////////////////////////////
// 	removeAllCallback
///////////////////////////////////////////////////////////////
void DspUIComponent::removeAllCallbacks()
{

	for (int i=(_numbCallbacks-1); i>=0; i--) {
		XtRemoveCallback( 
			_callbackList[i]->widget,
			_callbackList[i]->callback_name,
			_callbackList[i]->routine,
			(XtPointer) this );
	}
	_numbCallbacks = 0;
	delete [] _callbackList;
}
