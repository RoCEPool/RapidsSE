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
{
#ifdef __ENT_HEADER
  m_xHeader.vClass(CN_ENT);
  m_xHeader.vMagic();
#endif
  
  m_uEntryNo = C_ENT_END_TICKET;

#ifdef WITH_DEBUG_VERSION
  MemLib::vZero(m_taNextEntry,sizeof(m_taNextEntry),eClassNo);
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
/* get referenced buffer                                                      */
/*----------------------------------------------------------------------------*/
i8 Ent::iBuffer(void)
{
  return m_xRef.iBuffer();
}

/*----------------------------------------------------------------------------*/
/* set referenced buffer                                                      */
/*----------------------------------------------------------------------------*/
void Ent::vBuffer(i8 iBuffer)
{
  m_xRef.vBuffer(iBuffer);
}

/*----------------------------------------------------------------------------*/
/* get address of the element bytes of the key in this entry                  */
/*----------------------------------------------------------------------------*/
eCN Ent::eClass(void)
{
#ifdef __ENT_HEADER
  eCN eClass = m_xHeader.eClass();
#else
  eCN eClass = CN_ENT;
#endif
  
  return eClass;
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
  i8 nBytes;

  if (KE_GENERAL == m_xRef.eKind())
    nBytes = m_xKey.nElmSeqSize();
  else
    nBytes = 0;
  
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
/* set entry no                                                               */
/*----------------------------------------------------------------------------*/
i8 Ent::iEntryNo(void)
{
  return m_uEntryNo;
}

/*----------------------------------------------------------------------------*/
/* set entry no                                                               */
/*----------------------------------------------------------------------------*/
void Ent::vEntryNo(i8 iEntryNo)
{
  ASSERT(iEntryNo >= 0);
  
  m_uEntryNo = (u8)iEntryNo;
}

/*----------------------------------------------------------------------------*/
/* get integer elment 0 (permitted when m_uKind is KE_INT)                    */
/*----------------------------------------------------------------------------*/
i8 Ent::iIntElm0(void)
{
  ASSERT(KE_INT == m_xRef.eKind());
  
  return m_iIntElm0;
}

/*----------------------------------------------------------------------------*/
/* set integer elment 0 (permitted when m_uKind is KE_INT)                    */
/*----------------------------------------------------------------------------*/
void Ent::vIntElm0(i8 iIntElm0)
{
  ASSERT(KE_INT == m_xRef.eKind());
  
  m_iIntElm0 = iIntElm0;
}

/*----------------------------------------------------------------------------*/
/* get referenced item                                                        */
/*----------------------------------------------------------------------------*/
i8 Ent::iItem(void)
{
  return m_xRef.iItem();
}

/*----------------------------------------------------------------------------*/
/* set referenced item                                                        */
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
/* get entry key kind                                                        */
/*----------------------------------------------------------------------------*/
eKE Ent::eKind(void)
{
  eKE eKind = m_xRef.eKind();
  
  return eKind;
}

/*----------------------------------------------------------------------------*/
/* set entry key kind                                                        */
/*----------------------------------------------------------------------------*/
void Ent::vKind(eKE eKind)
{
  m_xRef.vKind(eKind);
}

/*----------------------------------------------------------------------------*/
/* get ticket of next entry for given level                                   */
/*----------------------------------------------------------------------------*/
tET Ent::tNextEntry(i8 i_Level)
{
  ASSERT(i_Level >= 0 && i_Level < C_MAX_LVL_LEVELS);

  tET tNextEntry = m_taNextEntry[i_Level];
  
  return tNextEntry;
}

/*----------------------------------------------------------------------------*/
/* set ticket of next entry for given level                                   */
/*----------------------------------------------------------------------------*/
void Ent::vNextEntry(i8 i_Level, tET tNextEntry)
{
  ASSERT(i_Level >= 0 && i_Level < C_MAX_LVL_LEVELS);

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
/* get address of Pay                                                         */
/*----------------------------------------------------------------------------*/
Pay* Ent::aPay(void)
{
  Key* aKey = this->aKey();
  
  i8   nElmSeqSize = this->nElmSeqSize();

  Pay* aPay = aAdd<Pay*>(aKey,sizeof(Key) + nElmSeqSize);
  
  return aPay;
}

/*----------------------------------------------------------------------------*/
/* get address of Pay                                                         */
/*----------------------------------------------------------------------------*/
byte* Ent::ayPayload(void)
{
  Pay* aPay = this->aPay();
  
  byte* ayPayload = aPay->ayaPayload();
  
  return ayPayload;
}

/*----------------------------------------------------------------------------*/
/* is there a payload?                                                        */
/*----------------------------------------------------------------------------*/
bool Ent::bPayload(void)
{
  bool bPayload = m_xRef.bPayload();
  
  return bPayload;
}

/*----------------------------------------------------------------------------*/
/* set is there a payload?                                                    */
/*----------------------------------------------------------------------------*/
void Ent::vPayload(bool bPayload)
{
  m_xRef.vPayload(bPayload);
}

/*----------------------------------------------------------------------------*/
/* get size of payload                                                        */
/*----------------------------------------------------------------------------*/
i8 Ent::nPaySize(void)
{
  Pay* aPay = this->aPay();
  
  i8 nPaySize = aPay->nPaySize();

  return nPaySize;
}

/*----------------------------------------------------------------------------*/
/* set size of payload                                                        */
/*----------------------------------------------------------------------------*/
void Ent::vPaySize(i8 nPaySize)
{
  Pay* aPay = this->aPay();
  
  aPay->vPayloadBytes(nPaySize);
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
  
  if (KE_INT == eKind()) {
    i8 iInt = aFromEntry->iIntElm0();
    m_iIntElm0 = iInt;
  }
  else {
    byte* aDstKey  = this->ayElmSeq();
    byte* aSrcKey  = aFromEntry->ayElmSeq();
    i8    nKeySize = aFromEntry->nElmSeqSize();
  
    m_xKey.vElmSeqSize(nKeySize);

    MemLib::vCopy(aDstKey,aSrcKey,nKeySize,CN_ENT);
  }
}

/*----------------------------------------------------------------------------*/
/* get size of entry                                                          */
/*----------------------------------------------------------------------------*/
i8 Ent::nSizeBytes(void)
{
  i8 nSizeEntProper = sizeof(Ent);
  
  i8 nSizeElmSeq = nElmSeqSize();
  
  i8 nSizePayload = 0;
  if (bPayload()) {
    Pay* aPay = this->aPay();
    nSizePayload = aPay->nPaySize();
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  i8 nProperBytes  = nSizeEntProper + nSizeElmSeq + nSizePayload;
  
  i8 nRoundedBytes = MemLib::nRoundUp(nProperBytes,C_ENT_QUANTA);

  return nRoundedBytes;
}

/*----------------------------------------------------------------------------*/
/* get number of skip levels                                                  */
/*----------------------------------------------------------------------------*/
i8 Ent::nSkipLevels(void)
{
  return m_xRef.nSkipLevels();
}

/*----------------------------------------------------------------------------*/
/* set number of skip levels                                                  */
/*----------------------------------------------------------------------------*/
void Ent::vSkipLevels(i8 nLevels)
{
  m_xRef.vSkipLevels(nLevels);
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
/* test for valid magic                                                       */
/*----------------------------------------------------------------------------*/
bool Ent::bValidMagic(void)
{
#ifdef __ENT_HEADER
  bool bValid = m_xHeader.bValidMagic();
#else
  bool bValid = true;
#endif
  
  return bValid;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
