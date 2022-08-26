/*******************************************************************************
 *   Ethereum 2 Deposit Application
 *   (c) 2020 Ledger
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 ********************************************************************************/

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "os.h"
#include "cx.h"

#include "morpho_plugin.h"

// List of selectors supported by this plugin.
static const uint32_t COMPOUND_SUPPLY_SELECTOR = 0x0c0a769b;
static const uint32_t COMPOUND_REPAY_SELECTOR = 0x1da649cf;
static const uint32_t COMPOUND_WITHDRAW_SELECTOR = 0xf3fef3a3;
static const uint32_t COMPOUND_BORROW_SELECTOR = 0x4b8a3529;
static const uint32_t COMPOUND_CLAIM_REWARDS_SELECTOR = 0x148e23ce;

static const uint32_t AAVE_SUPPLY_SELECTOR = 0x0c0a769b;
static const uint32_t AAVE_REPAY_SELECTOR = 0x1da649cf;
static const uint32_t AAVE_WITHDRAW_SELECTOR = 0xf3fef3a3;
static const uint32_t AAVE_BORROW_SELECTOR = 0x4b8a3529;

static const uint32_t COMMON_CLAIM_SELECTOR = 0x3d13f874;

// Array of all the different Morpho.sol and RewardsDistributors.sol selectors. (Compound / Aave and Common)
const uint32_t MORPHO_SELECTORS[NUM_SELECTORS] = {
    COMPOUND_SUPPLY_SELECTOR,
    COMPOUND_REPAY_SELECTOR,
    COMPOUND_WITHDRAW_SELECTOR,
    COMPOUND_BORROW_SELECTOR,
    COMPOUND_CLAIM_REWARDS_SELECTOR,
    AAVE_SUPPLY_SELECTOR,
    AAVE_REPAY_SELECTOR,
    AAVE_WITHDRAW_SELECTOR,
    AAVE_BORROW_SELECTOR,
    COMMON_CLAIM_SELECTOR,
};

// Function to dispatch calls from the ethereum app.
void dispatch_plugin_calls(int message, void *parameters)
{
    switch (message)
    {
    case ETH_PLUGIN_INIT_CONTRACT:
        PRINTF("GPIRIOU INIT CONTRACT\n");
        handle_init_contract(parameters);
        break;
    case ETH_PLUGIN_PROVIDE_PARAMETER:
        PRINTF("GPIRIOU PROVIDE PARAMETER\n");
        handle_provide_parameter(parameters);
        break;
    case ETH_PLUGIN_FINALIZE:
        PRINTF("GPIRIOU FINALIZE\n");
        handle_finalize(parameters);
        break;
    case ETH_PLUGIN_PROVIDE_INFO:
        PRINTF("GPIRIOU PROVIDE INFO\n");
        handle_provide_token(parameters);
        break;
    case ETH_PLUGIN_QUERY_CONTRACT_ID:
        PRINTF("GPIRIOU QUERY CONTRACT ID\n");
        handle_query_contract_id(parameters);
        break;
    case ETH_PLUGIN_QUERY_CONTRACT_UI:
        PRINTF("GPIRIOU QUERY CONTRACT UI\n");
        handle_query_contract_ui(parameters);
        break;
    default:
        PRINTF("Unhandled message %d\n", message);
        break;
    }
}

// Calls the ethereum app.
void call_app_ethereum()
{
    PRINTF("GPIRIOU CALL APP ETHEREUM\n");
    unsigned int libcall_params[3];
    libcall_params[0] = (unsigned int)"Ethereum";
    libcall_params[1] = 0x100;
    libcall_params[2] = RUN_APPLICATION;
    os_lib_call((unsigned int *)&libcall_params);
}

// Weird low-level black magic. No need to edit this.
__attribute__((section(".boot"))) int main(int arg0)
{
    // Exit critical section
    __asm volatile("cpsie i");

    // Ensure exception will work as planned
    os_boot();

    // Try catch block. Please read the docs for more information on how to use those!
    BEGIN_TRY
    {
        TRY
        {
            // Low-level black magic.
            check_api_level(CX_COMPAT_APILEVEL);

            // Check if we are called from the dashboard.
            if (!arg0)
            {
                // Called from dashboard, launch Ethereum app
                PRINTF("GPIRIOU ARG0\n");
                call_app_ethereum();
                return 0;
            }
            else
            {
                // Not called from dashboard: called from the ethereum app!
                const unsigned int *args = (const unsigned int *)arg0;

                // If `ETH_PLUGIN_CHECK_PRESENCE` is set, this means the caller is just trying to
                // know whether this app exists or not. We can skip `dispatch_plugin_calls`.
                PRINTF("GPIRIOU NOT ARG0\n");
                if (args[0] != ETH_PLUGIN_CHECK_PRESENCE)
                {
                    PRINTF("GPIRIOU ARG0 != ETH PLUGIN CHECK PRESENCE\n");
                    dispatch_plugin_calls(args[0], (void *)args[1]);
                }
                PRINTF("GPIRIOU OS LIB END\n");
                // Call `os_lib_end`, go back to the ethereum app.
                os_lib_end();
            }
        }
        FINALLY
        {
        }
    }
    END_TRY;

    // Will not get reached.
    return 0;
}
