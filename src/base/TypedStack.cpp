/*============================================================================*/
/* TypedStack.cpp -- Push/Pop TypedStack of Typed Cells                       */
/*============================================================================*/

#include "BASE.h"

#include "SP_Word.hpp"
#include "LocalVar.hpp"
#include "LocalStr.hpp"
#include "SizeArea.hpp"
#include "BinaryStack.hpp"

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
  m_pStack = BinaryStack::pCreate(eSS::TINY, eSizeToCls);

  _checkInitialised_(CN_STACK);
  _postcondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
TypedStack::~TypedStack(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  BinaryStack::vDestroy(m_pStack);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump contents of the stack                                                 */
/*----------------------------------------------------------------------------*/
void TypedStack::vDump(DicStack* pDicStack, Stz* aTitle)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  vTrace("%s: this %p m_ayStackPtr %p\n"_Stz,aTitle,this,pStackHead());

  i8 nValues = this->nValues();
  for (i8 i_Item = 0; i_Item < nValues; i_Item++) {
    
    TypeValueArea* aTypedCell = aPeekItem(i_Item);
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
TypeValueArea* TypedStack::aPeekItem(i8 i_Item)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);

  // Lambda function to get the size of an item on the stack
  auto fStackItemSize = [](byte* pyAddr) -> i8 {
    TypeValueArea* pyPeeked = reinterpret_cast<TypeValueArea*>(pyAddr);
    i8 iSize = pyPeeked->nStackUsage();
    return iSize;
  };

  byte* pyItem = m_pStack->pyPeekItem(i_Item, fStackItemSize);
  TypeValueArea* pyPeekItem = reinterpret_cast<TypeValueArea*>(pyItem);
  return pyPeekItem;
}

/*----------------------------------------------------------------------------*/
/* peek at given value on stack                                               */
/*----------------------------------------------------------------------------*/
void TypedStack::vPeekTypeValueArea(i8 i_Item, TypeValueArea* aTypedCell)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  TypeValueArea* aPeeked = aPeekItem(i_Item);
  
  aTypedCell->vCopy(aPeeked);
}

/*----------------------------------------------------------------------------*/
/* poke given value into stack                                                */
/*----------------------------------------------------------------------------*/
void TypedStack::vPokeItem(i8 i_Item, TypeValueArea* aTypedCell)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  TypeValueArea* aPoked = aPeekItem(i_Item);
  
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

  TypeValueArea* pHead = pStackHead();
  i8 iInt = pHead->iAsInt();
  vDiscardTop();
  
  return iInt;
}

/*----------------------------------------------------------------------------*/
/* pop bcd from stack                                                         */
/*----------------------------------------------------------------------------*/
i8 TypedStack::iPopBcd(const char* azCallingFunNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if ( ! bGuard(CN_BCDVAL) )
    BASE::vThrow(FK_DSTACK,
                        azCallingFunNm,__FILE__,__LINE__,
                        "Invalid d-stack"_Stz);

  TypeValueArea* pHead = pStackHead();
  i8 iBcd = pHead->iBcd();
  
  vDiscardTop();
  
  return iBcd;
}

/*----------------------------------------------------------------------------*/
/* pop bol from stack                                                         */
/*----------------------------------------------------------------------------*/
i8 TypedStack::iPopBol(const char* azCallingFunNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if ( ! bGuard(CN_BOLVAL) )
    BASE::vThrow(FK_DSTACK,
                        azCallingFunNm,__FILE__,__LINE__,
                        "Invalid d-stack"_Stz);

  TypeValueArea* pHead = pStackHead();
  i8 iBol = pHead->iBol();
  
  vDiscardTop();
  
  return iBol;
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

  TypeValueArea* pHead = pStackHead();
  CodeCell* aFip = pHead->aFip();
  
  vDiscardTop();
  
  return aFip;
}

