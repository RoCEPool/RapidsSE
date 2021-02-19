/*============================================================================*/
/* Metadata.hpp -- Metadata                                                       */
/*============================================================================*/
#pragma once

namespace STORE
{
  class Metadata : public Object
  {
  private:
      
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Metadata(eCN eClassNo);
   ~Metadata(void) noexcept(false);
  
  private:
    Footer* aFooter(void);
  };

Metadata* _pMetadata(void* aAddr);

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
