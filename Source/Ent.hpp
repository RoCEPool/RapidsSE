/*============================================================================*/
/* Ent.hpp -- Index Entry                                                     */
/*============================================================================*/
#pragma once

namespace Entries
{
  class Ent : public Header
  {
  private:
    Ref m_xRef;    // reference (this is either a sub-ref to a child EntBuffer,
                   //            or a row-ref to a row at H0 leaf buffers)
  
    i4  m_iaOffset[6];  // the offsets (not used yet)

#ifdef WITH_DEBUG_VERSION
    i8  m_uSkipNo; // verify of order of skip items
#endif
  
    Key m_xKey;    // the key
  
    // ..element sequence (of the key)
  
    // ..neo (1, 2 or 3 next entry offsets)
  
  public:

  public:
    void* operator new(size_t);
    void* operator new(size_t, Ent*);
  
    Ent(eCN eClassNo, i8 nSizeEnt);
   ~Ent(void) noexcept(false);
  
    i8    iBuffer(void);
    void  vBuffer(i8 iBuffer);
  
    byte* ayElmSeq(void);
    i8    nElmSeqSize(void);
    void  vElmSeqSize(i8 nElmSeqSize);
  
    i8    iItem(void);
    void  vItem(i8 iItem);

    Key*  aKey(void);

    i4    iNEO(i8 i_Level);
    void  vNEO(i8 i_Level, i4 i_Offset);
    void  vNEOdump(void);

    i8    nOffsets(void);
    void  vOffsets(i8 nLevels);
    
    Ref&  xRef(void);
    void  vRef(Ref xRef);
    Ref   wRef(void);
    void  vReset(void);
  
    void  vSetEntry(Ent* aFromEntry);

    i8    nSizeBytes(void);

    i8    iSkipNo(void);
    void  vSkipNo(i8 iSkipNo);

    bool  bValidEnt(void);
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
