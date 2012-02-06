/***************************************************************************//**
 * @file
 * @brief Interface for the perceptron class.
 * @details Perceptrons form the base of an ANN. These perceptrons are designed
 * so that the inputs are pointers of the same type as the outputs. Since all
 * values should be between 0 and 1, respectively, the char data type gives us a
 * perfectly reasonable storage type, represented as a fixed-point number with
 * three digits of precision. Individual perceptrons can be trained using the
 * generalized training rule. An implementation of this perceptron as a neural
 * net should have some way to handle parallelization of training, such as a
 * swappable buffer, since these facilities are not provided here.
 * @author Christopher Jensen
 * @date 6 February, 2012
 * @par Modifications and Development Timeline:
 * @verbatim
 * Date             Modification
 * ==============   ============================================================
 * 2 Feb. 2012      Created file as a c mock-up
 * 6 Feb. 2012      Changed the mock-up into a C++ class interface.
 * @endverbatim
 ******************************************************************************/
#ifndef PERCEP_H
#define PERCEP_H

#ifndef NUM_INPUT
/*! @brief The number of inputs on each perceptron.
 */
#define NUM_INPUT 21
#endif

class perceptron
{
  private:
  /*! @brief The inputs that the perceptron takes.
   */
  char* input[NUM_INPUT];
  /*! @brief The output of the perceptron. Type must match input to make sense.
   */
  int output;
  /*! @brief The weights, in fixed point, of each input.
   */
  char weight[NUM_INPUT];

  public:
  perceptron();
  ~perceptron();

  /*! @brief Connect an input pointer to another perceptron's weight */
  void connect(int index, perceptron other);

  /*! @brief Get the weights in a human-readable format */
  string getWeights(void);

  /*! 
   * @brief Seed the weights from a string, or randomize if null is passed in.
   */
  seed(string weights);

  /*! 
   * @brief Train this particular perceptron, and optionally output debug info. 
   */
  int train(char[] inputs, char[] actual, fstream &out);

  /*! @brief Train this particular perceptron. */
  int train(char[] actual);
}

#endif
