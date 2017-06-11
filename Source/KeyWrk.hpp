/*============================================================================*/
/* KeyWrk.hpp -- Key Working Object                                           */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class Key;
  
  class KeyWrk : public Object
  {
  private:
    WspFxd<C_1KB*15> m_xWsp;
    Footer           m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, KeyWrk*);
    
    KeyWrk(void);
    KeyWrk(Key* aKey, IdxColVec* pColVec);
    ~KeyWrk(void);
  
    void vShow(void);
    void vShow(eDM eDatum, ...);

    void vAddDatum(ValWrk& xValWrk, Column* pCol);
    void vAddDatum(StrVal* pValStr, Column* pCol);
    void vAddKey(Key* aKey, IdxColVec* pColVec);
  
    void  vReset(void);
  
    char* aWspArea(void);
    Str*  aWspStr(void);
    i8    nWspUsed(void);
    Key*  aKey(void);

    bool  bExtractElement(s48K& sRow, i8 iElement, eDM eDatum);

    Footer* pFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
