/*============================================================================*/
/* Key.cpp -- key part of an index entry                                      */
/*============================================================================*/

#include "Transform.h"

#include "Sprint.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Key* Transform::_aKey(void* aAddr)
{
  Key* aKey = reinterpret_cast<Key*>(aAddr);
  
  return aKey;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Key::operator new(size_t)
{
  vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
         "fatal: Key class cannot be new'd\n"_Stz);
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Key::operator new(size_t, Key* aKey)
{
  return aKey;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Key::Key(void)
{
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Key::~Key(void) noexcept(false)
{
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* compare left key with right key                                            */
/*----------------------------------------------------------------------------*/
eKC Key::eCompare(Key* aLeft, Key* aRight)
{
  i8 nKeyBytes_l = aLeft->nKeyBytes();
  i8 nKeyBytes_r = aRight->nKeyBytes();
  
  byte* ayKeyBytes_l = aLeft->ayElmSeq();
  byte* ayKeyBytes_r = aRight->ayElmSeq();
  
  i8 nCmpBytes = nKeyBytes_l < nKeyBytes_r ? nKeyBytes_l : nKeyBytes_r;
  
  i8 iCmp = MemLib::iCompareWords(ayKeyBytes_l,ayKeyBytes_r,nCmpBytes);
  
  if (iCmp < 0)
    return KC_BEFORE;
  
  if (iCmp == 0) {
    if (nKeyBytes_l < nKeyBytes_r)
      return KC_BEFORE;
    
    if (nKeyBytes_l == nKeyBytes_r)
      return KC_SAME;
    
    if (nKeyBytes_l > nKeyBytes_r)
      return KC_AFTER;
  }
  
  return KC_AFTER;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get first element as int (assuming int)                                    */
/*----------------------------------------------------------------------------*/
i8 Key::iGetInt(void)
{
  i8 iVal = *(i8*)ayElmSeq();
  
  return iVal;
}

/*----------------------------------------------------------------------------*/
/* increment the size of the element sequence                                 */
/*----------------------------------------------------------------------------*/
void Key::vIncElmSeqSize(i8 nDeltaBytes)
{
  m_nElmSeqSize += nDeltaBytes;
}

/*----------------------------------------------------------------------------*/
/* get size of the element sequence                                           */
/*----------------------------------------------------------------------------*/
i8 Key::nKeyBytes(void)
{
  return m_nElmSeqSize;
}

/*----------------------------------------------------------------------------*/
/* set size of the element sequence                                           */
/*----------------------------------------------------------------------------*/
void Key::vElmSeqSize(i8 nElmSeqSize)
{
  m_nElmSeqSize = (i4)nElmSeqSize;
}

/*----------------------------------------------------------------------------*/
/* show the elements of this key                                              */
/*----------------------------------------------------------------------------*/
void Key::vShow(IdxColVec* pIdxColVec)
{
  i8 nBytes;
  
  byte* ayCursor = ayElmSeq();
  
  char caLiteral[128];
  
  for (i8 i_Col = 0; i_Col < pIdxColVec->nItems(); i_Col++) {
    
    Column* pCol = pIdxColVec->pGet(i_Col);
    
    byte* ayElm = EntWrk::ayEachElm(pCol,nBytes,&ayCursor);
    
    eDM eDatum = pCol->eDatum();
    
    if (eDatum == DM_VCH)
      nBytes = StrLib::iWordFlipBack(caLiteral, (char*)ayElm);
    else
      CsvValue::vInto((Stz*)caLiteral,ayElm,pCol);
    
    vTrace("Elm[%lld %lldB] %s "_Stz,i_Col,nBytes,caLiteral);
  }
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
