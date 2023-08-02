/*============================================================================*/
/* Scope.hpp -- Table Scope                                                   */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "SubCall.hpp"
#include "Symbol.hpp"

namespace STORE
{
  class Scope : public Object
  {
  private:

    // something quick ...
    bool m_baKnown[26];

    // must be last
    Footer m_xFooter;
 
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Scope(void);
   ~Scope(void) noexcept(false);

  public:

    bool bInScope(char cTblSyn);
    bool bInScope(Symbol* pSymbol);
    void vInScope(Symbol* pSymbol, bool bInScope);

    Footer* aFooter(void);
  };
  
  Scope* _pScope(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
