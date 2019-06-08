/*============================================================================*/
/* Ent.hpp -- Index Entry                                                     */
/*============================================================================*/
#pragma once

namespace Data
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class NEO // next entry offset
  {
  public:
    i4 m_iOffset[1];  // offset where the next entry is
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Ent : public Header
  {
  protected:
    Id  m_xId; // id
    i4  m_nSizeItem_WithoutOffsets; // size of this item (not including offsets)
    // ..key bytes..
    // ..neo..
  
  public:
#ifdef WITH_DEBUG_VERSION
    i8  m_iSkipNo;    // verify of order of skip items
#endif

  public:
    void* operator new(size_t);
    void* operator new(size_t, Ent*);
  
    Ent(eCN eClassNo, i8 nSizeItem);
    ~Ent(void) noexcept(false);
  
    Id&   xId(void);
    void  vId(Id xId);
  
    byte* ayKeyBytes(void);
  
    i8    nLevels(void);
    void  vLevels(i8 nLevels);
  
    i4    iNEO(i8 iLevel);
    void  vNEO(i8 iLevel, i4 iNEO);
    void  vNEOdump(void);
  
    i8    nSizeBytes(void);
    i8    nSizeKey(void);
    i8    nSizeNeo(void);

    bool  bValidEnt(void);
  
  private:
    NEO*  aNEO(void);
 };
  
  /*--------------------------------------------------------------------------*/
  /* safe cast                                                                */
  /*--------------------------------------------------------------------------*/
  inline Ent* _aEnt(void* aAddr)
  {
    Ent* aEnt = reinterpret_cast<Ent*>(aAddr);
    
    return aEnt;
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
