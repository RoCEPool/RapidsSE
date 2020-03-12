/*============================================================================*/
/* RowBuffer.hpp -- Row Buffer                                                */
/*============================================================================*/
#pragma once

namespace DataBuffers
{
  typedef       HV <Record,StreamDef>        HV_Rec;
  
  class RowBuffer : public DataBuffer,
                    public RowHeap_t<Row2>
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
    
    Row2* aAllotRow(i8 nBytes);
  
    Row2* aFirstRow(void);
    Row2* aLastRow(i8 nRowBytes);
    Row2* aNextRow(Row2* aPriorRow, i8 nRowBytes);
    void vPrintRows(StreamDef* pStreamDef);
    void vShow(StreamDef* pStreamDef);
  };
  
  RowBuffer* _pRowBuffer(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
