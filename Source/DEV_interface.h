/*============================================================================*/
/* DEV_itf.h -- Interface for RLU Devices                                     */
/*============================================================================*/

i8 iDEV_MAP(void* aRLU, void* aDev, i8 eDO, void** aaCrs, void* aArg, i8 iArg);

i8 iDEV_MAP_DEL_ENT   (void* aRLU, void* aDev, void** aaCrs, void* aArg, i8 iArg);
i8 iDEV_MAP_DUMP      (void* aRLU, void* aDev, void** aaCrs, void* aArg, i8 iArg);
i8 iDEV_MAP_ENTRIES   (void* aRLU, void* aDev, void** aaCrs, void* aArg, i8 iArg);
i8 iDEV_MAP_INS_ENT   (void* aRLU, void* aDev, void** aaCrs, void* aArg, i8 iArg);
i8 iDEV_MAP_TRV_BEGIN (void* aRLU, void* aDev, void** aaCrs, void* aArg, i8 iArg);
i8 iDEV_MAP_TRV_NEXT  (void* aRLU, void* aDev, void** aaCrs, void* aArg, i8 iArg);


// prototypes
i8   iDev_ENT_COMPARE(void* pRLU, void* m_pDev);
i8   iDev_ENT_NEXT(void* pRLU, void* m_pDev);
void vDev_ENT_POSPRIOR(void* pRLU, void* m_pDev);
i8   iDev_ENT_PUSHELMI0(void* pRLU, void* m_pDev);
Stz* szDev_ENT_PUSHELMS0(void* pRLU, void* m_pDev);
i8   iDev_ENT_PUSHOFS(void* pRLU, void* m_pDev);

void vDev_ENTWRK_APPENDELM(void* pRLU, void* m_pDev);
void vDev_ENTWRK_APPENDELMI(void* pRLU, void* m_pDev, i8 iTopA);
void vDev_ENTWRK_APPENDELMS(void* pRLU, void* m_pDev, Stz* azSrc);

void vDev_IXT_DEL_ENT(void* pRLU, void* m_pDev);
void vDev_IXT_DUMP(void* pRLU, void* m_pDev);
void vDev_IXT_INS_ENT(void* pRLU, void* m_pDev);
i8   iDev_IXT_TMP_ENTRIES(void* pRLU, void* m_pDev);
i8   iDev_IXT_TRV_BEGIN(void* pRLU, void* m_pDev);
i8   iDev_IXT_TRV_NEXT(void* pRLU, void* m_pDev);

i8   iDev_PAY_FETCH_COLI(void* pRLU, void* m_pDev, i8 iArg);
Stz* szDev_PAY_FETCH_COLS(void* pRLU, void* m_pDev, i8 iArg);

void vDev_PAY_STORE_COLI(void* pRLU, void* m_pDev, i8 iArg, i8 iTopA);
void vDev_PAY_STORE_COLS(void* pRLU, void* m_pDev, i8 iArg, Stz* azDst);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
