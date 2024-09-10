///////////////////////////////////////////////////////////////////
// TpMatchBrowseControl.cc: This component displays general qualifier 
// information and allows user to change it.  It also allows user to 
// select tiepoints by typing the point sequence number.
///////////////////////////////////////////////////////////////////
#include "TpMatchBrowseControl.h"
#include "TpSelectionMgr.h"
#include "TpQualGroup.h"
#include "TpQualGroupMgr.h"
#include "TpMatchManager.h"
#include "QuestionDialogManager.h"
#include <Xm/Frame.h>
#include <Xm/RowColumn.h>
#include <Xm/TextF.h>
#include <Xm/ArrowB.h>
#include <Xm/Label.h>
#include <assert.h>
#include <stdio.h>

String TpMatchBrowseControl::_defaults[] = {
    "*number.columns: 3",
    "*value.columns: 21", 
    "*value.maxLength: 255",
    "*prev.arrowDirection: ARROW_DOWN",
    "*next.arrowDirection: ARROW_UP",
    NULL,
};

int _displayed = 0; //!!!

TpMatchBrowseControl::TpMatchBrowseControl(Widget parent, char *name, 
			TpSelectionMgr *sm, TpMatchManager *mm)
    : UIComponent(name)
{
    _selMgr = sm;
    _matchManager = mm;
    _numberInt = 0;

    setDefaultResources(parent, _defaults);

    _w = XtVaCreateWidget(_name, xmFrameWidgetClass, parent,
                          XmNshadowThickness, 3,
                          NULL);
    installDestroyHandler();

    Widget rc = XtVaCreateManagedWidget("rc", xmRowColumnWidgetClass, _w,
					XmNorientation, XmHORIZONTAL,
					NULL);

    _numberField = XtVaCreateManagedWidget("number", 
					   xmTextFieldWidgetClass, rc, 
					   NULL);
    _valueField = XtVaCreateManagedWidget("value",
					  xmTextFieldWidgetClass, rc, 
					  NULL);

    Widget prev = XtVaCreateManagedWidget("prev", 
					  xmArrowButtonWidgetClass, rc,
					  NULL);
    Widget next = XtVaCreateManagedWidget("next", 
                                          xmArrowButtonWidgetClass, rc,
                                          NULL);

    // Declare all necessary callbacks

    XtAddCallback(_numberField, XmNactivateCallback,
                  &TpMatchBrowseControl::setNumberCallback, 
		  (XtPointer)this);

    XtAddCallback(_valueField, XmNactivateCallback, 
                  &TpMatchBrowseControl::setValueCallback, 
		  (XtPointer)this);

    XtAddCallback(prev, XmNactivateCallback, 
		  &TpMatchBrowseControl::decNumberCallback, 
		  (XtPointer)this);
    XtAddCallback(next, XmNactivateCallback, 
                  &TpMatchBrowseControl::incNumberCallback, 
		  (XtPointer)this);
}

void TpMatchBrowseControl::setNumberCallback(Widget, XtPointer clientData,
					     XtPointer)
{
    TpMatchBrowseControl *obj = (TpMatchBrowseControl *)clientData;
    obj->setNumber();
}

void TpMatchBrowseControl::setValueCallback(Widget, XtPointer clientData,
					    XtPointer)
{
    TpMatchBrowseControl *obj = (TpMatchBrowseControl *)clientData;
    obj->setValue(False);
}

void TpMatchBrowseControl::confirm(void *clientData)
{
    TpMatchBrowseControl *obj = (TpMatchBrowseControl *)clientData;
    obj->setValue(True);
}

void TpMatchBrowseControl::incNumberCallback(Widget, XtPointer clientData,
					     XtPointer)
{
    TpMatchBrowseControl *obj = (TpMatchBrowseControl *)clientData;
    obj->incNumber();
}

void TpMatchBrowseControl::decNumberCallback(Widget, XtPointer clientData,
					     XtPointer)
{
    TpMatchBrowseControl *obj = (TpMatchBrowseControl *)clientData;
    obj->decNumber();
}

/////////////////////////////////////////////////////////////////////////////
// Execute this on user typing a new running id
/////////////////////////////////////////////////////////////////////////////
void TpMatchBrowseControl::setNumber()
{
    // Get the desired match number provided by the user

    char *numberString = XmTextFieldGetString(_numberField);
    _numberInt = atoi(numberString);

    setNumber(_numberInt);
}

