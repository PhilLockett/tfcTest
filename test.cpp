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
const std::string expectedDir{rootDir + "/expected"};

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
 * @section check test environment setup.
 *
 */

UNIT_TEST(test0, "Test environment'.")

    REQUIRE(system(NULL) != 0)

END_TEST


/**
 * @section test summary generation.
 *
 */

UNIT_TEST(test1, "Test summary generation for 'test1.txt'.")

    std::string fileName{"/test1.txt"};
    std::string inputFileName{inputDir + fileName};
    std::string outputFileName{outputDir + fileName};
    std::string expectedFileName{expectedDir + fileName};

    std::string command{"tfc -x -i " + inputFileName + " -o " + outputFileName};
    // std::cout << command << '\n';
    system(command.c_str());
    std::vector<char> expected = readBinaryFile(expectedFileName);
    std::vector<char> output = readBinaryFile(outputFileName);

    REQUIRE(std::equal(expected.begin(), expected.end(), output.begin()))

END_TEST

UNIT_TEST(test2, "Test summary generation for 'test2.txt'.")

    std::string fileName{"/test2.txt"};
    std::string inputFileName{inputDir + fileName};
    std::string outputFileName{outputDir + fileName};
    std::string expectedFileName{expectedDir + fileName};

    std::string command{"tfc -x -i " + inputFileName + " -o " + outputFileName};
    // std::cout << command << '\n';
    system(command.c_str());
    std::vector<char> expected = readBinaryFile(expectedFileName);
    std::vector<char> output = readBinaryFile(outputFileName);

    REQUIRE(std::equal(expected.begin(), expected.end(), output.begin()))

END_TEST

UNIT_TEST(test3, "Test summary generation for 'test3.txt'.")

    std::string fileName{"/test3.txt"};
    std::string inputFileName{inputDir + fileName};
    std::string outputFileName{outputDir + fileName};
    std::string expectedFileName{expectedDir + fileName};

    std::string command{"tfc -x -i " + inputFileName + " -o " + outputFileName};
    // std::cout << command << '\n';
    system(command.c_str());
    std::vector<char> expected = readBinaryFile(expectedFileName);
    std::vector<char> output = readBinaryFile(outputFileName);

    REQUIRE(std::equal(expected.begin(), expected.end(), output.begin()))

END_TEST

UNIT_TEST(test4, "Test summary generation for 'test4.txt'.")

    std::string fileName{"/test4.txt"};
    std::string inputFileName{inputDir + fileName};
    std::string outputFileName{outputDir + fileName};
    std::string expectedFileName{expectedDir + fileName};

    std::string command{"tfc -x -i " + inputFileName + " -o " + outputFileName};
    // std::cout << command << '\n';
    system(command.c_str());
    std::vector<char> expected = readBinaryFile(expectedFileName);
    std::vector<char> output = readBinaryFile(outputFileName);

    REQUIRE(std::equal(expected.begin(), expected.end(), output.begin()))

END_TEST


/**
 * @section test leading space generation.
 *
 */

UNIT_TEST(test1s, "Test leading space generation for 'test1.txt'.")

    std::string fileName{"/test1s.txt"};
    std::string inputFileName{inputDir + "/test1.txt"};
    std::string outputFileName{outputDir + fileName};
    std::string expectedFileName{expectedDir + fileName};

    std::string command{"tfc -s -i " + inputFileName + " -o " + outputFileName};
    // std::cout << command << '\n';
    system(command.c_str());
    std::vector<char> expected = readBinaryFile(expectedFileName);
    std::vector<char> output = readBinaryFile(outputFileName);

    REQUIRE(std::equal(expected.begin(), expected.end(), output.begin()))

END_TEST

