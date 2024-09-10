#include <Xm/Xm.h>

class SgResourceConverter {

  protected:

    static Boolean _firstTime;

    static Boolean stringsAreEqual(char *in_str, char *test_str);
    
    static void cvtStringToPixmap(XrmValue *args, Cardinal *numArgs,
				  XrmValue *fromVal, XrmValue *toVal);

  public:

    static void registerStringToPixmapConverter();

};
