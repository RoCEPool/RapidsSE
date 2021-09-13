/*============================================================================*/
/* TypedStack.cpp -- Push/Pop TypedStack of Typed Cells                                 */
/*============================================================================*/

#include "BASE.h"

#include "SP_Word.hpp"

/*

               +-------------------------+-------------------------+
               |         TypedCell 1     |         TypedCell 0     |
               +-------------------------+-------------------------+
           <<< | head : cell [ string⦰ ] | head : cell [ string⦰ ] |
               +-------------------------+-------------------------+
 
               ^m_ayStackPtr
               2 items

 */


/*----------------------------------------------------------------------------*/
/*-- Lifecycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
TypedStack* TypedStack::_pStack(void* aAddr)
{
  TypedStack* pStack = reinterpret_cast<TypedStack*>(aAddr);
  
  RASSERT_TYPE(pStack,CN_STACK);
  
  return pStack;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* TypedStack::operator new(size_t)
{
  void* pStack = Heaps::pAllocate(CN_STACK);
  
  return pStack;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void TypedStack::operator delete(void* pBufferAddress)
{
  TypedStack* pStack = _pStack(pBufferAddress);
  
  Heaps::vFree(pStack);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
TypedStack::TypedStack(eKK eKind)
: Object(CN_STACK)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  m_eKind = eKind;
  
  m_rTop.m_ayStackPtr = ayStackBase();
  m_rTop.m_nItems     = 0;
 
  m_rRetry.m_ayStackPtr = ayStackBase();
  m_rRetry.m_nItems     = 0;
  
#ifdef WITH_CHECK_INITIALISED
  m_xWsp.vFill(0x00,CN_STACK);
#endif
  
  _checkInitialised_(CN_STACK);
  
  _hazard_();

  _postcondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
TypedStack::~TypedStack(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* discard value on top of stack                                              */
/*----------------------------------------------------------------------------*/
void TypedStack::vDiscardTop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  _hazard_();
  
  m_rTop.vRewind();
  
  _hazard_();
}

/*----------------------------------------------------------------------------*/
/* dump contents of the stack                                                 */
/*----------------------------------------------------------------------------*/
void TypedStack::vDump(DicStack* pDicStack, Stz* aTitle)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  vTrace("%s:\n"_Stz,aTitle);
  
  i8 nValues = this->nValues();
  for (i8 i_Item = 0; i_Item < nValues; i_Item++) {
    
    TypedCell* aTypedCell = aPeekItem(i_Item);
    eCN eType = aTypedCell->eType();
    i8 nStackUsage = aTypedCell->nStackUsage();

    i8 nAuxUsage = 0;
    if (CN_STZVAL == eType) {
      nAuxUsage = aTypedCell->nStzSize();
    }

    vTrace("  [%02lld] %p %02lldB=(8B+8B+%02lldB) %6s "_Stz
           ,i_Item
           ,aTypedCell
           ,nStackUsage
           ,nAuxUsage
           ,aCN[eType]
           );

    switch (eType) {
        
      // -- -- -- -- -- --
      case CN_FIPVAL: {
        CodeCell* aFip  = aTypedCell->aFip();
        if (pDicStack) {
          Word* pWord = pDicStack->pLocateRoutine(aFip);
          
          if (pWord) {
//            vTrace("=%p word %s %s\n"_Stz
//                   ,aFip
//                   ,pWord->szWordNm()
//                   ,pWord->pCategory() ? pWord->pCategory()->szWordNm()
//                                       : ""_Stz
//                   );
            SP_Word spWord;
            vTrace("=%s\n"_Stz,spWord.sz(pWord));
          }
          else {
            vTrace("=%p word %s \n"_Stz,
                   aFip,
                   C_NO_WORD
                   );
          }
        }
        else {
          vTrace("%p word %s \n"_Stz,aFip,
                 "<UNKNOWN - No pDicStack>"_Stz);
        }
        break;
      }
        
      // -- -- -- -- -- --
      case CN_INTVAL: {
        i8 iInt = aTypedCell->iInt();
        
        vTrace("%lld\n"_Stz,iInt);
        break;
      }
        
      // -- -- -- -- -- --
      case CN_PTRVAL: {
        Object* pObj  =aTypedCell->pObj();
        eCN ePtdClass = pObj->eGetClassNo();
        
        if (CN_WORD == ePtdClass) {
          Word* pPtdWord = _pWord(pObj);
          vTrace("%p (%s) %s\n"_Stz,pObj,aCN[ePtdClass],pPtdWord->szWordNm());
        }
        else
          vTrace("%p (%s)\n"_Stz,pObj,aCN[ePtdClass]);

        break;
      }
        
      // -- -- -- -- -- --
      case CN_STZVAL: {
        char* azChar = aTypedCell->azChar();
        
        vTrace("%s\n"_Stz,azChar);
        break;
      }
        
      // -- -- -- -- -- --
      default: {
        break;
      }
        
    }
  }
  
}

