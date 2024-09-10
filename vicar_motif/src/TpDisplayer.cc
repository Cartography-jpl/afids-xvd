//////////////////////////////////////////////////////////////////////////////
// TpDisplayer.cc: This class manages multiple subdisplayer windows, of which
// at most three can be displayed on the screen at one time.
//////////////////////////////////////////////////////////////////////////////
#include "TpDisplayer.h"
#include "TpImageReference.h"
#include "TpSubDisplayer.h"
#include "TpMatchManager.h"
#include "TpMatch.h"
#include "TpPointModel.h"
#include "TpImageView.h"
#include "TpPosView.h"
#include "Application.h"
#include "ErrorManager.h"
#include "PrefManager.h"
#include "TpDisplayerPrefView.h"
#include <Xm/Form.h>
#include <Xm/Frame.h>
#include <assert.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

XtResource TpDisplayer::_resources[] = {
    {
	"numImagesDisplayed",
	"NumImagesDisplayed",
	XmRInt,
	sizeof(int),
	XtOffsetOf(TpDisplayer, _numWin),
	XmRImmediate,
	(XtPointer) 3,
    },
    {
	XvicNcursor,
	XvicCCursor,
	XtRString,
	sizeof(String),
	XtOffsetOf(TpDisplayer, _cursor),
	XtRImmediate,
	(XtPointer) "crosshair",
    },
    {
        XvicNcursorBackground,
        XvicCCursorBackground,
        XtRString,
        sizeof(String),
        XtOffsetOf(TpDisplayer, _cursorColor),
        XtRImmediate,
        (XtPointer) "white",
    },
};

//!!!! Comment out next line before delivery!
// #define DEBUG
#ifdef DEBUG
#define DPR(x) printf x
#else
#define DPR(x)
#endif

// Reload resources

void TpDisplayer::reload(TpDisplayer *copy)
{
    if (_numWin != copy->_numWin)
	setNumWin(copy->_numWin);
    if (strcmp(_cursor, copy->_cursor)) {
	setCursor(copy->_cursor);
    }
    if (strcmp(_cursorColor, copy->_cursorColor)) {
        setCursorColor(copy->_cursorColor);
    }
}

TpDisplayer::TpDisplayer(Widget parent, char *name, 
		TpMatchManager *mm, TpImageReference *ref)
	: UIComponent(name)
{
    _imageReference = ref;
    _win[0] = 0;
    _win[1] = 0;
    _win[2] = 0;
    _locks[0] = _locks[1] = _locks[2] = False;

    _matchManager = mm;

    for (int j = 0; j < TP_MAX_IMAGES; j++)
	_image[j] = NULL;
    _nimages = 0;

    _w = XtVaCreateWidget(_name, xmFrameWidgetClass, parent, NULL);
    installDestroyHandler();

    getResources(_resources, XtNumber(_resources));

    if (_numWin > 3) {
        fprintf(stderr, "numImagesDisplayed cannot be greater than 3!\n");
        fprintf(stderr, "Correct the resource file!\n");
        _numWin = 3;
    }
    if (_numWin < 1) {
        fprintf(stderr, "numImagesDisplayed cannot be less than 1!\n");
        fprintf(stderr, "Correct the resource file!\n");
        _numWin = 1;
    }

    char path[256];
    sprintf(path, "*%s*", _name);
    thePrefManager->registerResources(this,
                                      _resources, XtNumber(_resources),
                                      path, new TpDisplayerPrefView());
    
    _form = XtVaCreateManagedWidget("dispForm", xmFormWidgetClass, _w, NULL);

    // Callee is responsible for deleting ImageData when it's done!

    for (int i = 1; i < theApplication->getArgc(); i++) {
	addImage(theApplication->getParam(i));
    }

    layoutComponents();
    showComponents();

    Widget shell = parent;
    do {
        shell = XtParent(shell);
    } while (shell && !XtIsShell(shell));
    XtVaSetValues(shell, XtNallowShellResize, False, NULL);
}

