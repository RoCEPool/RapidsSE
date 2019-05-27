/*============================================================================*/
/* Catalog.hpp - Catalog                                                      */
/*============================================================================*/
#pragma once

namespace MetaData
{
  class Catalog : public Object
  {
  private:
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Catalog(void);
    ~Catalog(void) noexcept(false);
    
   
    Footer* aFooter(void);
  };
  
  Catalog* _pCatalog(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
