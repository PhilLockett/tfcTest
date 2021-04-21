/**
 * @file    test.cpp
 * @author  Phil Lockett <phillockett65@gmail.com>
 * @version 1.0
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * https://www.gnu.org/copyleft/gpl.html
 *
 * @section DESCRIPTION
 *
 * Unit test code for the 'tfc' utility.
 *
 * Build using:
 *    g++ -std=c++20 -c -o test.o test.cpp
 *    g++ -std=c++20 -c -o unittest.o unittest.cpp
 *    g++ -std=c++20 -o test test.o unittest.o
 *
 * Test using:
 *    ./test
 *
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <vector>
#include <sys/stat.h>

#include "unittest.h"


/**
 * @section basic utility code.
 */

const std::string rootDir{"testdata"};
const std::string inputDir{rootDir + "/input"};
const std::string outputDir{rootDir + "/output"};
const std::string compareDir{rootDir + "/compare"};

static bool createDirectory(const std::string & path)
{
    bool ret{std::filesystem::create_directories(path)};

    return ret;
}

static bool checkFileExists(const std::string & path)
{
    return std::filesystem::exists(path);
}

static void deleteDirectory(const std::string & path)
{
    std::filesystem::remove_all(path); // Delete directory and contents.
}

static int getFileLength(const std::string & fileName)
{
    std::ifstream infile(fileName, std::ifstream::in);
    if (!infile.is_open())
        return 0;

    int count = 0;
    std::string line;

    while (getline(infile, line))
    {
        if (!infile.eof() && line.length())
            count++;
    }

    infile.close();

    return count;
}

static bool checkFileLineLength(const std::string & fileName, int length)
{
    std::ifstream infile(fileName, std::ifstream::in);
    if (!infile.is_open())
        return false;

    std::string line;

    while (getline(infile, line))
    {
        if (!infile.eof() && line.length())
            if (line.length() != length)
                return false;
    }

    infile.close();

    return true;
}

static std::vector<std::string> fileToVector(const std::string & fileName, int reserve = 50)
{
//    std::cout << "fileToVector " << fileName << '\n';
    std::ifstream infile(fileName, std::ifstream::in);
    std::vector<std::string> ret;
    if (!infile.is_open())
        return ret;

    ret.reserve(reserve);
    std::string line;

    while (getline(infile, line))
    {
        if (!infile.eof() && line.length())
            ret.push_back(std::move(line));
    }

    infile.close();

    return ret;
}

static std::vector<char> readBinaryFile(const std::string & fileName, int reserve = 100)
{
//    std::cout << "readBinaryFile " << fileName << '\n';
    std::vector<char> ret;
    if (std::ifstream is{fileName, std::ios::binary})
    {
        ret.reserve(reserve);
        char event;
        for (is.get(event); !is.eof(); is.get(event))
        {
            ret.push_back(event);
        }
    }

    return ret;
}

/**
 * Process the user specified file.
 *
 * @return error value or 0 if no errors.
 */
static int writeBinaryFile(const std::string & fileName, const std::vector<char> & data)
{
    if (std::ofstream os{fileName, std::ios::binary})
    {
        for (auto & c : data)
        {
            os.put(c);
        }

        return 0;
    }

    return 1;
}

static int writeSummaryFile(const std::string & fileName, const std::string & line1, const std::string & line2)
{
    if (std::ofstream os{fileName, std::ios::out})
    {
        os << line1 << '\n' << line2 << '\n';
        return 0;
    }

    return 1;
}


/**
 * @section test statistics using direct calls to the static singleton.
 *
 * All of these are interdependent.
 */
UNIT_TEST(test0, "Test environment'.")

    REQUIRE(system(NULL) != 0)

END_TEST

UNIT_TEST(test1, "Test summary generation for 'test1.txt'.")

    std::string fileName{"/test1.txt"};
    std::string inputFileName{inputDir + fileName};
    std::string outputFileName{outputDir + fileName};
    std::string compareFileName{compareDir + fileName};

    std::string command{"tfc -x -i " + inputFileName + " -o " + outputFileName};
    // std::cout << command << '\n';
    system(command.c_str());
    std::vector<char> compare = readBinaryFile(compareFileName);
    std::vector<char> output = readBinaryFile(outputFileName);

    REQUIRE(std::equal(compare.begin(), compare.end(), output.begin()))

END_TEST

UNIT_TEST(test2, "Test summary generation for 'test2.txt'.")

    std::string fileName{"/test2.txt"};
    std::string inputFileName{inputDir + fileName};
    std::string outputFileName{outputDir + fileName};
    std::string compareFileName{compareDir + fileName};

    std::string command{"tfc -x -i " + inputFileName + " -o " + outputFileName};
    // std::cout << command << '\n';
    system(command.c_str());
    std::vector<char> compare = readBinaryFile(compareFileName);
    std::vector<char> output = readBinaryFile(outputFileName);

    REQUIRE(std::equal(compare.begin(), compare.end(), output.begin()))

END_TEST

UNIT_TEST(test3, "Test summary generation for 'test3.txt'.")

    std::string fileName{"/test3.txt"};
    std::string inputFileName{inputDir + fileName};
    std::string outputFileName{outputDir + fileName};
    std::string compareFileName{compareDir + fileName};

    std::string command{"tfc -x -i " + inputFileName + " -o " + outputFileName};
    // std::cout << command << '\n';
    system(command.c_str());
    std::vector<char> compare = readBinaryFile(compareFileName);
    std::vector<char> output = readBinaryFile(outputFileName);

    REQUIRE(std::equal(compare.begin(), compare.end(), output.begin()))

END_TEST

UNIT_TEST(test4, "Test summary generation for 'test4.txt'.")

    std::string fileName{"/test4.txt"};
    std::string inputFileName{inputDir + fileName};
    std::string outputFileName{outputDir + fileName};
    std::string compareFileName{compareDir + fileName};

    std::string command{"tfc -x -i " + inputFileName + " -o " + outputFileName};
    // std::cout << command << '\n';
    system(command.c_str());
    std::vector<char> compare = readBinaryFile(compareFileName);
    std::vector<char> output = readBinaryFile(outputFileName);

    REQUIRE(std::equal(compare.begin(), compare.end(), output.begin()))

END_TEST

int runTests(void)
{
    std::cout << "Executing all tests.\n";

    RUN_TEST(test0)
    RUN_TEST(test1)
    RUN_TEST(test2)
    RUN_TEST(test3)
    RUN_TEST(test4)

    const int err = FINISHED;
    if (err)
        std::cerr << err << " ERROR(S) encountered!.\n";
    else
        std::cout << "All tests passed.\n";

    return err;
}

/**
 * Test system entry point.
 *
 * @param  argc - command line argument count.
 * @param  argv - command line argument vector.
 * @return error value or 0 if no errors.
 */
int main(int argc, char *argv[])
{
    return runTests();
}

