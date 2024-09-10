////////////////////////////////////////////////////////
// HelpBrowser.h: Actually run the help browser.
// If the browser supports Mosaic- or Netscape-style
// remote-control, and we've already started one, then
// don't start another but instead re-use the existing
// window.  If remote-control is not supported,
// we always start a new browser. This is a global object,
// accessed via theHelpBrowser.
////////////////////////////////////////////////////////
#ifndef HELPBROWSER_H
#define HELPBROWSER_H
#include <Xm/Xm.h>
#include <stdint.h>

typedef uintptr_t HelpBrowserControlStyleType;
const HelpBrowserControlStyleType NETSCAPE_HELP = 0;
const HelpBrowserControlStyleType MOSAIC_HELP = 1;
const HelpBrowserControlStyleType DEFAULT_BROWSER_HELP = 2; 

class HelpBrowser {

  private:
  
    static int _helpBrowserInit;               // flag for class initialization
    static XtResource _resources[];

  protected:

    String _helpBrowserCommand;
    String _helpBaseLocation;
    String _helpLocation;
    Boolean _helpBrowserHasRemoteControl;
    
    HelpBrowserControlStyleType _helpBrowserControlStyle;

    int _browserPid;
    String _currentBrowserCmd;

  public:

    HelpBrowser();
    virtual ~HelpBrowser() {}
    virtual void run ( char *, char *, char *, Boolean=True,
		       HelpBrowserControlStyleType = DEFAULT_BROWSER_HELP ); 
                                                     // Constant location
    virtual void run ( char *, char *, Boolean=True,
		       HelpBrowserControlStyleType = DEFAULT_BROWSER_HELP );
                                                    // URL is predefined

    virtual void run(char *);			// Only the URL is rqr'd 
    virtual void run(Widget);			// Get location from widget
    virtual void killBrowser();			// Terminate browser process
};

extern HelpBrowser *theHelpBrowser;

#endif
