/*============================================================================*/
/* Local.hpp -- Forth Local                                                   */
/*============================================================================*/
#pragma once

namespace SenVM
{
  class Local : public Object
  {
  private:
    AllVal_small u;
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    Local(void);
      
    AnyVal* aLocalValue(void);
  
  private:
    Footer* aFooter(void);
  };
  
  Local* _pLocal(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
