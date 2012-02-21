#include <math.h>
#include <stdio.h>

struct __PERCEP
{
    double** input;
    double output;
    double* weights;
};
typedef struct __PERCEP perceptron;

struct __NEURAL
{
    perceptron *p;
    int* counts;
    int layers;
};
typedef struct __NEURAL ann;

int get_size(ann* net)
{
    int i;
    int size = 0;
    for(i = 0; i < net->layers; ++i)
    {
        size += net->counts[i];
    }
    return size;
}

void connect(perceptron output, perceptron &input, int index);
{
    input->input[index] = &output->output;
}

ann* new_ann(int layers, int* counts)
{
    int distance = 0;
    int i;
    int j;
    ann* a;
    int size = 0;
    int qoutient = 0;

    for(i = 0; i < layers; ++i)
    {
        size += counts[i];
    }

    a = malloc(sizeof(ann));
    a->p = malloc(sizeof(perceptron) * size);
    a->counts = malloc(sizeof(int) * layers);

    for(i = 0; i < layers; ++i)
    {
        while(distance < counts[i])
        {
            /* Make the inputs the size of the previous layer. */
            a->p[distance + quotient].input = malloc(layers[i] * sizeof(double*) );
            a->p[distance + quotient].weights = malloc(layers[i] * sizeof(double));
            /* Connect each node in the previous layer to this one. */
            for(j = 0; j < layers[i]; ++j)
            {
                connect(a->p[j], a->p + distance + quotient, j);
            }
        }
        qoutient += distance;
        distance = 0;
        /* Keep track of the nodes in each layer. */
        a->counts[i] = counts[i];
    }
    a->layers = layers;
    return a;
}

void free_ann(ann* net)
{
    int i;
    int size = 0;
    /* Since we're just freeing the data, not connecting or doing other
     * important things that require us to care about how much data is actually
     * stored in the perceptron, we can just walk through the p array linearly
     * while freeing everything, then call it a night.
     */
    for(i = 0; i < net->layers; ++i)
    {
        size += net->counts[i];
    }
    for(i = net->counts[1]; i < size; ++i)
    {
        free(net->p[i].weights);
        free(net->p[i].input);
    }
    free(net->counts);
}

/***************************************************************************//**
 *
 ******************************************************************************/
void seed(ann* net, char* weights = NULL)
{
    double holder;
    int i;
    int j;
    int running_total = 0;
    int since_last_index = 0;
    srand( time() );
    /* For each layer */
    for(i = 1; i < net->layers; ++i)
    {
        /* For each perceptron in that layer */
        while(running_total < net->counts[i])
        {
            /* For each weight in the perceptron */
            for(j = 0; j < net->counts[i-1]; ++j)
            {
                if(weights == NULL)
                /* Give the perceptron a random value. */
                net->p[running_total + since_last_index].weights[j] = (double)rand() / RAND_MAX + 1;
                else
                {
                    /* Read in the next weight from the string. */
                    if( sscanf(weights, "%f", &holder))
                    {
                        net->p[running_total + since_last_index].weights[j] = holder;
                    }
                }
            }
            /* Move to the next perceptron */
            ++running_total;
        }
        /* Move to the start of the next layer */
        since_last_index += running_total;
        running_total = 0;
    }
}

char* ann_str(ann* net, char* str = NULL)
{
    int i;
    int j;
    int running_total = 0;
    int since_last_index = 0;
    int size = 0;
    double holder;
    srand( time() );
    char* out;
    /* If we need to, we can dynamically allocate a string that will fit. */
    if(str == NULL)
    {
        /* Dynamic allocation requires a fixed amount of space based on the
         * size of the network. We will do a couple of things here: First, find
         * the number of weights that will be output. This is found by taking
         * the number of inputs to a given layer and multiplying that number
         * by the number of perceptrons in the layer, since all perceptrons
         * have the same number of weights. Each layer also needs its count
         * output, so we need to know how many digits are in each layer.
         * Each weight takes up 8 characters: 6 bits of precision, one
         * decimal point, and one space to delimit them. Each layer count
         * takes up its truncated base-10 log digits, plus two spaces. There
         * are two newlines total, so the final formula looks like
         * [width of each layer size] + [number of layers] + [newline]
         * + 8 * [number of weights] + [newline] + [Null Terminator] . */
        for(i = 0; i < net->layers; ++i)
        {
            j = net->counts[i];
            size += j * 8; /* 8 * [number of weights] */
            holder = log10(j);
            /* base 10 log, +1 because log(9) = 0, + 1 for space delimiter. */
            size += (int)holder + 2; 
        }
        /* Don't forget the two newlines and the null terminator. */
        size += 3;
        /* Now that we know the number of weights, calculate the allocation. */
        out = malloc(size * sizeof(char));
    }
    else
    {
        out = str;
    }

    /* First, print the layers. */
    for(i = 0; i < net->layers; ++i)
    {
        sprintf(out, "%d", net->counts[i]);
    }

    /* First newline. */
    sprintf(out, "\n");

    /* For each layer */
    for(i = 1; i < net->layers; ++i)
    {
        /* For each perceptron in that layer */
        while(running_total < net->counts[i])
        {
            /* For each weight in the perceptron */
            for(j = 0; j < net->counts[i-1]; ++j)
            {
                /* Get the weight */
                holder = net->p[running_total + since_last_index].weights[j];
                /* Print it in the string. */
                sprintf(out, "%7f ", holder);
            }
            /* Move to the next perceptron */
            ++running_total;
        }
        /* Move to the start of the next layer */
        since_last_index += running_total;
        running_total = 0;
    }
    /* Next newline, and let the string be null-terminated. */
    sprintf(out, "\n");
    return out;
}

double train(double** input, double* output, ann* net)
{
    int holder;
    int i;
    int j;
    int running_total = 0;
    double rms;
    int since_last_index = 0;
    int size;
    double sum;

    /* First, attach the inputs to the first layer. */
    for(i = 0; i < net->counts[0]; ++i)
    {
        net->p[i].input = input;
    }

    size = get_size(net);

    /* Now, hit each perceptron in turn and update its output. */
    for(i = 1; i < net->layers; ++i)
    {
        /* For each perceptron */
        while(running_total < net->counts[i])
        {
            sum = 0;
            /* For each weight in the perceptron */
            for(j = 0; j < net->counts[i-1]; ++j)
            {
                /* Get the weight */
                holder = net->p[running_total + since_last_index].weights[j];
                /* Take the product of its weights and sum them up.*/
                sum += net->p[running_total].input[j] * holder;
            }
            /* Update the output. */
            net->p[running_total].output = 1 / (1 - exp(sum) );
            /* Move to the next perceptron */
            ++running_total;
        }
        /* Move to the start of the next layer */
        since_last_index += running_total;
        running_total = 0;
    }
    /* Normally, one would think that a parallel update will require some form 
     * of stack. Not so in this case! We can actually do a parallel update here
     * because we do not change the output values after the above loop! This 
     * allows us to ignore that the output value AS CALCULATED will change, 
     * since we are not actually doing the calculation again.
     */
}
