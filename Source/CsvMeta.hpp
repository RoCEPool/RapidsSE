/*============================================================================*/
/* CsvMeta.hpp -- Metadata about a csv chunk                                  */
/*============================================================================*/
#pragma once

namespace DataBuffers
{
  class CsvMeta : public Object
  {
  private:
    
    s32  m_sOriginNm;  // on which node did the csv chunk come from
    s256 m_sFilePath;  // from which file
    i8   m_iOffset;    // at what offset
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    CsvMeta(void);
   ~CsvMeta(void) noexcept(false);
    
  public:
  
    Stz* szFilePath(void);
    void vFilePath(Stz* szFilePath);
    
    i8   iOffset(void);
    void vOffset(i8 iOffset);

    Stz* szOriginNm(void);
    void vOriginNm(Stz* szOriginNm);
  
  private:
    Footer* aFooter(void);
  };
  
  CsvMeta* _pCsvMeta(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
