////////////////////////////////////////////////////////////////////////
// SptParamMultiFileSel - Parameter class for a multiple file selection
// dialog (like on xvd).  Filename may come from user or from dataflow.
// When a new filename arrives, the given command is executed if it is
// non-NULL.  This command receives a dynamically-allocated filename string.
////////////////////////////////////////////////////////////////////////

#include "SptParamMultiFileSel.h"
#include "SptShowValueCmd.h"
#include "SptMultiFileSelView.h"

#include <Xm/Form.h>

////////////////////////////////////////////////////////////////////////
// Do the initial layout of the parameter area.  The layout consists of
// a mode menu at the top and the views below.  If the server does not
// exist, the mode menu is suppressed, so the view is stuck on Direct, always.
////////////////////////////////////////////////////////////////////////

void SptParamMultiFileSel::doLayout(Widget parent, char *name)
{
   _w = XtVaCreateWidget(name, xmFormWidgetClass, parent, NULL);
   installDestroyHandler();

   if (serverAvail()) {
      createModeMenu(_w, name);
      XtVaSetValues(_modeMenu->baseWidget(),
		XmNleftAttachment, XmATTACH_FORM,
		XmNtopAttachment, XmATTACH_FORM,
		NULL);
      _modeMenu->manage();
   }

   _viewContainer = XtVaCreateManagedWidget("viewContainer",
		xmFormWidgetClass, _w,
		XmNleftAttachment, XmATTACH_FORM,
		XmNrightAttachment, XmATTACH_FORM,
		XmNbottomAttachment, XmATTACH_FORM,
		NULL);
   if (serverAvail())
      XtVaSetValues(_viewContainer,
		XmNtopAttachment, XmATTACH_WIDGET,
		XmNtopWidget, _modeMenu->baseWidget(),
		NULL);
   else
      XtVaSetValues(_viewContainer,
		XmNtopAttachment, XmATTACH_FORM,
		NULL);

   changeModeView(_mode, _showValue);
}

////////////////////////////////////////////////////////////////////////
// Create the Direct view.
////////////////////////////////////////////////////////////////////////

void SptParamMultiFileSel::createDirectView(Widget parent)
{
   _directView = new SptMultiFileSelView(parent, "FileSelBox", this,
				_mainWindow);
   attachViewToContainer(_directView->baseWidget());
}

////////////////////////////////////////////////////////////////////////
// Create the ShowValue command.  It is disabled if there is no server.
////////////////////////////////////////////////////////////////////////

void SptParamMultiFileSel::createShowValueCmd()
{
   if (serverAvail())
      _showValueCmd = new SptShowValueCmd("Show Value", TRUE, _showValue, this);
   else
      _showValueCmd = NULL;
}

////////////////////////////////////////////////////////////////////////
// Create the mode menu.  It differs from SptInputParamBase only in that
// there is no Default mode available.
////////////////////////////////////////////////////////////////////////

void SptParamMultiFileSel::createModeMenu(Widget parent, char *name)
{
   if (serverAvail())
      _modeMenu = new SptModeMenu(parent, name, this, _mode, _showValueCmd,
		True, False, True, True, False);   // Disable None and Default
   else
      _modeMenu = new SptModeMenu(parent, name, this, _mode, _showValueCmd,
		True, False, False, False, False);
}

