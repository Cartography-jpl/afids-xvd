////////////////////////////////////////////////////////////////////////
// DspFileNameCmd: A Command class that loads a file name.
//		very similar to MOTIFAPPLIB/DspFileNameCmd. 
////////////////////////////////////////////////////////////////////////
#include "DspFileNameCmd.h"
#include "CmdValue.h"
#include <assert.h>

DspFileNameCmd::DspFileNameCmd(char *name, int active, 
				XtPointer obj, void (*fp)(XtPointer, char *))
		: NoUndoCmd(name, active)
{
   _fp = fp;
   _obj = obj;
   _fp = fp; 
}

void DspFileNameCmd::doit()
{
   assert(_fp);
   
   (*_fp)(_obj,(char *)_value); // set value in another cmd class
}

