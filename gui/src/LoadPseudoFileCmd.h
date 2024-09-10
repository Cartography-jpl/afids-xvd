/////////////////////////////////////////////////////////
// LoadPseudoFileCmd: A Command class that loads an IBIS file.  The Command 
// value is a dynamically allocated single string, suitable for passing in 
// to a PseudoValue subclass.
/////////////////////////////////////////////////////////
#ifndef LOADSINGLEFILECMD_H
#define LOADSINGLEFILECMD_H
#include "NoUndoCmd.h"

class PseudoValue;

class LoadPseudoFileCmd : public NoUndoCmd {

 protected:

   char *_filename;
   PseudoValue *_pseudoValue;
    
 public:

   LoadPseudoFileCmd(char *, int, PseudoValue *, char * = 0);

   virtual void doit();  
    
   virtual void freeValue(CmdValue value) { if (value) delete (LoadPseudoFileCmd*) value; }

   virtual const char *const className () { return "LoadPseudoFileCmd"; }
};
#endif