UNIT_TEST(test2s, "Test leading space generation for 'test2.txt'.")

    std::string fileName{"/test2s.txt"};
    std::string inputFileName{inputDir + "/test2.txt"};
    std::string outputFileName{outputDir + fileName};
    std::string expectedFileName{expectedDir + fileName};

    std::string command{"tfc -s -i " + inputFileName + " -o " + outputFileName};
    // std::cout << command << '\n';
    system(command.c_str());
    std::vector<char> expected = readBinaryFile(expectedFileName);
    std::vector<char> output = readBinaryFile(outputFileName);

    REQUIRE(std::equal(expected.begin(), expected.end(), output.begin()))

END_TEST

UNIT_TEST(test3s, "Test leading space generation for 'test3.txt'.")

    std::string fileName{"/test3s.txt"};
    std::string inputFileName{inputDir + "/test3.txt"};
    std::string outputFileName{outputDir + fileName};
    std::string expectedFileName{expectedDir + fileName};

    std::string command{"tfc -s -i " + inputFileName + " -o " + outputFileName};
    // std::cout << command << '\n';
    system(command.c_str());
    std::vector<char> expected = readBinaryFile(expectedFileName);
    std::vector<char> output = readBinaryFile(outputFileName);

    REQUIRE(std::equal(expected.begin(), expected.end(), output.begin()))

END_TEST

UNIT_TEST(test4s, "Test leading space generation for 'test4.txt'.")

    std::string fileName{"/test4s.txt"};
    std::string inputFileName{inputDir + "/test4.txt"};
    std::string outputFileName{outputDir + fileName};
    std::string expectedFileName{expectedDir + fileName};

    std::string command{"tfc -s -i " + inputFileName + " -o " + outputFileName};
    // std::cout << command << '\n';
    system(command.c_str());
    std::vector<char> expected = readBinaryFile(expectedFileName);
    std::vector<char> output = readBinaryFile(outputFileName);

    REQUIRE(std::equal(expected.begin(), expected.end(), output.begin()))

END_TEST


/**
 * @section test leading tab generation.
 *
 */

UNIT_TEST(test1t, "Test leading tab generation for 'test1.txt'.")

    std::string fileName{"/test1t.txt"};
    std::string inputFileName{inputDir + "/test1.txt"};
    std::string outputFileName{outputDir + fileName};
    std::string expectedFileName{expectedDir + fileName};

    std::string command{"tfc -t -i " + inputFileName + " -o " + outputFileName};
    // std::cout << command << '\n';
    system(command.c_str());
    std::vector<char> expected = readBinaryFile(expectedFileName);
    std::vector<char> output = readBinaryFile(outputFileName);

    REQUIRE(std::equal(expected.begin(), expected.end(), output.begin()))

END_TEST

UNIT_TEST(test2t, "Test leading tab generation for 'test2.txt'.")

    std::string fileName{"/test2t.txt"};
    std::string inputFileName{inputDir + "/test2.txt"};
    std::string outputFileName{outputDir + fileName};
    std::string expectedFileName{expectedDir + fileName};

    std::string command{"tfc -t -i " + inputFileName + " -o " + outputFileName};
    // std::cout << command << '\n';
    system(command.c_str());
    std::vector<char> expected = readBinaryFile(expectedFileName);
    std::vector<char> output = readBinaryFile(outputFileName);

    REQUIRE(std::equal(expected.begin(), expected.end(), output.begin()))

END_TEST

UNIT_TEST(test3t, "Test leading tab generation for 'test3.txt'.")

    std::string fileName{"/test3t.txt"};
    std::string inputFileName{inputDir + "/test3.txt"};
    std::string outputFileName{outputDir + fileName};
    std::string expectedFileName{expectedDir + fileName};

    std::string command{"tfc -t -i " + inputFileName + " -o " + outputFileName};
    // std::cout << command << '\n';
    system(command.c_str());
    std::vector<char> expected = readBinaryFile(expectedFileName);
    std::vector<char> output = readBinaryFile(outputFileName);

    REQUIRE(std::equal(expected.begin(), expected.end(), output.begin()))

END_TEST

