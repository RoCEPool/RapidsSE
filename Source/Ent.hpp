/*============================================================================*/
/* Ent.hpp -- Index Entry                                                     */
/*============================================================================*/
#pragma once

//#define __ENT_HEADER

namespace Entries
{
  static const i8 C_ENT_QUANTA = 32;

  typedef u2 tET; // opaque type for entry tickets

  static const tET C_ENT_END_TICKET = 0xFFFF;

  //----------------------------------------------------------------------------

  class Ent
  {
  private:

    #ifdef __ENT_HEADER
    _Header m_xHeader;
    #endif
  
    Ref m_xRef; // reference which is either :
                //   a) sub-ref to a child EntBuffer (Hn buffers), or
                //   b) row-ref to a crw row in a crwBuffer (occurs at H0 leaf buffers)
  
    tET m_taNextEntry[C_MAX_LVL_LEVELS]; // tickets to next entry by level
    union {
    u2  m_uEntryNo;  // used to verify the order of entries
    tET m_tOriginal; // used to verify the order of entries
    };

    union {
    Key m_xKey;     // the key (for the general case)
    i8  m_iIntElm0; // or first int element of a special case ent
    };
  
    // ... element sequence (for a KE_GENERAL key)
  
    // ... payload (m_uPayload would be true)
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, Ent*);
  
    Ent(eCN eClassNo, i8 nSizeEnt);
   ~Ent(void) noexcept(false);
  
    i8    iBuffer(void);
    void  vBuffer(i8 iBuffer);
  
    eCN   eClass(void);
  
    byte* ayElmSeq(void);
    i8    nElmSeqSize(void);
    void  vElmSeqSize(i8 nElmSeqSize);
    i8    iEntryNo(void);
    void  vEntryNo(i8 iEntryNo);

    void  vFree(bool bFree);
  
    i8    iIntElm0(void);
    void  vIntElm0(i8 iIntElm0);
    i8    iItem(void);
    void  vItem(i8 iItem);

    Key*  aKey(void);
    eKE   eKind(void);
    void  vKind(eKE eKind);

    tET   tNextEntry(i8 i_Level);
    void  vNextEntry(i8 i_Level, tET tNextEntry);
    void  vNextEntryDump(void);
    void  vNextEntryReset(void);

    Pay*  aPay(void);
    i8*   aiPayload(void);
    i8*   aiPayCell(i8 iCell);
    byte* ayPayload(void);
    bool  bPayload(void);
    void  vPayload(bool bPayload);
    i8    nPaySize(void);
    void  vPaySize(i8 nPaySize);

    Ref&  rxRef(void);
    void  vRef(Ref xRef);
    Ref   wRef(void);
    void  vReset(void);
  
    void  vSetEntry(Ent* aFromEntry);
    i8    nSizeBytes(void);
    i8    nSkipLevels(void);
    void  vSkipLevels(i8 nLevels);

    bool  bValidEnt(void);
    bool  bValidMagic(void);
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