TpDisplayer::~TpDisplayer()
{
    // Empty
}

void TpDisplayer::layoutComponents() const
{
    for (int i = 0; i < _nimages; i++)
	XtVaSetValues(_image[i]->baseWidget(),
		XmNx,   0,
		XmNy,   0,
		XmNtopAttachment,	XmATTACH_NONE,
		XmNleftAttachment,	XmATTACH_NONE,
		XmNrightAttachment,	XmATTACH_NONE,
		XmNbottomAttachment,	XmATTACH_NONE,
		NULL);

    if (_nimages < 1) return;

    XtVaSetValues(_image[_win[0]]->baseWidget(),
		XmNtopAttachment,       XmATTACH_FORM,
		XmNleftAttachment,      XmATTACH_FORM,
		XmNbottomAttachment,    XmATTACH_FORM,
		NULL);

    if (_numWin > 1)
	XtVaSetValues(_image[_win[0]]->baseWidget(),
		XmNrightAttachment,     XmATTACH_POSITION,
		XmNrightPosition,       100/_numWin,
		NULL);
    else 
	XtVaSetValues(_image[_win[0]]->baseWidget(),
		XmNrightAttachment,	XmATTACH_FORM,
		NULL);

    if (_nimages < 2) return;

    if (_numWin > 1)
	XtVaSetValues(_image[_win[1]]->baseWidget(),
		XmNtopAttachment,       XmATTACH_FORM,
		XmNleftAttachment,      XmATTACH_POSITION,
		XmNleftPosition,	100/_numWin,
		XmNbottomAttachment,    XmATTACH_FORM,
		NULL);

    if (_numWin > 2)
	XtVaSetValues(_image[_win[1]]->baseWidget(),
		XmNrightAttachment,	XmATTACH_POSITION,
		XmNrightPosition,	200/_numWin, // - 2,
		NULL);
    else
	XtVaSetValues(_image[_win[1]]->baseWidget(),
		XmNrightAttachment,	XmATTACH_FORM,
		NULL);

    if (_nimages < 3) return;

    if ((_numWin > 2) && _image[_win[2]])
	XtVaSetValues(_image[_win[2]]->baseWidget(),
		XmNtopAttachment,       XmATTACH_FORM,
		XmNleftAttachment,	XmATTACH_POSITION,
		XmNleftPosition,	200/_numWin, // + 2,
		XmNrightAttachment,     XmATTACH_FORM,
		XmNbottomAttachment,    XmATTACH_FORM,
		NULL);
}

///////////////////////////////////////////////////////////////////
// showComponents: Manages only the components to be shown
///////////////////////////////////////////////////////////////////
void TpDisplayer::showComponents() const
{
    if (_nimages < 1) return;

    if (_numWin > 0) {
	assert ((_win[0] >= 0) && (_win[0] < _nimages) &&
		(_win[0] >= 0) && (_win[0] < _nimages));
	_image[_win[0]]->manage();
    }

    if (_nimages < 2) return;

    if (_numWin > 1) {
	assert ((_win[1] >= 0) && (_win[1] < _nimages) &&
		(_win[1] >= 0) && (_win[1] < _nimages));
	_image[_win[1]]->manage();
    }

    if (_nimages < 3) return;

    if (_numWin > 2) {
	assert ((_win[2] >= 0) && (_win[2] < _nimages) &&
		(_win[2] >= 0) && (_win[2] < _nimages));
	_image[_win[2]]->manage();
    }
}

///////////////////////////////////////////////////////////////////
// hideComponents: Unmanages the images so as to hide them.
///////////////////////////////////////////////////////////////////
void TpDisplayer::hideComponents() const
{
    for (int i = 0; i < _nimages; i++)
	_image[i]->unmanage();
}