UNIT_TEST(test4t, "Test leading tab generation for 'test4.txt'.")

    std::string fileName{"/test4t.txt"};
    std::string inputFileName{inputDir + "/test4.txt"};
    std::string outputFileName{outputDir + fileName};
    std::string expectedFileName{expectedDir + fileName};

    std::string command{"tfc -t -i " + inputFileName + " -o " + outputFileName};
    // std::cout << command << '\n';
    system(command.c_str());
    std::vector<char> expected = readBinaryFile(expectedFileName);
    std::vector<char> output = readBinaryFile(outputFileName);

    REQUIRE(std::equal(expected.begin(), expected.end(), output.begin()))

END_TEST


/**
 * @section test trailing dos generation.
 *
 */

UNIT_TEST(test1d, "Test trailing dos generation for 'test1.txt'.")

    std::string fileName{"/test1d.txt"};
    std::string inputFileName{inputDir + "/test1.txt"};
    std::string outputFileName{outputDir + fileName};
    std::string expectedFileName{expectedDir + fileName};

    std::string command{"tfc -d -i " + inputFileName + " -o " + outputFileName};
    // std::cout << command << '\n';
    system(command.c_str());
    std::vector<char> expected = readBinaryFile(expectedFileName);
    std::vector<char> output = readBinaryFile(outputFileName);

    REQUIRE(std::equal(expected.begin(), expected.end(), output.begin()))

END_TEST

UNIT_TEST(test2d, "Test trailing dos generation for 'test2.txt'.")

    std::string fileName{"/test2d.txt"};
    std::string inputFileName{inputDir + "/test2.txt"};
    std::string outputFileName{outputDir + fileName};
    std::string expectedFileName{expectedDir + fileName};

    std::string command{"tfc -d -i " + inputFileName + " -o " + outputFileName};
    // std::cout << command << '\n';
    system(command.c_str());
    std::vector<char> expected = readBinaryFile(expectedFileName);
    std::vector<char> output = readBinaryFile(outputFileName);

    REQUIRE(std::equal(expected.begin(), expected.end(), output.begin()))

END_TEST

UNIT_TEST(test3d, "Test trailing dos generation for 'test3.txt'.")

    std::string fileName{"/test3d.txt"};
    std::string inputFileName{inputDir + "/test3.txt"};
    std::string outputFileName{outputDir + fileName};
    std::string expectedFileName{expectedDir + fileName};

    std::string command{"tfc -d -i " + inputFileName + " -o " + outputFileName};
    // std::cout << command << '\n';
    system(command.c_str());
    std::vector<char> expected = readBinaryFile(expectedFileName);
    std::vector<char> output = readBinaryFile(outputFileName);

    REQUIRE(std::equal(expected.begin(), expected.end(), output.begin()))

END_TEST

UNIT_TEST(test4d, "Test trailing dos generation for 'test4.txt'.")

    std::string fileName{"/test4d.txt"};
    std::string inputFileName{inputDir + "/test4.txt"};
    std::string outputFileName{outputDir + fileName};
    std::string expectedFileName{expectedDir + fileName};

    std::string command{"tfc -d -i " + inputFileName + " -o " + outputFileName};
    // std::cout << command << '\n';
    system(command.c_str());
    std::vector<char> expected = readBinaryFile(expectedFileName);
    std::vector<char> output = readBinaryFile(outputFileName);

    REQUIRE(std::equal(expected.begin(), expected.end(), output.begin()))

END_TEST


/**
 * @section test trailing unix generation.
 *
 */

UNIT_TEST(test1u, "Test trailing unix generation for 'test1.txt'.")

    std::string fileName{"/test1u.txt"};
    std::string inputFileName{inputDir + "/test1.txt"};
    std::string outputFileName{outputDir + fileName};
    std::string expectedFileName{expectedDir + fileName};

    std::string command{"tfc -u -i " + inputFileName + " -o " + outputFileName};
    // std::cout << command << '\n';
    system(command.c_str());
    std::vector<char> expected = readBinaryFile(expectedFileName);
    std::vector<char> output = readBinaryFile(outputFileName);

    REQUIRE(std::equal(expected.begin(), expected.end(), output.begin()))

