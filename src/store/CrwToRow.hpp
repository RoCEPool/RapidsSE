/*============================================================================*/
/* CrwToRow.hpp -- Crw to Row                                                 */
/*============================================================================*/
#pragma once

namespace STORE
{
  class CrwToRow : public Object
  {
  private:

    StreamDef* m_pStreamDef;
    PdsBufferIdQueue* m_pBufferIdQueue;

  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    CrwToRow(StreamDef* pStreamDef);
   ~CrwToRow(void) noexcept(false);

    static void vRepair(Object* pObject);

    bool bConvertCrwToRow(CrwBuffer* pCrwBuffer);
  
    RowBuffer* pDanglingRowBuffer(void);
    RowBuffer* pFullRowBuffer(void);
    RowBuffer* pRowBuffer(void);
  
  private:
  
    void vAllotNewRow(void);
    void vNewRowBuffer(void);
  
    Footer* aFooter(void);
  };

  CrwToRow* _pCrwToRow(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
