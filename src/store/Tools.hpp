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
  
    void vGenerativeStreamDefs(Bench* pBench);
    void vProjectiveStreamDefs(Bench* pBench);
    void vRenumberCards(Bench* pBench);
    void vStreamOrder(Bench* pBench);
    void vSubstituteOrdinals(Bench* pBench);

  private:
  
    static StreamDef* pProduceStreamDef(Card_ProjectList* pCard_ProjectList);
  
    static void vReplaceOrdinals(Card_Pred* pCard_Pred,
                                 StreamDef* pStreamDef,
                                 Index*     pIndex=nullptr);
  

    Footer* aFooter(void);
  };

  Tools* _pTools(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
