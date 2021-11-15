/*============================================================================*/
/* Tools.hpp -- Drafting Tools                                                */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Board.hpp"

namespace STORE
{
  class Tools : public Object
  {
  private:
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Tools(void);
   ~Tools(void) noexcept(false);
  
    void vExpandStreamDefs(Bench* pBench);
    void vSubstituteOrdinals(Bench* pBench);
    void vSubstituteOrdinals2(Bench* pBench);

  private:
    static void vReplaceOrdinals(Card_ComparePred* pCard_ComparePred,
                                 StreamDef* pStreamDef);
  
    static void vReplaceOrdinals(Card_IndexPred* pCard_IndexPred,
                                 StreamDef* pStreamDef,
                                 Index* pIndex);

    Footer* aFooter(void);
  };

  Tools* _pTools(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
