/////////////////////////////////////////////////////////////
// TpSavePointFileAsCmd.h: Saves currently selected match.
/////////////////////////////////////////////////////////////
#ifndef TPSAVEPOINTFILEASCMD_H
#define TPSAVEPOINTFILEASCMD_H
#include "NoUndoCmd.h"

class TpMatchManager;

class TpSavePointFileAsCmd : public NoUndoCmd {

  private:

    TpMatchManager *_matchManager;
    Cmd *_quitCmd;

    CmdValue _tmpValue;

    static void okCallback(void *clientData);

  protected:

    virtual void doit();

  public:

    TpSavePointFileAsCmd(char *name, int active, 
			 TpMatchManager *matchManager, Cmd *quitCmd = 0);
    virtual ~TpSavePointFileAsCmd() { };

    virtual void execute(CmdValue new_value = 0);

  virtual void freeValue(CmdValue value) { if (value) delete (TpSavePointFileAsCmd*) value; }

    virtual const char *const className () { return "TpSavePointFileAsCmd"; }
};
#endif
