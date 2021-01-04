/*============================================================================*/
/* Range.hpp -- Range values                                                  */
/*============================================================================*/
#pragma once

namespace STORE
{
  class Range : public Definition
  {
  public:

    TagSet m_oTagSet;
    eAM    m_eMacro;

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Range(void);
   ~Range(void) noexcept(false);

    eEC eRangeValues(Index* pIndex, Stz* sRangeColumns);

  private:
    void vInsertLowerBound(eCT eColumnType, char* sValue, i8 iValue);
    void vInsertUpperBound(eCT eColumnType, char* sValue, i8 iValue);

    Footer* aFooter(void);
  };
  
  Range* _pRange(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
