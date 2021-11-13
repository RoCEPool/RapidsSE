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

  private:
    static bool bIsOrdinalColumnAtom(Card_Atom* pCard_Atom);
    static bool bIsOrdinalIndexAtom(Card_Atom* pCard_Atom);
    static i8   iOrdinalColumnValue(Card_Atom* pCard_Atom);
    static i8   iOrdinalIndexValue(Card_Atom* pCard_Atom);

    static void vReplaceOrdinals(Card_ComparePred* pCard_ComparePred,
                                 StreamDef* pStreamDef);
    static void vReplaceOrdinals(Card_IndexPred* pCard_IndexPred,
                                 StreamDef* pStreamDef);

    Footer* aFooter(void);
  };

  Tools* _pTools(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
