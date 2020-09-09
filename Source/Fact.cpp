/*============================================================================*/
/* Fact.cpp - Fact                                                            */
/*============================================================================*/

#include "Facts.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(Facts,Fact,CN_FACT)
__aNewFunction(Fact,CN_FACT)
__vDeleteFunction(Fact,CN_FACT)
__aFooterFunction(Fact)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Fact::Fact(Kbase* pKbase, Fact* pSuper, Stz* szNote)
: Object(CN_FACT)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  ASSERT(!pSuper || pKbase->pFact());
  
  m_pKbase  = pKbase;
  
  m_iFactNo = pSuper ? m_pKbase->iNextFactNo() : 0;
  m_sNote   = szNote ? szNote : C_NO_NOTE;
  m_xProperty.m_pObject = nullptr;
    
  m_pSubclassOf    = pSuper;
  m_pAttributeOf   = nullptr;
  m_pElementOf     = nullptr;

  m_pAttributes    = nullptr;
  m_pAttributeNext = nullptr;

  m_pElements      = nullptr;
  m_pElementNext   = nullptr;
  
  _checkInitialised_(CN_FACT);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Fact::~Fact(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add attribute (a sub-fact)                                                 */
/*----------------------------------------------------------------------------*/
void Fact::vAttributeAdd(Fact* pAttribute)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);

  pAttribute->vAttributeOf(this);
  
  if (! m_pAttributes) {
    m_pAttributes = pAttribute;
    return;
  }
  
  Fact* pTail = m_pAttributes;
  while (pTail->pAttributeNext())
    pTail = pTail->pAttributeNext();
  
  pTail->vAttributeNext(pAttribute);
}
 
/*----------------------------------------------------------------------------*/
/* get next attribute                                                         */
/*----------------------------------------------------------------------------*/
Fact* Fact::pAttributeNext(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  return m_pAttributeNext;
}

/*----------------------------------------------------------------------------*/
/* set next attribute                                                         */
/*----------------------------------------------------------------------------*/
void Fact::vAttributeNext(Fact* pAttribute)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  m_pAttributeNext = pAttribute;
}

/*----------------------------------------------------------------------------*/
/* get which fact this is an attribute of                                     */
/*----------------------------------------------------------------------------*/
Fact* Fact::pAttributeOf(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  return m_pAttributeOf;
}

/*----------------------------------------------------------------------------*/
/* set which fact this is an attribute of                                     */
/*----------------------------------------------------------------------------*/
void Fact::vAttributeOf(Fact* pAttributeOf)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  m_pAttributeOf = pAttributeOf;
}

/*----------------------------------------------------------------------------*/
/* get first attribute of this fact                                           */
/*----------------------------------------------------------------------------*/
Fact* Fact::pAttributes(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  return m_pAttributes;
}

/*----------------------------------------------------------------------------*/
/* set first attribute of this fact                                           */
/*----------------------------------------------------------------------------*/
void Fact::vAttributes(Fact* pAttributes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  m_pAttributes = pAttributes;
}

/*----------------------------------------------------------------------------*/
/* get attribute that is a sub-class of given super fact                      */
/*----------------------------------------------------------------------------*/
Fact* Fact::pAttributeThatIsSubClass(Fact* pOfSuperFact)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);

  Fact* pScan = m_pAttributes;
  while (pScan && ! pScan->bIsSubClass(pOfSuperFact))
    pScan = pScan->pAttributeNext();
    
  return pScan;
}

/*----------------------------------------------------------------------------*/
/* add given fact element to this set                                         */
/*----------------------------------------------------------------------------*/
void Fact::vElementAdd(Fact* pElement)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);

  pElement->vElementOf(this);
  
  if (! m_pElements) {
    m_pElements = pElement;
    return;
  }
  
  Fact* pTail = m_pElements;
  while (pTail->pElementNext())
    pTail = pTail->pElementNext();
  
  pTail->vElementNext(pElement);
}

/*----------------------------------------------------------------------------*/
/* get next element                                                           */
/*----------------------------------------------------------------------------*/
Fact* Fact::pElementNext(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  return m_pElementNext;
}

/*----------------------------------------------------------------------------*/
/* set next fact element                                                      */
/*----------------------------------------------------------------------------*/
void Fact::vElementNext(Fact* pElement)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  m_pElementNext = pElement;
}

