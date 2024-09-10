////////////////////////////////////////////////////////////
// TpRemoveImageCmd.h: 
////////////////////////////////////////////////////////////
#include "TpRemoveImageCmd.h"
#include "TpDisplayer.h"
#include <stdlib.h>

TpRemoveImageCmd::TpRemoveImageCmd(char *name, int active, TpDisplayer *d) : 
                    WarnNoUndoCmd(name, active)
{
    _displayer = d;
}

void TpRemoveImageCmd::doit()
{
    int value = atoi((char *)_value);
    _displayer->deleteImage(value);
}       