/*----------------------------------------------------------------------------*/
/* pop d8 flt from stack                                                      */
/*----------------------------------------------------------------------------*/
d8 TypedStack::dPopFlt(const char* azCallingFunNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if ( ! bGuard(CN_FLTVAL) )
    BASE::vThrow(FK_DSTACK,
                        azCallingFunNm,__FILE__,__LINE__,
                        "Invalid d-stack"_Stz);

  TypeValueArea* pHead = pStackHead();
  d8 dFlt = pHead->dFlt();
  
  vDiscardTop();
  
  return dFlt;
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

  TypeValueArea* pHead = pStackHead();
  i8 iInt = pHead->iInt();
  
  vDiscardTop();
  
  return iInt;
}

/*----------------------------------------------------------------------------*/
/* pop i8 from stack                                                          */
/*----------------------------------------------------------------------------*/
i8 TypedStack::iPopIntBol(const char* azCallingFunNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if ( ! bGuard(CN_INTVAL) && ! bGuard(CN_BOLVAL) )
    BASE::vThrow(FK_DSTACK,
                        azCallingFunNm,__FILE__,__LINE__,
                        "Invalid d-stack"_Stz);

  TypeValueArea* pHead = pStackHead();
  i8 iInt = pHead->iInt();
  
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

  TypeValueArea* pHead = pStackHead();
  Object* pObj = pHead->pObj();

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

  TypeValueArea* pHead = pStackHead();
  
  pString->vCopy(pHead->szStz());
    
  vDiscardTop();
}

/*----------------------------------------------------------------------------*/
/* pop stz from stack                                                         */
/*----------------------------------------------------------------------------*/
void TypedStack::vPopStz(Text* psText, const char* azCallingFunNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if ( ! bGuard(CN_STZVAL) )
    BASE::vThrow(FK_DSTACK,
                        azCallingFunNm,__FILE__,__LINE__,
                        "Invalid d-stack"_Stz);
  
  TypeValueArea* pHead = pStackHead();

  psText->vSet(pHead->szStz());
    
  vDiscardTop();
}

/*----------------------------------------------------------------------------*/
/* pop value from stack                                                       */
/*----------------------------------------------------------------------------*/
void TypedStack::vPopSenCell(SenCell* aSenCell, const char* azCallingFunNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);

  if ( ! bGuard(1) )
    BASE::vThrow(FK_DSTACK,
                        azCallingFunNm,__FILE__,__LINE__,
                        "Invalid d-stack"_Stz);

  TypeValueArea* pHead = pStackHead();
  i8 nBytes = pHead->nStackUsage();

  aSenCell->aTypeValueArea()->vCopy(pHead,nBytes);

  vDiscardTop();
}

/*----------------------------------------------------------------------------*/
/* pop value from stack                                                       */
/*----------------------------------------------------------------------------*/
void TypedStack::vPopTypeValueArea(TypeValueArea* aTypeValueArea,
                                   const char* azCallingFunNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if ( ! bGuard(1) )
    BASE::vThrow(FK_DSTACK,
                        azCallingFunNm,__FILE__,__LINE__,
                        "Invalid d-stack"_Stz);
  
  TypeValueArea* pHead = pStackHead();
  i8 nBytes = pHead->nStackUsage();
  
  aTypeValueArea->vCopy(pHead,nBytes);
  
  vDiscardTop();
}

/*----------------------------------------------------------------------------*/
/* pop value from stack into localVar                                         */
/*----------------------------------------------------------------------------*/
void TypedStack::vPopLocalVar(LocalVar* axLocalVar,
                              const char* azCallingFunNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  UNUSED(azCallingFunNm);

  TypeValueArea* pHead = pStackHead();
  axLocalVar->vSet(pHead);

  vDiscardTop();
}

/*----------------------------------------------------------------------------*/
/* A function mapping binary stack size to a class enum                       */
/*----------------------------------------------------------------------------*/
eCN TypedStack::eSizeToCls(eSS eSize)
{
  switch (eSize) {

    case TINY:    return CN_TYPESTACKTINY;
    case SMALL:   return CN_TYPESTACKSMALL;
    case MEDIUM:  return CN_TYPESTACKMEDIUM;
    case LARGE:   return CN_TYPESTACKLARGE;
    default:      NEVER_REACH
  }
}


