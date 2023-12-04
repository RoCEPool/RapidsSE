/*============================================================================*/
/* ShortForm.cpp -- Deck Short Form                                           */
/*============================================================================*/

#include "ShortForm.hpp"

#include "CardIter.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, ShortForm, CN_SHORTFORM)
__aNewFunction(ShortForm, CN_SHORTFORM)
__vDeleteFunction(ShortForm, CN_SHORTFORM)
__aFooterFunction(ShortForm)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ShortForm::ShortForm(CardIter* pCI)
    : Object(CN_SHORTFORM)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_SHORTFORM, _FOOTER_);

  m_iPrint = pCI->m_pBench->iPrint();
  m_i_Stage_curr = C_UNKNOWN;

  m_nOps = 0;
  for (i8 i_Op = 0; i_Op < C_SHORTFORM; i_Op++) {
    m_axOp[m_nOps].m_i_Stage = C_UNKNOWN;
    m_axOp[m_nOps].m_i_Op    = C_UNKNOWN;
  }

  pCI->vBeginEach();
  while (pCI->G_bEachMain_NONE())
    vAdd(pCI);


  _postcondition_(_ADDRESS_, _MAGIC_, CN_SHORTFORM, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ShortForm::~ShortForm(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_SHORTFORM, _FOOTER_);

}


/*----------------------------------------------------------------------------*/
/*-- static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add operator of pCI                                                        */
/*----------------------------------------------------------------------------*/
void ShortForm::vAdd(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_SHORTFORM, _FOOTER_);

  if (pCI->G.p_MainCard->bIsCard(CK_c_STAGE)) {
    m_i_Stage_curr++;
    m_sStage_curr = pCI->G.p_MainCard->szFacet(SF_NONE);
  }

  // - - - - - - - - - - - - - - - - - - - - - -

  m_axOp[m_nOps].m_i_Stage = m_i_Stage_curr;
  m_axOp[m_nOps].m_i_Op    = pCI->G.i_MainCard;
  m_axOp[m_nOps].m_sStage  = m_sStage_curr;

  if (pCI->G.p_MainCard->bIsCard(CK_m_PROJECT) &&
      pCI->G.p_MainCard->pSymbol()->Y_bHas(P_Early))
    m_axOp[m_nOps].m_sOp     = "early"_Stz;
  else
    m_axOp[m_nOps].m_sOp     = pCI->G.p_MainCard->szCardNm();

  if ((pCI->G.p_table = pCI->G.p_MainCard->pSub_table()))
    pCI->G.p_table->vShortForm(m_axOp[m_nOps].m_sMeta, m_axOp[m_nOps].m_sBounds);

  if ((pCI->G.p_injectable = pCI->G.p_MainCard->pSub_injectable()))
    pCI->G.p_injectable->vShortForm(m_axOp[m_nOps].m_sInjected);
  if ((pCI->G.p_emit = pCI->G.p_MainCard->pSub_emit()))
    pCI->G.p_emit->vShortForm(m_axOp[m_nOps].m_sEmitted);
  if ((pCI->G.p_key = pCI->G.p_MainCard->pSub_key()))
    pCI->G.p_key->vShortForm(m_axOp[m_nOps].m_sKey);

  if (pCI->G.p_MainCard->bIsCard(CK_m_FILTER))
    pCI->G.p_filter->vShortForm(m_axOp[m_nOps].m_sFilter);
  else
  if ((pCI->G.p_filter = pCI->G.p_MainCard->pSub_filter()))
    pCI->G.p_filter->vShortForm(m_axOp[m_nOps].m_sFilter);

  m_nOps++;

  vValidate();
}

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void ShortForm::vDump(i8 i_Tab, bool bTrim)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_SHORTFORM, _FOOTER_);
  UNUSED(i_Tab);

  i8 nMax_b = 0;
  i8 nMax_f = 0;
  i8 nMax_i = 0;
  i8 nMax_k = 0;
  i8 nMax_m = 0;
  i8 nMax_o = 0;
  i8 nMax_p = 0;
  i8 nMax_s = 0;

  for (i8 i_Op = 0; i_Op < m_nOps; i_Op++) {

    i8 nLen_b = m_axOp[i_Op].m_sBounds.nLength();
    i8 nLen_f = m_axOp[i_Op].m_sFilter.nLength();
    i8 nLen_i = m_axOp[i_Op].m_sInjected.nLength();
    i8 nLen_k = m_axOp[i_Op].m_sKey.nLength();
    i8 nLen_m = m_axOp[i_Op].m_sMeta.nLength();
    i8 nLen_o = m_axOp[i_Op].m_sOp.nLength();
    i8 nLen_p = m_axOp[i_Op].m_sEmitted.nLength();
    i8 nLen_s = m_axOp[i_Op].m_sStage.nLength();

    if (nLen_b > nMax_b)  nMax_b = nLen_b;
    if (nLen_f > nMax_f)  nMax_f = nLen_f;
    if (nLen_i > nMax_i)  nMax_i = nLen_i;
    if (nLen_k > nMax_k)  nMax_k = nLen_k;
    if (nLen_m > nMax_m)  nMax_m = nLen_m;
    if (nLen_o > nMax_o)  nMax_o = nLen_o;
    if (nLen_p > nMax_p)  nMax_p = nLen_p;
    if (nLen_s > nMax_s)  nMax_s = nLen_s;
  }

  s256 sFormat;
  //                             s                  o            m          b            i          p          k          f
  sFormat.vSprint("%%s%%s `%%-%llds` `%%2lld] %%-%llds` : `%%-%llds` `%%-%llds` : `%%-%llds` `%%-%llds` `%%-%llds` `%%-%llds`\n"_Stz,
                  nMax_s,
                  nMax_o,
                  nMax_m,
                  nMax_b,
                  nMax_i,
                  nMax_p,
                  nMax_k,
                  nMax_f);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  vTrace("%sExecution ShortForm %lld::\n"_Stz,
         Stz::szTab(i_Tab),
         m_iPrint);

  vDebugPoint();

  for (i8 i_Op = 0; i_Op < m_nOps; i_Op++) {

    if (bTrim) {
      i8 nLen = m_axOp[i_Op].m_sMeta.nLength()
              + m_axOp[i_Op].m_sBounds.nLength()
              + m_axOp[i_Op].m_sInjected.nLength()
              + m_axOp[i_Op].m_sEmitted.nLength()
              + m_axOp[i_Op].m_sKey.nLength()
              + m_axOp[i_Op].m_sFilter.nLength()
              ;
      if (! nLen)
        continue;
    }

    vTrace(sFormat,
           Stz::szTab(i_Tab),
           Stz::szSpaces(2),

           m_axOp[i_Op].m_sStage.azChar(),
           m_axOp[i_Op].m_i_Op,
           m_axOp[i_Op].m_sOp.azChar(),

           m_axOp[i_Op].m_sMeta.azChar(),
           m_axOp[i_Op].m_sBounds.azChar(),

           m_axOp[i_Op].m_sInjected.azChar(),
           m_axOp[i_Op].m_sEmitted.azChar(),
           m_axOp[i_Op].m_sKey.azChar(),
           m_axOp[i_Op].m_sFilter.azChar()
    );
  }
  vDebugPoint();
}

/*----------------------------------------------------------------------------*/
/* validate                                                                   */
/*----------------------------------------------------------------------------*/
void ShortForm::vValidate(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_SHORTFORM, _FOOTER_);

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
