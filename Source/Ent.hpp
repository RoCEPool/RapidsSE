/*============================================================================*/
/* Ent.hpp -- Index Entry                                                     */
/*============================================================================*/
#pragma once

namespace Indexes
{ 
  static const i8 C_NEO_SIZE = sizeof(i4);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class NEO // next entry offset array
  {
  public:
    i4 m_iOffset[1];  // offset in buffer where the next entry is
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Ent : public Header
  {
  protected:
    Id  m_xRef;  // reference (this is either a sub-ref to child entBuffer,
                 //            or a row-ref at H0 leaf buffers to row)
  
#ifdef WITH_DEBUG_VERSION
    i8  m_iSkipNo; // verify of order of skip items
#endif
  
    Key m_xKey;  // the key
  
    // ..keybytes (of the key)
  
    // ..neo (0, 1 or 2 next entry offsets)
  
  public:

  public:
    void* operator new(size_t);
    void* operator new(size_t, Ent*);
  
    Ent(eCN eClassNo, i8 nSizeEnt);
    ~Ent(void) noexcept(false);
  
    i8    iBuffer(void);
    void  vBuffer(i8 iBuffer);
  
    i8    nEntSize(void);
  
    i8    iItem(void);
    void  vItem(i8 iItem);

    Key*  aKey(void);
    byte* ayKeyBytes(void);
    i8    nKeyBytes(void);
    void  vKeyBytes(i8 nKeyBytes);

    i8    nLevels(void);
    void  vLevels(i8 nLevels);
  
    i4    iNEO(i8 i_Level);
    void  vNEO(i8 i_Level, i4 i_Offset);
    void  vNEOdump(void);
    i8    nNEOsize(void);

    Id&   xRef(void);
    void  vRef(Id xRef);
  
    i8    nSizeBytes(void);

    i8    iSkipNo(void);
    void  vSkipNo(i8 iSkipNo);

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
