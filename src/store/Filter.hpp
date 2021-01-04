/*============================================================================*/
/* Filter.hpp -- Filter values                                                */
/*============================================================================*/
#pragma once

#include "eAM.h"

namespace STORE
{
  class Filter : public Definition
  {
  public:

    TagSet m_oTagSet;
    eAM    m_eMacro;

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Filter(void);
   ~Filter(void) noexcept(false);

    eEC eFilterValues(Index* pIndex, Stz* sFilterColumns);

  private:
    void vInsertLowerBound(eCT eColumnType, char* sValue, i8 iValue);
    void vInsertUpperBound(eCT eColumnType, char* sValue, i8 iValue);

    Footer* aFooter(void);
  };
  
  Filter* _pFilter(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
