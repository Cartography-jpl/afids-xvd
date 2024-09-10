////////////////////////////////////////////////////////////
// TpLoadConfigCmd.h: Load config file.
////////////////////////////////////////////////////////////
#ifndef TPLOADCONFIGCMD_H
#define TPLOADCONFIGCMD_H
#include "NoUndoCmd.h"

class TpWindow;

class TpLoadConfigCmd : public NoUndoCmd {

  protected:

    TpWindow *_window;
    
    virtual void doit();
    
  public:

    TpLoadConfigCmd(char *, int, TpWindow *);

  virtual void freeValue(CmdValue value) { if (value) delete (TpLoadConfigCmd*) value; }

    virtual const char *const className () { return "TpLoadConfigCmd"; }
};
#endif
