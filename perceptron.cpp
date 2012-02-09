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

/***************************************************************************//**
 * @author Christopher Jensen
 *
 * @par Description:
 * Connects a perceptron's input slot associated with index. This will be used
 * as a new input to the current perceptron.
 ******************************************************************************/
void perceptron::connect(int index, perceptron other)
{
    // Do some basic validation on the input, to prevent segfaults.
    if(index < 0 || index > NUM_INPUT || other == NULL) return;
    // Attach the input at index to the address of the other perceptron's output
    input[index] = &other.output;
}

string getWeights(char delimiter)
{
    string out;
    for(int i = 0; i < NUM_INPUT; ++i)
    {
        out += (int)input[i];
        // Since we want to use this for file i/o, delimit the weights.
        out += delimiter;
    }
    return out;
}
