#include "util.h"

#include <exception>
#include <iomanip>
#include <ostream>
#include <stdexcept>
#include <string>
#include <vector>

#include <lazycsv.hpp>

namespace
{
bool parse_double(const std::string &text, double& value)
{
    std::size_t pos = 0;
    try
    {
        value = std::stod(text, &pos);
    }
    catch (const std::exception&)
    {
        return false;
    }
    return pos == text.size();
}
} // namespace

GaussMatrix load_csv_to_matrix(const char *filename)
{
    std::vector<std::vector<double>> rows;
    lazycsv::parser parser{filename};

    for (const auto row : parser)
    {
        std::vector<double> values;
        bool numeric_row = true;
        for (const auto cell : row)
        {
            double value = 0.0;
            if (!parse_double(std::string(cell.raw()), value))
            {
                numeric_row = false;
                break;
            }
            values.push_back(value);
        }

        if (!numeric_row && rows.empty())
        {
            continue;
        }
        if (!numeric_row)
        {
            throw std::runtime_error("invalid numeric cell");
        }
        if (!values.empty())
        {
            rows.push_back(values);
        }
    }

    if (rows.empty())
    {
        throw std::runtime_error("empty csv");
    }

    const std::size_t cols = rows.front().size();
    for (const auto &row : rows)
    {
        if (row.size() != cols)
        {
            throw std::runtime_error("ragged csv");
        }
    }

    GaussMatrix matrix(static_cast<int>(rows.size()), static_cast<int>(cols));
    for (int i = 0; i < matrix.rows(); ++i)
    {
        for (int j = 0; j < matrix.cols(); ++j)
        {
            matrix(i, j) = rows[static_cast<std::size_t>(i)][static_cast<std::size_t>(j)];
        }
    }

    return matrix;
}

void print_matrix_as_csv(std::ostream& out, const GaussMatrix &matrix, int prec)
{
    out << std::fixed << std::setprecision(prec);
    for (int i = 0; i < matrix.rows(); ++i)
    {
        for (int j = 0; j < matrix.cols(); ++j)
        {
            if (j > 0)
            {
                out << ',';
            }
            out << matrix(i, j);
        }
        out << '\n';
    }
}

void print_vector_as_csv(std::ostream& out, const GaussVector &vector, int prec)
{
    out << "x\n";
    out << std::fixed << std::setprecision(prec);
    for (int i = 0; i < vector.rows(); ++i)
    {
        out << vector(i) << '\n';
    }
}