/*----------------------------------------------------------------------------*/
/* push BCD onto stack                                                        */
/*----------------------------------------------------------------------------*/
void TypedStack::vPushBcd(i8 iBcd)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  TypeValueArea* aTypedCell = aAllotBcdVal();
  
  aTypedCell->vBcd(iBcd);
}

/*----------------------------------------------------------------------------*/
/* push BOL onto stack                                                        */
/*----------------------------------------------------------------------------*/
void TypedStack::vPushBol(i8 iBol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  TypeValueArea* aTypedCell = aAllotBolVal();
  
  aTypedCell->vBol(iBol);
}

/*----------------------------------------------------------------------------*/
/* push CodeCell* onto stack                                                  */
/*----------------------------------------------------------------------------*/
void TypedStack::vPushFip(CodeCell* aFip)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  TypeValueArea* aTypedCell = aAllotFipVal();
  
  aTypedCell->vFip(aFip);
}

/*----------------------------------------------------------------------------*/
/* push d8 flt onto stack                                                     */
/*----------------------------------------------------------------------------*/
void TypedStack::vPushFlt(d8 dFlt)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  TypeValueArea* aTypedCell = aAllotFltVal();
  
  aTypedCell->vFlt(dFlt);
}

/*----------------------------------------------------------------------------*/
/* push i8 onto stack                                                         */
/*----------------------------------------------------------------------------*/
void TypedStack::vPushInt(i8 iInt)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  TypeValueArea* aTypedCell = aAllotIntVal();
  
  aTypedCell->vInt(iInt);
}

/*----------------------------------------------------------------------------*/
/* push pointer to proper object onto stack                                   */
/*----------------------------------------------------------------------------*/
void TypedStack::vPushPtr(Object* pObj)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  TypeValueArea* aTypedCell = aAllotPtrVal();
  
  aTypedCell->vObj(pObj);
}

/*----------------------------------------------------------------------------*/
/* push string onto stack                                                     */
/*----------------------------------------------------------------------------*/
void TypedStack::vPushStr(String* pStr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  i8 nSize = StrLib::nSize(pStr->azStr());
  TypeValueArea* aTypedCell = aAllotStzVal(nSize);

  aTypedCell->vStz(pStr->szStz(), nSize);
}

/*----------------------------------------------------------------------------*/
/* push stz onto stack                                                        */
/*----------------------------------------------------------------------------*/
void TypedStack::vPushStz(Stz* szStz)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  //vDump(nullptr,"Before:"_Stz);
  
  i8 nSize = szStz->nSize();
  TypeValueArea* aTypedCell = aAllotStzVal(nSize);
  
  aTypedCell->vStz(szStz, nSize);
}

/*----------------------------------------------------------------------------*/
/* push value onto stack                                                      */
/*----------------------------------------------------------------------------*/
void TypedStack::vPushSenCell(SenCell* aSenCell)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);

  i8 nBytes = aSenCell->nStackUsage();

  TypeValueArea* pHead = reinterpret_cast<TypeValueArea*>(m_pStack->pyAdvance(nBytes));
  pHead->vCopy(aSenCell->aTypeValueArea(),nBytes);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* push value onto stack                                                      */
/*----------------------------------------------------------------------------*/
void TypedStack::vPushTypeValueArea(TypeValueArea* aTypeValueArea)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);

  i8 nBytes = aTypeValueArea->nStackUsage();

  TypeValueArea* pHead = reinterpret_cast<TypeValueArea*>(m_pStack->pyAdvance(nBytes));
  pHead->vCopy(aTypeValueArea,nBytes);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* push value in localVar onto stack                                          */
/*----------------------------------------------------------------------------*/
void TypedStack::vPushLocalVar(LocalVar* axLocalVar)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);

  i8 jStackNeeded = axLocalVar->jStackUsage();
  TypeValueArea* pHead = reinterpret_cast<TypeValueArea*>(m_pStack->pyAdvance(jStackNeeded));

  axLocalVar->vGet(pHead);
}

