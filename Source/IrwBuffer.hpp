/*============================================================================*/
/* IrwBuffer.hpp -- Inflated Row Buffer                                       */
/*============================================================================*/
#pragma once

namespace Transform
{
  typedef       HV <Record,StreamDef>                            HV_Rec;
  
  class IrwBuffer : public DataBuffer,
                    public MiniHeap_t<Row>
  {
  protected:
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    IrwBuffer(void);
   ~IrwBuffer(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    
    Row* aAllotRow(ColumnVec* pColVec);
  
    Row* aFirstRow(void);
    Row* aLastRow(void);
    Row* aNextRow(Row* aPriorRow);
    void vPrintRows(StreamDef* pStreamDef);
    void vShow(StreamDef* pStreamDef);
    
  private:
    Footer* aFooter(void);
  };
  
  IrwBuffer* _pIrwBuffer(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
