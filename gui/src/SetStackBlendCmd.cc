///////////////////////////////////////////////////////////
// SetStackBlendCmd.C: Example, dummy command class
//////////////////////////////////////////////////////////
#include "SetStackBlendCmd.h"
#include "Histogram.h"
#include "HistBox.h"
#include <iostream>

SetStackBlendCmd::SetStackBlendCmd ( char *name, int active, HistBox *obj, CmdList *list=0 ) : RadioCmd ( name, active, list )
{
    _menuView = obj;
    if ( _menuView->getMethodType()== BLEND || _menuView->getHistB() == 0 ) {
      _value = ( CmdValue ) TRUE;
      newValue();
    }      
}

void SetStackBlendCmd::doit()
{
    if (_value) {
       _menuView->setMethodType( BLEND );
    }
}      


