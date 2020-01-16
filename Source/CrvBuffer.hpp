/*============================================================================*/
/* CrvBuffer.hpp -- Compact Row Version Buffer                                */
/*============================================================================*/
#pragma once

namespace ReadConsistency
{
  class CrvBuffer : public DataBuffer
  {
    static const i8 C_WSP_CSVBUFFER = C_SIZE_LARGE_OBJECT - C_4KB;
  
  public:
    StreamDef* m_pStreamDef;
  
    i8 m_nCrv; // number of crv's in this buffer
  
    Wsp_t<C_WSP_CSVBUFFER> m_xWsp;

    SeeMe m_xSeeMe;
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    CrvBuffer(StreamDef* pStreamDef);
   ~CrvBuffer(void) noexcept(false);

    bool bAddCrwVersion(Ref xwRef, Crw* pCrw);

  private:
    bool bHasSpace(i8 nBytes);
    Footer* aFooter(void);
  };
  
  CrvBuffer* _pCrvBuffer(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
