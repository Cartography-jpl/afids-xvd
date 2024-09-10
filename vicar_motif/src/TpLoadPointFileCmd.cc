////////////////////////////////////////////////////////////
// TpLoadPointFileCmd.cc: Load ibis-2 point file.
////////////////////////////////////////////////////////////
#include "TpLoadPointFileCmd.h"
#include "TpMatchManager.h"

TpLoadPointFileCmd::TpLoadPointFileCmd(char *name, int active, 
				       TpMatchManager *mm) 
    : NoUndoCmd(name, active)
{
    _matchManager = mm;
}

void TpLoadPointFileCmd::doit()
{
    _matchManager->readPointsIbis((char *)_value);
}       

