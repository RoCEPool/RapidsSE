/*============================================================================*/
/* MakeCode.hpp -- Call Synthesiser                                           */
/*============================================================================*/
#pragma once

#include "Bench.hpp"
#include "CardIter.hpp"

#include "PipelineDef.hpp"

namespace STORE
{
  class MakeCode : public Object
  {
  private:

    CardIter*    m_pCI;

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    MakeCode(CardIter* pCI);
   ~MakeCode(void) noexcept(false);

    void vSynthesise(void);

  private:
    PipelineDef* pCraftPipelineDef(Board* pBoard);

    StageDef*  pCraftStageDef(Card_OP* p_OP);
    OpDef*     pCraftOpDef(Card_OP* p_OP);
    void       vCraftArgDefs(Card_OP* p_OP, OpDef* pOpDef);

  protected:
    Footer* aFooter(void);
  };

  MakeCode* _pMakeCode(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
