/*============================================================================*/
/* IdxColVec.hpp -- Index Column Vector                                       */
/*============================================================================*/
#pragma once

namespace MetaData
{
  class IdxColVec : public VectorNL_t<CN_IDXCOLVEC,Column,1024>
  {
  private:
    Index* m_pIndex;  // which index
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    IdxColVec(void);
   ~IdxColVec(void) noexcept(false);
  
    Index*  pIndex(void);
    void    vIndex(Index* pIndex);

    Footer* aFooter(void);
  };
  
  IdxColVec* _pIdxColVec(void* pAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
