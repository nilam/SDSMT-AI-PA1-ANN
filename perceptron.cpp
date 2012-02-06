/***************************************************************************//**
 * @file
 * @brief Implementation of the perceptron class.
 * @author Christopher Jensen
 * @par Modifications and Development Timeline
 * @verbatim
 * Date             Modification
 * ==============   ============================================================
 * 6 Feb. 2012      Created the initial file, constructor and destructor.
 * @endverbatim
 ******************************************************************************/
#include "perceptron.h"
#include <string>
#include <cstdlib>
#include <time>

/***************************************************************************//**
 * @author Christopher Jensen
 *
 * @par Description:
 * This is the constructor for a perceptron. It initializes all input pointers
 * to 0, and sets the weights to non-zero, random values between 0.01 and 1.00,
 * approximately.
 ******************************************************************************/
perceptron::perceptron()
{
    for(int i = 0; i < NUM_INPUT; ++i)
    {
        input[i] = NULL;
        // Force the weight to be a small, random, non-zero value.
        weight[i] =(int)( rand_r( time() ) * 100.0 / RAND_MAX ) + 1;
    }
}

perceptron :: ~perceptron()
{
}
