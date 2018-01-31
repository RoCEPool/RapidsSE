/*============================================================================*/
/* KeyWrk.hpp -- Key Working Entry                                            */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  
  class KeyWrk_v2 : public EntWrk_v2
  {
  private:
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, KeyWrk_v2*);
    
    KeyWrk_v2(void);
    KeyWrk_v2(Key* aKey, IdxColVec* pColVec);
    ~KeyWrk_v2(void);
    
    void vAddDatum(ValWrk& xValWrk, Column* pCol);
    void vAddDatum(ValWrk& xValWrk, Column* pCol, Formatters* pFormatters);
    void vAddStz(Stz* aDatum);
    void vAddKey(Key* aKey, IdxColVec* pColVec);
    void vAddWord(w8 wWord);
    Key* aKey(void);
    i8   nKeySize(void);
    void vReset(void);
    void vShow(IdxColVec* pIdxColVec);
    
    //void   vAddKey(KeyWrk& xKeyWrk);
    //void   vForm(Id& xId, KeyWrk& xKeyWrk);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