/*----------------------------------------------------------------------------*/
/* get element of which set                                                   */
/*----------------------------------------------------------------------------*/
Fact* Fact::pElementOf(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  return m_pElementOf;
}

/*----------------------------------------------------------------------------*/
/* set element of which set                                                   */
/*----------------------------------------------------------------------------*/
void Fact::vElementOf(Fact* pElementOf)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  m_pElementOf = pElementOf;
}

/*----------------------------------------------------------------------------*/
/* get first element of this fact set                                         */
/*----------------------------------------------------------------------------*/
Fact* Fact::pElements(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  return m_pElements;
}

/*----------------------------------------------------------------------------*/
/* set first element of this fact set                                         */
/*----------------------------------------------------------------------------*/
void Fact::vElements(Fact* pElements)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  m_pElements = pElements;
}

/*----------------------------------------------------------------------------*/
/* get element that is a sub-class of given super fact                        */
/*----------------------------------------------------------------------------*/
Fact* Fact::pElementThatIsSubClass(Fact* pOfSuperFact)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);

  Fact* pScan = m_pElements;
  while (pScan && ! pScan->bIsSubClass(pOfSuperFact))
    pScan = pScan->pElementNext();
    
  return pScan;
}

/*----------------------------------------------------------------------------*/
/* get fact no                                                                */
/*----------------------------------------------------------------------------*/
i8 Fact::iFactNo(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  return m_iFactNo;
}

/*----------------------------------------------------------------------------*/
/* set fact no                                                                */
/*----------------------------------------------------------------------------*/
void Fact::vFactNo(i8 iFactNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  m_iFactNo = iFactNo;
}

/*----------------------------------------------------------------------------*/
/* find element fact (recursively)                                            */
/*----------------------------------------------------------------------------*/
Fact* Fact::pFindElement(Stz* szNote, Fact* pSuper)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  if (m_sNote.bSame(szNote) && (! pSuper || bIsSubClass(pSuper)))
    return this;
  
  Fact* pElement = nullptr;
  
  for (Fact* pScan = m_pElements;
       pScan && !pElement;
       pScan = pScan->m_pElementNext)
    pElement = pScan->pFindElement(szNote,pSuper);

  return pElement;
}

/*----------------------------------------------------------------------------*/
/* get fact that is immediate child of given fact (recursively)               */
/*----------------------------------------------------------------------------*/
Fact* Fact::pGetSuperChild(Fact* pOfFact)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  if (m_pSubclassOf == pOfFact)
    return this;
  
  if (!m_pSubclassOf)
    return nullptr;
  
  return m_pSubclassOf->pGetSuperChild(pOfFact);
}

/*----------------------------------------------------------------------------*/
/* has an attribute which is specifically given fact                          */
/*----------------------------------------------------------------------------*/
bool Fact::bHasSpecificAttribute(Fact* pFact)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);

  Fact* pScan = m_pAttributes;
  while (pScan && pScan != pFact)
    pScan = pScan->pAttributeNext();
    
  return pScan;
}
 
/*----------------------------------------------------------------------------*/
/* has an element which is specifically given fact                            */
/*----------------------------------------------------------------------------*/
bool Fact::bHasSpecificElement(Fact* pFact)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);

  Fact* pScan = m_pElements;
  while (pScan && pScan != pFact)
    pScan = pScan->pElementNext();
    
  return pScan;
}
 
/*----------------------------------------------------------------------------*/
/* is this fact a sub-class of given fact                         (recursive) */
/*----------------------------------------------------------------------------*/
bool Fact::bIsSubClass(Fact* pOfFact)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);

  if (this == pOfFact)
    return true;
  
  if (! m_pSubclassOf)
    return false;
  
  return m_pSubclassOf->bIsSubClass(pOfFact);
}

/*----------------------------------------------------------------------------*/
/* is this fact a sub-class of given fact                         (recursive) */
/*----------------------------------------------------------------------------*/
bool Fact::bIsSubClass(Stz* szNote)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);

  if (m_sNote.bSame(szNote))
    return true;
  
  if (! m_pSubclassOf)
    return false;
  
  return m_pSubclassOf->bIsSubClass(szNote);
}

/*----------------------------------------------------------------------------*/
/* is this fact an int?                                                       */
/*----------------------------------------------------------------------------*/
bool Fact::bIsInt(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);

  return bIsSubClass(m_pKbase->m_p_Int_) && this != m_pKbase->m_p_Int_;
}

