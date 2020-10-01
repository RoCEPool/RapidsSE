/*============================================================================*/
/* Method.cpp - Kbase Method                                                  */
/*============================================================================*/

#include "Facts.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(Facts,Method,CN_METHOD)
__aNewFunction(Method,CN_METHOD)
__vDeleteFunction(Method,CN_METHOD)
__aFooterFunction(Method)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Method::Method(Kbase* pKbase, Fact* pSelf)
: Object(CN_METHOD)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_METHOD,_FOOTER_);
  
  #ifdef WITH_CHECK_INITIALISED
    MemLib::vZero(m_eaCode,sizeof(m_eaCode),CN_METHOD);
    MemLib::vZero(m_iaDataStack,sizeof(m_iaDataStack),CN_METHOD);
  #endif

  m_pKbase = pKbase;
  m_pSelf  = pSelf;
  m_pSubA  = nullptr;
  m_pSubB  = nullptr;

  m_i_Code = 0;
  m_eaCode[0] = TQ_NONE;
    
  _checkInitialised_(CN_METHOD);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_METHOD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Method::~Method(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_METHOD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add code                                                                   */
/*----------------------------------------------------------------------------*/
void Method::vAddCode(eTQ eCode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_METHOD,_FOOTER_);
    
  m_eaCode[m_i_Code++] = eCode;
}

/*----------------------------------------------------------------------------*/
/* add code with address argument                                             */
/*----------------------------------------------------------------------------*/
void Method::vAddCodeAdr(eTQ eCode, Opaque* aOpaque)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_METHOD,_FOOTER_);
      
  m_eaCode[m_i_Code++] = eCode;

  Opaque** aaOpaque = (Opaque**) & m_eaCode[m_i_Code];
  * aaOpaque = aOpaque;
  
  m_i_Code += sizeof(Opaque*);
}

/*----------------------------------------------------------------------------*/
/* add code with integer argument                                             */
/*----------------------------------------------------------------------------*/
void Method::vAddCodeInt(eTQ eCode, i8 iInt)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_METHOD,_FOOTER_);
      
  m_eaCode[m_i_Code++] = eCode;

  i8* piInt = (i8*) & m_eaCode[m_i_Code];
  * piInt = iInt;
  
  m_i_Code += sizeof(i8);
}

/*----------------------------------------------------------------------------*/
/* add code with string argument                                              */
/*----------------------------------------------------------------------------*/
void Method::vAddCodeStz(eTQ eCode, Stz* szStz)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_METHOD,_FOOTER_);
    
  m_eaCode[m_i_Code++] = eCode;

  for (char* acChr = szStz->azChar(); *acChr; acChr++)
    m_eaCode[m_i_Code++] = (eTQ)*acChr;
  m_eaCode[m_i_Code++] = (eTQ)0;
}

/*----------------------------------------------------------------------------*/
/* get code                                                                   */
/*----------------------------------------------------------------------------*/
eTQ Method::eGetCode(i8 i_Code)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_METHOD,_FOOTER_);
    
  eTQ eCode = m_eaCode[i_Code];
  
  return eCode;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* generate tq code for given method                                          */
/*----------------------------------------------------------------------------*/
void Method::vGenMethod(i8 iMethodNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_METHOD,_FOOTER_);
  
  switch (iMethodNo) {
    
    case 0: vGenMethod000(); break;
      
    default:
      NEVER_REACH;
  }

}

