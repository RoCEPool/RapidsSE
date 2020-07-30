/*============================================================================*/
/* RLU_itf.h -- Interface for RLU                                             */
/*============================================================================*/

// IXT m_pIndexTree

void vRLU_IXT_DEL_ENT     (RLU* aRLU, void* aDev);
void vRLU_IXT_DUMP        (RLU* aRLU, void* aDev);
i8   iRLU_IXT_ENTRIES     (RLU* aRLU, void* aDev);
void vRLU_IXT_INS_ENT     (RLU* aRLU, void* aDev);
i8   iRLU_IXT_TRV_BEGIN   (RLU* aRLU, void* aDev);
i8   iRLU_IXT_TRV_NEXT    (RLU* aRLU, void* aDev);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
