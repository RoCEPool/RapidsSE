/*============================================================================*/
/* Local.hpp -- Forth Local                                                   */
/*============================================================================*/
#pragma once

namespace SenVM
{
  class Local : public Object
  {
  private:
    AnyVal_y8KB u;
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    Local(void);
  
    operator AnyVal*(void);
  
    AnyVal* aAnyValue(void);
  
  private:
    Footer* aFooter(void);
  };
  
  Local* _pLocal(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