/*----------------------------------------------------------------------------*/
/* peek at given item on stack                                                */
/*----------------------------------------------------------------------------*/
TypedCell* TypedStack::aPeekItem(i8 i_Item)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  TypedCell* aPeeked = aTopVal();
  while (i_Item--) {
    char* aAddress = reinterpret_cast<char*>(aPeeked);
    i8 iSize = aPeeked->nStackUsage();
    aAddress += iSize;
    aPeeked = reinterpret_cast<TypedCell*>(aAddress);
  }
  
  return aPeeked;
}

/*----------------------------------------------------------------------------*/
/* peek at given value on stack                                               */
/*----------------------------------------------------------------------------*/
void TypedStack::vPeekItem(i8 i_Item, TypedCell* aTypedCell)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  TypedCell* aPeeked = aPeekItem(i_Item);
  
  aTypedCell->vCopy(aPeeked);
}

/*----------------------------------------------------------------------------*/
/* poke given value into stack                                                */
/*----------------------------------------------------------------------------*/
void TypedStack::vPokeItem(i8 i_Item, TypedCell* aTypedCell)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  TypedCell* aPoked = aPeekItem(i_Item);
  
  aPoked->vCopy(aTypedCell);
}

/*----------------------------------------------------------------------------*/
/* pop value from stack as integer                                            */
/*----------------------------------------------------------------------------*/
i8 TypedStack::iPopAsInt(const char* azCallingFunNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if ( ! bGuard(1) )
    BASE::vThrow(FK_DSTACK,
                        azCallingFunNm,__FILE__,__LINE__,
                        "Invalid d-stack"_Stz);
  
  i8 iInt = m_rTop.m_aTypedCell->iAsInt();
  vDiscardTop();
  
  return iInt;
}

/*----------------------------------------------------------------------------*/
/* pop fip from stack                                                         */
/*----------------------------------------------------------------------------*/
CodeCell* TypedStack::aPopFip(const char* azCallingFunNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if ( ! bGuard(CN_FIPVAL) )
    BASE::vThrow(FK_DSTACK,
                        azCallingFunNm,__FILE__,__LINE__,
                        "Invalid d-stack"_Stz);
  
  CodeCell* aFip = m_rTop.m_aTypedCell->aFip();
  
  vDiscardTop();
  
  return aFip;
}

/*----------------------------------------------------------------------------*/
/* pop i8 from stack                                                          */
/*----------------------------------------------------------------------------*/
i8 TypedStack::iPopInt(const char* azCallingFunNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if ( ! bGuard(CN_INTVAL) )
    BASE::vThrow(FK_DSTACK,
                        azCallingFunNm,__FILE__,__LINE__,
                        "Invalid d-stack"_Stz);
  
  i8 iInt = m_rTop.m_aTypedCell->iInt();
  
  vDiscardTop();
  
  return iInt;
}

