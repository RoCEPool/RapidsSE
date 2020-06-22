/*============================================================================*/
/* Ent.cpp -- Index Entry                                                     */
/*============================================================================*/

#include "Entries.h"

#define __USE_ALT_OFFSETS

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Ent::operator new(size_t)
{
  vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
         "fatal: Ent class cannot be new'd\n"_Stz);
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Ent::operator new(size_t, Ent* aEnt)
{
  return aEnt;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Ent::Ent(eCN eClassNo, i8 nSizeEnt)
: Header(eClassNo)
{  
#ifdef WITH_DEBUG_VERSION
  MemLib::vZero(m__iaOffset__,sizeof(m__iaOffset__),eClassNo);
  
  m_uSkipNo = 0;
#endif
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Ent::~Ent(void) noexcept(false)
{
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get buffer                                                                 */
/*----------------------------------------------------------------------------*/
i8 Ent::iBuffer(void)
{
  return m_xRef.iBuffer();
}

/*----------------------------------------------------------------------------*/
/* set buffer                                                                 */
/*----------------------------------------------------------------------------*/
void Ent::vBuffer(i8 iBuffer)
{
  m_xRef.vBuffer(iBuffer);
}

/*----------------------------------------------------------------------------*/
/* get address of the element bytes of the key in this entry                  */
/*----------------------------------------------------------------------------*/
byte* Ent::ayElmSeq(void)
{
  byte* ayElmSeq = aKey()->ayElmSeq();
  
  ASSERT(ayElmSeq == aAdd<byte*>(this,sizeof(Ent)));
  
  return ayElmSeq;
}

/*----------------------------------------------------------------------------*/
/* get size of the element sequence                                           */
/*----------------------------------------------------------------------------*/
i8 Ent::nElmSeqSize(void)
{
  i8 nBytes = m_xKey.nElmSeqSize();
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* set size of the element sequence                                           */
/*----------------------------------------------------------------------------*/
void Ent::vElmSeqSize(i8 nElmSeqSize)
{
  m_xKey.vElmSeqSize(nElmSeqSize);
}

/*----------------------------------------------------------------------------*/
/* get size of alt entry using m__iaOffset__ with key  (no trailing neo)      */
/*----------------------------------------------------------------------------*/
i8 Ent::nEntSize_alt(void)
{
  i8 nSizeEnt_alt = sizeof(Ent);
  i8 nSizeElmSeq  = nElmSeqSize();

  i8 nSize_alt = nSizeEnt_alt + nSizeElmSeq;

  return nSize_alt;
}

/*----------------------------------------------------------------------------*/
/* get size of entry with key, but not with neo's                             */
/*----------------------------------------------------------------------------*/
i8 Ent::nEntKeySize(void)
{
  i8 nSizeEnt    = sizeof(Ent);
  i8 nSizeElmSeq = nElmSeqSize();

  i8 nSize = nSizeEnt + nSizeElmSeq;

  return nSize;
}

/*----------------------------------------------------------------------------*/
/* get item                                                                   */
/*----------------------------------------------------------------------------*/
i8 Ent::iItem(void)
{
  return m_xRef.iItem();
}

/*----------------------------------------------------------------------------*/
/* set item                                                                   */
/*----------------------------------------------------------------------------*/
void Ent::vItem(i8 iItem)
{
  m_xRef.vItem(iItem);
}

/*----------------------------------------------------------------------------*/
/* get address of key                                                         */
/*----------------------------------------------------------------------------*/
Key* Ent::aKey(void)
{
  return &m_xKey;
}

/*----------------------------------------------------------------------------*/
/* get number of levels                                                       */
/*----------------------------------------------------------------------------*/
i8 Ent::nOffsets(void)
{
  return m_xRef.nOffsets();
}

/*----------------------------------------------------------------------------*/
/* set number of levels                                                       */
/*----------------------------------------------------------------------------*/
void Ent::vOffsets(i8 nLevels)
{
  m_xRef.vOffsets(nLevels);
}

/*----------------------------------------------------------------------------*/
/* get next entry offset for given level                                      */
/*----------------------------------------------------------------------------*/
i4 Ent::iNEO(i8 i_Level)
{
  ASSERT(i_Level >= 0 && i_Level < nOffsets());

  NEO* aNEO = this->aNEO();

  i4 iNEO = aNEO->m_iOffset[i_Level];
  
  return iNEO;
}

/*----------------------------------------------------------------------------*/
/* set next entry offset for given level                                      */
/*----------------------------------------------------------------------------*/
void Ent::vNEO(i8 i_Level, i4 i_Offset)
{
  ASSERT(i_Level >= 0 && i_Level < nOffsets());

  NEO* aNEO = this->aNEO();

  aNEO->m_iOffset[i_Level] = i_Offset;
}

/*----------------------------------------------------------------------------*/
/* dump the NEO associated with this entry                                    */
/*----------------------------------------------------------------------------*/
void Ent::vNEOdump(void)
{
  NEO* aNEO = this->aNEO();
  i8 nLvls  = nOffsets();
  
  vTrace("Ent %p levels: %lld NEO: %p"_Stz,
         this,nLvls,aNEO);
  
  for (i8 i_Level = 0; i_Level < nLvls; i_Level++)
    vTrace(" %lld: %d"_Stz,i_Level,aNEO->m_iOffset[i_Level]);
  
  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* get size of the NEO after this entry                                       */
/*----------------------------------------------------------------------------*/
i8 Ent::nNEOsize(void)
{
#ifdef WITH_FIXED_NEO
  i8 nLvls  = AM::nLvlLevels();
#else
  i8 nLvls  = nOffsets();
#endif
  
  i8 nBytes = C_NEO_SIZE * nLvls;
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* get ref (sub-ref to child entBuffer or row-ref at H0 leaf buffers to row)  */
/*----------------------------------------------------------------------------*/
Ref& Ent::xRef(void)
{
  return m_xRef;
}

/*----------------------------------------------------------------------------*/
/* set reference                                                              */
/*----------------------------------------------------------------------------*/
void Ent::vRef(Ref xRef)
{
  m_xRef = xRef;
}

/*----------------------------------------------------------------------------*/
/* get ref (sub-ref to child entBuffer or row-ref at H0 leaf buffers to row)  */
/*----------------------------------------------------------------------------*/
Ref Ent::wRef(void)
{
  return m_xRef;
}

/*----------------------------------------------------------------------------*/
/* reset this entry                                                           */
/*----------------------------------------------------------------------------*/
void Ent::vReset(void)
{
  m_xRef.vReset();
  m_xKey.vReset();
}

/*----------------------------------------------------------------------------*/
/* set this entry from given entry                                            */
/*----------------------------------------------------------------------------*/
void Ent::vSetEntry(Ent* aFromEntry)
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set entry reference and offsets
  
  vRef(aFromEntry->xRef());
  vOffsets(aFromEntry->nOffsets());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // copy key into this entry
  
  byte* aDstKey  = this->ayElmSeq();
  byte* aSrcKey  = aFromEntry->ayElmSeq();
  i8    nKeySize = aFromEntry->nElmSeqSize();
  
  m_xKey.vElmSeqSize(nKeySize);

  MemLib::vCopy(aDstKey,aSrcKey,nKeySize,CN_ENT);
}

/*----------------------------------------------------------------------------*/
/* get size of entry with neo                                                 */
/*----------------------------------------------------------------------------*/
i8 Ent::nSizeBytes(void)
{
  i8 nBytes = sizeof(Ent) + nElmSeqSize() + nNEOsize();

  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* set skip no                                                                */
/*----------------------------------------------------------------------------*/
i8 Ent::iSkipNo(void)
{
#ifdef WITH_DEBUG_VERSION
  return m_uSkipNo;
#else
  return C_UNKNOWN;
#endif
}

/*----------------------------------------------------------------------------*/
/* set skip no                                                                */
/*----------------------------------------------------------------------------*/
void Ent::vSkipNo(i8 iSkipNo)
{
#ifdef WITH_DEBUG_VERSION
  m_uSkipNo = (u8)iSkipNo;
#endif
}

/*----------------------------------------------------------------------------*/
/* test for valid entry                                                       */
/*----------------------------------------------------------------------------*/
bool Ent::bValidEnt(void)
{  
  if (! bValidMagic()) {
    vTrace("Not valid magic\n"_Stz);
    return false;
  }
  
  if ( ! (CN_ENT == eClass() || CN_FREEITEM == eClass()) ) {
    vTrace("Not valid class\n"_Stz);
    return false;
  }
    
  return true;
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get address of next entry offset array                                     */
/*----------------------------------------------------------------------------*/
NEO* Ent::aNEO(void)
{
#ifdef __USE_ALT_OFFSETS

  NEO* aNEO = (NEO*)m__iaOffset__;
  
#else
  
  i8 i_NEO = sizeof(Ent) + m_xKey.nElmSeqSize();
  
  NEO* aNEO = aAdd<NEO*>(this,i_NEO);
  
#endif
  
  return aNEO;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
