////////////////////////////////////////////////////////////////////////
// XvdApplication.cc: 
////////////////////////////////////////////////////////////////////////
#include "XvdApplication.h"

XvdApplication *theXvdApplication = NULL;

// The subclass should contain all of these options.
// If you are updating record, always make sure 
// your changes are reflected in all the subclasses

XrmOptionDescRec  XvdApplication::_options[] = {
#ifdef ENABLE_SAGE
 {"-host",       "*host",                    XrmoptionSepArg,  NULL  },
 {"-port",       "*port",                    XrmoptionSepArg,  NULL  },
 {"-module",     "*module",                  XrmoptionSepArg,  NULL  },
#endif
 {"-min",        "*min",                     XrmoptionSepArg,  NULL  },
 {"-max",        "*max",                     XrmoptionSepArg,  NULL  },
 {"-fullscreen", "*fullScreenDisplay",       XrmoptionNoArg,   "True"},
 {"-fit",        "*fit",                     XrmoptionNoArg,   "True"},
 {"-height",     "*XVd.height",              XrmoptionSepArg,  NULL  },
 {"-width",      "*XVd.width",               XrmoptionSepArg,  NULL  },
 {"-x",          "*XVd.x",                   XrmoptionSepArg,  NULL  },
 {"-y",          "*XVd.y",                   XrmoptionSepArg,  NULL  },
 {"-help",       "*cmdLineHelpReqd",     XrmoptionNoArg,       "True"}
};

XvdApplication::XvdApplication ( char *appClassName ) : 
#ifdef ENABLE_SAGE
                                         SpbApplication ( appClassName )
#else
                                         Application ( appClassName )
#endif
{
    // Set the global Application pointer

    theXvdApplication = this;
}

XvdApplication::~XvdApplication()
{
}
