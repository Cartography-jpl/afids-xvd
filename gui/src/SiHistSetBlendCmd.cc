///////////////////////////////////////////////////////////
// SiHistSetBlendCmd.C:  Display histogram in blend mode.
//////////////////////////////////////////////////////////
#include "SiHistSetBlendCmd.h"
#include "SiHistBox.h"

SiHistSetBlendCmd::SiHistSetBlendCmd ( char *name, int active, 
		SiHistBox *box, CmdList *list=0 ) 
	: RadioCmd ( name, active, list )
{
    _box = box;

    if ( _box->getMethodType() == BLEND || _box->getHistB() == 0 )
	_value = (CmdValue)TRUE;
    else 
	_value = (CmdValue)FALSE;

    newValue();
}

void SiHistSetBlendCmd::doit()
{
    _oldValue = _box->getMethodType();

    if (_value) {
	_box->setMethodType ( BLEND );
    }
}      

void SiHistSetBlendCmd::undoit()
{
    _box->setMethodType ( _oldValue );

    if ( _oldValue == BLEND || _box->getHistB() == 0 ) 
	_value = (CmdValue)TRUE;
    else
	_value = (CmdValue)FALSE;

    newValue();
}
