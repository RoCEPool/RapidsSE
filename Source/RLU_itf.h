/*============================================================================*/
/* RLU_itf.h -- Interface for RLU                                             */
/*============================================================================*/

i8 iDEV_MAP(void* aRLU, void* aDev, i8 eDO, void** aaCrs, void* aArg, i8 iArg);

i8 iDEV_MAP_DEL_ENT   (void* aRLU, void* aDev, void** aaCrs, void* aArg, i8 iArg);
i8 iDEV_MAP_DUMP      (void* aRLU, void* aDev, void** aaCrs, void* aArg, i8 iArg);
i8 iDEV_MAP_ENTRIES   (void* aRLU, void* aDev, void** aaCrs, void* aArg, i8 iArg);
i8 iDEV_MAP_INS_ENT   (void* aRLU, void* aDev, void** aaCrs, void* aArg, i8 iArg);
i8 iDEV_MAP_TRV_BEGIN (void* aRLU, void* aDev, void** aaCrs, void* aArg, i8 iArg);
i8 iDEV_MAP_TRV_NEXT  (void* aRLU, void* aDev, void** aaCrs, void* aArg, i8 iArg);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
