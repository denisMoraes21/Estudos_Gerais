#include "logger.h"
#include "phy_connection.h"
#include "unity.h"

#include "lwip.h"

#include <stdarg.h>
#include <string.h>


struct netif gnetif;

static uint32_t fake_tick;
static unsigned int delay_calls;
static unsigned int info_calls;
static unsigned int warn_calls;
static bool connect_during_first_delay;


uint32_t HAL_GetTick(void) { return fake_tick; }


void osDelay(uint32_t milliseconds) {
  ++delay_calls;
  fake_tick += milliseconds;

  if (connect_during_first_delay && delay_calls == 1U) {
    gnetif.is_up = true;
    gnetif.is_link_up = true;
  }
}


void log_write(log_level_t level, const char *format, ...) {
  (void)format;
  if (level == LOG_LEVEL_INFO) {
    ++info_calls;
  } else if (level == LOG_LEVEL_WARN) {
    ++warn_calls;
  }
}

void setUp(void) {
  memset(&gnetif, 0, sizeof(gnetif));
  fake_tick = 0U;
  delay_calls = 0U;
  info_calls = 0U;
  warn_calls = 0U;
  connect_during_first_delay = false;
}


void tearDown(void) {}


void test_wait_phy_conection_returns_true_immediately_when_phy_is_ready(void) {
  gnetif.is_up = true;
  gnetif.is_link_up = true;

  TEST_ASSERT_TRUE(wait_phy_conection());
  TEST_ASSERT_EQUAL_UINT(0U, delay_calls);
  TEST_ASSERT_EQUAL_UINT(0U, info_calls);
  TEST_ASSERT_EQUAL_UINT(0U, warn_calls);
}

void test_wait_phy_conection_waits_until_interface_and_link_are_ready(void) {
  connect_during_first_delay = true;

  TEST_ASSERT_TRUE(wait_phy_conection());
  TEST_ASSERT_EQUAL_UINT(1U, delay_calls);
  TEST_ASSERT_EQUAL_UINT(1U, info_calls);
  TEST_ASSERT_EQUAL_UINT(0U, warn_calls);
}

void test_wait_phy_conection_returns_false_after_timeout(void) {
  TEST_ASSERT_FALSE(wait_phy_conection());
  TEST_ASSERT_EQUAL_UINT(5U, delay_calls);
  TEST_ASSERT_EQUAL_UINT(5U, info_calls);
  TEST_ASSERT_EQUAL_UINT(1U, warn_calls);
}

void test_wait_phy_conection_handles_tick_counter_wraparound(void) {
  fake_tick = UINT32_MAX - 500U;
  connect_during_first_delay = true;

  TEST_ASSERT_TRUE(wait_phy_conection());
  TEST_ASSERT_EQUAL_UINT(1U, delay_calls);
  TEST_ASSERT_EQUAL_UINT(0U, warn_calls);
}
