////////////////////////////////////////////////////////
// HelpOnContextCmd.h: Command used to implement context-sensitive help
////////////////////////////////////////////////////////
#ifndef HELPONCONTEXTCMD_H
#define HELPONCONTEXTCMD_H
#include "NoUndoCmd.h"
#include <Xm/Xm.h>

class HelpOnContextCmd : public NoUndoCmd {

  protected:

    Widget _baseW;		// Base widget to use for XmTrackingEvent()

    virtual void doit();

  public:
    
    HelpOnContextCmd(char*, int, Widget);
    virtual const char *const className () { return "HelpOnContextCmd"; }
};
#endif

