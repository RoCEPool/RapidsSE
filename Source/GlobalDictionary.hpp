/*============================================================================*/
/* GlobalDictionary.hpp - dictionary of global words                          */
/*============================================================================*/

#pragma once

#include "Dictionaries.h"

namespace Dictionaries
{
  class Word;
  
  class GlobalDictionary
  {
  public:
    Dictionary m_xDictionary;
    
    void* operator new(size_t);
    void  operator delete(void*);
    
    GlobalDictionary(void);
    ~GlobalDictionary(void);
    
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
