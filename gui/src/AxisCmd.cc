/////////////////////////////////////////////////////////////
// AxisCmd.h: Displays or hides histogram axis.
/////////////////////////////////////////////////////////////
#include "AxisCmd.h"
#include "HistBox.h"
#include <iostream>
#include <stdint.h>
using namespace std;

AxisCmd::AxisCmd ( char *name, int active, HistBox *box ) 
	: Cmd ( name, active )
{
    _box = box;
    uintptr_t value = (uintptr_t) _box->AxisIsDisplayed ( );
    _value = (CmdValue) value; 
    newValue ( );
}

void AxisCmd::doit ( )
{
    _oldValue = _box->AxisIsDisplayed ( );
    _box->showAxis( (_value != 0) );
}      

void AxisCmd::undoit ( )
{
    _box->showAxis( _oldValue );
    _value = (CmdValue) ((uintptr_t) _oldValue);
    newValue ( );
}
