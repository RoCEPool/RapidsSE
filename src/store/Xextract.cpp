/*============================================================================*/
/* Xextract.cpp --                                                       */
/*============================================================================*/

#include "Xextract.hpp"

#include "Table.hpp"
#include "PtnDef.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__aFooterFunction(Xextract)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Xextract::Xextract(eCN eClass)
: Xaccess(eClass)
{
  //_precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Xextract::~Xextract(void) noexcept(false)
{
  //_precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* get column ordinal of column ref                                           */
/*----------------------------------------------------------------------------*/
i8 Xextract::E_iColumnRef_ColumnOrd(Card_ColumnRef* p_columnRef)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  Stz* szColumnRef = p_columnRef->sz();

  char* azColumnOrd = (szColumnRef->azChar() + 3);

  i8 iColumnOrd = StrLib::iGleanInt(azColumnOrd);

  return iColumnOrd;
}

/*----------------------------------------------------------------------------*/
/* get table syn of column ref                                                */
/*----------------------------------------------------------------------------*/
char Xextract::E_cColumnRef_TableSyn(Card_ColumnRef* p_columnRef)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  Stz* szColumnRef = p_columnRef->sz();

  char cFirst  = szColumnRef->azChar()[0]; // $
  char cSecond = szColumnRef->azChar()[1]; // $
  char cThird  = szColumnRef->azChar()[2]; // c

  return cThird;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
char Xextract::E_cColumnRef_TableSyn_x(Card_ColumnRef* p_columnRef)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);
  Stz* szColumnRef = p_columnRef->sz();

  char cFirst  = szColumnRef->azChar()[0]; // $
  char cSecond = szColumnRef->azChar()[1]; // $
  char cThird  = szColumnRef->azChar()[2]; // c

  return cThird;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xextract::E_vColumnRef_TableSyn_x(Card_ColumnRef* p_columnRef, String* psTableSyn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  psTableSyn->vSprint("%c"_Stz, E_cColumnRef_TableSyn_x(p_columnRef));
}



/*----------------------------------------------------------------------------*/
/* get rank                                                                   */
/*----------------------------------------------------------------------------*/
i8 Xextract::E_iCandIndex_Rank(Card_CandIndex* p_candIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  i8 iRank = p_candIndex->Y_iVal(P_Rank);

  return iRank;
}

/*----------------------------------------------------------------------------*/
/* set rank                                                                   */
/*----------------------------------------------------------------------------*/
void Xextract::E_vCandIndex_Rank(Card_CandIndex* p_candIndex, i8 iRank)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  p_candIndex->Y_vSet(P_Rank, iRank);
}

/*----------------------------------------------------------------------------*/
/* get stream ordinal low                                                     */
/*----------------------------------------------------------------------------*/
i8 Xextract::E_iStream_OrdLo(Card_Stream* p_stream)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  i8 iOrdLo = p_stream->Y_iVal(P_OrdLo);

  return iOrdLo;
}

/*----------------------------------------------------------------------------*/
/* get stream ordinal high                                                    */
/*----------------------------------------------------------------------------*/
i8 Xextract::E_iStream_OrdHi(Card_Stream* p_stream)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  i8 iOrdHi = p_stream->Y_iVal(P_OrdHi);

  return iOrdHi;
}

/*----------------------------------------------------------------------------*/
/* get stream ordinal high                                                    */
/*----------------------------------------------------------------------------*/
Card_SubSet* Xextract::E_pSubset_ChildrenBetween(Card* pCard, i8 i_From, i8 i_Till)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  Card_SubSet* p_subSet = pCard->pDupSubCards(i_From,i_Till);

  return p_subSet;
}

/*----------------------------------------------------------------------------*/
/* evaluate bound                                                             */
/*----------------------------------------------------------------------------*/
ePB Xextract::E_eTerm_Bound(Card_Term* p_term)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  eRO eRelOp = this->E_eTerm_RelOp(p_term);

  switch (eRelOp) {
    case RO_LESS_THAN:     return PB_UBOUND;
    case RO_LESS_EQUAL:    return PB_UBOUND;
    case RO_EQUAL_TO:      return PB_EBOUND;
    case RO_GREATER_EQUAL: return PB_LBOUND;
    case RO_GREATER_THAN:  return PB_LBOUND;

    default:
      vTrace("%s(%lld): unexpected E_eTerm_RelOp %lld %s\n"_Stz
          ,__func__
          ,(i8)__LINE__
          ,(i8)eRelOp
          ,aRO[eRelOp]);
      RASSERT(0);
      break;
  }
}

