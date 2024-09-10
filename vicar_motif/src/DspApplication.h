////////////////////////////////////////////////////////////
// DspApplication.h: 
////////////////////////////////////////////////////////////
#ifndef DSPAPPLICATION_H
#define DSPAPPLICATION_H
#include "Application.h"
#include "DspWindowBasic.h"

class DspApplication : public Application {
    
    
  private:
  
  protected:
  
#if (XlibSpecificationRelease>=5)   
    virtual void initialize_X ( int *argcp, char ** argv);  
#else
    virtual void initialize_X ( unsigned int *argcp, char ** argv);  
#endif
  
  public:
    
    DspApplication ( char * appClassName ) : Application(appClassName) {}
    virtual ~DspApplication(){}     
    
    virtual const char *const className() { return "DspApplication"; }

};
#endif
