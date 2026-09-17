#include "standardTypes.h"
#include "crypto_hal.h"
#include "user_config.h"

#ifdef MAC_SUPPORTED

typedef enum{
    SIGN,
    VERIFY
}macParamType;

#define CRYPTO_START_SEC_VAR_INIT_UNSPECIFIED_NO_CACHEABLE
#include "Crypto_MemMap.h"

static Crypto_PrimitiveInfoType signInfo = {
    CRYPTO_MACGENERATE,                  /* service                     - Contains the enum of the used service, e.g. Encrypt */
    {
        CRYPTO_ALGOFAM_AES,              /* family                      - The family of the algorithm */
        CRYPTO_ALGOFAM_NOT_SET,          /* secondaryFamily             - The secondary family of the algorithm  */
        AES_KEY_LEN,                     /* keyLength                   - The key length in bits to be used with that algorithm */
        CRYPTO_ALGOMODE_CMAC             /* mode                        - The operation mode to be used with that algorithm */
    }
};

static Crypto_PrimitiveInfoType verifyInfo = {
    CRYPTO_MACVERIFY,                  /* service                     - Contains the enum of the used service, e.g. Encrypt */
    {
        CRYPTO_ALGOFAM_AES,              /* family                      - The family of the algorithm */
        CRYPTO_ALGOFAM_NOT_SET,          /* secondaryFamily             - The secondary family of the algorithm  */
        AES_KEY_LEN,                     /* keyLength                   - The key length in bits to be used with that algorithm */
        CRYPTO_ALGOMODE_CMAC             /* mode                        - The operation mode to be used with that algorithm */
    }
};

static Crypto_JobPrimitiveInfoType signJobPriInfo =
{
    0U,                                     /* callbackId                 - Identifier of the callback function, to be called, if the configured service finished. */
    &signInfo,                            /* primitiveInfo              - Pointer to a structure containing further configuration of the crypto primitives */
    AES_KEY,                                /* cryIfKeyId                 - Identifier of the CryIf key. */
    CRYPTO_PROCESSING_SYNC,                 /* processingType             - Determines the synchronous or asynchronous behavior. */
};

static Crypto_JobPrimitiveInfoType verifyJobPriInfo =
{
    0U,                                     /* callbackId                 - Identifier of the callback function, to be called, if the configured service finished. */
    &verifyInfo,                            /* primitiveInfo              - Pointer to a structure containing further configuration of the crypto primitives */
    AES_KEY,                                /* cryIfKeyId                 - Identifier of the CryIf key. */
    CRYPTO_PROCESSING_SYNC,                 /* processingType             - Determines the synchronous or asynchronous behavior. */
};


static Crypto_JobType signJobInfo =
{
    1U,                                     /* jobId                      - Identifier for the job structure */
    CRYPTO_JOBSTATE_IDLE,                   /* jobState                   - Determines the current job state */
    {
        NULL_PTR,                           /* inputPtr                   - Pointer to the input data. */
        0,                                  /* inputLength                - Contains the input length in bytes. */
        NULL_PTR,                           /* secondaryInputPtr          - Pointer to the secondary input data (for MacVerify, SignatureVerify). */
        0U,                                 /* secondaryInputLength       - Contains the secondary input length in bytes. */
        NULL_PTR,                           /* tertiaryInputPtr           - Pointer to the tertiary input data (for MacVerify, SignatureVerify). */
        0U,                                 /* tertiaryInputLength        - Contains the tertiary input length in bytes. */
        NULL_PTR,                           /* outputPtr                  - Pointer to the output data. */
        0U,                                 /* outputLengthPtr            - Holds a pointer to a memory location containing the output length in bytes. */
        NULL_PTR,                           /* secondaryOutputPtr         - Pointer to the secondary output data. */
        NULL_PTR,                           /* secondaryOutputLengthPtr   - Holds a pointer to a memory location containing the secondary output length in bytes. */
        NULL_PTR,                           /* verifyPtr                  - Output pointer to a memory location holding a Crypto_VerifyResultType */
        CRYPTO_OPERATIONMODE_SINGLECALL,    /* mode                       - Indicator of the mode(s)/operation(s) to be performed */
        AES_KEY,                            /* cryIfKeyId                 - Holds the CryIf key id for key operation services. */
        0U,                                 /* targetCryIfKeyId           - Holds the target CryIf key id for key operation services. */
    },
    &signJobPriInfo,                            /* jobPrimitiveInfo           - Pointer to a structure containing further information, which depends on the job and the crypto primitive */
    NULL_PTR,                               /* jobRedirectionInfoRef      - Pointer to a structure containing further information on the usage of keys as input and output for jobs. */
    AES_KEY,                                /* cryptoKeyId                - Identifier of the Crypto Driver key. The identifier shall be written by the Crypto Interface */
    0U,                                     /* targetCryptoKeyId          - Target identifier of the Crypto Driver key. The identifier shall be written by the Crypto Interface. */
    0U                                      /* jobPriority                - Specifies the importance of the job (the higher, the more important). */
};

