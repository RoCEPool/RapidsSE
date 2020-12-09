/*============================================================================*/
/* CrwToRow.hpp -- Crw to Row                                                 */
/*============================================================================*/
#pragma once

namespace Load
{
  class CrwToRow : public Object
  {
  private:

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    CrwToRow(StreamDef* pStreamDef);
   ~CrwToRow(void) noexcept(false);
  
    bool bConvertCrwToRow(CrwBuffer* pCrwBuffer);

  private:
    Footer* aFooter(void);
  };

  CrwToRow* _pCrwToRow(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
