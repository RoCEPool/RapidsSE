/*============================================================================*/
/* WrdVal.hpp - word pointer value                                            */
/*============================================================================*/

#pragma once

#include "Dictionaries.h"

namespace Dictionaries
{
  class Word;
  
  class WrdVal : public Value
  {
  private:
    Word* m_pWord;
    
  public:
    void* operator new(size_t);
    
    WrdVal(void);
    
    void operator =(Word* pVal);
    
    i8    iSize(void) const;
    Word* pWord(void) const;

    void  vToCommand(char* aWorkspace);
    void  vToString(char* aWorkspace);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