/*----------------------------------------------------------------------------*/
/* is this fact an object?                                                    */
/*----------------------------------------------------------------------------*/
bool Fact::bIsObject(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);

  return bIsSubClass(m_pKbase->m_p_Object_) && this != m_pKbase->m_p_Object_;
}

/*----------------------------------------------------------------------------*/
/* is this fact an opaque?                                                    */
/*----------------------------------------------------------------------------*/
bool Fact::bIsOpaque(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);

  return bIsSubClass(m_pKbase->m_p_Opaque_) && this != m_pKbase->m_p_Opaque_;
}

/*----------------------------------------------------------------------------*/
/* is this fact a text?                                                       */
/*----------------------------------------------------------------------------*/
bool Fact::bIsText(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);

  return bIsSubClass(m_pKbase->m_p_Text_) && this != m_pKbase->m_p_Text_;
}

/*----------------------------------------------------------------------------*/
/* is this fact a word?                                                       */
/*----------------------------------------------------------------------------*/
bool Fact::bIsWord(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);

  return bIsSubClass(m_pKbase->m_p_Word_) && this != m_pKbase->m_p_Word_;
}

/*----------------------------------------------------------------------------*/
/* is this fact an cross-ref?                                                 */
/*----------------------------------------------------------------------------*/
bool Fact::bIsXref(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);

  return bIsSubClass(m_pKbase->m_p_Xref_) && this != m_pKbase->m_p_Xref_;
}

/*----------------------------------------------------------------------------*/
/* create a new instance of this fact                                         */
/*----------------------------------------------------------------------------*/
Fact* Fact::pNewInstance(Stz* szNote)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  Fact* pFact = new Fact(m_pKbase, this, szNote);
  
  if (! szNote) {
    pFact->m_sNote.vSprint("%s"_Stz,m_sNote.azChar());
    StrLib::vRemovePrefix(pFact->m_sNote.azChar(),'_');
    StrLib::vRemoveSuffix(pFact->m_sNote.azChar(),'_');
  }
  
  return pFact;
}

/*----------------------------------------------------------------------------*/
/* get fact name                                                              */
/*----------------------------------------------------------------------------*/
Stz* Fact::szNote(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  return m_sNote;
}

/*----------------------------------------------------------------------------*/
/* set fact note                                                              */
/*----------------------------------------------------------------------------*/
void Fact::vNote(Stz* szNote)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  m_sNote = szNote;
}