int TpDisplayer::addImage(char *filename)
{
    char name[16];
    sprintf(name,"form%d", _nimages+1);
    TpSubDisplayer *sd = new TpSubDisplayer(_form, name, filename, 
					    _nimages + 1, _matchManager, 
					    this);
    if (sd->failed()) return -1;

    _image[_nimages] = sd;
    _imageReference->indicateLoadedImage(_nimages);
    if (_nimages < 3) {
	_win[_nimages] = _nimages;
	_imageReference->setToVisible(_nimages);
	_nimages++;
	layoutComponents();
	showComponents();
	return 0;
    }
    _nimages++;
    return 0;
}

int TpDisplayer::deleteImage(int n)
{
    if (n <= 0)
	return 0;
    if (n > _nimages)
	return 0;

    _matchManager->deleteAllPoints(_image[n-1]->getImageData());

    hideComponents();

    //!!!delete _image[n-1];
    for (int i = n; i < _nimages; i++) {
	_image[i-1] = _image[i];
	_image[i-1]->setNumber(i);
    }
    _nimages--;

    for (int j = 0; j < 3; j++) {
	_win[j] = (_nimages > j) ? j : 0;
    }

    layoutComponents();
    showComponents();

    return 0;
}

void TpDisplayer::shiftRight()
{
    if ((_nimages == 1) || (_nimages == 2))
	return;

    hideComponents();
    _imageReference->setAllToInvisible();

    if (_nimages == 2) {
/*
	if (!_locks[0] && !_locks[1]) {
	    _win[1] = _win[0];
	    do (_win[0] > 0) ? _win[0]-- : (_win[0] = _nimages-1);
	    while (_win[0] == _win[1]);
	}
	if (!_locks[0] && _locks[1]) {
	    do (_win[0] > 0) ? _win[0]-- : (_win[0] = _nimages-1);
            while (_win[0] == _win[1]);
	}
	if (_locks[0] && !_locks[1]) {
	    
        }
	*/
    }
    else {
	if (!_locks[0] && !_locks[1] && !_locks[2]) {
	    _win[2] = _win[1];
	    _win[1] = _win[0];
	    do (_win[0] > 0) ? _win[0]-- : (_win[0] = _nimages-1);
	    while ((_win[0] == _win[1]) || (_win[0] == _win[2]));
	}
	if (!_locks[0] && !_locks[1] && _locks[2]) {
	    _win[1] = _win[0];
	    do (_win[0] > 0) ? _win[0]-- : (_win[0] = _nimages-1);
	    while ((_win[0] == _win[2]) || (_win[0] == _win[1]));
	}
	if (!_locks[0] && _locks[1] && !_locks[2]) {
	    _win[2] = _win[0];
	    do (_win[0] > 0) ? _win[0]-- : (_win[0] = _nimages-1);
	    while ((_win[0] == _win[1]) || (_win[0] == _win[2]));
	}
	if (!_locks[0] && _locks[1] && _locks[2]) {
	    do (_win[0] > 0) ? _win[0]-- : (_win[0] = _nimages-1);
	    while ((_win[0] == _win[1]) || (_win[0] == _win[2]));
	}
	if (_locks[0] && !_locks[1] && !_locks[2]) {
	    _win[2] = _win[1];
	    do (_win[1] > 0) ? _win[1]-- : (_win[1] = _nimages-1);
	    while ((_win[1] == _win[0]) || (_win[1] == _win[2]));
	}
	if (_locks[0] && !_locks[1] && _locks[2]) {
	    do (_win[1] > 0) ? _win[1]-- : (_win[1] = _nimages-1);
	    while ((_win[1] == _win[0]) || (_win[1] == _win[2]));
	}
	if (_locks[0] && _locks[1] && !_locks[2]) {
	    do (_win[2] > 0) ? _win[2]-- : (_win[2] = _nimages-1);
	    while ((_win[2] == _win[0]) || (_win[2] == _win[1]));
	}
    }
    DPR(("%d %d %d\n", _win[0], _win[1], _win[2]));

    layoutComponents();
    showComponents();

    _imageReference->setToVisible(_win[0]);
    _imageReference->setToVisible(_win[1]);
    _imageReference->setToVisible(_win[2]);
}

