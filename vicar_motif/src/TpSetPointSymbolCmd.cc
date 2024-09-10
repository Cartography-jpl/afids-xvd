///////////////////////////////////////////////////////////////////////////////
// TpSetPointSymbolCmd.cc: 
///////////////////////////////////////////////////////////////////////////////
#include "TpSetPointSymbolCmd.h"
#include "TpMatchManager.h"
#include <stdlib.h>
#include <stdio.h>

TpSetPointSymbolCmd::TpSetPointSymbolCmd(char *name, int active, 
					 CmdList *radCmdList, 
					 TpPointSymbolShapeType shape, 
					 TpMatchManager *mm)
    : RadioCmd(name, active, radCmdList)
{
    _shape = shape;
    _matchManager = mm;
}

void TpSetPointSymbolCmd::doit()
{
    if (_value) {
	_matchManager->setPointShape(_shape);
    }
}