/*----------------------------------------------------------------------------*/
/* print fact (recursive)                                                     */
/*----------------------------------------------------------------------------*/
void Fact::vPrint_r(i8 iDepth, i8 iAs)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  if (m_sNote.bSame("question"_Stz))
    vDebugPoint();

  if (iAs) {
    if (bIsSubClass("_folder_"_Stz))
      vTrace("f:%s"_Stz,Stz::szTab(iDepth));
    else
    if (bIsSubClass("_tuple_"_Stz))
      vTrace("t:%s"_Stz,Stz::szTab(iDepth));
    else
    if (bIsSubClass("_questions_"_Stz))
      vTrace("e:%s"_Stz,Stz::szTab(iDepth));
  }
  
  switch (iAs) {
    case 0:

      vTrace("%lld [%lld] %s"_Stz,
             iAs,
         
             m_iFactNo,
             m_sNote.azChar()
             );
      break;
      
    case 1:

      vTrace("%lld [%lld] %s"_Stz,
             iAs,
         
             m_iFactNo,
             m_sNote.azChar()
             );
      break;
      
    case 2:

      vTrace("a:%s%lld [%lld] %s"_Stz,
             Stz::szTab(iDepth),
             iAs,
         
             m_iFactNo,
             m_sNote.azChar()
             );
      break;
      
    case 3:

      vTrace("%lld [%lld] %s"_Stz,
             iAs,
         
             m_iFactNo,
             m_sNote.azChar()
             );
      break;
  }
  /*
  if (bIsInt())
    vTrace(" = Int %lld"_Stz,m_xProperty.m_iInt);
  else if (bIsObject())
    vTrace(" = Object %p"_Stz,m_xProperty.m_pObject);
  else if (bIsOpaque())
    vTrace(" = Opaque %p"_Stz,m_xProperty.m_aOpaque);
  else if (bIsText())
    vTrace(" = Text \"%s\""_Stz,
           m_xProperty.m_pText ? m_xProperty.m_pText->szGet()
                               : ""_Stz);
  else if (bIsWord())
    vTrace(" = Word %s"_Stz,
           m_xProperty.m_pWord ? m_xProperty.m_pWord->szWordNm()
                              : ""_Stz);
  else if (bIsXref())
    vTrace(" = Xref %s"_Stz,
           m_xProperty.m_pXref ? m_xProperty.m_pXref->szNote()
                               : ""_Stz);
  else { }
  */
  
  Fact* pSubProperty = pGetSuperChild(m_pKbase->pProperty());
  if (pSubProperty) {
    if (this == pSubProperty) {
      vTrace(" = eFU %lld %s"_Stz,m_xProperty.m_eUsage,aFU[m_xProperty.m_eUsage]);
    }
    else {
      switch (pSubProperty->rxProperty().m_eUsage) {
        case FU_INT:
          vTrace(" = Int %lld"_Stz,m_xProperty.m_iInt);
          break;
        case FU_METHOD:
          vTrace(" = Method %p"_Stz,m_xProperty.m_pMethod);
          break;
        case FU_OBJECT:
          vTrace(" = Object %p"_Stz,m_xProperty.m_pObject);
          break;
        case FU_OPAQUE:
          vTrace(" = Opaque %p"_Stz,m_xProperty.m_aOpaque);
          break;
        case FU_TEXT:
          vTrace(" = Text \"%s\""_Stz,
                 m_xProperty.m_pText ? m_xProperty.m_pText->szGet()
                                     : ""_Stz);
          break;
        case FU_USAGE:
          vTrace(" = Usage ???"_Stz);
          break;
        case FU_WORD:
          vTrace(" = Word %s"_Stz,
                 m_xProperty.m_pWord ? m_xProperty.m_pWord->szWordNm()
                                    : ""_Stz);
          break;
        case FU_XREF:
          vTrace(" = Xref %s"_Stz,
                 m_xProperty.m_pXref ? m_xProperty.m_pXref->szNote()
                                     : ""_Stz);
          break;
        default:
          NEVER_REACH;
          break;
      }
    }
  }
  
  if (m_pAttributeOf || m_pElementOf || m_pSubclassOf)
    vTrace(" //"_Stz);
  if (m_pAttributeOf)
    vTrace(" [Atr:%lld]%s"_Stz,m_pAttributeOf->iFactNo(),m_pAttributeOf->szNote());
  if (m_pElementOf)
    vTrace(" [Elm:%lld]%s"_Stz,m_pElementOf->iFactNo(),m_pElementOf->szNote());
  if (m_pSubclassOf)
    vTrace(" [Sub:%lld]%s"_Stz,m_pSubclassOf->iFactNo(),m_pSubclassOf->szNote());

  switch (iAs) {
    case 0: vTrace("\n"_Stz); break;
    case 1: vTrace("\n"_Stz); break;
    case 2: vTrace("\n"_Stz); break;
    case 3: vTrace("\n"_Stz); break;
  }

  for (Fact* pAttribute = m_pAttributes;
       pAttribute;
       pAttribute = pAttribute->m_pAttributeNext)
    pAttribute->vPrint_r(iDepth + 1, iAs ? 2 : 0);

  switch (iAs) {
    case 0: break;
    case 1: break;
    case 2: break;
    case 3: break;
  }
  
  for (Fact* pElement = m_pElements;
       pElement;
       pElement = pElement->m_pElementNext)
    pElement->vPrint_r(iDepth + 1, iAs ? 3 : 0);
  
  switch (iAs) {
    case 0: break;
    case 1: break;
    case 2: break;
    case 3: break;
  }
}

/*----------------------------------------------------------------------------*/
/* get property                                                               */
/*----------------------------------------------------------------------------*/
Property& Fact::rxProperty(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  return m_xProperty;
}

/*----------------------------------------------------------------------------*/
/* get super-class fact                                                       */
/*----------------------------------------------------------------------------*/
Fact* Fact::pSubclassOf(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  return m_pSubclassOf;
}

/*----------------------------------------------------------------------------*/
/* set super-class fact                                                       */
/*----------------------------------------------------------------------------*/
void Fact::vSubclassOf(Fact* pSubclassOf)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  m_pSubclassOf = pSubclassOf;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