/////////////////////////////////////////////////////////////////////////////
// Given the new running id, check it first, then select new tiepoint and
// display correspoding general qualifier value.
/////////////////////////////////////////////////////////////////////////////
void TpMatchBrowseControl::setNumber(int)
{
    if (_matchManager->numMatches() == 0)
	_numberInt = 0;

    setNumberField("");

    if (_numberInt > _matchManager->numMatches())
        _numberInt = _matchManager->numMatches();
    else if ((_numberInt <= 0) && (_matchManager->numMatches() > 0))
        _numberInt = 1;
    
    TpMatch *match = _matchManager->getNthMatch(_numberInt);
    if (match)
	_selMgr->selectMatchAndPoints(match);
    
    char buf[16];
    sprintf(buf, "%d", _numberInt);
    setNumberField(buf);

    displayValue();
}

/////////////////////////////////////////////////////////////////////////////
// Find out general qualifier value and show it.
/////////////////////////////////////////////////////////////////////////////
void TpMatchBrowseControl::displayValue()
{
    // Get currently-selected match

    TpMatch *match = _selMgr->getMatch();
    if (match == NULL) {
	setValueField(NULL);
	return;
    }

    TpQualGroup *genQual = match->getGenQual();

    char *buf = NULL;
    if (genQual->getNumQuals() > _displayed) {
	buf = genQual->valueToString(_displayed);
    }
    else {
	buf = sdup("No qualifier defined");
    }
    setValueField(buf);
    if (buf) delete [] buf;
}

/////////////////////////////////////////////////////////////////////////////
// Execute this on user typing a new qualifier value.  Note that only one 
// general qualifier is supported right now, although TpQualGroup class
// has support for multiple qualifiers.  That is why setGenQual() function 
// always gets 1 as a second argument right now.  This should be changed 
// to the qualifier number when it will be possible to set more than one 
// qualifier. 
/////////////////////////////////////////////////////////////////////////////
void TpMatchBrowseControl::setValue(Boolean confirmed)
{
    char *valueString = XmTextFieldGetString(_valueField);

    TpMatch *match = _selMgr->getMatch();
    if (match == NULL) {
	setValueField(NULL);
	return;
    }

    TpQualGroup *genQual = match->getGenQual();

    if (genQual->getNumQuals() <= _displayed) {
	setValueField("No qualifier defined");
	return;
    }

    if (_matchManager->isCheckingGenQualUnique() && !confirmed) {
	if (!_matchManager->getGenQualMgr()->isUnique(genQual, _displayed, 
						      valueString)) {
	    char msg[] ="You are entering non-unique qualifier. Are you sure?";
	    theQuestionDialogManager->post(msg, (void *)this,
					   &TpMatchBrowseControl::confirm);
	    return;
	}
    }
    genQual->setValue(_displayed, valueString);
}

/////////////////////////////////////////////////////////////////////////////
// Execute this on user pressing increment by one button
/////////////////////////////////////////////////////////////////////////////
void TpMatchBrowseControl::incNumber()
{
    setNumber(_numberInt++);
}

/////////////////////////////////////////////////////////////////////////////
// Execute this on user pressing decrement by one button
/////////////////////////////////////////////////////////////////////////////
void TpMatchBrowseControl::decNumber()
{
    setNumber(_numberInt--);
}

/////////////////////////////////////////////////////////////////////////////
// Just a convenience function to set the running id text field.
/////////////////////////////////////////////////////////////////////////////
void TpMatchBrowseControl::setNumberField(char *text)
{
    if (text && strlen(text))
	XmTextFieldSetString(_numberField, text);
    else
	XmTextFieldSetString(_numberField, "");
}

/////////////////////////////////////////////////////////////////////////////
// Just a convenience function to set the qualifier value text field.
/////////////////////////////////////////////////////////////////////////////
void TpMatchBrowseControl::setValueField(char *text)
{
    if (text)
	XmTextFieldSetString(_valueField, text);
    else
	XmTextFieldSetString(_valueField, "No Tiepoints Selected");
}

////////////////////////////////////////////////////////////////////////////
// This function gets called every time the user made a selection of a 
// different match.
////////////////////////////////////////////////////////////////////////////
void TpMatchBrowseControl::newMatchSelected(TpMatch *match)
{
    if (match == NULL) {
	setNumberField(NULL);
	setValueField(NULL);
	return;
    }

    _numberInt = _matchManager->getMatchNumber(match);
    char buf[16];
    sprintf(buf, "%d", _numberInt);
    setNumberField(buf);

    displayValue();
}

