/**
 * @brief Unit tests for the smart temperature sensor.
 */
#include <cstdint>

#include <gtest/gtest.h>

#include "arch/avr/hw_platform.h"
#include "driver/adc/atmega328p.h"
#include "driver/tempsensor/smart.h"
#include "utils/utils.h"

#ifdef TESTSUITE

//! @todo Remove this #ifdef in lecture 1 to enable these tests.
#ifdef LECTURE1

namespace driver
{
namespace
{
/**
 * @brief Smart temp sensor initialization test.
 * 
 *        Verify that:
 *            - Invalid pin numbers are not accepted and the sensor is not initialized 
 *              for out-of-range values.
 *            - Untrained linear regression models aren't accepted.
 */
TEST(TempSensor_Smart, Initialization)
{
    //! @todo Add implementation here!
}

/**
 * @brief Smart temp sensor happy path test.
 * 
 *        Verify that the temp sensor predicts accurately when the model is properly trained 
 *        and valid input is provided.
 */
TEST(TempSensor_Smart, HappyPath)
{
    //! @todo Add implementation here!
}
} // namespace
} // namespace driver.

//! @todo Remove this #endif in lecture 1 to enable these tests.
#endif /** LECTURE1 */

#endif /** TESTSUITE */
