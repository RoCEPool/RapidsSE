/*============================================================================*/
/* RowBuffer.hpp -- Row Buffer                                                */
/*============================================================================*/
#pragma once

namespace DataBuffers
{
  typedef       HV <Record,StreamDef>        HV_Rec;
  
  class RowBuffer : public DataBuffer,
                    public RowHeap_t<Row>
  {
  protected:
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    RowBuffer(eCN eClassNo);
   ~RowBuffer(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    
    Row* aAllotRow(i8 nBytes);
  
    Row* aFirstRow(void);
    Row* aLastRow(i8 nRowBytes);
    Row* aNextRow(Row* aPriorRow, i8 nRowBytes);
    void vPrintRows(StreamDef* pStreamDef);
    void vShow(StreamDef* pStreamDef);
  };
  
  RowBuffer* _pRowBuffer(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
