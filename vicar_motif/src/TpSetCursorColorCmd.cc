///////////////////////////////////////////////////////////////////////////////
// TpSetCursorColorCmd.cc: 
///////////////////////////////////////////////////////////////////////////////
#include "TpSetCursorColorCmd.h"
#include "TpDisplayer.h"
#include <stdlib.h>
#include <stdio.h>

TpSetCursorColorCmd::TpSetCursorColorCmd(char *name, int active, 
					 CmdValue value, TpDisplayer *d)
    : Cmd(name, active, value)
{
    _displayer = d;
}

void TpSetCursorColorCmd::doit()
{
    _displayer->setCursorColor((char *)_value);
}
