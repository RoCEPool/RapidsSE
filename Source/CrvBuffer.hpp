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
    Wsp_t<C_WSP_CSVBUFFER> m_xWsp;

    SeeMe m_xSeeMe;
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    CrvBuffer(void);
   ~CrvBuffer(void) noexcept(false);

    void vAddCrwVersion(Ref xwRef, Crw* pCrw);
    bool bSpaceFor(i8 nBytes);

  private:
    Footer* aFooter(void);
  };
  
  CrvBuffer* _pCrvBuffer(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
