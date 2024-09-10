////////////////////////////////////////////////////////////
// TpNumDisplaysCmd.h: Set number of displays to either one, 
// two, or three.
////////////////////////////////////////////////////////////
#ifndef TpNumDisplaysCmd_H
#define TpNumDisplaysCmd_H
#include "RadioCmd.h"
#include "TpDefs.h"

class TpDisplayer;

class TpNumDisplaysCmd : public RadioCmd {

  protected:

    TpDisplayer *_displayer;
    TpDisplayType _displayType;

    virtual void doit();
    
  public:

    TpNumDisplaysCmd(char *, int, CmdValue, CmdList *, 
			TpDisplayer *, TpDisplayType);

    virtual const char *const className () { return "TpNumDisplaysCmd"; }
};
#endif