/*----------------------------------------------------------------------------*/
/* generate tq code for method 0                                              */
/*----------------------------------------------------------------------------*/
void Method::vGenMethod000(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_METHOD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* schedule next transform                                                    */
/*----------------------------------------------------------------------------*/
void Method::vSchedule(Fact** ppAnchor)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_METHOD,_FOOTER_);
  
  Fact* pPlan = m_pKbase->pElementFind("plan"_Stz);
  
  Fact* pPlanPrime = pPlan->pAttributeFindNm("planPrime"_Stz);
  
  i8 iPlanPrime = pPlanPrime->iValue();
  
  vTrace("PlanPrime %lld\n"_Stz,iPlanPrime);
    
  switch (iPlanPrime) {
      
    case  0: vTransformAdd("TQ_IMPORT_TABLES"_Stz);      pPlanPrime->vValue(1);  return;
    case  1: vTransformAdd("TQ_FROM_RESOLUTION"_Stz);    pPlanPrime->vValue(2);  return;
    case  2: vTransformAdd("TQ_PROJECT_RESOLUTION"_Stz); pPlanPrime->vValue(3);  return;
    case  3: vTransformAdd("TQ_OUTPUT_STREAMDEF"_Stz);   pPlanPrime->vValue(4);  return;
    case  4: /*vTransformAdd(""_Stz);*/ pPlanPrime->vValue(5);  return;
    case  5: /*vTransformAdd(""_Stz);*/ pPlanPrime->vValue(6);  return;
    case  6: /*vTransformAdd(""_Stz);*/ pPlanPrime->vValue(7);  return;
    case  7: /*vTransformAdd(""_Stz);*/ pPlanPrime->vValue(8);  return;
    case  8: /*vTransformAdd(""_Stz);*/ pPlanPrime->vValue(9);  return;
    case  9: /*vTransformAdd(""_Stz);*/ pPlanPrime->vValue(10); return;
    case 10: /*vTransformAdd(""_Stz);*/ pPlanPrime->vValue(11); return;
    case 11: /*vTransformAdd(""_Stz);*/ pPlanPrime->vValue(12); return;
    case 12: /*vTransformAdd(""_Stz);*/ pPlanPrime->vValue(13); return;

    default:
      break;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Fact* pElmOf = nullptr;

  pElmOf = (*ppAnchor)->pElementOf();
  pElmOf->vElementCut(*ppAnchor);
  
  Fact::vDeleteRecursively(ppAnchor);
}

/*----------------------------------------------------------------------------*/
/* add call to next transform                                                 */
/*----------------------------------------------------------------------------*/
void Method::vTransformAdd(Stz* szTransformNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_METHOD,_FOOTER_);

  Fact* pCall = m_pKbase->pElm("_call_"_Stz,"contents"_Stz);
                  m_pKbase->vAtr("_name_"_Stz,szTransformNm,pCall);
                  m_pKbase->vAtr("_priority_"_Stz,(i8)1,pCall);
                  m_pKbase->vAtr("_nArgs_"_Stz,(i8)0,pCall);
}

/*----------------------------------------------------------------------------*/
/* cut and recursively delete call to transform                               */
/*----------------------------------------------------------------------------*/
void Method::vTransformDelete(Fact** ppTransform)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_METHOD,_FOOTER_);
  
  Fact* pElmOf = nullptr;

  pElmOf = (*ppTransform)->pElementOf();
  pElmOf->vElementCut(*ppTransform);
  
  Fact::vDeleteRecursively(ppTransform);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                */
/*----------------------------------------------------------------------------*/
void Method::vTQ_ACCESS_METHODS(Fact** ppTransform)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_METHOD,_FOOTER_);
  
  vTransformDelete(ppTransform);
}

/*----------------------------------------------------------------------------*/
/*                                                */
/*----------------------------------------------------------------------------*/
void Method::vTQ_ADD_ASM_HANDLERS(Fact** ppTransform)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_METHOD,_FOOTER_);

  vTransformDelete(ppTransform);
}

/*----------------------------------------------------------------------------*/
/*                                                */
/*----------------------------------------------------------------------------*/
void Method::vTQ_ADD_ASM_SUBROUTINES(Fact** ppTransform)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_METHOD,_FOOTER_);

  vTransformDelete(ppTransform);
}

/*----------------------------------------------------------------------------*/
/*                                                */
/*----------------------------------------------------------------------------*/
void Method::vTQ_ADD_OPERATOR(Fact** ppTransform)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_METHOD,_FOOTER_);

  vTransformDelete(ppTransform);
}

/*----------------------------------------------------------------------------*/
/*                                                */
/*----------------------------------------------------------------------------*/
void Method::vTQ_ADD_OUTPUT_STAGE(Fact** ppTransform)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_METHOD,_FOOTER_);

  vTransformDelete(ppTransform);
}

