/*============================================================================*/
/* Locator.hpp - Item Locator                                                 */
/*============================================================================*/
#pragma once

namespace BASE
{
  class Locator : public Object
  {
  private:
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Locator(void);
   ~Locator(void) noexcept(false);
  
    static void vFlist(void);

    Footer* aFooter(void);
  };

  Locator* _pLocator(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
