/*============================================================================*/
/* SkipRoots.hpp -- roots of the skips list in an entry buffer                */
/*============================================================================*/
#pragma once

namespace Indexing
{
  static const i8 C_SRT_ENT_INT = C_32KB;

  int iEntBufSortCompare_IntElm0(const void * a, const void * b);
  int iEntBufSortCompare_Lvl_IntElm0(const void * a, const void * b);


  class EntBufSorter : public Object
  {
  private:
 
    tET m_taRevisedTicket[C_SRT_ENT_INT];
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    EntBufSorter(void);
   ~EntBufSorter(void) noexcept(false);
  
    void vSort_int(EntBuffer* pEntBuffer);
  
  private:
    Footer* aFooter(void);
  };

  EntBufSorter* _pEntBufSorter(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
