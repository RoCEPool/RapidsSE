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
  
    IndexPtn* m_pIndexPtn; // the index for this 
  
    CrvBuffer* m_pCrvBufferHead;  // head of crv buffer list
    CrvBuffer* m_pCrvBufferTail;  // tail of crv buffer list
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    ReadApi(void);
   ~ReadApi(void) noexcept(false);
  
    void vAddCrwVersion(Ref xwCurrentRef, Crw* pCrw);
    Crw* aGetCrwConsistent(Ref xwCurrentRef, t8 tPerspective);
    Crw* aGetCrwVersion(Ref xwCurrentRef, t8 tPerspective);
    Crw* aGetCrwVersion(Ref xwCurrentRef, Crw* aCurrentCrw, t8 tPerspective);
    void vPurgePriorVersions(t8 tPerspective);

    void vStreamDef(StreamDef* pStreamDef);

  private:
    void vAddVersionEntry(Ref xwCurrentRef,
                          Ref xwVersionRef, t8 tVersionPsp);
  
    Ref  xwAddCrvItem(Ref xwCurrentRef, Crw* pCrw);
    void vPurgeEntBuffers(t8 tPerspective);
    void vPurgeCrvBuffers(t8 tPerspective);
    
    Footer* aFooter(void);
  };
  
  ReadApi* _pReadApi(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
