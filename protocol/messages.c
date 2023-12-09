#include "messages.h"
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

// ----------------- UDP messages --------------------
static const char *RLI_OK() {
    return "successful login\n";
}

static const char *RLI_NOK() {
    return "incorrect login attempt\n";
}

static const char *RLI_REG() {
    return "new user registered\n";
}

static const char *RLO_OK() {
    return "successful logout\n";
}

static const char *RLO_NOK() {
    return "user not logged in\n";
}

static const char *RLO_UNR() {
    return "unknown user\n";
}

static const char *RUR_OK() {
    return "successful unregister\n";
}

static const char *RUR_NOK() {
    return " incorrect unregister attempt\n";
}

static const char *RUR_UNR() {
    return "unknown user\n";
}

static const char *RMA_NOK() {
    return "user is not involved in any auctions\n";
}

static const char *RMA_NLG() {
    return "user not logged in\n";
}

static const char *RMB_NOK() {
    return "user has not placed any bids\n";
}

static const char *RMB_NLG() {
    return "user not logged in\n";
}

static const char *RLS_NOK() {
    return "no auctions were yet created\n";
}

static const char *RRC_NOK() {
    return "auction AID does not exist\n";
}

// ---------------------------------------------------

// ----------------- TCP messages --------------------
static const char *ROA_OK(char *AID) {
    char *result = (char *)malloc(snprintf(NULL, 0, "auction %s successfully opened\n", AID) + 1);
    sprintf(result, "auction %s successfully opened\n", AID);
    return result;
}

static const char *ROA_NOK() {
    return "auction could not be started\n";
}

static const char *ROA_NLG() {
    return "user not logged in\n";
}

static const char *RCL_OK() {
    return "auction successfully closed\n";
}

static const char *RCL_NOK(int UID) {
    char *result = (char *)malloc(snprintf(NULL, 0, "user %d does not exist\n", UID) + 1);
    sprintf(result, "user %d does not exist\n", UID);
    return result;
}

static const char *RCL_NLG() {
    return "user not logged in\n";
}

static const char *RCL_EAU(int AID) {
    char *result = (char *)malloc(snprintf(NULL, 0, "auction %d does not exist\n", AID) + 1);
    sprintf(result, "auction %d does not exist\n", AID);
    return result;
}

static const char *RCL_EOW(int UID) {
    char *result = (char *)malloc(snprintf(NULL, 0, "auction is not owned by user %d\n", UID) + 1);
    sprintf(result, "auction is not owned by user %d\n", UID);
    return result;
}

static const char *RCL_END(int AID, int UID) {
    char *result = (char *)malloc(snprintf(NULL, 0, "auction %d owned by user %d has already finished.\n", AID, UID) + 1);
    sprintf(result, "auction %d owned by user %d has already finished.\n", AID, UID);
    return result;
}

static const char *RSA_NOK() {
    return "no file to be sent\n";
}

static const char *RSA_OK(int fsize, char *filename) {
    char *result = (char *)malloc(snprintf(NULL, 0, "filename: %s fsize: %d\n", filename, fsize) + 1);
    sprintf(result, "filename: %s fsize: %d\n", filename, fsize);
    return result;
}

static const char *RBD_NOK(int AID) {
    char *result = (char *)malloc(snprintf(NULL, 0, "auction %d is not active\n", AID) + 1);
    sprintf(result, "auction %d is not active\n", AID);
    return result;
}

static const char *RBD_NLG() {
    return "user not logged in\n";
}

static const char *RBD_ACC(int AID) {
    char *result = (char *)malloc(snprintf(NULL, 0, "auction %d bid accepted\n", AID) + 1);
    sprintf(result, "auction %d bid accepted\n", AID);
    return result;
}

static const char *RBD_REF() {
    return "bid was refused - larger bid has already been placed previously\n";
}

static const char *RBD_ILG() {
    return "cannot bid on own auction\n";
}


// ---------------------------------------------------

// Create an instance of the structure
const struct ResponseMessages Responses = {
    .RLI_OK = RLI_OK,
    .RLI_NOK = RLI_NOK,
    .RLI_REG = RLI_REG,
    .RLO_OK = RLO_OK,
    .RLO_NOK = RLO_NOK,
    .RLO_UNR = RLO_UNR,
    .RUR_OK = RUR_OK,
    .RUR_NOK = RUR_NOK,
    .RUR_UNR = RUR_UNR,
    .RMA_NOK = RMA_NOK,
    .RMA_NLG = RMA_NLG,
    .RMB_NOK = RMB_NOK,
    .RMB_NLG = RMB_NLG,
    .RLS_NOK = RLS_NOK,
    .RRC_NOK = RRC_NOK,
    .ROA_NOK = ROA_NOK,
    .ROA_OK = ROA_OK,
    .ROA_NLG = ROA_NLG,
    .RCL_OK = RCL_OK,
    .RCL_NOK = RCL_NOK,
    .RCL_NLG = RCL_NLG,
    .RCL_EAU = RCL_EAU,
    .RCL_EOW = RCL_EOW,
    .RCL_END = RCL_END,
    .RSA_NOK = RSA_NOK,
    .RSA_OK = RSA_OK,
    .RBD_NOK = RBD_NOK,
    .RBD_NLG = RBD_NLG,
    .RBD_ACC = RBD_ACC,
    .RBD_REF = RBD_REF,
    .RBD_ILG = RBD_ILG
};