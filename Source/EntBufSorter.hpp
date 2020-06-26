/*============================================================================*/
/* SkipRoots.hpp -- roots of the skips list in an entry buffer                */
/*============================================================================*/
#pragma once

namespace Indexing
{
static const i8 C_SRT_ENT_INT = C_32KB;

  class EntBufSorter : public Object
  {
  private:
 
    Ent_int* m_aaEnt_int[C_SRT_ENT_INT];
  
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
