/*============================================================================*/
/* IdxColVec.hpp -- Index Column Vector                                       */
/*============================================================================*/
#pragma once

namespace MetaData
{
  class IdxColVec : public VectorNL_t<CN_IDXCOLVEC,Column,C_MAX_IDXCOLVEC>
  {
  private:
    IndexDef* m_pIndexDef;  // which index definition
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    IdxColVec(IndexDef* pIndexDef);
   ~IdxColVec(void) noexcept(false);
  
    IndexDef* pIndexDef(void);
    void vIndexDef(IndexDef* pIndexDef);

    Footer* aFooter(void);
  };
  
  IdxColVec* _pIdxColVec(void* pAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
