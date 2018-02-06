/*============================================================================*/
/* KeyWrk.hpp -- Key Working Object                                           */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class Key;
  
  class KeyWrk_v1 : public Object
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

    void vAddDatum(ValWrk& xValWrk, Column* pCol);
    void vAddDatum(ValWrk& xValWrk, Column* pCol, Formatters* pFormatters);
    void vAddStz(Stz* aDatum);
    void vAddKey(Key* aKey, IdxColVec* pColVec);
    void vAddWord(w8 wWord);
  
    void  vReset(void);
  
    char* aWspArea(void);
    Stz*  aWspStr(void);
    i8    nWspUsed(void);
    Key*  aKey(void);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
