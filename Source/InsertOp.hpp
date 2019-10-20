/*============================================================================*/
/* InsertOp.hpp -- Insert                                                     */
/*============================================================================*/
#pragma once

namespace DataOperators
{
  class InsertOp : public Op
  {
  private:
    
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    InsertOp(OpSpec* pOpSpec, Pipeline* pPipeline);
   ~InsertOp(void) noexcept(false);
  
    static Op* pNew(OpSpec* pOpSpec, Pipeline* pPipeline);
    static eOR eCall(void* aThis);
    static eOR eDrain(void* aThis);

    eOR  eOpLoop(void);
    eOR  eDrainLoop(void);

  private:
    Footer* aFooter(void);
  };

  InsertOp* _pInsertOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
