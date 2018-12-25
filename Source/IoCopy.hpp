/*============================================================================*/
/* IoCopy.hpp -- I/O Copy                                                     */
/*============================================================================*/
#pragma once

namespace Chunks
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class IoCopy : public Object
  {
  private:
    IoChunk*  m_pInChunk;
    IoChunk*  m_pOutChunk;

    ByteRate* m_pByteRate;
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    IoCopy(void);
    ~IoCopy(void) noexcept(false);
  
    void  vCloseBoth(void);
    bool  bCopyChunk(void);
    void  vOpenRead(s256& sPath);
    void  vOpenWrite(s256& sPath);
    void  vRewindInput(void);
    void  vWithStatistics(Stz* aPrefix);
  
  private:
    Footer* aFooter(void);
  };
  
  IoCopy* _pIoCopy(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
