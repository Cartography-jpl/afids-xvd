///////////////////////////////////////////////////////////
// SetStackNoBlendCmd.C: Example, dummy command class
//////////////////////////////////////////////////////////
#include "SetStackNoBlendCmd.h"
#include "Application.h"
#include "Histogram.h"
#include "HistBox.h"
#include <iostream>

SetStackNoBlendCmd::SetStackNoBlendCmd ( char *name, int active, HistBox *obj, CmdList *list=0 ) : RadioCmd ( name, active, list )
{
    _menuView = obj;
    if (_menuView->getMethodType() == STACKED  || _menuView->getHistB() == 0 ) {
      _value = ( CmdValue ) TRUE;
      newValue();
    }
}

void SetStackNoBlendCmd::doit()
{
    if (_value) {
       _menuView->setMethodType ( STACKED );
     }
}      



