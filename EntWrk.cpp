/*============================================================================*/
/* EntWrk.cpp -- Working Index Entry                                          */
/*============================================================================*/

#include "Data.h"
#include "StorageEngine.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* EntWrk::operator new(size_t)
{
  vFatal("fatal: EntWrk class cannot be new'd\n"_Stz);
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* EntWrk::operator new(size_t, EntWrk* pEntWrk)
{
  return pEntWrk;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
EntWrk::EntWrk(void)
: Object(CN_ENTWRK)
{
  _precondition_(_MAGIC_,CN_ENTWRK);
  
  // check initialisation of rowid
  ASSERT(0 == m_xId.iBuffer() &&
         0 == m_xId.iItem()   &&
         0 == m_xId.nLevels());
  
#ifdef WITH_CHECK_INITIALISED
  m_xWsp.vZero(CN_ENTWRK);
#endif
  
  m_xWsp.vInit();
  
  m_ayEmbedded = m_xWsp.ayArea();
  
  _checkInitialisedSize_(sizeof(EntWrk));
  
  _postcondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
EntWrk::~EntWrk(void) noexcept(false)
{
  _precondition_(_MAGIC_,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add element to key                                                         */
/*----------------------------------------------------------------------------*/
void EntWrk::vAddElement(Row* pRow, Column* pCol)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  i8 nBytes = 0;
  
  char* aDst = (char*)m_xWsp.ayLatter();
  char* aSrc = (char*)pRow->m_aiValue[pCol->iColumnNo()];
  
  if (pRow->m_aiValue[pCol->iColumnNo()] == C_DATUM_NULL) {
    nBytes = DtmLib::nCopy(pCol->eDatumType(),aDst,&aSrc);
    vIncKeyBytes(nBytes);
    return;
  }
  
  if (DM_VCH == pCol->eDatumType()) {
    nBytes = StrLib::iWordFlip(aDst, aSrc);
  }
  else {
    nBytes = DtmLib::nCopy(pCol->eDatumType(),aDst,&aSrc);
  }
  
  vIncKeyBytes(nBytes);
}

/*----------------------------------------------------------------------------*/
/* add min lower value to key                                                 */
/*----------------------------------------------------------------------------*/
void EntWrk::vAddMinusInfinity(void)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  char* aDst = (char*)m_xWsp.ayLatter();
  
  i8 iInt = C_DATUM_MINUS_INFINITY;
  Int::nCopy(aDst,(void*)&iInt);
  
  vIncKeyBytes(C_BYTES_PER_WORD);
}

/*----------------------------------------------------------------------------*/
/* add max upper value to key                                                 */
/*----------------------------------------------------------------------------*/
void EntWrk::vAddPlusInfinity(void)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  char* aDst = (char*)m_xWsp.ayLatter();
  
  i8 iInt = C_DATUM_PLUS_INFINITY;
  Int::nCopy(aDst,(void*)&iInt);
  
  vIncKeyBytes(C_BYTES_PER_WORD);
}

/*----------------------------------------------------------------------------*/
/* get buffer no from id                                                      */
/*----------------------------------------------------------------------------*/
i8 EntWrk::iBufferNo(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  i8 iBuffer = m_xId.iBuffer();
  
  return iBuffer;
}

/*----------------------------------------------------------------------------*/
/* set buffer no in id                                                        */
/*----------------------------------------------------------------------------*/
void EntWrk::vBufferNo(i8 iBufferNo)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  m_xId.vBuffer(iBufferNo);
}

/*----------------------------------------------------------------------------*/
/* get address of embedded object                                             */
/*----------------------------------------------------------------------------*/
byte* EntWrk::ayEmbedded(void)
{  
  _precondition_(_MAGIC_,_FOOTER_);
  
  return m_ayEmbedded;
}

/*----------------------------------------------------------------------------*/
/* set address of embedded object                                             */
/*----------------------------------------------------------------------------*/
void EntWrk::vEmbedded(byte* ayEmbedded)     //! i'd like to get rid of this fun
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  m_ayEmbedded = ayEmbedded;
}

/*----------------------------------------------------------------------------*/
/* get the size of the entry required to contain the embedded object          */
/*----------------------------------------------------------------------------*/
i8 EntWrk::nEntrySize(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  i8 nData = nKeySize();
  
  i8 nBytes = sizeof(Ent) + nData;
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* form given entry from this working entry                                   */
/*----------------------------------------------------------------------------*/
void EntWrk::vFormEntry(Ent* pEntry)
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set entry id and levels
  
  pEntry->vId(xId());
  pEntry->vLevels(nLevels());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // copy key into entry
  
  Key* aSrcKey  = aKey();
  Key* aDstKey  = _aKey(pEntry->ayKeyBytes());
  i8   nKeySize = this->nKeySize();
  
  MemLib::vCopy(aDstKey,aSrcKey,nKeySize,CN_ENT);
}