/*----------------------------------------------------------------------------*/
/* completely reset stack                                                     */
/*----------------------------------------------------------------------------*/
void TypedStack::vReset(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);

  m_pStack->vReset();
}

/*----------------------------------------------------------------------------*/
/* stack contents as string                                                   */
/*----------------------------------------------------------------------------*/
void TypedStack::vStackContents(s48K& sStackContents)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  sStackContents = ""_Stz;
  
  i8 iParamValues = nValues();
  
  sStackContents.vCatSprint("("_Stz);
  for (i8 iValue = iParamValues - 1; iValue >= 0; iValue--) {
    
    sStackContents.vCatSprint(" "_Stz);
    
    TypeValueArea* aTypedCell = aPeekItem(iValue);
    
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

  TypeValueArea* pTop = pStackHead();
  if (CN_INTVAL == pTop->eType() && 0 == pTop->iInt())
    return true;
  
  return false;
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
    
    TypeValueArea* aTypedCell0 = aPeekItem(0);
    eCN eClass = aTypedCell0->eType();
    
    
    if (CN_BCDVAL == eClass) {
      
      i8 iInt = iPopBcd(__func__);
      
      char caValue[32];
      StrLib::vToString(caValue, sizeof(caValue), iInt);
      
      for (char* pv = caValue; *pv; )
        *acDst++ = *pv++;
      
      acFmt++;
    }
    
    if (CN_BOLVAL == eClass) {
      
      i8 iInt = iPopBol(__func__);
      
      char caValue[32];
      StrLib::vToString(caValue, sizeof(caValue), (bool) iInt);
      
      for (char* pv = caValue; *pv; )
        *acDst++ = *pv++;
      
      acFmt++;
    }
    
    // test for FltVal
    else if (CN_FLTVAL == eClass) {
      
      d8 dFlt = dPopFlt(__func__);
      
      char caValue[32];
      StrLib::vToString(caValue, sizeof(caValue), dFlt);
      
      for (char* pv = caValue; *pv; )
        *acDst++ = *pv++;
      
      acFmt++;
    }
    
    // test for IntVal
    else if (CN_INTVAL == eClass) {
      
      i8 iInt = iPopInt(__func__);
      
      char caValue[32];
      StrLib::vToString(caValue, sizeof(caValue), iInt);
      
      for (char* pv = caValue; *pv; )
        *acDst++ = *pv++;
      
      acFmt++;
    }

    // test for PtrVal
    else if (CN_PTRVAL == eClass) {
      
      Object* pObject = pPopPtr(__func__);
      
      char caValue[32];
      StrLib::vToPointer(caValue, sizeof(caValue), pObject);
      
      for (char* pv = caValue; *pv; )
        *acDst++ = *pv++;
      
      acFmt++;
    }
    
    // test for StzVal
    else if (CN_STZVAL == eClass) {
      
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
    StrLib::vPad(acDst, sString.nAreaBytes(), nPad,' ');
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
  
  return m_pStack->nItems();
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* allot an bcd value on top of stack                                         */
/*----------------------------------------------------------------------------*/
TypeValueArea* TypedStack::aAllotBcdVal(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  TypeValueArea* aTypedCell = aAllotTypedCell(CN_BOLVAL,C_NO_ADDITIONAL_BYTES);
    
  return aTypedCell;
}

/*----------------------------------------------------------------------------*/
/* allot an bol value on top of stack                                         */
/*----------------------------------------------------------------------------*/
TypeValueArea* TypedStack::aAllotBolVal(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  TypeValueArea* aTypedCell = aAllotTypedCell(CN_BOLVAL,C_NO_ADDITIONAL_BYTES);
    
  return aTypedCell;
}

/*----------------------------------------------------------------------------*/
/* allot a fip value on top of stack                                          */
/*----------------------------------------------------------------------------*/
TypeValueArea* TypedStack::aAllotFipVal(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  TypeValueArea* aTypedCell = aAllotTypedCell(CN_FIPVAL,C_NO_ADDITIONAL_BYTES);
    
  return aTypedCell;
}

/*----------------------------------------------------------------------------*/
/* allot an flt value on top of stack                                         */
/*----------------------------------------------------------------------------*/
TypeValueArea* TypedStack::aAllotFltVal(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  TypeValueArea* aTypedCell = aAllotTypedCell(CN_FLTVAL,C_NO_ADDITIONAL_BYTES);
    
  return aTypedCell;
}

/*----------------------------------------------------------------------------*/
/* allot an int value on top of stack                                         */
/*----------------------------------------------------------------------------*/
TypeValueArea* TypedStack::aAllotIntVal(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  TypeValueArea* aTypedCell = aAllotTypedCell(CN_INTVAL,C_NO_ADDITIONAL_BYTES);
    
  return aTypedCell;
}

/*----------------------------------------------------------------------------*/
/* allot a ptr value on top of stack                                          */
/*----------------------------------------------------------------------------*/
TypeValueArea* TypedStack::aAllotPtrVal(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  TypeValueArea* aTypedCell = aAllotTypedCell(CN_PTRVAL,C_NO_ADDITIONAL_BYTES);
    
  return aTypedCell;
}

/*----------------------------------------------------------------------------*/
/* allot a str value on top of stack                                          */
/*----------------------------------------------------------------------------*/
TypeValueArea* TypedStack::aAllotStzVal(i8 nStrSize)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  i8 nWithAdditionalBytes = nStrSize;
  
  TypeValueArea* aTypedCell = aAllotTypedCell(CN_STZVAL,nWithAdditionalBytes);
    
  return aTypedCell;
}

/*----------------------------------------------------------------------------*/
/* allot a value on top of stack                                              */
/*----------------------------------------------------------------------------*/
TypeValueArea* TypedStack::aAllotTypedCell(eCN eType, i8 nWithAdditionalBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  i8 nAllotBytes = sizeof(_Header) + sizeof(Dtm) + nWithAdditionalBytes;

  TypeValueArea* pTop = reinterpret_cast<TypeValueArea*>(m_pStack->pyAdvance(nAllotBytes));
  ASSERT(pTop);

  pTop->vMagic();
  pTop->vType(eType);
  
  return pTop;
}

/*----------------------------------------------------------------------------*/
/* guard stack has at least nValues on it                                     */
/*----------------------------------------------------------------------------*/
void TypedStack::vDiscardTop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);

  TypeValueArea* pHead = pStackHead();
  i8 nBytes = pHead->nStackUsage();

  m_pStack->vRewind(nBytes);
}

