/*============================================================================*/
/* KeyWsp.hpp -- Key Workspace                                                */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class Key;
  
  class KeyWsp : public Object
  {
  private:
    WspFxd<C_1KB*15> m_xWsp;
    Footer           m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, KeyWsp*);
    
    KeyWsp(void);
    ~KeyWsp(void);
  
    void vShow(void);
    void vShow(eDM eDatum, ...);

    void vAddElement(ValWsp& xValWsp, Column* pCol);
    void vAddElement(StrVal* pValStr, Column* pCol);
    void vAddKey(Key* pKey, IdxColVec* pColVec);
  
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
