////////////////////////////////////////////////////////////////
// SiCursorStretchEnableCmd - Enable or disable the cursor stretch.
// This command has a most unusual Undo... undo does not turn the
// command back on, it ensures that the command is off then resets
// the stretch back to what it was when it was turned on.
////////////////////////////////////////////////////////////////

#include "SiCursorStretchEnableCmd.h"
#include "SiCursorStretchInterface.h"
#include "StretchValue.h"

SiCursorStretchEnableCmd::SiCursorStretchEnableCmd(char *name, int active,
		SiCursorStretchInterface *csif, Cmd *stretchCmd)
	: Cmd(name, active)
{
   _interface = csif;
   _stretchCmd = stretchCmd;
   _saveStretchForUndo = 0;
}

SiCursorStretchEnableCmd::~SiCursorStretchEnableCmd()
{
   if (_saveStretchForUndo)
      delete _saveStretchForUndo;
}

void SiCursorStretchEnableCmd::doit()
{
   if (_value) {			// activate
      if (_saveStretchForUndo)
         delete _saveStretchForUndo;
      _saveStretchForUndo = 0;
      StretchValue *v = (StretchValue *)_stretchCmd->getValue();
      if (v != 0)
         _saveStretchForUndo = new StretchValue(*v);

      _interface->activate();
   }
   else {				// deactivate (leave Undo alone)
      _interface->deactivate();
   }
}

void SiCursorStretchEnableCmd::undoit()
{
   _interface->deactivate();		// make sure

   _stretchCmd->execute(_saveStretchForUndo); // do original stretch again
   _saveStretchForUndo = 0;	// execute() takes possession of memory

   _value = (CmdValue)False;		// update button
   newValue();
}