END_TEST

UNIT_TEST(test2u, "Test trailing unix generation for 'test2.txt'.")

    std::string fileName{"/test2u.txt"};
    std::string inputFileName{inputDir + "/test2.txt"};
    std::string outputFileName{outputDir + fileName};
    std::string expectedFileName{expectedDir + fileName};

    std::string command{"tfc -u -i " + inputFileName + " -o " + outputFileName};
    // std::cout << command << '\n';
    system(command.c_str());
    std::vector<char> expected = readBinaryFile(expectedFileName);
    std::vector<char> output = readBinaryFile(outputFileName);

    REQUIRE(std::equal(expected.begin(), expected.end(), output.begin()))

END_TEST

UNIT_TEST(test3u, "Test trailing unix generation for 'test3.txt'.")

    std::string fileName{"/test3u.txt"};
    std::string inputFileName{inputDir + "/test3.txt"};
    std::string outputFileName{outputDir + fileName};
    std::string expectedFileName{expectedDir + fileName};

    std::string command{"tfc -u -i " + inputFileName + " -o " + outputFileName};
    // std::cout << command << '\n';
    system(command.c_str());
    std::vector<char> expected = readBinaryFile(expectedFileName);
    std::vector<char> output = readBinaryFile(outputFileName);

    REQUIRE(std::equal(expected.begin(), expected.end(), output.begin()))

END_TEST

UNIT_TEST(test4u, "Test trailing unix generation for 'test4.txt'.")

    std::string fileName{"/test4u.txt"};
    std::string inputFileName{inputDir + "/test4.txt"};
    std::string outputFileName{outputDir + fileName};
    std::string expectedFileName{expectedDir + fileName};

    std::string command{"tfc -u -i " + inputFileName + " -o " + outputFileName};
    // std::cout << command << '\n';
    system(command.c_str());
    std::vector<char> expected = readBinaryFile(expectedFileName);
    std::vector<char> output = readBinaryFile(outputFileName);

    REQUIRE(std::equal(expected.begin(), expected.end(), output.begin()))

END_TEST


/**
 * @section test leading space and trailing dos generation.
 *
 */

UNIT_TEST(test1sd, "Test leading space and trailing dos generation for 'test1.txt'.")

    std::string fileName{"/test1sd.txt"};
    std::string inputFileName{inputDir + "/test1.txt"};
    std::string outputFileName{outputDir + fileName};
    std::string expectedFileName{expectedDir + fileName};

    std::string command{"tfc -s -d -i " + inputFileName + " -o " + outputFileName};
    // std::cout << command << '\n';
    system(command.c_str());
    std::vector<char> expected = readBinaryFile(expectedFileName);
    std::vector<char> output = readBinaryFile(outputFileName);

    REQUIRE(std::equal(expected.begin(), expected.end(), output.begin()))

END_TEST

UNIT_TEST(test2sd, "Test leading space and trailing dos generation for 'test2.txt'.")

    std::string fileName{"/test2sd.txt"};
    std::string inputFileName{inputDir + "/test2.txt"};
    std::string outputFileName{outputDir + fileName};
    std::string expectedFileName{expectedDir + fileName};

    std::string command{"tfc -s -d -i " + inputFileName + " -o " + outputFileName};
    // std::cout << command << '\n';
    system(command.c_str());
    std::vector<char> expected = readBinaryFile(expectedFileName);
    std::vector<char> output = readBinaryFile(outputFileName);

    REQUIRE(std::equal(expected.begin(), expected.end(), output.begin()))

END_TEST

