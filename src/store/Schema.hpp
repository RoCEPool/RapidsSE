/*============================================================================*/
/* Schema.hpp -- Schema                                                       */
/*============================================================================*/
#pragma once

#include "Folder.hpp"

namespace STORE
{
  class Schema : public Folder
  {
  private:
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Schema(void);
   ~Schema(void) noexcept(false);
    
    static Stz* szCurrSchema(DicStack* pDicStack);

    Footer* aFooter(void);
  };
  
  Schema* _pSchema(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