static Crypto_JobType verifyJobInfo =
{
    1U,                                     /* jobId                      - Identifier for the job structure */
    CRYPTO_JOBSTATE_IDLE,                   /* jobState                   - Determines the current job state */
    {
        NULL_PTR,                           /* inputPtr                   - Pointer to the input data. */
        0,                                  /* inputLength                - Contains the input length in bytes. */
        NULL_PTR,                           /* secondaryInputPtr          - Pointer to the secondary input data (for MacVerify, SignatureVerify). */
        0U,                                 /* secondaryInputLength       - Contains the secondary input length in bytes. */
        NULL_PTR,                           /* tertiaryInputPtr           - Pointer to the tertiary input data (for MacVerify, SignatureVerify). */
        0U,                                 /* tertiaryInputLength        - Contains the tertiary input length in bytes. */
        NULL_PTR,                           /* outputPtr                  - Pointer to the output data. */
        0U,                                 /* outputLengthPtr            - Holds a pointer to a memory location containing the output length in bytes. */
        NULL_PTR,                           /* secondaryOutputPtr         - Pointer to the secondary output data. */
        NULL_PTR,                           /* secondaryOutputLengthPtr   - Holds a pointer to a memory location containing the secondary output length in bytes. */
        NULL_PTR,                           /* verifyPtr                  - Output pointer to a memory location holding a Crypto_VerifyResultType */
        CRYPTO_OPERATIONMODE_SINGLECALL,    /* mode                       - Indicator of the mode(s)/operation(s) to be performed */
        AES_KEY,                            /* cryIfKeyId                 - Holds the CryIf key id for key operation services. */
        0U,                                 /* targetCryIfKeyId           - Holds the target CryIf key id for key operation services. */
    },
    &verifyJobPriInfo,                      /* jobPrimitiveInfo           - Pointer to a structure containing further information, which depends on the job and the crypto primitive */
    NULL_PTR,                               /* jobRedirectionInfoRef      - Pointer to a structure containing further information on the usage of keys as input and output for jobs. */
    AES_KEY,                                /* cryptoKeyId                - Identifier of the Crypto Driver key. The identifier shall be written by the Crypto Interface */
    0U,                                     /* targetCryptoKeyId          - Target identifier of the Crypto Driver key. The identifier shall be written by the Crypto Interface. */
    0U                                      /* jobPriority                - Specifies the importance of the job (the higher, the more important). */
};

static uint32 outDataLen = AES_CMAC_OUTPUT_LEN;
static Crypto_VerifyResultType verifyResult;

#define CRYPTO_STOP_SEC_VAR_INIT_UNSPECIFIED_NO_CACHEABLE

static void CRYPTO_HAL_WriteJobInputOutput
(
    macParamType param, 
    const uint8 *pInData, 
    uint32 inDataLen, 
    uint8 *pOutData,
    const uint8 *pMacData,
    uint32 inMacDataLen
)

{
    switch(param)
    {
        case SIGN:
            signJobInfo.jobPrimitiveInputOutput.inputPtr = pInData;
            signJobInfo.jobPrimitiveInputOutput.inputLength = inDataLen;
            signJobInfo.jobPrimitiveInputOutput.outputPtr = pOutData;
            signJobInfo.jobPrimitiveInputOutput.outputLengthPtr = &outDataLen;
            break;
            
        case VERIFY:
            verifyJobInfo.jobPrimitiveInputOutput.inputPtr = pInData;
            verifyJobInfo.jobPrimitiveInputOutput.inputLength = inDataLen;
            verifyJobInfo.jobPrimitiveInputOutput.secondaryInputPtr = pMacData;
            verifyJobInfo.jobPrimitiveInputOutput.secondaryInputLength = inMacDataLen;
            verifyJobInfo.jobPrimitiveInputOutput.verifyPtr = &verifyResult;
            break;
    }
}


boolean CRYPTO_HAL_SetKey(const uint8 *pKeyStr, uint32 keyLenInBits)
{
	Crypto_43_HSE_Exts_FormatKeyCatalogs();

    if(E_NOT_OK == Crypto_43_HSE_KeyElementSet(AES_KEY, AES_KEY_ELEMENT_0, pKeyStr, keyLenInBits/8))
    {
        return FALSE;
    }

    if(E_NOT_OK == Crypto_43_HSE_KeySetValid(AES_KEY))
    {
        return FALSE;
    }

    return TRUE;
}


boolean CRYPTO_HAL_GetMAC(const uint8 *pInData, uint32 inDataLen, uint8 *pOutData)
{
    CRYPTO_HAL_WriteJobInputOutput(SIGN, pInData, inDataLen, pOutData, NULL_PTR, NULL_PTR);
    
    if(E_NOT_OK == Crypto_43_HSE_ProcessJob(SYMMETRIC_OBJECT_ID, &signJobInfo))
    {
        return FALSE;
    }

    return TRUE;
}


/*
typedef enum
{
    CRYPTO_E_VER_OK     = 0x00U,    
    CRYPTO_E_VER_NOT_OK = 0x01U    
} Crypto_VerifyResultType;
*/
boolean CRYPTO_HAL_VerifyMAC(const uint8 *pInData, uint32 inDataLen, const uint8 *pMacData, uint32 inMacDataLen)
{
    CRYPTO_HAL_WriteJobInputOutput(VERIFY, pInData, inDataLen, NULL_PTR, pMacData, inMacDataLen*8);
    
    if(E_OK != Crypto_43_HSE_ProcessJob(SYMMETRIC_OBJECT_ID, &verifyJobInfo))
    {
        return FALSE; // check CRYPTO_43_HSE_RET_VERIFY_FAILED?
    }

    return (verifyResult==CRYPTO_E_VER_OK)? TRUE: FALSE;
}

#endif