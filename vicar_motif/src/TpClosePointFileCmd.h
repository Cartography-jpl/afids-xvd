////////////////////////////////////////////////////////////
// TpClosePointFileCmd.h: Close ibis-2 point file.
////////////////////////////////////////////////////////////
#ifndef TPCLOSEPOINTFILECMD_H
#define TPCLOSEPOINTFILECMD_H
#include "NoUndoCmd.h"

class TpMatchManager;

class TpClosePointFileCmd : public NoUndoCmd {

  protected:

    TpMatchManager *_matchManager;
    
    virtual void doit();
    
  public:

    TpClosePointFileCmd(char *, int, TpMatchManager *);

  virtual void freeValue(CmdValue value) { if (value) delete (TpClosePointFileCmd*) value; }

    virtual const char *const className () { return "TpClosePointFileCmd"; }
};
#endif
