/*============================================================================*/
/* IndexDef.hpp -- Index Definition                                           */
/*============================================================================*/
#pragma once

namespace MetaData
{
  class IndexDef : public Object
  {
  friend class Index;
  
  private:
    s32  m_sIndexNm;      //
    eIT  m_eIndexType;    //
  
    bool m_bIsPrimaryKey; //
    bool m_bIsUnique;     //
  
    IdxColVec*   m_pIdxColVec;    //
  
    s4K m_sCmd;  // the command which created this def

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    IndexDef(void);
   ~IndexDef(void) noexcept(false);
  
    Stz*   pAnalyze(void);
    Stz*   szIndexNm(void);
    eEC    eParse(Stz* szCmd);
    bool   bIsPrimaryKey(void);
    void   vIsPrimaryKey(bool bUnique);
    bool   bIsUnique(void);
    void   vIsUnique(bool bUnique);
    void   vShowColumnVec(void);
    i8     iSortKeyDatum(void);
  
    IdxColVec* pIdxColVec(void);
  
    Footer* aFooter(void);
  };
  
  IndexDef* _pIndexDef(void* pAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
