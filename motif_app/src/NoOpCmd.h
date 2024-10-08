///////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//         This example code is from the book:
//
//           Object-Oriented Programming with C++ and OSF/Motif
//         by
//           Douglas Young
//           Prentice Hall, 1992
//           ISBN 0-13-630252-1	
//
//         Copyright 1991 by Prentice Hall
//         All Rights Reserved
//
//  Permission to use, copy, modify, and distribute this software for 
//  any purpose except publication and without fee is hereby granted, provided 
//  that the above copyright notice appear in all copies of the software.
///////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////
// NoOpCmd.h: Example, dummy command class
/////////////////////////////////////////////////////////////
#ifndef NOOPCMD_H
#define NOOPCMD_H
#include "Cmd.h"

class NoOpCmd : public Cmd {

  protected:

    virtual void doit();   
    virtual void undoit(); 

  public:

    NoOpCmd ( char *, int );
    virtual const char *const className () { return "NoOpCmd"; }
};
#endif
