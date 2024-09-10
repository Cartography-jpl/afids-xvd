///////////////////////////////////////////////////////////
// SetPopUpDirRowCmd.C: Example, dummy command class
//////////////////////////////////////////////////////////
#include "SetPopUpDirRowCmd.h"
#include "Application.h"
#include "Histogram.h"
#include "HistBox.h"
#include <iostream>

SetPopUpDirRowCmd::SetPopUpDirRowCmd ( char *name, int active, HistBox *obj, CmdList *list=0 ) : RadioCmd ( name, active, list )
{
    _menuView = obj;
    if ( (_menuView->getPopupDirectionType() == ROW && _menuView->getMethodType() == POPUP) || _menuView->getHistB() == 0) {
      _value = ( CmdValue ) TRUE;
      newValue();
    }
}

void SetPopUpDirRowCmd::doit()
{
    if (_value) {
        _menuView->setPopupDirectionType( ROW );
     }
}      


