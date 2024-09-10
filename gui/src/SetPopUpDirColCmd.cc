///////////////////////////////////////////////////////////
// SetPopUpDirColCmd.C: Example, dummy command class
//////////////////////////////////////////////////////////
#include "SetPopUpDirColCmd.h"
#include "Histogram.h"
#include "HistBox.h"
#include <iostream>
using namespace std;

SetPopUpDirColCmd::SetPopUpDirColCmd ( char *name, int active, HistBox *obj, CmdList *list=0 ) : RadioCmd ( name, active, list )
{
    _menuView = obj;
    if ( (_menuView->getPopupDirectionType() == COLUMN && _menuView->getMethodType() == POPUP) || _menuView->getHistB() == 0 ) {
      _value = ( CmdValue ) TRUE;
      newValue();
    }
  }

void SetPopUpDirColCmd::doit()
{

    if (_value) {
       _menuView->setPopupDirectionType( COLUMN );
     }
}      