void TpDisplayer::shiftLeft()
{
    if ((_nimages == 1) || (_nimages == 2))
        return;

    hideComponents();

    if (!_locks[0] && !_locks[1] && !_locks[2]) {
	_win[0] = _win[1];
	_win[1] = _win[2];
	(_win[2] < _nimages-1) ? _win[2]++ : (_win[2] = 0);
    }
    if (!_locks[0] && !_locks[1] && _locks[2]) {
	_win[0] = _win[1];
	do (_win[1] < _nimages-1) ? _win[1]++ : (_win[1] = 0);
	while ((_win[1] == _win[0]) || (_win[1] == _win[2]));
    }
    if (!_locks[0] && _locks[1] && !_locks[2]) {
	_win[0] = _win[2];
	do (_win[2] < _nimages-1) ? _win[2]++ : (_win[2] = 0);
	while ((_win[2] == _win[0]) || (_win[2] == _win[1]));
    }
    if (!_locks[0] && _locks[1] && _locks[2]) {
	do (_win[0] < _nimages-1) ? _win[0]++ : (_win[0] = 0);
	while ((_win[0] == _win[1]) || (_win[0] == _win[2]));
    }
    if (_locks[0] && !_locks[1] && !_locks[2]) {
	_win[1] = _win[2];
	do (_win[2] < _nimages-1) ? _win[2]++ : (_win[2] = 0);
	while ((_win[2] == _win[0]) || (_win[2] == _win[1]));
    }
    if (_locks[0] && !_locks[1] && _locks[2]) {
	do (_win[1] < _nimages-1) ? _win[1]++ : (_win[1] = 0);
	while ((_win[1] == _win[0]) || (_win[1] == _win[2]));
    }
    if (_locks[0] && _locks[1] && !_locks[2]) {
	do (_win[2] < _nimages-1) ? _win[2]++ : (_win[2] = 0);
	while ((_win[2] == _win[0]) || (_win[2] == _win[1]));
    }
    DPR(("%d %d %d\n", _win[0], _win[1], _win[2]));

    layoutComponents();
    showComponents();
}

void TpDisplayer::setLock(int i)
{
    _locks[i] = True;
    _image[_win[i]]->setLock(True);
}

void TpDisplayer::unSetLock(int i)
{
    _locks[i] = False;
    _image[_win[i]]->setLock(False);
}

void TpDisplayer::setDisplayType(TpDisplayType type)
{
    int numWin;
    switch (type) {
      case ONE: numWin = 1;
      break;
      case TWO: numWin = 2;
      break;
      case THREE: numWin = 3;
      break;
      default: printf("Error in setDisplayType\n");
    }
    setNumWin(numWin);
}

void TpDisplayer::setNumWin(int numWin)
{
    if (numWin == _numWin) return;
    _numWin = numWin;
    hideComponents();
    layoutComponents();
    showComponents();
}

void TpDisplayer::setCursor(String newCursor)
{
    _cursor = sdup(newCursor);
    for (int i = 0; i < _numWin; i++) {
	_image[i]->setCursor(newCursor);
    }
}

void TpDisplayer::setCursorColor(String newCursorColor)
{
    _cursorColor = sdup(newCursorColor);
    for (int i = 0; i < _numWin; i++) {
        _image[i]->setCursorColor(newCursorColor);
    }
}

void TpDisplayer::newMatchSelected(TpMatch *match)
{
    if (match == NULL)
	return;
    SL_ListWatch<TpPointModel *> watch;
    match->initScan(&watch);
    TpPointModel *apoint;
    while ((apoint = match->next()) != NULL) {
	_image[apoint->getImageNumber()-1]->getZoomView()->setCenter(
	    apoint->getX(), apoint->getY());
	_image[apoint->getImageNumber()-1]->getImageView()->setCenter(
	    apoint->getX(), apoint->getY());
	_image[apoint->getImageNumber()-1]->getPosView()->displayValues();
    }
}