UNIT_TEST(test3sd, "Test leading space and trailing dos generation for 'test3.txt'.")

    std::string fileName{"/test3sd.txt"};
    std::string inputFileName{inputDir + "/test3.txt"};
    std::string outputFileName{outputDir + fileName};
    std::string expectedFileName{expectedDir + fileName};

    std::string command{"tfc -s -d -i " + inputFileName + " -o " + outputFileName};
    // std::cout << command << '\n';
    system(command.c_str());
    std::vector<char> expected = readBinaryFile(expectedFileName);
    std::vector<char> output = readBinaryFile(outputFileName);

    REQUIRE(std::equal(expected.begin(), expected.end(), output.begin()))

END_TEST

UNIT_TEST(test4sd, "Test leading space and trailing dos generation for 'test4.txt'.")

    std::string fileName{"/test4sd.txt"};
    std::string inputFileName{inputDir + "/test4.txt"};
    std::string outputFileName{outputDir + fileName};
    std::string expectedFileName{expectedDir + fileName};

    std::string command{"tfc -s -d -i " + inputFileName + " -o " + outputFileName};
    // std::cout << command << '\n';
    system(command.c_str());
    std::vector<char> expected = readBinaryFile(expectedFileName);
    std::vector<char> output = readBinaryFile(outputFileName);

    REQUIRE(std::equal(expected.begin(), expected.end(), output.begin()))

END_TEST


/**
 * @section test leading tab and trailing dos generation.
 *
 */

UNIT_TEST(test1td, "Test leading tab and trailing dos generation for 'test1.txt'.")

    std::string fileName{"/test1td.txt"};
    std::string inputFileName{inputDir + "/test1.txt"};
    std::string outputFileName{outputDir + fileName};
    std::string expectedFileName{expectedDir + fileName};

    std::string command{"tfc -t -d -i " + inputFileName + " -o " + outputFileName};
    // std::cout << command << '\n';
    system(command.c_str());
    std::vector<char> expected = readBinaryFile(expectedFileName);
    std::vector<char> output = readBinaryFile(outputFileName);

    REQUIRE(std::equal(expected.begin(), expected.end(), output.begin()))

END_TEST

UNIT_TEST(test2td, "Test leading tab and trailing dos generation for 'test2.txt'.")

    std::string fileName{"/test2td.txt"};
    std::string inputFileName{inputDir + "/test2.txt"};
    std::string outputFileName{outputDir + fileName};
    std::string expectedFileName{expectedDir + fileName};

    std::string command{"tfc -t -d -i " + inputFileName + " -o " + outputFileName};
    // std::cout << command << '\n';
    system(command.c_str());
    std::vector<char> expected = readBinaryFile(expectedFileName);
    std::vector<char> output = readBinaryFile(outputFileName);

    REQUIRE(std::equal(expected.begin(), expected.end(), output.begin()))

END_TEST

UNIT_TEST(test3td, "Test leading tab and trailing dos generation for 'test3.txt'.")

    std::string fileName{"/test3td.txt"};
    std::string inputFileName{inputDir + "/test3.txt"};
    std::string outputFileName{outputDir + fileName};
    std::string expectedFileName{expectedDir + fileName};

    std::string command{"tfc -t -d -i " + inputFileName + " -o " + outputFileName};
    // std::cout << command << '\n';
    system(command.c_str());
    std::vector<char> expected = readBinaryFile(expectedFileName);
    std::vector<char> output = readBinaryFile(outputFileName);

    REQUIRE(std::equal(expected.begin(), expected.end(), output.begin()))

END_TEST

UNIT_TEST(test4td, "Test leading tab and trailing dos generation for 'test4.txt'.")

    std::string fileName{"/test4td.txt"};
    std::string inputFileName{inputDir + "/test4.txt"};
    std::string outputFileName{outputDir + fileName};
    std::string expectedFileName{expectedDir + fileName};

    std::string command{"tfc -t -d -i " + inputFileName + " -o " + outputFileName};
    // std::cout << command << '\n';
    system(command.c_str());
    std::vector<char> expected = readBinaryFile(expectedFileName);
    std::vector<char> output = readBinaryFile(outputFileName);

    REQUIRE(std::equal(expected.begin(), expected.end(), output.begin()))

