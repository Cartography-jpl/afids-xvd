/////////////////////////////////////////////////////////
// DspFileNameCmd: A Command class that loads a file name and 
//	copies it to function pointer.  The Command value
// 	is a dynamically allocated single string.
/////////////////////////////////////////////////////////
#ifndef DSPFILENAMECMD_H
#define DSPFILENAMECMD_H
#include "NoUndoCmd.h"
#include <Xm/Xm.h>


class DspFileNameCmd : public NoUndoCmd {

 protected:

   XtPointer		_obj;
   void 		(*_fp)(XtPointer, char *);
    
 public:

   DspFileNameCmd(char *name, int active, XtPointer obj, void (*fp)(XtPointer, char *));

   virtual void doit();  
    
  virtual void freeValue(CmdValue value) { if (value) delete (DspFileNameCmd*) value; }

   virtual const char *const className () { return "DspFileNameCmd"; }
};
#endif
