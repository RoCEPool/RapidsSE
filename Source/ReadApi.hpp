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
  
    IndexTree* m_pIndexTree; // the index for this 
  
    CrvBuffer* m_pCrvBufferHead;  // head of crv buffer list
    CrvBuffer* m_pCrvBufferTail;  // tail of crv buffer list
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    ReadApi(void);
   ~ReadApi(void) noexcept(false);
  
    void vAddCrwVersion(Rrf xwCurrentRef, Crw* pCrw);
    Crw* aGetCrwConsistent(Rrf xwCurrentRef, t8 tPerspective);
    Crw* aGetCrwVersion(Rrf xwCurrentRef, t8 tPerspective);
    Crw* aGetCrwVersion(Rrf xwCurrentRef, Crw* aCurrentCrw, t8 tPerspective);
    void vPurgePriorVersions(t8 tPerspective);

    void vStreamDef(StreamDef* pStreamDef);

  private:
    void vAddVersionEntry(Rrf xwCurrentRef,
                          Rrf xwVersionRef, t8 tVersionPsp);
  
    Rrf  xwAddCrvItem(Rrf xwCurrentRef, Crw* pCrw);
    void vPurgeEntBuffers(t8 tPerspective);
    void vPurgeCrvBuffers(t8 tPerspective);
    
    Footer* aFooter(void);
  };
  
  ReadApi* _pReadApi(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
