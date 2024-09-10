////////////////////////////////////////////////////////////////
// ZoomSpecialCmd.cc
//
//	This class is a zoom control for managing a 
//	dialog with zoom commands.
////////////////////////////////////////////////////////////////
#include "ZoomSpecialCmd.h"
#include "ZoomSpecialCmdList.h"
#include "ZoomDialog.h"
#include "CmdList.h"
#include <assert.h>

ZoomSpecialCmd::ZoomSpecialCmd(char *name, int active, CmdList *radioCmdList,
			CmdValue startState, ZoomSpecialCmdList *zSCL) 
		: ZoomBaseCmd(name, active, startState, radioCmdList, 0) 
{
   _zoomDialog = 0;
   _zoomSpecialCmdList = zSCL;
}

ZoomSpecialCmd::~ZoomSpecialCmd()
{
   delete _zoomSpecialCmdList;
}

////////////////////////////////////////////////////////////////
// Post the dialog and execute the ZoomSpecialCmdList.
////////////////////////////////////////////////////////////////
void ZoomSpecialCmd::doit()
{
   assert(_zoomDialog != 0);
   assert(_zoomSpecialCmdList != 0);

   if (_value) {
      _zoomDialog->post();
      _zoomSpecialCmdList->execute();
   }
}

////////////////////////////////////////////////////////////////
// When another zoom radio button is undone, and we're the previous one,
// then we can't just get executed again because zoomSpecialCmdList
// has forgotten what was on its list.  If we're also current, then
// the undo is done differently (sigh).
////////////////////////////////////////////////////////////////
void ZoomSpecialCmd::undoZoom()
{
   if (_value)
      _zoomSpecialCmdList->undoZoom(True);
   else
      _zoomSpecialCmdList->undoZoom(False);
}

