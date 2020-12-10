/*============================================================================*/
/* CrwToRow.hpp -- Crw to Row                                                 */
/*============================================================================*/
#pragma once

namespace Load
{
  class CrwToRow : public Object
  {
  private:

    StreamDef* m_pStreamDef;
    RowBuffer* m_pRowBuffer;
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    CrwToRow(StreamDef* pStreamDef);
   ~CrwToRow(void) noexcept(false);
  
    bool bConvertCrwToRow(CrwBuffer* pCrwBuffer);
  
    RowBuffer* pDanglingRowBuffer(void);
    RowBuffer* pFullRowBuffer(void);
  
  private:
  
    void vNewRowBuffer(void);
  
    Footer* aFooter(void);
  };

  CrwToRow* _pCrwToRow(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