/*----------------------------------------------------------------------------*/
/* guard stack has at least nValues on it                                     */
/*----------------------------------------------------------------------------*/
bool TypedStack::bGuard(i8 nItems)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);

  if (m_pStack->nItems() < nItems)
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

  TypeValueArea* aPeeked = aPeekItem(0);
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
  
  TypeValueArea* aPeeked0 = aPeekItem(0);
  if (eClass0 != aPeeked0->eType())
    return false;
  
  TypeValueArea* aPeeked1 = aPeekItem(1);
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
  
  TypeValueArea* aPeeked0 = aPeekItem(0);
  if (eClass0 != aPeeked0->eType())
    return false;
  
  TypeValueArea* aPeeked1 = aPeekItem(1);
  if (eClass1 != aPeeked1->eType())
    return false;

  TypeValueArea* aPeeked2 = aPeekItem(2);
  if (eClass2 != aPeeked2->eType())
    return false;
  
  return true;
}


/*----------------------------------------------------------------------------*/
/* Get the head of the stack as a TypeValueArea*                              */
/*----------------------------------------------------------------------------*/
TypeValueArea* TypedStack::pStackHead(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_STACK, _FOOTER_);

  return reinterpret_cast<TypeValueArea*>(m_pStack->pyStackHead());
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(TypedStack)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