/*----------------------------------------------------------------------------*/
/*                                                */
/*----------------------------------------------------------------------------*/
void Method::vTQ_ADD_PIPELINE(Fact** ppTransform)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_METHOD,_FOOTER_);
 
  vTransformDelete(ppTransform);
}

/*----------------------------------------------------------------------------*/
/*                                                */
/*----------------------------------------------------------------------------*/
void Method::vTQ_ADD_START_STAGE(Fact** ppTransform)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_METHOD,_FOOTER_);

  vTransformDelete(ppTransform);
}

/*----------------------------------------------------------------------------*/
/*                                                */
/*----------------------------------------------------------------------------*/
void Method::vTQ_FROM_RESOLUTION(Fact** ppTransform)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_METHOD,_FOOTER_);

  vTrace("vTQ_FROM_RESOLUTION\n"_Stz);
  
  //Question* pQuestion = new Question(m_pKbase,"fromList"_Stz,"fromNm"_Stz);
  Question* pQouter =
    new Question(QN_ELEMENTS_BY_ATR_NAME,m_pKbase,nullptr,
                 "fromList"_Stz,"fromNm"_Stz);

  while (pQouter->bEachMatch()) {

    Fact* pFromElm = pQouter->pElement();
    Fact* pFromAtr = pQouter->pAttribute();
    
    vTrace("FromNm: %s\n"_Stz,pFromAtr->szText());
    
    Question* pQinner =
      new Question(QN_ELEMENTS_BY_ATR_NAME_VALUE,m_pKbase,nullptr,
                   "tables"_Stz,"tableNm"_Stz,pFromAtr->szText());

    while (pQinner->bEachMatch()) {

      Fact* pToElm = pQinner->pElement();
      Fact* pToAtr = pQinner->pAttribute();
      vTrace("tableNm: %s\n"_Stz,pToAtr->szText());
      
      pFromElm->vAttributeAdd("_tableRef_"_Stz,pToElm);

      break;
    }
    
    delete pQinner;
  }
  
  delete pQouter;

  vTransformDelete(ppTransform);
}

/*----------------------------------------------------------------------------*/
/*                                                */
/*----------------------------------------------------------------------------*/
void Method::vTQ_IMPORT_TABLES(Fact** ppTransform)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_METHOD,_FOOTER_);

  vTrace("vTQ_IMPORT_TABLES\n"_Stz);
  
  Question* pQuestion = new Question(m_pKbase);

  pQuestion->vAsk("fromList"_Stz,"fromNm"_Stz);
  
  while (pQuestion->bEach()) {
    
    Fact* pFromNm = pQuestion->pAttribute();
    
    Stz* szTableNm = pFromNm->rxValue().m_pText->szGet();

    vTrace("Import: %s\n"_Stz,pFromNm->rxValue().m_pText->szGet());
    
    Table* pTable = AM::pTableFind(szTableNm);
    
    if (pTable)
      m_pKbase->vImportTable(pTable);
  }
  
  delete pQuestion;
  
  vTransformDelete(ppTransform);
}
/*----------------------------------------------------------------------------*/
//void Method::vTQ_IMPORT_TABLES(Fact** ppTransform)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_METHOD,_FOOTER_);
//
//  vTrace("vTQ_IMPORT_TABLES\n"_Stz);
//
//  Question* pAsk = new Question(m_pKbase);
//
//  Fact* pFromList = m_pKbase->pFactFind("fromList"_Stz);
//
//  for (Fact* pElement = pFromList->pElements();
//       pElement;
//       pElement = pElement->pElementNext()) {
//
//    Fact* pFromNm = pElement->pAttributeFindNm("fromNm"_Stz);
//
//    vTrace("Import: %s\n"_Stz,pFromNm->rxValue().m_pText->szGet());
//  }
//
//  delete pAsk;
//
//  vTransformDelete(ppTransform);
//}

/*----------------------------------------------------------------------------*/
/*                                                */
/*----------------------------------------------------------------------------*/
void Method::vTQ_INITIAL_PLAN(Fact** ppTransform)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_METHOD,_FOOTER_);

  vTransformDelete(ppTransform);
}

