/*============================================================================*/
/* Tenants.hpp -- Tenants                                                     */
/*============================================================================*/
#pragma once

namespace Folders
{
  class Tenants : public Folder
  {
  private:
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Tenants(void);
   ~Tenants(void) noexcept(false);
  
    Footer* aFooter(void);
  };
  
  Tenants* _pTenants(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
