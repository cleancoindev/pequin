
#include <iostream>
#include <fstream>
#include <string>
#include "libff/algebra/curves/alt_bn128/alt_bn128_pp.hpp"

constexpr auto v_dir = "./verification_material/";
constexpr auto p_dir = "./proving_material/";
constexpr auto shared_dir = "./prover_verifier_shared/";

typedef libff::Fr<libff::alt_bn128_pp> FieldT;

#include "libff/algebra/curves/alt_bn128/alt_bn128_pp.hpp"
std::string HexStringFromLibsnarkBigint(libff::bigint<libff::alt_bn128_r_limbs> _x){
    uint8_t x[32];
    for (unsigned i = 0; i < 4; i++)
        for (unsigned j = 0; j < 8; j++)
                        x[i * 8 + j] = uint8_t(uint64_t(_x.data[3 - i]) >> (8 * (7 - j)));

        std::stringstream ss;
        ss << std::setfill('0');
        for (unsigned i = 0; i<32; i++) {
                ss << std::hex << std::setw(2) << (int)x[i];
        }

                std::string str = ss.str();
                return str.erase(0, std::min(str.find_first_not_of('0'), str.size()-1));
}

std::string outputPointG1AffineAsHex(libff::alt_bn128_G1 _p)
{
        libff::alt_bn128_G1 aff = _p;
        aff.to_affine_coordinates();
        return
                "0x" +
                HexStringFromLibsnarkBigint(aff.X.as_bigint()) +
                ", 0x" +
                HexStringFromLibsnarkBigint(aff.Y.as_bigint());
}

std::string outputPointG2AffineAsHex(libff::alt_bn128_G2 _p)
{
        libff::alt_bn128_G2 aff = _p;
        aff.to_affine_coordinates();
        return
                "[0x" +
                HexStringFromLibsnarkBigint(aff.X.c1.as_bigint()) + ", 0x" +
                HexStringFromLibsnarkBigint(aff.X.c0.as_bigint()) + "], [0x" +
                HexStringFromLibsnarkBigint(aff.Y.c1.as_bigint()) + ", 0x" +
                HexStringFromLibsnarkBigint(aff.Y.c0.as_bigint()) + "]";
}

struct comp_params {
    int n_constraints;
    int n_inputs;
    int n_outputs;
    int n_vars;
};

comp_params parse_params(string paramFilename) {
    std::ifstream paramFile(paramFilename);
    if (!paramFile.is_open()) {
        std::cerr << "ERROR: " << paramFilename << " not found. (Try running `make ` " << std::string(NAME) << ".params)" << std::endl;
        exit(1);
    }
    int num_constraints, num_inputs, num_outputs, num_vars;
    std::string comment;
    paramFile >> num_constraints >> comment >> num_inputs >> comment >> num_outputs >> comment >> num_vars;
    paramFile.close();

    return comp_params{num_constraints, num_inputs, num_outputs, num_vars};
}
