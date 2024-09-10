////////////////////////////////////////////////////////////////////////
// SptMultiFileSelView - View object for Multiple File Selection dialog
////////////////////////////////////////////////////////////////////////

#include "SptMultiFileSelView.h"
#include "SptParamBase.h"
#include "SptDirectStringValueCmd.h"

#include "MainWindow.h"
#include "FileSelBox.h"

#include "DD.H"

SptMultiFileSelView::SptMultiFileSelView(Widget parent, char *name,
			SptParamBase *pbase, MainWindow *mainWindow)
	: SptDirectBase(name, pbase)
{

   _stringValueCmd = new SptDirectStringValueCmd(name, TRUE, _paramBase);

   // _w is shared with _ci so we don't have an extra, unnecessary, manager
   // widget in here.  When base widgets are shared, though, each class
   // should install their own destroy handler.

   _fileSelBox = new FileSelBox(parent, name, _stringValueCmd, mainWindow);
   _w = _fileSelBox->baseWidget();
   installDestroyHandler();

   // Add our drop functionality to the standard Motif drop.
   _dropSite = _paramBase->createDropSite(_w, TRUE, TRUE);

//!!!!   displayValue();

//!!!!   _paramBase->createTextDragSite(_w, "process-bdrag()");
}

////////////////////////////////////////////////////////////////////////
// See SptParamBase::createDropSite for why this is needed
////////////////////////////////////////////////////////////////////////

void SptMultiFileSelView::manage()
{
   SptDirectBase::manage();
   if (_dropSite)
      _dropSite->activate();
}

void SptMultiFileSelView::unmanage()
{
   SptDirectBase::unmanage();
   if (_dropSite)
      _dropSite->deactivate();
}

////////////////////////////////////////////////////////////////////////

void SptMultiFileSelView::displayValue()
{
#if 0		//!!!!
   char *value;
   StatusType status;

   status = _paramBase->getParamValue(value);
   if (status == StatusNoCurrentValue)
      displayValue(NULL);
   else if (status != StatusSuccess)
      printf("error getting param value, status=%d!!!!\n", status);
   else
      displayValue(value);
   if (value) delete value;
#endif		//!!!!
}

////////////////////////////////////////////////////////////////////////

void SptMultiFileSelView::displayValue(char *value)
{
   if (value)
      _stringValueCmd->setValueDirect(strdup(value));
   else
      _stringValueCmd->setValueDirect(NULL);
}

////////////////////////////////////////////////////////////////////////

void SptMultiFileSelView::paramValueChanged()
{
   displayValue();
}

////////////////////////////////////////////////////////////////////////
// Since deferred execution makes no sense when the FileSelBox has its
// own OK/Apply/Cancel buttons, we disable it here.  Besides, there is
// no easy way to get at the actual CmdInterface from the FileSelBox.
////////////////////////////////////////////////////////////////////////

void SptMultiFileSelView::setDeferredExec(CmdList *)
{
   // Empty
}