/*----------------------------------------------------------------------------*/
/* get ordinal index value  (i.e. $In)                                        */
/*----------------------------------------------------------------------------*/
i8 Xextract::E_iTerm_IndexColNo(Card_Term* p_term)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  i8 i_Ord = StrLib::iGleanInt(p_term->szValue()->azChar());

  return i_Ord;
}

/*----------------------------------------------------------------------------*/
/* evaluate relational op                                                     */
/*----------------------------------------------------------------------------*/
eRO Xextract::E_eTerm_RelOp(Card_Term* p_term)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  Stz* szTerm = p_term->sz();

  if (szTerm->bSame("<"_Stz))   return RO_LESS_THAN;
  else if (szTerm->bSame("<="_Stz))  return RO_LESS_EQUAL;
  else if (szTerm->bSame("="_Stz))   return RO_EQUAL_TO;
  else if (szTerm->bSame("=="_Stz))  return RO_EQUAL_TO;
  else if (szTerm->bSame("!="_Stz))  return RO_NOT_EQUAL;
  else if (szTerm->bSame(">="_Stz))  return RO_GREATER_EQUAL;
  else if (szTerm->bSame(">"_Stz))   return RO_GREATER_THAN;

  else return RO_NONE;
}

/*----------------------------------------------------------------------------*/
/* get stream column ordinal n  (i.e. $n)                                     */
/*----------------------------------------------------------------------------*/
i8 Xextract::E_iTerm_StreamColOdn(Card_Term* p_term)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  i8 i_Col = StrLib::iGleanInt(p_term->szValue()->azChar());

  return i_Col;
}

/*----------------------------------------------------------------------------*/
/* get table column number n (i.e. $$c.n)                                     */
/*----------------------------------------------------------------------------*/
i8 Xextract::E_iTerm_TableColNo(Card_Term* p_term)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  i8 i_Col = StrLib::iGleanInt(p_term->szValue()->azChar());

  return i_Col;
}

/*----------------------------------------------------------------------------*/
/* get table synonym c (i.e. $$c.n)                                           */
/*----------------------------------------------------------------------------*/
char Xextract::E_cTerm_TableSyn(Card_Term* p_term)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  Stz* szTerm = p_term->sz();

  char cSyn = szTerm->azChar()[2];

  return cSyn;
}

/*----------------------------------------------------------------------------*/
/* evaluate term                                                              */
/*----------------------------------------------------------------------------*/
eTE Xextract::E_eTerm_TermEval(Card_Term* p_term)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  Stz* szTerm = p_term->sz();

       if (szTerm->bSame("and"_Stz))     return TE_AND;
  else if (szTerm->bSame("}"_Stz))       return TE_EXECUTE;
  else if (szTerm->bSame("exists"_Stz))  return TE_EXISTS;
  else if (szTerm->bSame("in"_Stz))      return TE_IN;
  else if (szTerm->bSame("or"_Stz))      return TE_OR;

  else if (szTerm->bSame("<"_Stz))       return TE_COMPARE;
  else if (szTerm->bSame("<="_Stz))      return TE_COMPARE;
  else if (szTerm->bSame("="_Stz))       return TE_COMPARE;
  else if (szTerm->bSame("=="_Stz))      return TE_COMPARE;
  else if (szTerm->bSame("!="_Stz))      return TE_COMPARE;
  else if (szTerm->bSame("<>"_Stz))      return TE_COMPARE;
  else if (szTerm->bSame(">="_Stz))      return TE_COMPARE;
  else if (szTerm->bSame(">"_Stz))       return TE_COMPARE;

  else return TE_OTHER;
}

/*----------------------------------------------------------------------------*/
/* get stream column ordinal n  (i.e. $n)                                     */
/*----------------------------------------------------------------------------*/
i8 Xextract::E_iValue_StreamColOdn(Stz* szValue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  i8 i_Col = StrLib::iGleanInt(szValue->azChar());

  return i_Col;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
