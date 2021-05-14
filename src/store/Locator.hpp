/*============================================================================*/
/* Locator.hpp - Item Locator                                                 */
/*============================================================================*/
#pragma once

namespace STORE
{
  class Locator : public Object
  {
  private:
  
    Dic* m_pTenant;
    Dic* m_pCatalog;
    Dic* m_pSchema;

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Locator(void);
    Locator(Location* pLocation);
   ~Locator(void) noexcept(false);
  
    static void vFlist(void);

    void vLocate(Location* pLocation);

    Dic* pTenant(void);
    Dic* pCatalog(void);
    Dic* pSchema(void);

    Footer* aFooter(void);
  };

  Locator* _pLocator(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
