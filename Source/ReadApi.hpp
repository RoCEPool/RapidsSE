/*============================================================================*/
/* ReadApi.hpp -- Read Consistency API                                        */
/*============================================================================*/
#pragma once

namespace ReadConsistency
{
  class ReadApi : public Object
  {
  private:
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    ReadApi(void);
   ~ReadApi(void) noexcept(false);
  
    void vAddCrwVersion(Crw* pCrw, Ref xRef);
    Crw* aGetCrwVersion(t8 tPerspective);
    void vPurgePriorVersions(t8 tPerspective);

  private:
    Footer* aFooter(void);
  };
  
  ReadApi* _pReadApi(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
