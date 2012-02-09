/***************************************************************************//**
 * @file
 * @brief Function prototypes associated with the Artificial Neural Net class.
 * @details The Nerual Network is the primary objective of this assignment.
 * This particular implementation uses a dynamically-expanded 2-D array of
 * perceptrons, with the first dimension indicating layer and the second index
 * indicating individual perceptrons.
 * Updates to this network are handled in parallel; all changes are pushed onto
 * a stack in the first pass through the perceptrons in the net, then popped off
 * on the return trip.
 * @author Christopher Jensen
 * @date 6 February, 2012
 * @par Modifications and Development Timeline:
 * @verbatim
 * Date             Modification
 * ==============   ============================================================
 * 9 Feb. 2012      Created interface and basic documentation.
 ******************************************************************************/
#ifndef NEURAL_NET_H
#define NEURAL_NET_H
#include "perceptron.h"
#include <stack>
#include <vector>
using namespace std;

class ann{
    private:
        /*!
         * @brief A stack that is used for parallel network update */
        stack<char> update_stack;
        /*!
         * @brief The vector of vectors underlying the network */
        vector< vector<perceptron> > network;
    public:
        /*!
         * @brief
         * Creates a new neural network. The first argument is the number of
         * hidden layers to include in the network and does not include the
         * output layer. The second argument is an array of integers which 
         * describes how many perceptrons should be in the hidden layers. This
         * should be an array with size equal to the number specified in the
         * first argument, plus one argument which should be 0, indicating an
         * end to the array. The third argument is an array of pointers to
         * the same data type used in the network, and should also be
         * null-terminated.
         */
        ann(int layers, int[] perceptrons, char** inputs);
        /*!
         * @brief
         * Destructor for the neural network. Frees any memory allocated by the
         * class. Since the STL is used in this implementation, it technically
         * shouldn't have to do anything.
         */
        ~ann(void);
        /*!
         * @brief
         * Gets the weights, as a string which can be written directly to a
         * file. The string is formatted hierarchically; individual weights are
         * space-separated, perceptrons are comma-separated, and layers of
         * perceptrons are newline-separated.
         */
        string get_weights(void);
        /*!
         * @brief
         * Takes in a sequence of weights, with layers indicated by newline
         * characters, perceptrons separated by commas, and individual weights
         * separated by whitespace. Same format as output by get_weights.
         */
        void set_weights(string weights);
        /*!
         * @brief
         * Ask the network to predict outputs based on its current weights.
         */
        vector<char> predict(vector<char> inputs);
        /*!
         * @brief
         * Train the weights of the network. This is a guided training, so
         * the expected outputs must be provided.
         */
        void train(vector<char> inputs, vector<char> outputs);
};
#endif
