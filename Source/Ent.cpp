/*============================================================================*/
/* Ent.cpp -- Index Entry                                                     */
/*============================================================================*/

#include "Entries.h"

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
  MemLib::vZero(m_taNextEntry,sizeof(m_taNextEntry),eClassNo);
  
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
/* get next entry ticket for given level                                      */
/*----------------------------------------------------------------------------*/
tET Ent::tNextEntry(i8 i_Level)
{
  ASSERT(i_Level >= 0 && i_Level < nSkipLevels());

  tET tNextEntry = m_taNextEntry[i_Level];
  
  return tNextEntry;
}

/*----------------------------------------------------------------------------*/
/* set next entry ticket for given level                                      */
/*----------------------------------------------------------------------------*/
void Ent::vNextEntry(i8 i_Level, tET tNextEntry)
{
  ASSERT(i_Level >= 0 && i_Level < nSkipLevels());

  m_taNextEntry[i_Level] = tNextEntry;
}

/*----------------------------------------------------------------------------*/
/* dump the NEO associated with this entry                                    */
/*----------------------------------------------------------------------------*/
void Ent::vNextEntryDump(void)
{
  i8 nLvls  = nSkipLevels();
  
  vTrace("Ent %p levels: %lld"_Stz,
         this,nLvls);
  
  for (i8 i_Level = 0; i_Level < nLvls; i_Level++)
    vTrace(" %lld: %lld"_Stz,i_Level,(i8)m_taNextEntry[i_Level]);
  
  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* reset the neo array                                                        */
/*----------------------------------------------------------------------------*/
void Ent::vNextEntryReset(void)
{
  for (i8 i_Level = 0; i_Level < C_MAX_LVL_LEVELS; i_Level++)
    m_taNextEntry[i_Level] = C_ENT_END_TICKET;
}

/*----------------------------------------------------------------------------*/
/* get ref (sub-ref to child entBuffer or row-ref at H0 leaf buffers to row)  */
/*----------------------------------------------------------------------------*/
Ref& Ent::rxRef(void)
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
  
  vRef(aFromEntry->rxRef());
  vSkipLevels(aFromEntry->nSkipLevels());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // copy key into this entry
  
  byte* aDstKey  = this->ayElmSeq();
  byte* aSrcKey  = aFromEntry->ayElmSeq();
  i8    nKeySize = aFromEntry->nElmSeqSize();
  
  m_xKey.vElmSeqSize(nKeySize);

  MemLib::vCopy(aDstKey,aSrcKey,nKeySize,CN_ENT);
}

/*----------------------------------------------------------------------------*/
/* get size of entry                                                          */
/*----------------------------------------------------------------------------*/
i8 Ent::nSizeBytes(void)
{
  i8 nProperBytes = sizeof(Ent) + nElmSeqSize();
  
  i8 nRoundedBytes = MemLib::nRoundUp(nProperBytes,C_ENT_QUANTA);

  return nRoundedBytes;
}

/*----------------------------------------------------------------------------*/
/* get number of skip levels                                                  */
/*----------------------------------------------------------------------------*/
i8 Ent::nSkipLevels(void)
{
  return m_xRef.nOffsets();
}

/*----------------------------------------------------------------------------*/
/* set number of skip levels                                                  */
/*----------------------------------------------------------------------------*/
void Ent::vSkipLevels(i8 nLevels)
{
  m_xRef.vOffsets(nLevels);
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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
