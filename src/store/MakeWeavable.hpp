/*============================================================================*/
/* MakeWeavable.hpp -- Make Weavable                                          */
/*============================================================================*/
#pragma once

#include "Bench.hpp"


namespace STORE
{
  class MakeWeavable : public Object
  {
  private:

    CardIter* m_pCI;

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    MakeWeavable(CardIter* pCI);
   ~MakeWeavable(void) noexcept(false);

    void vWeavable(void);

  private:

    void vAddStageCasts(void);
    void vCullNonusedBounds(void);
    void vCullUsedBoundsFromFilter(void);
    void vIndividuateFilter(void);
    void vReinstateStages(void);
    void vStmColToColNmOfKeyExists(void);
    void vTblColToStmCol(void);

  protected:
    Footer* aFooter(void);
  };

  MakeWeavable* _pMakeWeavable(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
