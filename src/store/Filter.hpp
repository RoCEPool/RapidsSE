/*============================================================================*/
/* Filter.hpp -- Filter columns                                             */
/*============================================================================*/
#pragma once

namespace STORE
{
  class Filter : public Definition
  {
  public:

    u8      m_uFilterColumns;
    s4K     m_sAsmText;

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Filter(void);
   ~Filter(void) noexcept(false);

    eEC eFilterRows(StreamDef* pStreamDefIn, Stz* sFilter);

  private:
    eEC eFilterColumn(StreamDef* pStreamDefIn, TokenVec* pTknVec);

    Footer* aFooter(void);
  };
  
  Filter* _pFilter(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
