#include <iostream>

#include "libsnark/gadgetlib1/gadget.hpp"
#include "libff/common/default_types/ec_pp.hpp"
#include <libff/algebra/curves/alt_bn128/alt_bn128_pp.hpp>

#include <libsnark/gadgetlib1/gadgets/hashes/sha256/sha256_gadget.hpp>
#include <libsnark/zk_proof_systems/ppzksnark/r1cs_ppzksnark/r1cs_ppzksnark.hpp>

using namespace libsnark;
using namespace libff;

typedef libff::Fr<alt_bn128_pp> FieldT;

const uint NUM_VARS = 25047;

void constraint_to_json(linear_combination<FieldT> constraints, std::stringstream &ss, int column)
{
    for (const linear_term<FieldT>& lt : constraints.terms)
    {
        uint index = 0;
        if (lt.index == 0) {
            // 0 stays 0
        }
        if (lt.index <= 512) {
            index = NUM_VARS + lt.index; // [1,..,512] -> [25048,..,25559]
        } else if (lt.index == 513) {
            index = NUM_VARS + lt.index + 1; // 513 -> 25.561
        } else {
            index = lt.index - 513; // [514, ..  25560] -> [1, 25047]
        }
        if (lt.coeff == 0) {
            continue;
        } 
        ss << index << ' ' << column << ' ' << lt.coeff << '\n';
    }
}

std::string r1cs_to_json(protoboard<FieldT> pb)
{
    r1cs_constraint_system<FieldT> constraints = pb.get_constraint_system();
    std::stringstream ss;

    for (size_t c = 0; c < constraints.num_constraints(); ++c)
    {
        constraint_to_json(constraints.constraints[c].c, ss, c + 1); // +1 is offset from python compiler (needs to be passed as an argument)
    }
    ss.rdbuf()->pubseekpos(0, std::ios_base::out);
    return ss.str();
}

std::string _sha256Constraints()
{
    libff::alt_bn128_pp::init_public_params();
    protoboard<FieldT> pb;

    digest_variable<FieldT> left(pb, SHA256_digest_size, "left");
    digest_variable<FieldT> right(pb, SHA256_digest_size, "right");
    digest_variable<FieldT> output(pb, SHA256_digest_size, "output");

    sha256_two_to_one_hash_gadget<FieldT> f(pb, left, right, output, "f");
    f.generate_r1cs_constraints();
    
    return r1cs_to_json(pb);
}

int main() 
{ 
    std::cout << _sha256Constraints();
    return 0; 
} 