/*----------------------------------------------------------------------------*/
/*                                                */
/*----------------------------------------------------------------------------*/
void Method::vTQ_OUTPUT_STREAMDEF(Fact** ppTransform)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_METHOD,_FOOTER_);
  
  Fact* pStreamDefOut = m_pKbase->pElementInstance("plan"_Stz,"_streamDefOut_"_Stz);

  //Question* pQuestion = new Question(m_pKbase,"fromList"_Stz,"fromNm"_Stz);
  Question* pQouter =
    new Question(QN_ELEMENTS_BY_ELM_NAME,m_pKbase,nullptr,
                 "projectList"_Stz,"project"_Stz);

  while (pQouter->bEachMatch()) {

    Fact* pProject = pQouter->pElement();
    vTrace("project: %s %p\n"_Stz,pProject->szName(),pProject);

  }
  
  delete pQouter;

  vTransformDelete(ppTransform);
}

/*----------------------------------------------------------------------------*/
/*                                                */
/*----------------------------------------------------------------------------*/
void Method::vTQ_PROJECT_RESOLUTION(Fact** ppTransform)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_METHOD,_FOOTER_);
  
  Question* pQouter =
    new Question(QN_ELEMENTS_BY_ATR_NAME,m_pKbase,nullptr,
                 "projectList"_Stz,"projectNm"_Stz);

  while (pQouter->bEachMatch()) {

    Fact* pProjectElm = pQouter->pElement();
    Fact* pProjectAtr = pQouter->pAttribute();
    
    vTrace("projectNm: %s\n"_Stz,pProjectAtr->szText());
    
    Question* pQinner =
      new Question(QN_ELEMENTS_BY_ATR_NAME_VALUE,m_pKbase,nullptr,
                   "tables"_Stz,"columnNm"_Stz,pProjectAtr->szText());

    while (pQinner->bEachMatch()) {

      Fact* pToElm = pQinner->pElement();
      Fact* pToAtr = pQinner->pAttribute();
      vTrace("columnNm: %s\n"_Stz,pToAtr->szText());
      
      pProjectElm->vAttributeAdd("_columnRef_"_Stz,pToElm);

      break;
    }
    
    delete pQinner;
  }
  
  delete pQouter;

  vTransformDelete(ppTransform);
}

/*----------------------------------------------------------------------------*/
/*                                                */
/*----------------------------------------------------------------------------*/
void Method::vTQ_WRITE_EOASM(Fact** ppTransform)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_METHOD,_FOOTER_);

  vTransformDelete(ppTransform);
}

/*----------------------------------------------------------------------------*/
/*                                                */
/*----------------------------------------------------------------------------*/
void Method::vTQ_WRITE_PIPELINE(Fact** ppTransform)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_METHOD,_FOOTER_);

  vTransformDelete(ppTransform);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

#define PrmNil() \
  ASSERT(0 == TqPrms[eCode].m_nArgBytes);