END_TEST


/**
 * @section test leading space and trailing unix generation.
 *
 */

UNIT_TEST(test1su, "Test leading space and trailing unix generation for 'test1.txt'.")

    std::string fileName{"/test1su.txt"};
    std::string inputFileName{inputDir + "/test1.txt"};
    std::string outputFileName{outputDir + fileName};
    std::string expectedFileName{expectedDir + fileName};

    std::string command{"tfc -s -u -i " + inputFileName + " -o " + outputFileName};
    // std::cout << command << '\n';
    system(command.c_str());
    std::vector<char> expected = readBinaryFile(expectedFileName);
    std::vector<char> output = readBinaryFile(outputFileName);

    REQUIRE(std::equal(expected.begin(), expected.end(), output.begin()))

END_TEST

UNIT_TEST(test2su, "Test leading space and trailing unix generation for 'test2.txt'.")

    std::string fileName{"/test2su.txt"};
    std::string inputFileName{inputDir + "/test2.txt"};
    std::string outputFileName{outputDir + fileName};
    std::string expectedFileName{expectedDir + fileName};

    std::string command{"tfc -s -u -i " + inputFileName + " -o " + outputFileName};
    // std::cout << command << '\n';
    system(command.c_str());
    std::vector<char> expected = readBinaryFile(expectedFileName);
    std::vector<char> output = readBinaryFile(outputFileName);

    REQUIRE(std::equal(expected.begin(), expected.end(), output.begin()))

END_TEST

UNIT_TEST(test3su, "Test leading space and trailing unix generation for 'test3.txt'.")

    std::string fileName{"/test3su.txt"};
    std::string inputFileName{inputDir + "/test3.txt"};
    std::string outputFileName{outputDir + fileName};
    std::string expectedFileName{expectedDir + fileName};

    std::string command{"tfc -s -u -i " + inputFileName + " -o " + outputFileName};
    // std::cout << command << '\n';
    system(command.c_str());
    std::vector<char> expected = readBinaryFile(expectedFileName);
    std::vector<char> output = readBinaryFile(outputFileName);

    REQUIRE(std::equal(expected.begin(), expected.end(), output.begin()))

END_TEST

UNIT_TEST(test4su, "Test leading space and trailing unix generation for 'test4.txt'.")

    std::string fileName{"/test4su.txt"};
    std::string inputFileName{inputDir + "/test4.txt"};
    std::string outputFileName{outputDir + fileName};
    std::string expectedFileName{expectedDir + fileName};

    std::string command{"tfc -s -u -i " + inputFileName + " -o " + outputFileName};
    // std::cout << command << '\n';
    system(command.c_str());
    std::vector<char> expected = readBinaryFile(expectedFileName);
    std::vector<char> output = readBinaryFile(outputFileName);

    REQUIRE(std::equal(expected.begin(), expected.end(), output.begin()))

END_TEST


/**
 * @section test leading tab and trailing unix generation.
 *
 */

UNIT_TEST(test1tu, "Test leading tab and trailing unix generation for 'test1.txt'.")

    std::string fileName{"/test1tu.txt"};
    std::string inputFileName{inputDir + "/test1.txt"};
    std::string outputFileName{outputDir + fileName};
    std::string expectedFileName{expectedDir + fileName};

    std::string command{"tfc -t -u -i " + inputFileName + " -o " + outputFileName};
    // std::cout << command << '\n';
    system(command.c_str());
    std::vector<char> expected = readBinaryFile(expectedFileName);
    std::vector<char> output = readBinaryFile(outputFileName);

    REQUIRE(std::equal(expected.begin(), expected.end(), output.begin()))

END_TEST

