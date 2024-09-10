////////////////////////////////////////////////////////////
// MenuItem.h: A collection of small classes that maintain info about
// how a menu item is presented, and create the items
////////////////////////////////////////////////////////////

#ifndef MENUITEM_H
#define MENUITEM_H

#include <Xm/Xm.h>

class Cmd;
class CmdList;

class MenuItem {
  protected:
    void setMneAcc( Widget );

  public:
    char _mnemonic;
    char *_accelerator;
    char *_accel_text;
    virtual ~MenuItem() {}
    virtual void createItem ( Widget, Cmd *, Widget=0 ) = 0;
};

class MenuItemButton : public MenuItem {
  public:
    virtual void createItem ( Widget, Cmd *, Widget=0 );
};

class MenuItemCheckBox : public MenuItem {
  public:
    virtual void createItem ( Widget, Cmd *, Widget=0 );
};

class MenuItemOption : public MenuItem {
  public:
    CmdList *_deferList;
    virtual void createItem ( Widget, Cmd *, Widget=0 );
};

class MenuItemRadioButton : public MenuItem {
  public:
    virtual void createItem ( Widget, Cmd *, Widget=0 );
};

class MenuItemSeparator : public MenuItem {
  public:
    virtual void createItem ( Widget, Cmd *, Widget=0 );
};

class MenuItemSubmenu : public MenuItem {
  public:
    virtual void createItem ( Widget, Cmd *, Widget=0 );
};

#endif
