#pragma once

#include "eth_internals.h"
#include "eth_plugin_interface.h"
#include "debug.h"
#include <string.h>

// Number of decimals used when the token wasn't found in the Crypto Asset List.
#define DEFAULT_DECIMAL WEI_TO_ETHER
#define ETH_DECIMAL WEI_TO_ETHER

// Compound addresses.
extern const uint8_t CWETH_ADDRESS[ADDRESS_LENGTH];
extern const uint8_t CCOMP_ADDRESS[ADDRESS_LENGTH];
extern const uint8_t CUNI_ADDRESS[ADDRESS_LENGTH];
extern const uint8_t CUSDC_ADDRESS[ADDRESS_LENGTH];
extern const uint8_t CDAI_ADDRESS[ADDRESS_LENGTH];
extern const uint8_t CFEI_ADDRESS[ADDRESS_LENGTH];
extern const uint8_t CWBTC_ADDRESS[ADDRESS_LENGTH];
extern const uint8_t CUSDT_ADDRESS[ADDRESS_LENGTH];

// Corresponding token addresses for UI.
extern const uint8_t AWETH_ADDRESS[ADDRESS_LENGTH];
extern const uint8_t AAAVE_ADDRESS[ADDRESS_LENGTH];
extern const uint8_t AUNI_ADDRESS[ADDRESS_LENGTH];
extern const uint8_t AUSDC_ADDRESS[ADDRESS_LENGTH];
extern const uint8_t ADAI_ADDRESS[ADDRESS_LENGTH];
extern const uint8_t AFEI_ADDRESS[ADDRESS_LENGTH];
extern const uint8_t AWBTC_ADDRESS[ADDRESS_LENGTH];
extern const uint8_t AUSDT_ADDRESS[ADDRESS_LENGTH];

#define NUM_TOKENS_SUPPORTED 8

// Enumeration of the different selectors possible.
// Should follow the exact same order as the array declared in main.c
typedef enum
{
    SUPPLY,
    REPAY,
    WITHDRAW,
    BORROW,
    CLAIM_REWARDS,
} selector_t;

// Number of selectors defined in this plugin. Should match the enum `selector_t`.
#define NUM_SELECTORS 5

extern const uint32_t MORPHO_SELECTORS[NUM_SELECTORS];

// Shared global memory with Ethereum app. Must be at most 5 * 32 bytes.
// 124 / 160
typedef struct __attribute__((__packed__)) context_t
{
    uint8_t pool_token_address[ADDRESS_LENGTH];
    uint8_t token_address[ADDRESS_LENGTH];
    char token_ticker[MAX_TICKER_LEN];
    uint8_t token_decimals;
    uint8_t amount[INT256_LENGTH];
    uint8_t on_behalf[ADDRESS_LENGTH];
    uint16_t next_param;
    uint8_t trade_for_morpho;
    uint8_t user_warning;
    uint8_t token_warning;
    selector_t selectorIndex; // method id
} context_t;

typedef enum
{
    _POOL_TOKEN_ADDRESS_SUPPLY_REPAY,
    _ON_BEHALF,
    _AMOUNT_SUPPLY_REPAY,
} supply_repay_parameters;

typedef enum
{
    _POOL_TOKEN_ADDRESS_WITHDRAW_BORROW,
    _AMOUNT_WITHDRAW_BORROW,
} withdraw_borrow_parameters;

typedef enum
{
    OFFSET_C_TOKEN_ADDRESSES,
    _TRADE_FOR_MORPHO_TOKEN,
    NONE,
} claim_rewards_parameters;

// Piece of code that will check that the above structure is not bigger than 5 * 32. Do not remove
// this check.
_Static_assert(sizeof(context_t) <= 5 * 32, "Structure of parameters too big.");

void handle_provide_parameter(void *parameters);
void handle_query_contract_ui(void *parameters);
void handle_init_contract(void *parameters);
void handle_finalize(void *parameters);
void handle_provide_token(void *parameters);
void handle_query_contract_id(void *parameters);