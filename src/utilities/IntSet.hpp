/*============================================================================*/
/* IntSet.hpp -- Int Set                                                      */
/*============================================================================*/
#pragma once

namespace Utilities
{
  class IntSet : public Object
  {
  private:
    s256 m_sDesc;
  
    ArrayNL_t<CN_INTSET,i8,C_64KB>*  m_pIntAry;
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    IntSet(void);
    IntSet(i8 nInts);
   ~IntSet(void) noexcept(false);
  
    void vAscending(void);
    void vAscending(i8 iStart, i8 iStride);
    void vDescending(void);
    void vDescending(i8 iStart, i8 iStride);
    void vRandomise(void);
    void vRandoms(i8 iMin, i8 iMax);
    void vRandomsUnique(i8 iMin, i8 iMax);
    void vSort(void);
    void vSortDescend(void);

    Stz* szDesc(void);
    void vDescReset(void);
    bool bHas(i8 iInt);
    i8 iFind(i8 iInt);
    i8 iGet(i8 i_Item);
    i8 nInts(void);
    void vPrint(bool bNewLine = true);
    bool bSearch(i8 iInt);
    void vSet(i8 i_Item, i8 iInt);

    Footer* aFooter(void);
  };

  int iSortCompare_ascend(const void * a, const void * b);
  int iSortCompare_descend(const void * a, const void * b);

  IntSet* _pIntSet(void* aAddr);
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
