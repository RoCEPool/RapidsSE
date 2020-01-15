/*============================================================================*/
/* ReadApi.hpp -- Read Consistency API                                        */
/*============================================================================*/
#pragma once

namespace ReadConsistency
{
  class ReadApi : public Object
  {
  private:
    StreamDef* m_pStreamDef; // definition of the rows
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    ReadApi(void);
   ~ReadApi(void) noexcept(false);
  
    void vAddCrwVersion(Ref xwRef, Crw* pCrw);
    Crw* aGetCrwVersion(Ref xwRef, t8 tPerspective);
    void vPurgePriorVersions(t8 tPerspective);

    void vStreamDef(StreamDef* pStreamDef);

  private:
    Footer* aFooter(void);
  };
  
  ReadApi* _pReadApi(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
