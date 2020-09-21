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

  Fact* pElmOf = nullptr;
  Fact* pElmNext = nullptr;
  Fact* pPaste = nullptr;

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
      case TQ_CUT: PrmNil();
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
        
        pElmOf = pAnchor->pElementOf();

        pPaste = m_pKbase->pPaste();
        m_pKbase->vPaste(nullptr);
        
        pElmOf->vElementAppend(pPaste);
        pPaste = nullptr;

        pElmOf->vElementCut(pAnchor);
        Fact::vDeleteRecursively(&pAnchor);

        break;

      case TQ_SWAP: PrmNil();
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
