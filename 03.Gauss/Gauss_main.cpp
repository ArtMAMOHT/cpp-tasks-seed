#include <exception>
#include <iostream>

#include "Gauss_solve.h"
#include "util.h"

int main(int argc, const char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Use: gauss input.csv\n";
        return 1;
    }

    try
    {
        GaussMatrix ab = load_csv_to_matrix(argv[1]);
        const GaussVector x = Gauss_solve(ab);
        print_vector_as_csv(std::cout, x);
    }
    catch (const std::exception& err)
    {
        std::cerr << err.what() << '\n';
        return 1;
    }

    return 0;
}