UNIT_TEST(test2tu, "Test leading tab and trailing unix generation for 'test2.txt'.")

    std::string fileName{"/test2tu.txt"};
    std::string inputFileName{inputDir + "/test2.txt"};
    std::string outputFileName{outputDir + fileName};
    std::string expectedFileName{expectedDir + fileName};

    std::string command{"tfc -t -u -i " + inputFileName + " -o " + outputFileName};
    // std::cout << command << '\n';
    system(command.c_str());
    std::vector<char> expected = readBinaryFile(expectedFileName);
    std::vector<char> output = readBinaryFile(outputFileName);

    REQUIRE(std::equal(expected.begin(), expected.end(), output.begin()))

END_TEST

UNIT_TEST(test3tu, "Test leading tab and trailing unix generation for 'test3.txt'.")

    std::string fileName{"/test3tu.txt"};
    std::string inputFileName{inputDir + "/test3.txt"};
    std::string outputFileName{outputDir + fileName};
    std::string expectedFileName{expectedDir + fileName};

    std::string command{"tfc -t -u -i " + inputFileName + " -o " + outputFileName};
    // std::cout << command << '\n';
    system(command.c_str());
    std::vector<char> expected = readBinaryFile(expectedFileName);
    std::vector<char> output = readBinaryFile(outputFileName);

    REQUIRE(std::equal(expected.begin(), expected.end(), output.begin()))

END_TEST

UNIT_TEST(test4tu, "Test leading tab and trailing unix generation for 'test4.txt'.")

    std::string fileName{"/test4tu.txt"};
    std::string inputFileName{inputDir + "/test4.txt"};
    std::string outputFileName{outputDir + fileName};
    std::string expectedFileName{expectedDir + fileName};

    std::string command{"tfc -t -u -i " + inputFileName + " -o " + outputFileName};
    // std::cout << command << '\n';
    system(command.c_str());
    std::vector<char> expected = readBinaryFile(expectedFileName);
    std::vector<char> output = readBinaryFile(outputFileName);

    REQUIRE(std::equal(expected.begin(), expected.end(), output.begin()))

END_TEST


/**
 * @section test leading space to tab replacement.
 *
 */

UNIT_TEST(testSpace2, "Test leading space to tab replacement 'testSpace2.txt'.")

    std::string fileName{"/testSpace2.txt"};
    std::string inputFileName{inputDir + "/testSpace.txt"};
    std::string outputFileName{outputDir + fileName};
    std::string expectedFileName{expectedDir + fileName};

    std::string command{"tfc -t -2 -i " + inputFileName + " -o " + outputFileName};
    // std::cout << command << '\n';
    system(command.c_str());
    std::vector<char> expected = readBinaryFile(expectedFileName);
    std::vector<char> output = readBinaryFile(outputFileName);

    REQUIRE(std::equal(expected.begin(), expected.end(), output.begin()))

END_TEST

UNIT_TEST(testSpace4, "Test leading space to tab replacement 'testSpace4.txt'.")

    std::string fileName{"/testSpace4.txt"};
    std::string inputFileName{inputDir + "/testSpace.txt"};
    std::string outputFileName{outputDir + fileName};
    std::string expectedFileName{expectedDir + fileName};

    std::string command{"tfc -t -4 -i " + inputFileName + " -o " + outputFileName};
    // std::cout << command << '\n';
    system(command.c_str());
    std::vector<char> expected = readBinaryFile(expectedFileName);
    std::vector<char> output = readBinaryFile(outputFileName);

    REQUIRE(std::equal(expected.begin(), expected.end(), output.begin()))

END_TEST

UNIT_TEST(testSpace8, "Test leading space to tab replacement 'testSpace8.txt'.")

    std::string fileName{"/testSpace8.txt"};
    std::string inputFileName{inputDir + "/testSpace.txt"};
    std::string outputFileName{outputDir + fileName};
    std::string expectedFileName{expectedDir + fileName};

    std::string command{"tfc -t -8 -i " + inputFileName + " -o " + outputFileName};
    // std::cout << command << '\n';
    system(command.c_str());
    std::vector<char> expected = readBinaryFile(expectedFileName);
    std::vector<char> output = readBinaryFile(outputFileName);

    REQUIRE(std::equal(expected.begin(), expected.end(), output.begin()))

