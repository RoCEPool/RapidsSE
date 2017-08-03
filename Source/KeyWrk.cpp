/*============================================================================*/
/* KeyWrk.cpp -- Key Working Object                                           */
/*============================================================================*/

#include "DBobjects.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* KeyWrk::operator new(size_t iSize)
{
  RASSERT(iSize);
  vFatal("fatal: KeyWrk class cannot be new'd\n"_Stz);
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* KeyWrk::operator new(size_t iSize, KeyWrk* pKeyWrk)
{
  RASSERT(iSize);
  return pKeyWrk;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
KeyWrk::KeyWrk(void)
: Object(CN_KEYWRK)
{
  _precondition_(_MAGIC_,CN_KEYWRK);
  
  _checkInitialisedSize_(sizeof(KeyWrk));
  
#ifdef __DEBUG_CHECKS
  m_xWsp.vFill(0,CN_KEYWRK);
#endif
  
  _postcondition_(_MAGIC_,CN_KEYWRK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
KeyWrk::KeyWrk(Key* aKey, IdxColVec* pColVec)
: KeyWrk()
{
  _precondition_(_MAGIC_,CN_KEYWRK);
  
  vAddKey(aKey, pColVec);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
KeyWrk::~KeyWrk(void)
{
  _precondition_(_MAGIC_,CN_KEYWRK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
void KeyWrk::vAddKey(Key* aKey, IdxColVec* pColVec)
{
  _precondition_(_MAGIC_,CN_KEYWRK);
  
  char* aCursor = aKey->aWspArea();
  i8 nBytes;
  for (i8 iColumn = 0; iColumn < pColVec->nItems(); iColumn++) {
    Column* pColumn = pColVec->pGet(iColumn);
    
    char* aElement = aKey->aEachDatum(pColumn, nBytes,&aCursor);
    
    char* pDst = m_xWsp.aLatter();
    char* pSrc = aElement;
    
    DtmLib::vCopy(pColumn->eDatumType(),pDst,pSrc,pColumn);
    
    m_xWsp.vTake(nBytes);
    
  }
  
  _checkInitialisedSize_(sizeof(KeyWrk));
  
  _postcondition_(_MAGIC_,CN_KEYWRK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void KeyWrk::vShow(void)
{
  _precondition_(_MAGIC_,CN_KEYWRK,_FOOTER_);
  
  i8 iKey = *((i8*)m_xWsp.aArea());
  vTrace("KeyWrk %p: bytesUsed %lld value %lld\n"_Stz,
         this,
         nWspUsed(),
         iKey);
  
  
}

/*----------------------------------------------------------------------------*/
/* decode show of KeyWrk contents                                             */
/*----------------------------------------------------------------------------*/
void KeyWrk::vShow(eDM eDatum, ...)
{
  _precondition_(_MAGIC_,CN_KEYWRK,_FOOTER_);
  
  va_list xVarArgs;
  
  va_start(xVarArgs,eDatum);
  
  vTrace("KeyWrk %p: bytesUsed %lld "_Stz,
         this,
         nWspUsed()
         );
  
  i8 nBytesTaken;
  s48K sSprintBuffer;
  
  // first element
  DtmLib::vSprint(eDatum,sSprintBuffer.aStr(), (Stz*)m_xWsp.aArea(), nullptr);
  vTrace("%s|"_Stz,sSprintBuffer.s());
  
  // subsequent elements
  i8 iOffset = nBytesTaken;
  while (eDM eDatum = (eDM)va_arg(xVarArgs,int)) {
    
    DtmLib::vSprint(eDatum,
                    sSprintBuffer.aStr(),
                    (Stz*)m_xWsp.aArea() + iOffset,
                    nullptr);
    
    vTrace("%s|"_Stz,sSprintBuffer.s());
    
    iOffset += nBytesTaken;
  }

  va_end(xVarArgs);

  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* add element to key                                                         */
/*----------------------------------------------------------------------------*/
void KeyWrk::vAddDatum(Values::StrVal* pStrVal, Column* pCol)
{
  _precondition_(_MAGIC_,CN_KEYWRK,_FOOTER_);
  
  i8 nBytes;
  
  char* pDst = m_xWsp.aLatter();
  char* pSrc = (char*)pStrVal->aStr();
  i8 iValue;
  
  switch(pCol->eColumnType()) {
      
    case CT_BOOLEAN:
      iValue = StrLib::iToI8(pSrc);
      nBytes = Bol::nCopy((char*)pDst,(char*)pSrc);
      break;
      
    case CT_INTEGER:
      iValue = StrLib::iToI8(pSrc);
      nBytes = Int::nCopy(pDst,(void*)&iValue);
      break;
      
    case CT_VARCHAR:
      nBytes = Vch::nCopy(pDst,pSrc);
      break;
      
    default:
      NEVER_REACH;
  }
  
  m_xWsp.vTake(nBytes);
  
}

/*----------------------------------------------------------------------------*/
/* add element to key                                                         */
/*----------------------------------------------------------------------------*/
void KeyWrk::vAddDatum(ValWrk& xValWrk, Column* pCol)
{
  _precondition_(_MAGIC_,CN_KEYWRK,_FOOTER_);
  
  i8 nBytes;
    
  char* pDst = m_xWsp.aLatter();
  char* pSrc;
  
  if (DM_VCH == pCol->eDatumType())
    pSrc = (char*)(_pStrVal(xValWrk.pNestedValueSelf())->aStr());
  else
    pSrc = pAdd<char*>(xValWrk.pNestedValueSelf(),sizeof(Value));

  nBytes = DtmLib::nCopy(pCol->eDatumType(),pDst,pSrc,pCol);
  
  m_xWsp.vTake(nBytes);
  
}

/*----------------------------------------------------------------------------*/
/* get wsp area as key                                                        */
/*----------------------------------------------------------------------------*/
Key* KeyWrk::aKey(void)
{
  _precondition_(_MAGIC_,CN_KEYWRK,_FOOTER_);
  
  return reinterpret_cast<Key*>(m_xWsp.aArea());
}

/*----------------------------------------------------------------------------*/
/* reset the key wsp                                                          */
/*----------------------------------------------------------------------------*/
void KeyWrk::vReset(void)
{
  _precondition_(_MAGIC_,CN_KEYWRK,_FOOTER_);
 
  m_xWsp.vUsed(0);
}

/*----------------------------------------------------------------------------*/
/* get wsp area                                                               */
/*----------------------------------------------------------------------------*/
char* KeyWrk::aWspArea(void)
{
  _precondition_(_MAGIC_,CN_KEYWRK,_FOOTER_);
  
  return m_xWsp.aArea();
}

/*----------------------------------------------------------------------------*/
/* get wsp area                                                               */
/*----------------------------------------------------------------------------*/
Stz* KeyWrk::aWspStr(void)
{
  _precondition_(_MAGIC_,CN_KEYWRK,_FOOTER_);
  
  return m_xWsp.aStr();
}

/*----------------------------------------------------------------------------*/
/* get wsp bytes used                                                         */
/*----------------------------------------------------------------------------*/
i8 KeyWrk::nWspUsed(void)
{
  _precondition_(_MAGIC_,CN_KEYWRK,_FOOTER_);
  
  return m_xWsp.nUsed();
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
_aFooter_(KeyWrk)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
