#include <stdint.h>

#define INPUT_SIZE 512
#define OUTPUT_SIZE 256

struct In { uint32_t preimage[INPUT_SIZE]; };
struct Out { uint32_t hash[OUTPUT_SIZE]; };

struct Hash { 
    uint32_t values[OUTPUT_SIZE];
};

void compute(struct In *input, struct Out *output){
    uint32_t *gadget_inputs[1] = {input->preimage};
    uint32_t lens[1] = {INPUT_SIZE};

    struct Hash hash[1];

    ext_gadget(gadget_inputs,lens,hash,0);

    output->hash[0] = hash[0].values[0];
}
