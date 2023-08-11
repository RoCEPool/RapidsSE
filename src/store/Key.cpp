/*============================================================================*/
/* Key.cpp -- key part of an index entry                                      */
/*============================================================================*/

#include "BASE.h"

#include "Index.hpp"

#include "RowSingle.hpp"
#include "EntWrk.hpp"

#include "Sprint.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Key* STORE::_aKey(void* aAddr)
{
  Key* aKey = reinterpret_cast<Key*>(aAddr);
  
  return aKey;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Key::operator new(size_t)
{
  vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
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
eKC Key::eCompare(IndexDef* pIndexDef, Key* aKey_l, Key* aKey_r)
{

  if (pIndexDef->bHasDescending()) {
    return eMixCompare(pIndexDef, aKey_l, aKey_r);
  }

  i8 nKeyBytes_l = aKey_l->nElmSeqSize();
  i8 nKeyBytes_r = aKey_r->nElmSeqSize();
  
  Dtm* ayKeyBytes_l = aKey_l->aElmSeq_dtm();
  Dtm* ayKeyBytes_r = aKey_r->aElmSeq_dtm();
  
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
/* is left key is equal to right key ?                                        */
/*----------------------------------------------------------------------------*/
bool Key::bEqualTo(IndexDef* pIndexDef, Key* aKey_l, Key* aKey_r)
{
  eKC eCmp = Key::eCompare(pIndexDef,aKey_l,aKey_r);
  
  return eCmp == KC_SAME ? true : false;
}

/*----------------------------------------------------------------------------*/
/* is left key greater than right key ?                                       */
/*----------------------------------------------------------------------------*/
bool Key::bGreaterThan(IndexDef* pIndexDef, Key* aKey_l, Key* aKey_r)
{
  eKC eCmp = Key::eCompare(pIndexDef,aKey_l,aKey_r);

  return eCmp == KC_AFTER ? true : false;

}

/*----------------------------------------------------------------------------*/
/* is left key greater than equal to right key ?                              */
/*----------------------------------------------------------------------------*/
bool Key::bGreaterEqual(IndexDef* pIndexDef, Key* aKey_l, Key* aKey_r)
{
  eKC eCmp = Key::eCompare(pIndexDef, aKey_l,aKey_r);
  
  return eCmp == KC_AFTER || eCmp == KC_SAME ? true : false;
}

/*----------------------------------------------------------------------------*/
/* is left key less than eqiual to right key ?                                */
/*----------------------------------------------------------------------------*/
bool Key::bLessEqual(IndexDef* pIndexDef, Key* aKey_l, Key* aKey_r)
{
  eKC eCmp = Key::eCompare(pIndexDef,aKey_l,aKey_r);
  
  return eCmp == KC_BEFORE || eCmp == KC_SAME ? true : false;
}

/*----------------------------------------------------------------------------*/
/* is left key less than right key ?                                          */
/*----------------------------------------------------------------------------*/
bool Key::bLessThan(IndexDef* pIndexDef, Key* aKey_l, Key* aKey_r)
{
  eKC eCmp = Key::eCompare(pIndexDef,aKey_l,aKey_r);
  
  return eCmp == KC_BEFORE ? true : false;
}

/*----------------------------------------------------------------------------*/
/* is left key not equal to right key ?                                       */
/*----------------------------------------------------------------------------*/
bool Key::bNotEqual(IndexDef* pIndexDef, Key* aKey_l, Key* aKey_r)
{
  eKC eCmp = Key::eCompare(pIndexDef,aKey_l,aKey_r);
  
  return eCmp != KC_SAME ? true : false;
}

/*----------------------------------------------------------------------------*/
/* show the elements of this key                                              */
/*----------------------------------------------------------------------------*/
eKC Key::eMixCompare(IndexDef* pIndexDef, Key* aKey_l, Key* aKey_r)
{

  i8 nKeyBytes_l = 0;
  i8 nKeyBytes_r = 0;

  Dtm* ayCursor_l = aKey_l->aElmSeq_dtm();
  Dtm* ayCursor_r = aKey_r->aElmSeq_dtm();

  //aKey_l->vShow(pIndexDef);
  //vTrace("\n"_Stz);
  //aKey_r->vShow(pIndexDef);
  //vTrace("\n"_Stz);

  for (i8 i_Col = 0; i_Col < pIndexDef->nColumns(); i_Col++) {

    Column *pCol = pIndexDef->pGetColumn(i_Col);

    byte *ayKeyBytes_l = EntWrk::ayEachElm(pCol, nKeyBytes_l, (byte **) &ayCursor_l);
    byte *ayKeyBytes_r = EntWrk::ayEachElm(pCol, nKeyBytes_r, (byte **) &ayCursor_r);

    if (pCol->eDatum() == DM_VCH) {
      nKeyBytes_l = Key::iGetStrSize(ayKeyBytes_l);
      nKeyBytes_r = Key::iGetStrSize(ayKeyBytes_r);
    }

    i8 nCmpBytes = nKeyBytes_l < nKeyBytes_r ? nKeyBytes_l : nKeyBytes_r;

    i8 iCmp = MemLib::iCompareWords(ayKeyBytes_l, ayKeyBytes_r, nCmpBytes);

    if (pCol->bIsAscending()) {

      if (iCmp < 0) {
        return KC_BEFORE;
      }

      if (iCmp == 0) {
        if (nKeyBytes_l < nKeyBytes_r) {
          return KC_BEFORE;
        }

        if (nKeyBytes_l == nKeyBytes_r) {
          //vTrace("SAME\n"_Stz);
          continue;
        }

        if (nKeyBytes_l > nKeyBytes_r)
          return KC_AFTER;
      }

      if (iCmp > 0) {
        //vTrace("AFTER"_Stz);
        return KC_AFTER;
      }

    } else {

      if (iCmp < 0) {
        //vTrace("AFTER\n"_Stz);
        return KC_AFTER;
      }

      if (iCmp == 0) {
        if (nKeyBytes_l < nKeyBytes_r) {
          //vTrace("AFTER\n"_Stz);
          return KC_AFTER;
        }

        if (nKeyBytes_l == nKeyBytes_r) {
          //vTrace("SAME\n"_Stz);
          continue;
        }


        if (nKeyBytes_l > nKeyBytes_r) {
          //vTrace("BEFORE\n"_Stz);
          return KC_BEFORE;
        }

      }

      if (iCmp > 0) {
        //vTrace("BEFORE"_Stz);
        return KC_BEFORE;
      }
    }

  }

  //vTrace("BEFORE\n"_Stz);
  return KC_SAME;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get first element as int (assuming int)                                    */
/*----------------------------------------------------------------------------*/
i8 Key::iGetIntElm(void)
{
  //RASSERT(0);
  i8 iVal = *(i8*)aElmSeq_dtm();
  
  return iVal;
}

/*----------------------------------------------------------------------------*/
/* get element as int (assuming int)                                          */
/*----------------------------------------------------------------------------*/
i8 Key::iGetIntElm(IndexDef* pIndexDef, i8 i_Element)
{
  i8 nBytes = 0;
  Dtm* ayCursor = aElmSeq_dtm();
  i8 iVal = C_UNKNOWN;
  
  for (i8 i_Col = 0; i_Col < pIndexDef->nColumns(); i_Col++) {
    
    Column* pCol = pIndexDef->pGetColumn(i_Col);
    
    byte* ayElm = EntWrk::ayEachElm(pCol,nBytes,(byte**)&ayCursor);
  
    if (i_Element == i_Col) {
      iVal  = *(i8*)ayElm;
      break;
    }
  }
  RASSERT(iVal != C_UNKNOWN);
  return iVal;
}

/*----------------------------------------------------------------------------*/
/* get element as str (assuming str)                                         */
/*----------------------------------------------------------------------------*/
i8 Key::iGetStrElm(IndexDef* pIndexDef, i8 i_Element, char* sKeyValue)
{
  i8 nBytes = 0;
  Dtm* ayCursor = aElmSeq_dtm();
  byte* ayElm;
  i8 nSize = 0;

  for (i8 i_Col = 0; i_Col < pIndexDef->nColumns(); i_Col++) {

    Column* pCol = pIndexDef->pGetColumn(i_Col);
    RASSERT(pCol->eColumnType() == CT_VARCHAR ||
                     pCol->eColumnType() == CT_CHAR);

    ayElm = EntWrk::ayEachElm(pCol,nBytes,(byte**)&ayCursor);

    if (i_Element == i_Col) {
      nSize = StrLib::iWordFlipBack(sKeyValue, (char*)ayElm);
      break;
    }
  }


  return nSize;
}

/*----------------------------------------------------------------------------*/
/* get str element size                                                       */
/*--------------------------
 *
 *
 * --------------------------------------------------*/
i8 Key::iGetStrSize(byte* ayKeyBytes)
{
  i8 nSize = 0;

  i8 iLenSrc = StrLib::nSize((char*)ayKeyBytes);

  i8 iRoundUp = 0;
  i8 iRemainder = iLenSrc % C_BYTES_PER_WORD;
  if (iRemainder)
    iRoundUp = C_BYTES_PER_WORD - iRemainder;

  nSize = iLenSrc + iRoundUp;
  ASSERT(0 == nSize % C_BYTES_PER_WORD);

  return nSize;
}

/*----------------------------------------------------------------------------*/
/* get first element as stz (assuming stz)                                    */
/*----------------------------------------------------------------------------*/
Stz* Key::szGetStrElm(void)
{
  Stz* szVal = (Stz*)aElmSeq_dtm();
  
  return szVal;
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
i8 Key::nElmSeqSize(void)
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
/* reset                                                                      */
/*----------------------------------------------------------------------------*/
void Key::vReset(void)
{
  m_nElmSeqSize = 0;
}

/*----------------------------------------------------------------------------*/
/* show the elements of this key                                              */
/*----------------------------------------------------------------------------*/
void Key::vShow(IndexDef* pIndexDef)
{
  bool bReadApi = false;
  
  if (pIndexDef->szIndexNm()->bSame(S_READAPI_I))
    bReadApi = true;
  
  i8 nBytes = 0;
  
  Dtm* ayCursor = aElmSeq_dtm();
  
  char caLiteral[128];
  
  for (i8 i_Col = 0; i_Col < pIndexDef->nColumns(); i_Col++) {
    
    Column* pCol = pIndexDef->pGetColumn(i_Col);
    
    byte* ayElm = EntWrk::ayEachElm(pCol,nBytes,(byte**)&ayCursor);
    
    eDM eDatum = pCol->eDatum();
    
    if (0 == i_Col && bReadApi) {
      Rrf xwRef;
      xwRef = * (Rrf*)ayElm;
      StrLib::vSprint(caLiteral,
                      sizeof(caLiteral),
                      "%lld.%lld"_Stz,
                      xwRef.iBuffer(),
                      xwRef.iItem()
                      );
    }
    else
    if (eDatum == DM_VCH) {
      nBytes = StrLib::iWordFlipBack(caLiteral, (char*)ayElm);
    }
    else {
      Literal::vSprint((Stz*)caLiteral,(Dtm*)ayElm,pCol);
    }
    
    vTrace(" Elm[%lld %lldB %s]"_Stz,i_Col,nBytes,caLiteral);
  }
}

/*----------------------------------------------------------------------------*/
/* show the elements of this integer k                                        */
/*----------------------------------------------------------------------------*/
void Key::vShowIntKey(void)
{
  i8 i_Col  = 0;
  i8 nBytes = 8;
  char caLiteral[128];

  i8 iInt = (i8)* (i8*)aElmSeq_dtm();
  StrLib::vSprint(caLiteral,
                  sizeof(caLiteral),
                  "%05lld"_Stz,
                  iInt);
  
  vTrace(" Elm[%lld %lldB %s]"_Stz,i_Col,nBytes,caLiteral);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