/*----------------------------------------------------------------------------*/
/* compute hash of key                                                        */
/*----------------------------------------------------------------------------*/
i8 EntWrk::iHashKey(void)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  i8 iHash = HshLib::iHash((byte*)aKey(),nKeySize());
  
  return iHash;
}

/*----------------------------------------------------------------------------*/
/* get row id                                                                 */
/*----------------------------------------------------------------------------*/
Id& EntWrk::xId(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return m_xId;
}

/*----------------------------------------------------------------------------*/
/* set row id                                                                 */
/*----------------------------------------------------------------------------*/
void EntWrk::vId(Id& xId)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  m_xId = xId;
}

/*----------------------------------------------------------------------------*/
/* inc total key bytes by size of element                                     */
/*----------------------------------------------------------------------------*/
void EntWrk::vIncKeyBytes(i8 nElementBytes)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  m_xWsp.vTake(nElementBytes);
}

/*----------------------------------------------------------------------------*/
/* get item no from id                                                        */
/*----------------------------------------------------------------------------*/
i8 EntWrk::iItemNo(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return m_xId.iItem();
}

/*----------------------------------------------------------------------------*/
/* set item no in id                                                          */
/*----------------------------------------------------------------------------*/
void EntWrk::vItemNo(i8 i_Item)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  m_xId.vItem(i_Item);
}

/*----------------------------------------------------------------------------*/
/* get wsp area as key                                                        */
/*----------------------------------------------------------------------------*/
Key* EntWrk::aKey(void)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  return reinterpret_cast<Key*>(ayEmbedded());
}

/*----------------------------------------------------------------------------*/
/* get wsp area as key                                                        */
/*----------------------------------------------------------------------------*/
byte* EntWrk::ayKeyLatter(void)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  return m_xWsp.ayLatter();
}

/*----------------------------------------------------------------------------*/
/* get key size                                                               */
/*----------------------------------------------------------------------------*/
i8 EntWrk::nKeySize(void)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  i8 nSize = m_xWsp.nUsed();
  
  return nSize;
}

/*----------------------------------------------------------------------------*/
/* set key size                                                               */
/*----------------------------------------------------------------------------*/
void EntWrk::vKeySize(i8 nKeySize)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  m_xWsp.vUsed(nKeySize);
}

/*----------------------------------------------------------------------------*/
/* get nLevel                                                                 */
/*----------------------------------------------------------------------------*/
i8 EntWrk::nLevels(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return m_xId.nLevels();
}

/*----------------------------------------------------------------------------*/
/* get the size of the next entry offset(s) in bytes                          */
/*----------------------------------------------------------------------------*/
i8 EntWrk::nOffsetsSize(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  i8 nLevels = m_xId.nLevels();
  i8 nBytes  = sizeof(i4) * nLevels;
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* set number of levels                                                       */
/*----------------------------------------------------------------------------*/
void EntWrk::vLevels(i8 nLevels)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  m_xId.vLevels(nLevels);
}

/*----------------------------------------------------------------------------*/
/* set a random number of levels                                              */
/*----------------------------------------------------------------------------*/
void EntWrk::vRandomLevels(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  Task* pTask = Task::pExecutingTask();
  
  i8 nLevels = Random::iLogLevel(3,C_FLAT_FREQ + 1,pTask->i_NextRnd()) + 1;

  m_xId.vLevels(nLevels);
}

/*----------------------------------------------------------------------------*/
/* set this entwrk using given details                                        */
/*----------------------------------------------------------------------------*/
void EntWrk::vSetWrk(i8 iBufferNo, i8 i_Item, Ent* pEntry)
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set id, transaction and offsets
  
  i8 nLevels = pEntry->nLevels();
  vLevels(nLevels);
  
  Id xId(iBufferNo,i_Item,nLevels);
  vId(xId);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // copy key into this entry
  
  Key* aKeyData = _aKey(pEntry->ayKeyBytes());
  i8 iKeySize   = pEntry->nSizeKey();
  RASSERT(iKeySize % 8 == 0);
  
  vEmbedded((byte*)aKeyData);
  
  vKeySize(iKeySize);
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void EntWrk::vShow(IdxColVec* pIdxColVec)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  Key* aKey = this->aKey();
  aKey->vShow(pIdxColVec);
  vTrace("\n"_Stz);
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(EntWrk)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
