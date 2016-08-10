/*============================================================================*/
/* IPval.hpp - instruction pointer value                                      */
/*============================================================================*/

#pragma once

#include "Dictionaries.h"

namespace Dictionaries
{
  class IPval : public OnStack
  {
  private:
    union {
      Cell*  m_pCell;
    };
    
  public:
    void* operator new(size_t);
    
    IPval(void);
    
    void operator =(Cell* pCell);
    
    i8    iSize(void) const;
    Cell* pCell(void) const;
    
    void vToCommand(char* aWorkspace);
    void vToString(char* aWorkspace);    
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
