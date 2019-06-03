/*============================================================================*/
/* Tenant.hpp - Tenant                                                        */
/*============================================================================*/
#pragma once

namespace MetaData
{
  class Tenant : public Object
  {
  private:
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Tenant(void);
    ~Tenant(void) noexcept(false);
    
   
    Footer* aFooter(void);
  };
  
  Tenant* _pTenant(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
