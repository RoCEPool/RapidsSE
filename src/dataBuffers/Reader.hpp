/*============================================================================*/
/* Reader.hpp - Assistant File Reader                                         */
/*============================================================================*/
#pragma once

namespace DataBuffers
{
  class Reader : public Object
  {
  public:
  
    File* m_pFile; // file being used

    i8   m_i_Seek;

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void operator delete(void*);

    Reader(void);
   ~Reader() noexcept(false);
  
    void  vCloseFile(void);
    bool  bOpenRead(Stz* szPath);
    i8    nReadLines(Stz* szInto, i8 nMaxBytes);
  
    CsvBuffer* pReadCsvBuffer(void);

    Footer* aFooter(void);
  };

  Reader* _pReader(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
