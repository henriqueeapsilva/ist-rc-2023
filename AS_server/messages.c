#include "messages.h"

// ----------------- Implementation of UDP messages --------------------

const char *LIN_OK() {
    return "RLI OK\n";
}

const char *LIN_NOK() {
    return "RLI NOK\n";
}

const char *LIN_REG() {
    return "RLI REG\n";
}

const char *LOU_OK() {
    return "RLO OK\n";
}

const char *LOU_NOK() {
    return "RLO NOK\n";
}

const char *LOU_UNR() {
    return "RLO UNR\n";
}

const char *UNR_OK() {
    return "RUR OK\n";
}

const char *UNR_NOK() {
    return "RUR NOK\n";
}

const char *UNR_UNR() {
    return "RUR UNR\n";
}

const char *LMA_OK(char *buffer) {
    char *result = (char *)malloc(snprintf(NULL, 0, "RMA OK %s\n", buffer) + 1);
    sprintf(result, "RMA OK %s\n", buffer);
    return result;
}

const char *LMA_NOK() {
    return "RMA NOK\n";
}

const char *LMA_NLG() {
    return "RMA NLG\n";
}

const char *LMB_OK(char *buffer) {
    char *result = (char *)malloc(snprintf(NULL, 0, "RMB OK %s\n", buffer) + 1);
    sprintf(result, "RMB OK %s\n", buffer);
    return result;
}

const char *LMB_NOK() {
    return "RMB NOK\n";
}

const char *LMB_NLG() {
    return "RMB NLG\n";
}

const char *LST_OK(char *buffer) {
    char *result = (char *)malloc(snprintf(NULL, 0, "RLS OK %s\n", buffer) + 1);
    sprintf(result, "RLS OK %s\n", buffer);
    return result;
}

const char *LST_NOK() {
    return "RLS NOK\n";
}

const char *SRC_OK(char *buffer) {
    char *result = (char *)malloc(snprintf(NULL, 0, "RRC OK %s\n", buffer) + 1);
    sprintf(result, "RRC OK %s\n", buffer);
    return result;
}

const char *SRC_NOK() {
    return "RRC NOK\n";
}

// ----------------- Implementation of TCP messages --------------------

const char *OPA_OK(char *buffer) {
    char *result = (char *)malloc(snprintf(NULL, 0, "ROA OK %s\n", buffer) + 1);
    sprintf(result, "ROA OK %s\n", buffer);
    return result;
}

const char *OPA_NOK() {
    return "ROA NOK\n";
}

const char *OPA_NLG() {
    return "ROA NLG\n";
}

const char *CLS_OK() {
    return "RCL OK\n";
}

const char *CLS_NOK() {
    return "RCL NOK\n";
}

const char *CLS_NLG() {
    return "RCL NLG\n";
}

const char *CLS_EAU() {
    return "RCL EAU\n";
}

const char *CLS_EOW() {
    return "RCL EOW\n";
}

const char *CLS_END() {
    return "RCL END\n";
}

const char *SAS_OK(char *buffer) {
    char *result = (char *)malloc(snprintf(NULL, 0, "RSA OK %s\n", buffer) + 1);
    sprintf(result, "RSA OK %s\n", buffer);
    return result;

}

const char *SAS_NOK() {
    return "RSA NOK\n";
}

const char *BID_OK() {
    return "RBD OK\n";
}

const char *BID_NOK() {
    return "RBD NOK\n";
}

const char *BID_NLG() {
    return "RBD NLG\n";
}

const char *BID_ACC() {
    return "RBD ACC\n";
}

const char *BID_REF() {
    return "RBD REF\n";
}

const char *BID_ILG() {
    return "RBD ILG\n";
}

const struct ResponseMessages Responses = {
    .LIN_OK = LIN_OK,
    .LIN_NOK = LIN_NOK,
    .LIN_REG = LIN_REG,
    .LOU_OK = LOU_OK,
    .LOU_NOK = LOU_NOK,
    .LOU_UNR = LOU_UNR,
    .UNR_OK = UNR_OK,
    .UNR_NOK = UNR_NOK,
    .UNR_UNR = UNR_UNR,
    .LMA_OK = LMA_OK,
    .LMA_NOK = LMA_NOK,
    .LMA_NLG = LMA_NLG,
    .LMB_OK = LMB_OK,
    .LMB_NOK = LMB_NOK,
    .LMB_NLG = LMB_NLG,
    .LST_OK = LST_OK,
    .LST_NOK = LST_NOK,
    .SRC_OK = SRC_OK,
    .SRC_NOK = SRC_NOK,
    .OPA_OK = OPA_OK,
    .OPA_NOK = OPA_NOK,
    .OPA_NLG = OPA_NLG,
    .CLS_OK = CLS_OK,
    .CLS_NOK = CLS_NOK,
    .CLS_NLG = CLS_NLG,
    .CLS_EAU = CLS_EAU,
    .CLS_EOW = CLS_EOW,
    .CLS_END = CLS_END,
    .SAS_OK = SAS_OK,
    .SAS_NOK = SAS_NOK,
    .BID_OK = BID_OK,
    .BID_NOK = BID_NOK,
    .BID_NLG = BID_NLG,
    .BID_ACC = BID_ACC,
    .BID_REF = BID_REF,
    .BID_ILG = BID_ILG
};