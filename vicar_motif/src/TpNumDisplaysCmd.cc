////////////////////////////////////////////////////////////
// TpNumDisplaysCmd.cc: Set number of displays.
////////////////////////////////////////////////////////////
#include "TpNumDisplaysCmd.h"
#include "TpDisplayer.h"

TpNumDisplaysCmd::TpNumDisplaysCmd(char *name, int active, 
		CmdValue starting_value, CmdList *radioList,
		TpDisplayer *displayer, TpDisplayType displayType)
	: RadioCmd(name, active, starting_value, radioList)
{
    _displayer = displayer;
    _displayType = displayType;
}

void TpNumDisplaysCmd::doit()
{
    if (_value)
	_displayer->setDisplayType(_displayType);
}

