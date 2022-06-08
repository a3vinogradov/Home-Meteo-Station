#ifndef CFSControllerFile
#define CFSControllerFile

#include <FS.h>

class CFSController
{
  public:
    CFSController();
    void Setup();
    void Exec();
    String ReadFile(String pageName);
};

#endif