/*----------------------------------------------------------------------------*/
/* pop Object* from stack                                                     */
/*----------------------------------------------------------------------------*/
Object* TypedStack::pPopPtr(const char* azCallingFunNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if (! bGuard(CN_PTRVAL) && ! bTopIsZero() )
    BASE::vThrow(FK_DSTACK,
                        azCallingFunNm,__FILE__,__LINE__,
                        "Invalid d-stack"_Stz);
  
  Object* pObj = m_rTop.m_aTypedCell->pObj();

  vDiscardTop();
  
  return pObj;
}

/*----------------------------------------------------------------------------*/
/* pop string from stack                                                      */
/*----------------------------------------------------------------------------*/
void TypedStack::vPopStr(String* pString, const char* azCallingFunNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if ( ! bGuard(CN_STZVAL) )
    BASE::vThrow(FK_DSTACK,
                        azCallingFunNm,__FILE__,__LINE__,
                        "Invalid d-stack"_Stz);
  
  _hazard_();
  
  TypedCell* aTypedCell = _aTypedCell(aTopVal());
  
  pString->vCopy(aTypedCell->szStz());
    
  vDiscardTop();
  
  _hazard_();
}

/*----------------------------------------------------------------------------*/
/* pop value from stack                                                       */
/*----------------------------------------------------------------------------*/
void TypedStack::vPopItem(TypedCell* aTypedCell, const char* azCallingFunNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if ( ! bGuard(1) )
    BASE::vThrow(FK_DSTACK,
                        azCallingFunNm,__FILE__,__LINE__,
                        "Invalid d-stack"_Stz);
  
  _hazard_();
  
  i8 nBytes = aTopVal()->nStackUsage();
  
  aTypedCell->vCopy(m_rTop.m_aTypedCell,nBytes);
  
  m_rTop.vRewind();
    
  _hazard_();
}

/*----------------------------------------------------------------------------*/
/* push CodeCell* onto stack                                                      */
/*----------------------------------------------------------------------------*/
void TypedStack::vPushFip(CodeCell* aFip)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  TypedCell* aTypedCell = aAllotFipVal();
  
  aTypedCell->vFip(aFip);
}

/*----------------------------------------------------------------------------*/
/* push i8 onto stack                                                         */
/*----------------------------------------------------------------------------*/
void TypedStack::vPushInt(i8 iInt)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  TypedCell* aTypedCell = aAllotIntVal();
  
  aTypedCell->vInt(iInt);
}

/*----------------------------------------------------------------------------*/
/* push pointer to proper object onto stack                                   */
/*----------------------------------------------------------------------------*/
void TypedStack::vPushPtr(Object* pObj)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  TypedCell* aTypedCell = aAllotPtrVal();
  
  aTypedCell->vObj(pObj);
}

/*----------------------------------------------------------------------------*/
/* push string onto stack                                                     */
/*----------------------------------------------------------------------------*/
void TypedStack::vPushStr(String* pStr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  i8 nSize = StrLib::nSize(pStr->azStr());
  TypedCell* aTypedCell = aAllotStzVal(nSize);

  aTypedCell->vStz(pStr->szStz());
}

/*----------------------------------------------------------------------------*/
/* push stz onto stack                                                        */
/*----------------------------------------------------------------------------*/
void TypedStack::vPushStz(Stz* szStz)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  //vDump(nullptr,"Before:"_Stz);
  
  i8 nSize = szStz->nSize();
  TypedCell* aTypedCell = aAllotStzVal(nSize);
  
  aTypedCell->vStz(szStz);
}

/*----------------------------------------------------------------------------*/
/* push value onto stack                                                      */
/*----------------------------------------------------------------------------*/
void TypedStack::vPushItem(TypedCell* aTypedCell)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);

  _hazard_();
  
  i8 nBytes = aTypedCell->nStackUsage();
  
  m_rTop.vAdvance(nBytes);
  
  m_rTop.m_aTypedCell->vCopy(aTypedCell,nBytes);
    
  _hazard_();
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* completely reset stack                                                     */
/*----------------------------------------------------------------------------*/
void TypedStack::vReset(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  m_rTop.m_ayStackPtr  = ayStackBase();
  m_rTop.m_nItems       = 0;
}

