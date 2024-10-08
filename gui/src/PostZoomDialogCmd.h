////////////////////////////////////////////////////////////////
// PostZoomDialogCmd.h - simple command to post the zoom dialog
////////////////////////////////////////////////////////////////
#ifndef POSTZOOMDIALOGCMD_H
#define POSTZOOMDIALOGCMD_H

#include "NoUndoCmd.h"
#include "ZoomDialog.h"
#include <assert.h>
 
class PostZoomDialogCmd : public NoUndoCmd {

 protected:

   ZoomDialog *_dialog;
 
 public:
 
   PostZoomDialogCmd(char *name, int active, ZoomDialog *z)
			: NoUndoCmd(name, active)  
		{ _dialog = z; }

   virtual void doit() { assert(_dialog != 0); _dialog->post(); }

};
 
#endif

