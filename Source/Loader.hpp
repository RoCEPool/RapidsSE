/*============================================================================*/
/* Loader.hpp -- Table Loader                                                 */
/*============================================================================*/

#pragma once

#include "DBobjects.h"

namespace DBobjects
{
  class Loader : public Object
  {
  private:
    
  public:
    void* operator new(size_t);
    void  operator delete(void* pLoaderAddress);
 
    Loader(void);
    ~Loader(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