/*----------------------------------------------------------------------------*/
/* set stack back to retry point                                              */
/*----------------------------------------------------------------------------*/
void TypedStack::vRetry(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);

  m_rTop.m_aTypedCell = m_rRetry.m_aTypedCell;
  m_rTop.m_nItems  = m_rRetry.m_nItems;
}

/*----------------------------------------------------------------------------*/
/* mark retry point                                                           */
/*----------------------------------------------------------------------------*/
void TypedStack::vRetryPoint(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  m_rRetry.m_aTypedCell = m_rTop.m_aTypedCell;
  m_rRetry.m_nItems  = m_rTop.m_nItems;
}

/*----------------------------------------------------------------------------*/
/* stack contents as string                                                   */
/*----------------------------------------------------------------------------*/
void TypedStack::vStackContents(s48K& sStackContents)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  sStackContents = ""_Stz;
  
  i8 iParamValues = nValues();
  
  sStackContents.vCatSprint("("_Stz);
  for (i8 iValue = iParamValues - 1; iValue >= 0; iValue--) {
    
    sStackContents.vCatSprint(" "_Stz);
    
    TypedCell* aTypedCell = aPeekItem(iValue);
    
    eCN eClass = aTypedCell->eType();
    
    if (CN_INTVAL == eClass) {
      sStackContents.vCatSprint("%lld"_Stz,aTypedCell->iInt());
    }
    else if (CN_PTRVAL == eClass) {
      if (aTypedCell->pObj()) {
        
        Object* pPtr = aTypedCell->pObj();
        
        eCN ePtrClass = aTypedCell->pObj()->eGetClassNo();
        
        if (CN_WORD == ePtrClass)
          sStackContents.vCatSprint("^%s=%p:%s"_Stz,
                                    aCN[ePtrClass],
                                    pPtr,
                                    ((Word*)pPtr)->szWordNm());
        else if (CN_QUEUE == ePtrClass) {
          sStackContents.vCatSprint("^%s=%p:%s"_Stz,
                                    aCN[ePtrClass],
                                    pPtr,
                                    "queue"_Stz);
        }
//        else if (CN_TASK == ePtrClass) {
//          Task* pTask = _pTask(pPtr);
//          sStackContents.vCatSprint("^%s=%p:%s"_Stz,
//                                    aCN[ePtrClass],
//                                    pPtr,
//                                    pTask->szJobName());
//        }
        else {
          if (pPtr->bIsMagicValid() && pPtr->eGetClassNo() < CN_ENUMS)
            sStackContents.vCatSprint("^%s=%p"_Stz,
                                      aCN[ePtrClass],
                                      pPtr);
          //else
          //  sStackContents.vCatSprint("%p"_Stz,
          //                     pValWrk->pPtr());
        }
      }
      else
        sStackContents.vCatSprint("^%s=%p"_Stz,
                                  aCN[aTypedCell->eType()],
                                  aTypedCell->pObj());
    }
    else if (CN_FIPVAL == eClass) {
      sStackContents.vCatSprint("%p"_Stz,
                                aTypedCell->aFip());
    }
    else if (CN_STZVAL == eClass) {
      sStackContents.vCatSprint("\"%s\""_Stz,
                                aTypedCell->azChar());
    }
    else {
      sStackContents.vCatSprint("<unknown:class=%lld>"_Stz,
                                aTypedCell->eType());
    }
  }
  
  sStackContents.vCatSprint(" )"_Stz);
}

