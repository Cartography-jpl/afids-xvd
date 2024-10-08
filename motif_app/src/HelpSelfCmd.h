////////////////////////////////////////////////////////
// HelpSelfCmd.h: Command used in a Help menu to refer to context-
// sensitive help on widget named in the constructor (usually itself).
////////////////////////////////////////////////////////
#ifndef HELPSELFCMD_H
#define HELPSELFCMD_H
#include "NoUndoCmd.h"
#include <Xm/Xm.h>

class HelpSelfCmd : public NoUndoCmd {

  protected:

    Widget _baseWidget;
    char *_nameForHelp;

    virtual void doit();

  public:

    HelpSelfCmd(char*, int, Widget, char *);
    virtual const char *const className () { return "HelpSelfCmd"; }
};
#endif

