/*============================================================================*/
/* Reader.hpp - Assistant File Reader                                         */
/*============================================================================*/
#pragma once

namespace Chunks
{
  class Reader : public Object
  {
  public:
  
    File m_oFile; // file being used

    i8   m_i_Seek;

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void operator delete(void*);

    Reader(void);
   ~Reader() noexcept(false);
  
    void  vCloseFile(void);
    bool  bOpenRead(s256& sPath);
    i8    nReadLines(Stz* szInto, i8 nMaxBytes);

    Footer* aFooter(void);
  };

  Reader* _pReader(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
