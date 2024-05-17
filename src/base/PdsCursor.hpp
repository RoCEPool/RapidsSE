/*============================================================================*/
/* PdsCursor.hpp -- Cursor for Primary Data Structure (PDS)                   */
/*============================================================================*/
#pragma once

#include "BASE.h"
#include "Pds.hpp"

namespace BASE
{
  class PdsCursor : public Object
  {

  public:
    Pds* m_pPds;

    SkipListBlock* m_pCurrentBlock;

    // Save the last usable offset for asserts

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    PdsCursor(Pds* pPds);
    PdsCursor(Pds* pPds, SkipListBlock* pCurr);
    ~PdsCursor(void) noexcept(false);

    static void vRepair(Object *pObject);

    bool bIsCurrNull(void);
    void vCurr(SkipListBlock* pCurr);
    Row* pCurrRow(void);
    void* pCurrKey(void);
    void vNext(void);
    void vReset(void);



  private:
    Footer* aFooter(void);
  };

  PdsCursor* _pPdsCursor(void* aAddr);

}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