/*----------------------------------------------------------------------------*/
/* is top value a zero?                                                       */
/*----------------------------------------------------------------------------*/
bool TypedStack::bTopIsZero(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if (CN_INTVAL == m_rTop.m_aTypedCell->eType() &&
      0 == m_rTop.m_aTypedCell->iInt())
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* peek at value on top of stack                                              */
/*----------------------------------------------------------------------------*/
TypedCell* TypedStack::aTopVal(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  return m_rTop.m_aTypedCell;
}

/*----------------------------------------------------------------------------*/
/* sprint of stack using given format string                                  */
/*----------------------------------------------------------------------------*/
void TypedStack::vValSprint(s48K& sFormat, s48K& sString)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  char* acFmt = sFormat.azChar();
  char* acDst = sString.azChar();
  
  while (*acFmt) {
  
    // test for ordinary character
    if ('%' != *acFmt) {
      *acDst++ = *acFmt++;
      continue;
    }
        
    // we have a '%'
    acFmt++;
    
    // test for the %% case
    if ('%' == *acFmt) {
      *acDst++ = *acFmt++;
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // we have a %[width]v field
    
    i8 nWidth = 0;

    // record where output for this value began
    char* acFieldBegan = acDst;
    
    // do we have a %v of the form: %widthV ?
    if (StrLib::bIsDigit( * acFmt )) {
      i8 nDigits = 0;
      StrLib::bExtractInteger(&acFmt,&nWidth,nDigits);
      ASSERT(nDigits);
    }
        
    // should be %v or %V
    ASSERT('v' == *acFmt || 'V' == *acFmt);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // handle a StzVal, IntVal or a PtrVal
    
    TypedCell* aTypedCell0 = aPeekItem(0);
    eCN eClass = aTypedCell0->eType();
    
    // test for StzVal
    if (CN_STZVAL == eClass) {
      
      s48K sStr;
      vPopStr(sStr,__func__);
      
      if ('V' == *acFmt)
        *acDst++ = '"';
      
      for (char* pv = sStr.azChar(); *pv; )
        *acDst++ = *pv++;
      
      if ('V' == *acFmt)
        *acDst++ = '"';
      
      acFmt++;
    }
    
    // test for IntVal
    else
    if (CN_INTVAL == eClass) {
      
      i8 iInt = iPopInt(__func__);
      
      char caValue[32];
      StrLib::wToString(iInt,caValue);
      
      for (char* pv = caValue; *pv; )
        *acDst++ = *pv++;
      
      acFmt++;
    }
    
    // test for PtrVal
    else
    if (CN_PTRVAL == eClass) {
      
      Object* pObject = pPopPtr(__func__);
      
      char caValue[32];
      StrLib::wToPointer(pObject,caValue);
      
      for (char* pv = caValue; *pv; )
        *acDst++ = *pv++;
      
      acFmt++;
    }
    
    // just copy % ???
    else {
      *acDst++ = '%';
      *acDst++ = *acFmt++;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // handle optional field width case
    
    // test for field width
    if (! nWidth)
      continue;
    
    i8 nDiff = nDifference(acDst,acFieldBegan);
    
    if (nDiff >= nWidth)
      continue;
    
    i8 nPad = nWidth - nDiff;
    StrLib::vPad(acDst,nPad,' ');
    acDst += nPad;
  }
  
  // final termination
  *acDst = '\0';
}




/*----------------------------------------------------------------------------*/
/* get number of values currently on stack                                    */
/*----------------------------------------------------------------------------*/
i8 TypedStack::nValues(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  return m_rTop.m_nItems;
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* allot a value on top of stack                                               */
/*----------------------------------------------------------------------------*/
TypedCell* TypedStack::aAllotTypedCell(eCN eType, i8 nWithAdditionalBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  i8 nAllotBytes = sizeof(_Header) + sizeof(Dtm) + nWithAdditionalBytes;
  
  m_rTop.vAdvance(nAllotBytes);
  ASSERT(m_rTop.m_ayStackPtr >= ayStackLimit());
  
  m_rTop.m_aTypedCell->vMagic();

  m_rTop.m_aTypedCell->vType(eType);
  
  return m_rTop.m_aTypedCell;
}

/*----------------------------------------------------------------------------*/
/* allot a fip value on top of stack                                          */
/*----------------------------------------------------------------------------*/
TypedCell* TypedStack::aAllotFipVal(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  TypedCell* aTypedCell = aAllotTypedCell(CN_FIPVAL,C_NO_ADDITIONAL_BYTES);
    
  return aTypedCell;
}

/*----------------------------------------------------------------------------*/
/* allot an int value on top of stack                                         */
/*----------------------------------------------------------------------------*/
TypedCell* TypedStack::aAllotIntVal(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  TypedCell* aTypedCell = aAllotTypedCell(CN_INTVAL,C_NO_ADDITIONAL_BYTES);
    
  return aTypedCell;
}

/*----------------------------------------------------------------------------*/
/* allot a ptr value on top of stack                                          */
/*----------------------------------------------------------------------------*/
TypedCell* TypedStack::aAllotPtrVal(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  TypedCell* aTypedCell = aAllotTypedCell(CN_PTRVAL,C_NO_ADDITIONAL_BYTES);
    
  return aTypedCell;
}

/*----------------------------------------------------------------------------*/
/* allot a str value on top of stack                                          */
/*----------------------------------------------------------------------------*/
TypedCell* TypedStack::aAllotStzVal(i8 nStrSize)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  i8 nWithAdditionalBytes = nStrSize;
  
  TypedCell* aTypedCell = aAllotTypedCell(CN_STZVAL,nWithAdditionalBytes);
    
  return aTypedCell;
}

/*----------------------------------------------------------------------------*/
/* guard stack has at least nValues on it                                     */
/*----------------------------------------------------------------------------*/
bool TypedStack::bGuard(i8 nItems)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if (m_rTop.m_nItems < nItems)
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* guard top value on stack has given class                                   */
/*----------------------------------------------------------------------------*/
bool TypedStack::bGuard(eCN eClass0)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if (nValues() < 1)
    return false;
  
  TypedCell* aPeeked = aPeekItem(0);
  if (eClass0 != aPeeked->eType())
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* guard top 2 values on stack have given classes                             */
/*----------------------------------------------------------------------------*/
bool TypedStack::bGuard(eCN eClass0, eCN eClass1)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if (nValues() < 2)
    return false;
  
  TypedCell* aPeeked0 = aPeekItem(0);
  if (eClass0 != aPeeked0->eType())
    return false;
  
  TypedCell* aPeeked1 = aPeekItem(1);
  if (eClass1 != aPeeked1->eType())
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* guard top 3 values on stack have given classes                             */
/*----------------------------------------------------------------------------*/
bool TypedStack::bGuard(eCN eClass0, eCN eClass1, eCN eClass2)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if (nValues() < 3)
    return false;
  
  TypedCell* aPeeked0 = aPeekItem(0);
  if (eClass0 != aPeeked0->eType())
    return false;
  
  TypedCell* aPeeked1 = aPeekItem(1);
  if (eClass1 != aPeeked1->eType())
    return false;

  TypedCell* aPeeked2 = aPeekItem(2);
  if (eClass2 != aPeeked2->eType())
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* compute the base of this stack                                             */
/*----------------------------------------------------------------------------*/
byte* TypedStack::ayStackBase(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  byte* ayBase = & m_xWsp.ayArea()[C_WSP_STACK - 1];
  
  return pPoolCheck<byte*>(ayBase,__func__);
}

/*----------------------------------------------------------------------------*/
/* compute the limit of this stack                                            */
/*----------------------------------------------------------------------------*/
byte* TypedStack::ayStackLimit(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  byte* aLimit = & m_xWsp.ayArea()[0];
  
  return pPoolCheck<byte*>(aLimit,__func__);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(TypedStack)

/*----------------------------------------------------------------------------*/
/*-- Hazard ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aHazardFunction(TypedStack,CN_STACK)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
