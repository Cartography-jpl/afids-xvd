////////////////////////////////////////////////////////////
// TpDisplayModeDialog.h: Dialog containing display mode values.
////////////////////////////////////////////////////////////
#include "TpDisplayModeDialog.h"
#include "TpNumDisplaysCmd.h"
#include "TpRotateImageCmd.h"
#include "TpSwapLockCmd.h"
#include "TpDisplayer.h"
#include "RadioCmdBox.h"
#include "CmdList.h"
#include "RadioCmd.h"
#include "OptionCmdMenu.h"
#include "CheckBoxInterface.h"
#include "TpDefs.h"
#include <Xm/Label.h>
#include <Xm/RowColumn.h>
#include <Xm/Frame.h>
#include <Xm/LabelG.h>
#include <stdio.h>

TpDisplayModeDialog::TpDisplayModeDialog(char *name, 
		TpDisplayer *displayer)
	: CustomDialog(name, Default, Visible, Visible, Visible, Visible)
{
    _displayer = displayer;
}

Widget TpDisplayModeDialog::createWorkArea(Widget parent)
{
    Widget rc = XtVaCreateWidget("workArea", 
			xmRowColumnWidgetClass, parent, 
			XmNorientation, XmVERTICAL,
			XmNnumColumns, 1,
			NULL);

    XtVaCreateManagedWidget("displayModeLabel", xmLabelWidgetClass, rc, NULL);

    ////////////////////////////////////////////////////////////////
    // Radio box for specifying number of images displayed.
    ///////////////////////////////////////////////////////////////
    Widget displayTypeFrame = XtVaCreateManagedWidget("displayFrame",
			xmFrameWidgetClass, rc,
			NULL);
    XtVaCreateManagedWidget ("displayFrameLabel", 
			     xmLabelGadgetClass, displayTypeFrame, 
			     XmNchildType, XmFRAME_TITLE_CHILD,
			     XmNchildVerticalAlignment, XmALIGNMENT_CENTER,
			     NULL);
    int initNumImages = _displayer->getNumWin();
    int i = 0;
    CmdList *dispTypeRadList = new CmdList;
    RadioCmd *dispTypeCmd[3];
    dispTypeCmd[i] = new TpNumDisplaysCmd("one", True, 
				(CmdValue) (!(i+1 - initNumImages)),
				dispTypeRadList, _displayer, ONE);
    i++;
    dispTypeCmd[i] = new TpNumDisplaysCmd("two", True,
				(CmdValue) (!(i+1 - initNumImages)), 
				dispTypeRadList, _displayer, TWO);
    i++;
    dispTypeCmd[i] = new TpNumDisplaysCmd("three", True, 
				(CmdValue) (!(i+1 - initNumImages)),
				dispTypeRadList, _displayer, THREE);
    i++;
    UIComponent *radioBox = new RadioCmdBox(displayTypeFrame, "displayTypes", 
					dispTypeRadList, _applyCmdList);
    radioBox->manage();

    ////////////////////////////////////////////////////////////////
    // Option menus for specifying the rotation of images displayed.
    ///////////////////////////////////////////////////////////////
    Widget rotationFrame = XtVaCreateManagedWidget("rotationFrame",
			xmFrameWidgetClass, rc,
			NULL);
    XtVaCreateManagedWidget ("rotationFrameLabel", 
                             xmLabelGadgetClass, rotationFrame,
                             XmNchildType, XmFRAME_TITLE_CHILD,
                             XmNchildVerticalAlignment, XmALIGNMENT_CENTER,
			     NULL);
    Widget rotationRC = XtVaCreateManagedWidget("rotationRC", 
			xmRowColumnWidgetClass, rotationFrame,
			NULL);
    RadioCmd *rotationCmd[4];
    for (int j = 0; j < TP_MAX_IMAGES; j++) {
	i = 0;
	CmdList *rotRadList = new CmdList;
	rotationCmd[i++] = new TpRotateImageCmd("0", True,
			rotRadList, _displayer->getSubDisplayer(j), ROTATE_NO);
	rotationCmd[i++] = new TpRotateImageCmd("90", True,
			rotRadList, _displayer->getSubDisplayer(j), ROTATE_CW);
	rotationCmd[i++] = new TpRotateImageCmd("180", True,
			rotRadList, _displayer->getSubDisplayer(j), ROTATE_FULL);
	rotationCmd[i++] = new TpRotateImageCmd("270", True,
			rotRadList, _displayer->getSubDisplayer(j), ROTATE_CCW);
	char name[16];
	sprintf(name, "rotOptMenu%d", j);
	UIComponent *optionMenu = new OptionCmdMenu(rotationRC, 
			name, rotRadList, _applyCmdList);
	optionMenu->manage();
    }

    ////////////////////////////////////////////////////////////////
    // Image swap locking options
    ////////////////////////////////////////////////////////////////
    Widget swapFrame = XtVaCreateManagedWidget("swapFrame",
			xmFrameWidgetClass, rc,
			NULL);
    XtVaCreateManagedWidget ("swapFrameLabel",
                             xmLabelGadgetClass, swapFrame,
                             XmNchildType, XmFRAME_TITLE_CHILD,
                             XmNchildVerticalAlignment, XmALIGNMENT_CENTER,
                             NULL);
    Widget swapRC = XtVaCreateManagedWidget("swapRC",
			xmRowColumnWidgetClass, swapFrame,
			NULL);
    Cmd *swapCmd[3];
    CmdInterface *swapCmdInterface[3];
    for (i = 0; i < 3; i++) {
	swapCmd[i] = new TpSwapLockCmd("swapLock", True, _displayer, i);
	swapCmdInterface[i] = new CheckBoxInterface(swapRC, swapCmd[i]);
	swapCmdInterface[i]->manage();
	swapCmdInterface[i]->setDeferredExec(_applyCmdList);
    }

    return rc;
}
