#ifndef RESPONSE_MESSAGES_H
#define RESPONSE_MESSAGES_H

// Define a structure to hold function pointers
struct ResponseMessages {
    const char *(*RLI_OK)();
    const char *(*RLI_NOK)();
    const char *(*RLI_REG)();
    
    const char *(*RLO_OK)();
    const char *(*RLO_NOK)();
    const char *(*RLO_UNR)();

    const char *(*RUR_OK)();
    const char *(*RUR_NOK)();
    const char *(*RUR_UNR)();

    const char *(*RMA_NOK)();
    const char *(*RMA_NLG)();

    const char *(*RMB_NOK)();
    const char *(*RMB_NLG)();

    const char *(*RLS_NOK)();

    const char *(*RRC_NOK)();

    const char *(*ROA_NOK)();
    const char *(*ROA_OK)(char *AID);
    const char *(*ROA_NLG)();

    const char *(*RCL_OK)();
    const char *(*RCL_NOK)(int UID);
    const char *(*RCL_NLG)();
    const char *(*RCL_EAU)(char *AID);
    const char *(*RCL_EOW)(int UID);
    const char *(*RCL_END)(char *AID, int UID);

    const char *(*RSA_NOK)();
    const char *(*RSA_OK)(int fsize, char *filename);

    const char *(*RBD_OK)();
    const char *(*RBD_NOK)(char *AID);
    const char *(*RBD_NLG)();
    const char *(*RBD_ACC)(char *AID);
    const char *(*RBD_REF)();
    const char *(*RBD_ILG)();
};

extern const struct ResponseMessages Responses;

#endif