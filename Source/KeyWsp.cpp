/*============================================================================*/
/* KeyWsp.cpp -- Key Workspace                                                */
/*============================================================================*/

#include "DBobjects.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* KeyWsp::operator new(size_t iSize)
{
  RASSERT(iSize);
  vFatal("fatal: KeyWsp class cannot be new'd\n"_Str);
  
  RASSERT(0);
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* KeyWsp::operator new(size_t iSize, KeyWsp* pKeyWsp)
{
  RASSERT(iSize);
  return pKeyWsp;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
KeyWsp::KeyWsp(void)
: Object(CN_KEYWSP)
{
  _precondition_(_MAGIC_,CN_KEYWSP);
  
  _checkInitialisedSize_(sizeof(KeyWsp));
  
  _postcondition_(_MAGIC_,CN_KEYWSP,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
KeyWsp::~KeyWsp(void)
{
  _precondition_(_MAGIC_,CN_KEYWSP,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void KeyWsp::vShow(void)
{
  _precondition_(_MAGIC_,CN_KEYWSP,_FOOTER_);
  
  vTrace("KeyWsp %p: bytesUsed %lld\n"_Str,
         this,
         nWspUsed()
         );
}

/*----------------------------------------------------------------------------*/
/* decode show of keywsp contents                                             */
/*----------------------------------------------------------------------------*/
void KeyWsp::vShow(eDM eDatum, ...)
{
  _precondition_(_MAGIC_,CN_KEYWSP,_FOOTER_);
  
  va_list xVarArgs;
  
  va_start(xVarArgs,eDatum);
  
  vTrace("KeyWsp %p: bytesUsed %lld "_Str,
         this,
         nWspUsed()
         );
  
  i8 nBytesTaken;
  s48K sSprintBuffer;
  
  // first element
  Datums::vSprint(sSprintBuffer.aStr(), (Str*)m_xWsp.aArea(), eDatum, nBytesTaken);
  vTrace("%s|"_Str,sSprintBuffer.s());
  
  // subsequent elements
  i8 iOffset = nBytesTaken;
  while (eDM eDatum = (eDM)va_arg(xVarArgs,int)) {
    
    Datums::vSprint(sSprintBuffer.aStr(),
                    (Str*)m_xWsp.aArea() + iOffset,
                    eDatum,
                    nBytesTaken);
    
    vTrace("%s|"_Str,sSprintBuffer.s());
    
    iOffset += nBytesTaken;
  }

  va_end(xVarArgs);

  vTrace("\n"_Str);
}

/*----------------------------------------------------------------------------*/
/* add element to key                                                         */
/*----------------------------------------------------------------------------*/
void KeyWsp::vAddElement(Values::ValWsp &xValWsp)
{
  _precondition_(_MAGIC_,CN_KEYWSP,_FOOTER_);
  
  // for the moment we only allow one element per key
  ASSERT( ! nWspUsed() );
  
  eCN eClass = xValWsp.eClass();
  char* pDst = m_xWsp.aLatter();
  char* pSrc = xValWsp.aWspArea();
  
  i8 iSrc = *(i8*)pSrc;

  i8 nBytesTaken = ValLib::nCopyDatum(eClass,pDst,pSrc);
  
  m_xWsp.vTake(nBytesTaken);
}

/*----------------------------------------------------------------------------*/
/* get wsp area                                                               */
/*----------------------------------------------------------------------------*/
char* KeyWsp::aWspArea(void)
{
  _precondition_(_MAGIC_,CN_KEYWSP,_FOOTER_);
  
  return m_xWsp.aArea();
}

/*----------------------------------------------------------------------------*/
/* get wsp area                                                               */
/*----------------------------------------------------------------------------*/
Str* KeyWsp::aWspStr(void)
{
  _precondition_(_MAGIC_,CN_KEYWSP,_FOOTER_);
  
  return m_xWsp.aStr();
}

/*----------------------------------------------------------------------------*/
/* get wsp bytes used                                                         */
/*----------------------------------------------------------------------------*/
i8 KeyWsp::nWspUsed(void)
{
  _precondition_(_MAGIC_,CN_KEYWSP,_FOOTER_);
  
  return m_xWsp.nUsed();
}

/*----------------------------------------------------------------------------*/
/* extract element from row                                                   */
/*----------------------------------------------------------------------------*/
bool KeyWsp::bExtractElement(s48K& sRow, i8 iElement, eDM eDatum)
{
  _precondition_(_MAGIC_,CN_KEYWSP,_FOOTER_);
  
  vTrace("Extracting element %lld %s from %s\n"_Str,
         iElement,
         aDM[eDatum],
         sRow.s());
  
  Str* aElem = sRow.aStr();
  i8 iFound = 0;
  
  while (iFound != iElement) {
    if ('|' == *aElem->s())
      iFound++;
    aElem++;
  }
  
  i8 nBytesTaken;
  bool b = Datums::bExtractDatum((Str*)m_xWsp.aLatter(),
                                 aElem,eDatum,nBytesTaken);
  
  m_xWsp.vTake(nBytesTaken);

  MemLib::vHexDump(m_xWsp.aArea(),m_xWsp.nUsed());
  
  return b;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
_pFooter_(KeyWsp)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
