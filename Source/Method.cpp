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
        break;

      case TQ_ADD_ASM_HANDLERS: PrmNil();
        break;

      case TQ_ADD_ASM_SUBROUTINES: PrmNil();
        break;

      case TQ_ADD_OPERATOR: PrmNil();
        break;

      case TQ_ADD_OUTPUT_STAGE: PrmNil();
        break;

      case TQ_ADD_PIPELINE: PrmNil();
        break;

      case TQ_ADD_START_STAGE: PrmNil();
        break;

      case TQ_FROM_RESOLUTION: PrmNil();
        break;

      case TQ_IMPORT_TABLES: PrmNil();
        break;

      case TQ_INITIAL_PLAN: PrmNil();
        break;

      case TQ_OUTPUT_STREAMDEF: PrmNil();
        break;

      case TQ_PROJECT_RESOLUTION: PrmNil();
        break;

      case TQ_WRITE_EOASM: PrmNil();
        break;

      case TQ_WRITE_PIPELINE: PrmNil();
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
