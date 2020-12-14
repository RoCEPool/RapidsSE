/*============================================================================*/
/* Encoding_.hpp -- Row Encoding                                              */
/*============================================================================*/
#pragma once

namespace CompactBuffers
{
  class Encoding : public Object
  {
  private:
  
    Encoding_v2* m_pEncoding_v2;

    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Encoding(StreamDef* pStreamDef);
   ~Encoding(void) noexcept(false);
  
    i8   nBytesNeeded(void);
  
    void vEncodeRow(Row* pRow);
  
    void vMorphEncoding(_ColEnc* aColEnc);
    bool bOverFlowed(void);
    i8   nOverSize(void);
    void vSplitOver(void);
  
  private:
    Footer* aFooter(void);
  };
  
  Encoding* _pEncoding(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
