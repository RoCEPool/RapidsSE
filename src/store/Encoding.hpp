/*============================================================================*/
/* Encoding_.hpp -- Row Encoding                                              */
/*============================================================================*/
#pragma once

namespace STORE
{
  class Encoding : public Encoding_v2
  {
  private:
  
    //Encoding_v2* m_pEncoding_v2;

    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Encoding(StreamDef* pStreamDef);
   ~Encoding(void) noexcept(false);
  

  
  private:
    Footer* aFooter(void);
  };
  
  Encoding* _pEncoding(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
