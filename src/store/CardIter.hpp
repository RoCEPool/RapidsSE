/*============================================================================*/
/* CardIter.hpp --                                                            */
/*============================================================================*/
#pragma once

#include "Card.hpp"
#include "CardIterMembers.hpp"
#include "PermGen.hpp"
#include "Index.hpp"
#include "Bench.hpp"

namespace STORE
{
  class CardIter : public Object
                 , public ListExo2_t<CardIterMembers>
 {
  public:

    CardIterMembers G;

    // Xgearing
    Bench*      m_pBench;

    i8          m_iXform;
    i8          m_iTrace;

    s32         m_sXhand;
    s256        m_sXform;
    s256        m_sXdesc;

    s32         m_sSqyNm;
    s32         m_sSqyArg;
    s32         m_sSqyCol;
    s32         m_sSqyRO;
    s32         m_sSqyRsl;

    PermGen*    m_pPermGen;

    Column*     m_pColumn;
    IdxVec*     m_pIdxVec;
    Index*      m_pIndex;
    IndexDef*   m_pIndexDef;
    StreamDef*  m_pStreamDef;
    Table*      m_pTable;

    CardCtl*     m_pCardCtl;

    Card* m_pCard_child;
    Card* m_pCard_copied;
    Card* m_pCard_dup;
    Card* m_pCard_found;
    Card* m_pCard_located;
    Card* m_pCard_member;
    Card* m_pCard_new;
    Card* m_pCard_parent;
    Card* m_pCard_popped;
    Card* m_pCard_resolved;
    Card* m_pCard_temp;
    Card* m_pCard_yanked;

    Card_Term* m_pTerm_0;
    Card_Term* m_pTerm_1;
    Card_Term* m_pTerm_2;
    Card_Term* m_pTerm_corrPred;

    s256 m_sT;

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    CardIter(void);
    CardIter(eCN eClass);
    CardIter(Card* pCard);
    CardIter(CardIter* pCardIter_src);
    CardIter(eCN eClass, Card* pCard);
    CardIter(eCN eClass, Bench* pBench);
   ~CardIter(void) noexcept(false);

    void G_vAppendChild(eCK eParent, eCK eChild);
    void G_vAppendChild(eCK eParent, Card* pChild);
    void G_vAppendNewChild(Card* pParent, eCK eChild);
    void G_vAppendNewChild(Card* pParent, eCK eChild, Stz* szValue);

    void vChangeCard(Card* pCard);
    i8 i_Child(void);
    Card* pChild(void);
    void G_vClearMembers(void);
    void G_vClearRegisters(void);
    void G_vClearRegistersIf(Card* pCard);


    void vDontDescend(void);

    bool  bEachChild(void);
    Card* pEachChild(void);
    Card* pEachChild(i8& ri_Child);
    Card* pEachChild(eCK eCard);
    Card* pEachChild(eCK eCard, i8& ri_Main);
    Card* pEachChild(i8& ri_Main, eCK& reCard);
    Card* pEachChild_reverse(eCK eCard);

    void vForward(void);

    Card* pGetChild(i8 i_Child);
    Card* G_pGetMember(eCK eMember);

    bool bJustRestarted(void);

    bool bLess(void);

    Stz* szMemberNm(eCK eMember);
    bool bMore(void);

    void  G_vNewMember(eCK eMember, bool bUpdateMember= true);
    void  G_vNewMember(eCK eMember, Stz* szValue, bool bUpdateMember= true);
    Card* pNextChild(void);
    Card* pNextChildIf(eCK eCard);

    Card* pPrevChild(void);
    Card* pPrevChildIf(eCK eCard);

    Card* pRecurseEach(void);
    Card* pRecurseEach(eCK eCard);
    void vRemoveChild(Card* pChild);
    void vRemoveCurr(eCK eCrossCheck);
    void vRemoveDeleteChild(Card*& rpChild);
    void vRemoveDeleteCurr(eCK eCrossCheck);
    void vRestart(void);
    void vRestore(void);
    void vRewind(void);
    Card* pRewindChild(void);

    void vSave(void);
    i8   iSentinalsDepth(void);
    void vSet(Card* pChild);
    void vSet(CardIter* pCardIter_src);
    void vSet(i8 iChild, Card* pChild);

    bool bIsTracing(void);

    void G_vUpdateMember(eCK eMember, Card* pCard, const char* asFuncNm);

    Stz* szXform(void);
    Stz* szXhand(void);

  protected:

    bool bPopRecurseIter(void);
    void vPushRecurseIter(void);

  private:

    Footer* aFooter(void);
  };

  CardIter* _pCardIter(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
