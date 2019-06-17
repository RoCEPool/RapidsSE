/*============================================================================*/
/* IrwBuffer.hpp -- Inflated Row Buffer                                       */
/*============================================================================*/
#pragma once

namespace Transform
{
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
    Row* aNextRow(Row* aPriorRow);
    void vShow(void);
    
  private:
    Footer* aFooter(void);
  };
  
  IrwBuffer* _pIrwBuffer(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