/*----------------------------------------------------------------------------*/
/* execute method                                                             */
/*----------------------------------------------------------------------------*/
void Method::vExecute(Fact* pAnchor)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_METHOD,_FOOTER_);
  
  eTQ* aeCode = & m_eaCode[0];
  i8* aiDstk = m_iaDataStack;

  Fact* pElmOf   = nullptr;
  Fact* pElmNext = nullptr;
  Fact* pPaste   = nullptr;

  Fact* pHandler      = nullptr;
  Fact* pOperator     = nullptr;
  Fact* pPipeLine     = nullptr;
  Fact* pPlan         = nullptr;
  Fact* pProject      = nullptr;
  Fact* pStage        = nullptr;
  Fact* pStreamDefOut = nullptr;
  Fact* pSubroutine   = nullptr;

  Fact* pPipeLine_Text = nullptr;
  Fact* pEoAsm_Text    = nullptr;

  for (;;) {

    eTQ eCode = *aeCode++;

    switch (eCode) {

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      case TQ_FACT_NAME: PrmNil();
        break;
        
      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      case TQ_FIND_SUBA: PrmNil();
        break;
            
      case TQ_FIND_SUBB: PrmNil();
        break;
              
      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      case TQ_IS_SUBCLASS_OF: PrmNil();
        break;
      
      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      case TQ_POP_SUBA: PrmNil();
        break;
        
      case TQ_POP_SUBB: PrmNil();
        break;
      
      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      case TQ_PUSH_SELF: PrmNil();
        break;
        
      case TQ_PUSH_SUBA: PrmNil();
        break;
        
      case TQ_PUSH_SUBB: PrmNil();
        break;
      
      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      case TQ_SCHEDULE: PrmNil();
        vTrace("TQ_SCHEDULE \n"_Stz);
        vSchedule(&pAnchor);
        break;
        
      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      case TQ_CUT: PrmNil();
        
        pPaste = m_pKbase->pPaste();
        if (pPaste) {
          m_pKbase->vPaste(nullptr);
          Fact::vDeleteRecursively(&pAnchor);
        }

        pElmOf = pAnchor->pElementOf();
        pElmNext = pAnchor->pElementNext();
        vTrace("Anchor %p ElmOf %p, ElmNext %p\n"_Stz,pAnchor,pElmOf,pElmNext);
        
        pElmOf->vElementCut(pElmNext);
        pElmOf->vElementCut(pAnchor);
        
        if (m_pKbase->pPaste()) {
          Fact* pPaste = m_pKbase->pPaste();
          m_pKbase->vPaste(nullptr);
          
          Fact::vDeleteRecursively(&pPaste);
        }

        m_pKbase->vPaste(pElmNext);
        
        Fact::vDeleteRecursively(&pAnchor);

        break;
        
      case TQ_DELETE: PrmNil();
        break;
        
      case TQ_PASTE: PrmNil();
        
        pPaste = m_pKbase->pPaste();
        if (!pPaste)
          break;
        
        m_pKbase->vPaste(nullptr);

        pElmOf = pAnchor->pElementOf();
        
        pElmOf->vElementAppend(pPaste);
        pPaste = nullptr;

        pElmOf->vElementCut(pAnchor);
        Fact::vDeleteRecursively(&pAnchor);

        break;

      case TQ_SWAP: PrmNil();
        break;

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      case TQ_ACCESS_METHODS: PrmNil();
        vTQ_ACCESS_METHODS(&pAnchor);
        break;

      case TQ_ADD_ASM_HANDLERS: PrmNil();
        vTQ_ADD_ASM_HANDLERS(&pAnchor);
        break;

      case TQ_ADD_ASM_SUBROUTINES: PrmNil();
        vTQ_ADD_ASM_SUBROUTINES(&pAnchor);
        break;

      case TQ_ADD_OPERATOR: PrmNil();
        vTQ_ADD_OPERATOR(&pAnchor);
        break;

      case TQ_ADD_OUTPUT_STAGE: PrmNil();
        vTQ_ADD_OUTPUT_STAGE(&pAnchor);
        break;

      case TQ_ADD_PIPELINE: PrmNil();
        vTQ_ADD_PIPELINE(&pAnchor);
        break;

      case TQ_ADD_START_STAGE: PrmNil();
        vTQ_ADD_START_STAGE(&pAnchor);
        break;

      case TQ_FROM_RESOLUTION: PrmNil();
        vTQ_FROM_RESOLUTION(&pAnchor);
        break;

      case TQ_IMPORT_TABLES: PrmNil();
        vTQ_IMPORT_TABLES(&pAnchor);
        break;

      case TQ_INITIAL_PLAN: PrmNil();
        vTQ_INITIAL_PLAN(&pAnchor);
        break;

      case TQ_OUTPUT_STREAMDEF: PrmNil();
        vTQ_OUTPUT_STREAMDEF(&pAnchor);
        break;

      case TQ_PROJECT_RESOLUTION: PrmNil();
        vTQ_PROJECT_RESOLUTION(&pAnchor);
        break;

      case TQ_WRITE_EOASM: PrmNil();
        vTQ_WRITE_EOASM(&pAnchor);
        break;

      case TQ_WRITE_PIPELINE: PrmNil();
        vTQ_WRITE_PIPELINE(&pAnchor);
        break;


      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
      case TQ_RETURN: PrmNil();
        return;
        
      default:
        vTrace("Unknown TQ eCode %lld\n"_Stz,(i8)eCode);
        return;
        
    } // switch (ecode)
    
  } // for (;;)
  
}

#undef PrmNil

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