END_TEST


/**
 * @section test leading tab to space replacement.
 *
 */

UNIT_TEST(testTab2, "Test leading space to tab replacement 'testTab2.txt'.")

    std::string fileName{"/testTab2.txt"};
    std::string inputFileName{inputDir + "/testTab.txt"};
    std::string outputFileName{outputDir + fileName};
    std::string expectedFileName{expectedDir + fileName};

    std::string command{"tfc -s -2 -i " + inputFileName + " -o " + outputFileName};
    // std::cout << command << '\n';
    system(command.c_str());
    std::vector<char> expected = readBinaryFile(expectedFileName);
    std::vector<char> output = readBinaryFile(outputFileName);

    REQUIRE(std::equal(expected.begin(), expected.end(), output.begin()))

END_TEST

UNIT_TEST(testTab4, "Test leading space to tab replacement 'testTab4.txt'.")

    std::string fileName{"/testTab4.txt"};
    std::string inputFileName{inputDir + "/testTab.txt"};
    std::string outputFileName{outputDir + fileName};
    std::string expectedFileName{expectedDir + fileName};

    std::string command{"tfc -s -4 -i " + inputFileName + " -o " + outputFileName};
    // std::cout << command << '\n';
    system(command.c_str());
    std::vector<char> expected = readBinaryFile(expectedFileName);
    std::vector<char> output = readBinaryFile(outputFileName);

    REQUIRE(std::equal(expected.begin(), expected.end(), output.begin()))

END_TEST

UNIT_TEST(testTab8, "Test leading space to tab replacement 'testTab8.txt'.")

    std::string fileName{"/testTab8.txt"};
    std::string inputFileName{inputDir + "/testTab.txt"};
    std::string outputFileName{outputDir + fileName};
    std::string expectedFileName{expectedDir + fileName};

    std::string command{"tfc -s -8 -i " + inputFileName + " -o " + outputFileName};
    // std::cout << command << '\n';
    system(command.c_str());
    std::vector<char> expected = readBinaryFile(expectedFileName);
    std::vector<char> output = readBinaryFile(outputFileName);

    REQUIRE(std::equal(expected.begin(), expected.end(), output.begin()))

END_TEST


int runTests(void)
{
    std::cout << "Executing all tests.\n";

    RUN_TEST(test0)
    RUN_TEST(test1)
    RUN_TEST(test2)
    RUN_TEST(test3)
    RUN_TEST(test4)
    RUN_TEST(test1s)
    RUN_TEST(test2s)
    RUN_TEST(test3s)
    RUN_TEST(test4s)
    RUN_TEST(test1t)
    RUN_TEST(test2t)
    RUN_TEST(test3t)
    RUN_TEST(test4t)
    RUN_TEST(test1d)
    RUN_TEST(test2d)
    RUN_TEST(test3d)
    RUN_TEST(test4d)
    RUN_TEST(test1u)
    RUN_TEST(test2u)
    RUN_TEST(test3u)
    RUN_TEST(test4u)
    RUN_TEST(test1sd)
    RUN_TEST(test2sd)
    RUN_TEST(test3sd)
    RUN_TEST(test4sd)
    RUN_TEST(test1td)
    RUN_TEST(test2td)
    RUN_TEST(test3td)
    RUN_TEST(test4td)
    RUN_TEST(test1su)
    RUN_TEST(test2su)
    RUN_TEST(test3su)
    RUN_TEST(test4su)
    RUN_TEST(test1tu)
    RUN_TEST(test2tu)
    RUN_TEST(test3tu)
    RUN_TEST(test4tu)
    RUN_TEST(testSpace2)
    RUN_TEST(testSpace4)
    RUN_TEST(testSpace8)
    RUN_TEST(testTab2)
    RUN_TEST(testTab4)
    RUN_TEST(testTab8)

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

