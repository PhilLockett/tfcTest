/**
 * @file    gen.cpp
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
 * Test file generator for the 'tfc' utility.
 *
 * Build using:
 *    g++ -std=c++20 -o gen gen.cpp
 *
 * Test using:
 *    ./gen
 *
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <vector>
#include <tuple>
#include <sys/stat.h>


std::vector<std::tuple<std::string, std::string, std::string>> tests;

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

static void addTest(const std::string & test, const std::string & output, const std::string & compare)
{
    tests.emplace_back(test, output, compare);
}

static int genTestScript(const std::string & fileName, const char * program)
{
    if (std::ofstream os{fileName, std::ios::out})
    {
        os << "#!/bin/sh\n";
        os << '\n';
        os << "# This file was generated as \"" << fileName << "\" using " << program << '\n';
        os << "#\n";
        os << '\n';
        for (auto [test, output, compare] : tests)
        {
            os << "echo Comparing " << output << " with " << compare << '\n';
            os << "tfc " << test;
            if (!output.empty())
                os << " -o " << output;
            os << '\n' << "diff " << output << ' ' << compare << '\n';
        }

        os.close();

        // Ensure output scripts are executable.
        chmod(fileName.c_str(), S_IRWXU|S_IRGRP|S_IXGRP|S_IROTH|S_IXOTH);

        return 0;
    }

    return 1;
}

/**
 * @section test logging code.
 */



/**
 * @section test sending log entries in various ways to check that low priority entires are filtered out..
 */

void checkFile(const std::vector<std::string> & comp, const std::string currentLogFileName, int targetCount)
{
    std::vector<std::string> entries = fileToVector(currentLogFileName, targetCount);
}

static std::vector<char> readBinaryFile(const std::string & fileName, int reserve = 50)
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
    std::cout << "Generating file " << fileName << "\n";
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
    std::cout << "Generating summary file " << fileName << "\n";
    if (std::ofstream os{fileName, std::ios::out})
    {
        os << line1 << '\n' << line2 << '\n';
        return 0;
    }

    return 1;
}


/**
 * @section launch the tests and check the results.
 */

int runTests(void)
{
    std::cout << "Executing all tests.\n";
    const int err = 0;
    if (err)
        std::cerr << err << " ERROR(S) encountered!.\n";
    else
        std::cout << "All tests passed.\n";

    return err;
}


/**
 * @section test summary generation.
 *
 */
int summaryTests(int argc, char *argv[])
{
    std::string filename{};
    std::string input{};
    std::string output{};
    std::string compare{};
    // std::string before{inputDir + "/test1.txt"};
    // const std::string after{compareDir + "/" + filename};
    // std::cout << filename << " [" << before << "] -> [" << after << "]\n";

/* A mix of space and tab leading, space and tab in middle and CR LF EOL.
testdata/input/test1.txt
  Total Lines:  9
Line begining:
  Space only:   1
  Tab only:     1
  Neither:      3
  Both:         4
Line ending:
  Dos:          9
  Unix:         0
  Malformed:    0
*/
    std::vector<char> test1{ 
        '\t', ' ', ' ', 'S', 'u', 'b', ' ', '1', '\r', '\n', 
        ' ', '\t', ' ', ' ', 'C', 'R', 'L', 'F', '.', 'm', '\r', '\n', 
        ' ', '\t', '\r', '\n', 
        '\t', ' ', '\r', '\n', 
        '\t', 'H', '\t', 'i', '\r', '\n', 
        ' ', 'H', '\t', 'i', '\r', '\n', 
        'H', '\t', 'i', '\r', '\n', 
        'H', ' ', 'i', '\r', '\n', 
        '\r', '\n' 
    };
    filename = "/test1.txt";
    input = inputDir + filename;
    output = outputDir + filename;
    compare = compareDir + filename;
    
    writeBinaryFile(input, test1);
    writeSummaryFile(compare, input, "9 1 1 3 4 9 0 0");
    addTest("-x -i " + input, output, compare);

/* A mix of space and tab leading, space and tab in middle and only LF EOL.
testdata/input/test2.txt
  Total Lines:  9
Line begining:
  Space only:   1
  Tab only:     1
  Neither:      3
  Both:         4
Line ending:
  Dos:          0
  Unix:         9
  Malformed:    0
*/
    std::vector<char> test2{ 
        '\t', ' ', ' ', 'S', 'u', 'b', ' ', '1', '\n', 
        ' ', '\t', ' ', ' ', 'L', 'F', '.', 'm', '\n', 
        ' ', '\t', '\n', 
        '\t', ' ', '\n', 
        '\t', 'H', '\t', 'i', '\n', 
        ' ', 'H', '\t', 'i', '\n', 
        'H', '\t', 'i', '\n', 
        'H', ' ', 'i', '\n', 
        '\n' 
    };
    filename = "/test2.txt";
    input = inputDir + filename;
    output = outputDir + filename;
    compare = compareDir + filename;

    writeBinaryFile(input, test2);
    writeSummaryFile(compare, input, "9 1 1 3 4 0 9 0");
    addTest("-x -i " + input, output, compare);

/* A mix of space and tab leading, space and tab in middle and mix of CR LF and LF EOL.
testdata/input/test3.txt
  Total Lines:  9
Line begining:
  Space only:   1
  Tab only:     1
  Neither:      3
  Both:         4
Line ending:
  Dos:          6
  Unix:         3
  Malformed:    0
*/
    std::vector<char> test3{ 
        '\t', ' ', ' ', 'M', 'i', 'x', ' ', '1', '\r', '\n', 
        ' ', '\t', ' ', ' ', 'C', 'R', 'L', 'F', '.', 'm', '\n', 
        ' ', '\t', '\r', '\n', 
        '\t', ' ', '\n', 
        '\t', 'H', '\t', 'i', '\r', '\n', 
        ' ', 'H', '\t', 'i', '\r', '\n', 
        'H', '\t', 'i', '\n', 
        'H', ' ', 'i', '\r', '\n', 
        '\r', '\n' 
    };
    filename = "/test3.txt";
    input = inputDir + filename;
    output = outputDir + filename;
    compare = compareDir + filename;

    writeBinaryFile(input, test3);
    writeSummaryFile(compare, input, "9 1 1 3 4 6 3 0");
    addTest("-x -i " + input, output, compare);

/* A mix of space and tab leading, space and tab in middle and malformed EOL.
testdata/input/test4.txt
  Total Lines:  9
Line begining:
  Space only:   1
  Tab only:     1
  Neither:      3
  Both:         4
Line ending:
  Dos:          0
  Unix:         0
  Malformed:    9
*/
    std::vector<char> test4{ 
        '\t', ' ', ' ', 'S', 'u', 'b', ' ', '1', '\n', '\r', 
        ' ', '\t', ' ', ' ', 'L', 'F', 'C', 'R', '.', 'm', '\n', '\r', 
        ' ', '\t', '\n', '\r', 
        '\t', ' ', '\n', '\r', 
        '\t', 'H', '\t', 'i', '\n', '\r', 
        ' ', 'H', '\t', 'i', '\n', '\r', 
        'H', '\t', 'i', '\n', '\r', 
        'H', ' ', 'i', '\n', '\r', 
        '\n', '\r' 
    };
    filename = "/test4.txt";
    input = inputDir + filename;
    output = outputDir + filename;
    compare = compareDir + filename;

    writeBinaryFile(input, test4);
    writeSummaryFile(compare, input, "9 1 1 3 4 0 0 9");
    addTest("-x -i " + input, output, compare);

    return 0;
}


/**
 * @section test leading space generation.
 *
 * tfc -s -i testdata/input/testX.txt -o testdata/output/testXs.txt
 */
int spaceTests(void)
{
    const std::string & postfix{"s"};
    std::string input{};

// test1.txt : A mix of space and tab leading, space and tab in middle and CR LF EOL.
    std::vector<char> test1{ 
        ' ', ' ', ' ', ' ', ' ', ' ', 'S', 'u', 'b', ' ', '1', '\r', '\n', 
        ' ', ' ', ' ', ' ', ' ', ' ', 'C', 'R', 'L', 'F', '.', 'm', '\r', '\n', 
        ' ', ' ', ' ', ' ', '\r', '\n', 
        ' ', ' ', ' ', ' ', ' ', '\r', '\n', 
        ' ', ' ', ' ', ' ', 'H', '\t', 'i', '\r', '\n', 
        ' ', 'H', '\t', 'i', '\r', '\n', 
        'H', '\t', 'i', '\r', '\n', 
        'H', ' ', 'i', '\r', '\n', 
        '\r', '\n' 
    };
    input = compareDir + "/test1" + postfix + ".txt";
    writeBinaryFile(input, test1);

// test2.txt : A mix of space and tab leading, space and tab in middle and only LF EOL.
    std::vector<char> test2{ 
        ' ', ' ', ' ', ' ', ' ', ' ', 'S', 'u', 'b', ' ', '1', '\n', 
        ' ', ' ', ' ', ' ', ' ', ' ', 'L', 'F', '.', 'm', '\n', 
        ' ', ' ', ' ', ' ', '\n', 
        ' ', ' ', ' ', ' ', ' ', '\n', 
        ' ', ' ', ' ', ' ', 'H', '\t', 'i', '\n', 
        ' ', 'H', '\t', 'i', '\n', 
        'H', '\t', 'i', '\n', 
        'H', ' ', 'i', '\n', 
        '\n' 
    };
    input = compareDir + "/test2" + postfix + ".txt";
    writeBinaryFile(input, test2);

// test3.txt : A mix of space and tab leading, space and tab in middle and mix of CR LF and LF EOL.
    std::vector<char> test3{ 
        ' ', ' ', ' ', ' ', ' ', ' ', 'M', 'i', 'x', ' ', '1', '\r', '\n', 
        ' ', ' ', ' ', ' ', ' ', ' ', 'C', 'R', 'L', 'F', '.', 'm', '\n', 
        ' ', ' ', ' ', ' ', '\r', '\n', 
        ' ', ' ', ' ', ' ', ' ', '\n', 
        ' ', ' ', ' ', ' ', 'H', '\t', 'i', '\r', '\n', 
        ' ', 'H', '\t', 'i', '\r', '\n', 
        'H', '\t', 'i', '\n', 
        'H', ' ', 'i', '\r', '\n', 
        '\r', '\n' 
    };
    input = compareDir + "/test3" + postfix + ".txt";
    writeBinaryFile(input, test3);

// test4.txt : A mix of space and tab leading, space and tab in middle and malformed EOL.
    std::vector<char> test4{ 
        ' ', ' ', ' ', ' ', ' ', ' ', 'S', 'u', 'b', ' ', '1', '\n', '\r', 
        ' ', ' ', ' ', ' ', ' ', ' ', 'L', 'F', 'C', 'R', '.', 'm', '\n', '\r', 
        ' ', ' ', ' ', ' ', '\n', '\r', 
        ' ', ' ', ' ', ' ', ' ', '\n', '\r', 
        ' ', ' ', ' ', ' ', 'H', '\t', 'i', '\n', '\r', 
        ' ', 'H', '\t', 'i', '\n', '\r', 
        'H', '\t', 'i', '\n', '\r', 
        'H', ' ', 'i', '\n', '\r', 
        '\n', '\r' 
    };
    input = compareDir + "/test4" + postfix + ".txt";
    writeBinaryFile(input, test4);

    return 0;
}


/**
 * @section test leading tab generation.
 *
 * tfc -t -i testdata/input/testX.txt -o testdata/output/testXt.txt
 */
int tabTests(void)
{
    const std::string & postfix{"t"};
    std::string input{};

// test1.txt : A mix of space and tab leading, space and tab in middle and CR LF EOL.
    std::vector<char> test1{ 
        '\t', ' ', ' ', 'S', 'u', 'b', ' ', '1', '\r', '\n', 
        '\t', ' ', ' ', 'C', 'R', 'L', 'F', '.', 'm', '\r', '\n', 
        '\t', '\r', '\n', 
        '\t', ' ', '\r', '\n', 
        '\t', 'H', '\t', 'i', '\r', '\n', 
        ' ', 'H', '\t', 'i', '\r', '\n', 
        'H', '\t', 'i', '\r', '\n', 
        'H', ' ', 'i', '\r', '\n', 
        '\r', '\n' 
    };
    input = compareDir + "/test1" + postfix + ".txt";
    writeBinaryFile(input, test1);

// test2.txt : A mix of space and tab leading, space and tab in middle and only LF EOL.
    std::vector<char> test2{ 
        '\t', ' ', ' ', 'S', 'u', 'b', ' ', '1', '\n', 
        '\t', ' ', ' ', 'L', 'F', '.', 'm', '\n', 
        '\t', '\n', 
        '\t', ' ', '\n', 
        '\t', 'H', '\t', 'i', '\n', 
        ' ', 'H', '\t', 'i', '\n', 
        'H', '\t', 'i', '\n', 
        'H', ' ', 'i', '\n', 
        '\n' 
    };
    input = compareDir + "/test2" + postfix + ".txt";
    writeBinaryFile(input, test2);

// test3.txt : A mix of space and tab leading, space and tab in middle and mix of CR LF and LF EOL.
    std::vector<char> test3{ 
        '\t', ' ', ' ', 'M', 'i', 'x', ' ', '1', '\r', '\n', 
        '\t', ' ', ' ', 'C', 'R', 'L', 'F', '.', 'm', '\n', 
        '\t', '\r', '\n', 
        '\t', ' ', '\n', 
        '\t', 'H', '\t', 'i', '\r', '\n', 
        ' ', 'H', '\t', 'i', '\r', '\n', 
        'H', '\t', 'i', '\n', 
        'H', ' ', 'i', '\r', '\n', 
        '\r', '\n' 
    };
    input = compareDir + "/test3" + postfix + ".txt";
    writeBinaryFile(input, test3);

// test4.txt : A mix of space and tab leading, space and tab in middle and malformed EOL.
    std::vector<char> test4{ 
        '\t', ' ', ' ', 'S', 'u', 'b', ' ', '1', '\n', '\r', 
        '\t', ' ', ' ', 'L', 'F', 'C', 'R', '.', 'm', '\n', '\r', 
        '\t', '\n', '\r', 
        '\t', ' ', '\n', '\r', 
        '\t', 'H', '\t', 'i', '\n', '\r', 
        ' ', 'H', '\t', 'i', '\n', '\r', 
        'H', '\t', 'i', '\n', '\r', 
        'H', ' ', 'i', '\n', '\r', 
        '\n', '\r' 
    };
    input = compareDir + "/test4" + postfix + ".txt";
    writeBinaryFile(input, test4);

    return 0;
}


/**
 * @section test trailing dos generation.
 *
 * tfc -d -i testdata/input/testX.txt -o testdata/output/testXd.txt
 */
int dosTests(void)
{
    const std::string & postfix{"d"};
    std::string input{};

// test1.txt : A mix of space and tab leading, space and tab in middle and CR LF EOL.
    std::vector<char> test1{ 
        '\t', ' ', ' ', 'S', 'u', 'b', ' ', '1', '\r', '\n', 
        ' ', '\t', ' ', ' ', 'C', 'R', 'L', 'F', '.', 'm', '\r', '\n', 
        ' ', '\t', '\r', '\n', 
        '\t', ' ', '\r', '\n', 
        '\t', 'H', '\t', 'i', '\r', '\n', 
        ' ', 'H', '\t', 'i', '\r', '\n', 
        'H', '\t', 'i', '\r', '\n', 
        'H', ' ', 'i', '\r', '\n', 
        '\r', '\n' 
    };
    input = compareDir + "/test1" + postfix + ".txt";
    writeBinaryFile(input, test1);

// test2.txt : A mix of space and tab leading, space and tab in middle and only LF EOL.
    std::vector<char> test2{ 
        '\t', ' ', ' ', 'S', 'u', 'b', ' ', '1', '\r', '\n', 
        ' ', '\t', ' ', ' ', 'L', 'F', '.', 'm', '\r', '\n', 
        ' ', '\t', '\r', '\n', 
        '\t', ' ', '\r', '\n', 
        '\t', 'H', '\t', 'i', '\r', '\n', 
        ' ', 'H', '\t', 'i', '\r', '\n', 
        'H', '\t', 'i', '\r', '\n', 
        'H', ' ', 'i', '\r', '\n', 
        '\r', '\n' 
    };
    input = compareDir + "/test2" + postfix + ".txt";
    writeBinaryFile(input, test2);

// test3.txt : A mix of space and tab leading, space and tab in middle and mix of CR LF and LF EOL.
    std::vector<char> test3{ 
        '\t', ' ', ' ', 'M', 'i', 'x', ' ', '1', '\r', '\n', 
        ' ', '\t', ' ', ' ', 'C', 'R', 'L', 'F', '.', 'm', '\r', '\n', 
        ' ', '\t', '\r', '\n', 
        '\t', ' ', '\r', '\n', 
        '\t', 'H', '\t', 'i', '\r', '\n', 
        ' ', 'H', '\t', 'i', '\r', '\n', 
        'H', '\t', 'i', '\r', '\n', 
        'H', ' ', 'i', '\r', '\n', 
        '\r', '\n' 
    };
    input = compareDir + "/test3" + postfix + ".txt";
    writeBinaryFile(input, test3);

// test4.txt : A mix of space and tab leading, space and tab in middle and malformed EOL.
    std::vector<char> test4{ 
        '\t', ' ', ' ', 'S', 'u', 'b', ' ', '1', '\r', '\n', 
        ' ', '\t', ' ', ' ', 'L', 'F', 'C', 'R', '.', 'm', '\r', '\n', 
        ' ', '\t', '\r', '\n', 
        '\t', ' ', '\r', '\n', 
        '\t', 'H', '\t', 'i', '\r', '\n', 
        ' ', 'H', '\t', 'i', '\r', '\n', 
        'H', '\t', 'i', '\r', '\n', 
        'H', ' ', 'i', '\r', '\n', 
        '\r', '\n' 
    };
    input = compareDir + "/test4" + postfix + ".txt";
    writeBinaryFile(input, test4);

    return 0;
}


/**
 * @section test trailing unix generation.
 *
 * tfc -u -i testdata/input/testX.txt -o testdata/output/testXu.txt
 */
int unixTests(void)
{
    const std::string & postfix{"u"};
    std::string input{};

// test1.txt : A mix of space and tab leading, space and tab in middle and CR LF EOL.
    std::vector<char> test1{ 
        '\t', ' ', ' ', 'S', 'u', 'b', ' ', '1', '\n', 
        ' ', '\t', ' ', ' ', 'C', 'R', 'L', 'F', '.', 'm', '\n', 
        ' ', '\t', '\n', 
        '\t', ' ', '\n', 
        '\t', 'H', '\t', 'i', '\n', 
        ' ', 'H', '\t', 'i', '\n', 
        'H', '\t', 'i', '\n', 
        'H', ' ', 'i', '\n', 
        '\n' 
    };
    input = compareDir + "/test1" + postfix + ".txt";
    writeBinaryFile(input, test1);

// test2.txt : A mix of space and tab leading, space and tab in middle and only LF EOL.
    std::vector<char> test2{ 
        '\t', ' ', ' ', 'S', 'u', 'b', ' ', '1', '\n', 
        ' ', '\t', ' ', ' ', 'L', 'F', '.', 'm', '\n', 
        ' ', '\t', '\n', 
        '\t', ' ', '\n', 
        '\t', 'H', '\t', 'i', '\n', 
        ' ', 'H', '\t', 'i', '\n', 
        'H', '\t', 'i', '\n', 
        'H', ' ', 'i', '\n', 
        '\n' 
    };
    input = compareDir + "/test2" + postfix + ".txt";
    writeBinaryFile(input, test2);

// test3.txt : A mix of space and tab leading, space and tab in middle and mix of CR LF and LF EOL.
    std::vector<char> test3{ 
        '\t', ' ', ' ', 'M', 'i', 'x', ' ', '1', '\n', 
        ' ', '\t', ' ', ' ', 'C', 'R', 'L', 'F', '.', 'm', '\n', 
        ' ', '\t', '\n', 
        '\t', ' ', '\n', 
        '\t', 'H', '\t', 'i', '\n', 
        ' ', 'H', '\t', 'i', '\n', 
        'H', '\t', 'i', '\n', 
        'H', ' ', 'i', '\n', 
        '\n' 
    };
    input = compareDir + "/test3" + postfix + ".txt";
    writeBinaryFile(input, test3);

// test4.txt : A mix of space and tab leading, space and tab in middle and malformed EOL.
    std::vector<char> test4{ 
        '\t', ' ', ' ', 'S', 'u', 'b', ' ', '1', '\n', 
        ' ', '\t', ' ', ' ', 'L', 'F', 'C', 'R', '.', 'm', '\n', 
        ' ', '\t', '\n', 
        '\t', ' ', '\n', 
        '\t', 'H', '\t', 'i', '\n', 
        ' ', 'H', '\t', 'i', '\n', 
        'H', '\t', 'i', '\n', 
        'H', ' ', 'i', '\n', 
        '\n' 
    };
    input = compareDir + "/test4" + postfix + ".txt";
    writeBinaryFile(input, test4);

    return 0;
}


/**
 * @section test leading space and trailing dos generation.
 *
 * tfc -s -d -i testdata/input/testX.txt -o testdata/output/testXsd.txt
 */
int spaceDosTests(void)
{
    const std::string & postfix{"sd"};
    std::string input{};

// test1.txt : A mix of space and tab leading, space and tab in middle and CR LF EOL.
    std::vector<char> test1{ 
        ' ', ' ', ' ', ' ', ' ', ' ', 'S', 'u', 'b', ' ', '1', '\r', '\n', 
        ' ', ' ', ' ', ' ', ' ', ' ', 'C', 'R', 'L', 'F', '.', 'm', '\r', '\n', 
        ' ', ' ', ' ', ' ', '\r', '\n', 
        ' ', ' ', ' ', ' ', '\r', '\n', 
        ' ', ' ', ' ', ' ', 'H', '\t', 'i', '\r', '\n', 
        ' ', 'H', '\t', 'i', '\r', '\n', 
        'H', '\t', 'i', '\r', '\n', 
        'H', ' ', 'i', '\r', '\n', 
        '\r', '\n' 
    };
    input = compareDir + "/test1" + postfix + ".txt";
    writeBinaryFile(input, test1);

// test2.txt : A mix of space and tab leading, space and tab in middle and only LF EOL.
    std::vector<char> test2{ 
        ' ', ' ', ' ', ' ', ' ', ' ', 'S', 'u', 'b', ' ', '1', '\r', '\n', 
        ' ', ' ', ' ', ' ', ' ', ' ', 'L', 'F', '.', 'm', '\r', '\n', 
        ' ', ' ', ' ', ' ', '\r', '\n', 
        ' ', ' ', ' ', ' ', '\r', '\n', 
        ' ', ' ', ' ', ' ', 'H', '\t', 'i', '\r', '\n', 
        ' ', 'H', '\t', 'i', '\r', '\n', 
        'H', '\t', 'i', '\r', '\n', 
        'H', ' ', 'i', '\r', '\n', 
        '\r', '\n' 
    };
    input = compareDir + "/test2" + postfix + ".txt";
    writeBinaryFile(input, test2);

// test3.txt : A mix of space and tab leading, space and tab in middle and mix of CR LF and LF EOL.
    std::vector<char> test3{ 
        ' ', ' ', ' ', ' ', ' ', ' ', 'M', 'i', 'x', ' ', '1', '\r', '\n', 
        ' ', ' ', ' ', ' ', ' ', ' ', 'C', 'R', 'L', 'F', '.', 'm', '\r', '\n', 
        ' ', ' ', ' ', ' ', '\r', '\n', 
        ' ', ' ', ' ', ' ', '\r', '\n', 
        ' ', ' ', ' ', ' ', 'H', '\t', 'i', '\r', '\n', 
        ' ', 'H', '\t', 'i', '\r', '\n', 
        'H', '\t', 'i', '\r', '\n', 
        'H', ' ', 'i', '\r', '\n', 
        '\r', '\n' 
    };
    input = compareDir + "/test3" + postfix + ".txt";
    writeBinaryFile(input, test3);

// test4.txt : A mix of space and tab leading, space and tab in middle and malformed EOL.
    std::vector<char> test4{ 
        ' ', ' ', ' ', ' ', ' ', ' ', 'S', 'u', 'b', ' ', '1', '\r', '\n', 
        ' ', ' ', ' ', ' ', ' ', ' ', 'L', 'F', 'C', 'R', '.', 'm', '\r', '\n', 
        ' ', ' ', ' ', ' ', '\r', '\n', 
        ' ', ' ', ' ', ' ', '\r', '\n', 
        ' ', ' ', ' ', ' ', 'H', '\t', 'i', '\r', '\n', 
        ' ', 'H', '\t', 'i', '\r', '\n', 
        'H', '\t', 'i', '\r', '\n', 
        'H', ' ', 'i', '\r', '\n', 
        '\r', '\n' 
    };
    input = compareDir + "/test4" + postfix + ".txt";
    writeBinaryFile(input, test4);

    return 0;
}


/**
 * @section test leading tab and trailing dos generation.
 *
 * tfc -t -d -i testdata/input/testX.txt -o testdata/output/testXtd.txt
 */
int tabDosTests(void)
{
    const std::string & postfix{"td"};
    std::string input{};

// test1.txt : A mix of space and tab leading, space and tab in middle and CR LF EOL.
    std::vector<char> test1{ 
        '\t', ' ', ' ', 'S', 'u', 'b', ' ', '1', '\r', '\n', 
        '\t', ' ', ' ', 'C', 'R', 'L', 'F', '.', 'm', '\r', '\n', 
        '\t', '\r', '\n', 
        '\t', ' ', '\r', '\n', 
        '\t', 'H', '\t', 'i', '\r', '\n', 
        ' ', 'H', '\t', 'i', '\r', '\n', 
        'H', '\t', 'i', '\r', '\n', 
        'H', ' ', 'i', '\r', '\n', 
        '\r', '\n' 
    };
    input = compareDir + "/test1" + postfix + ".txt";
    writeBinaryFile(input, test1);

// test2.txt : A mix of space and tab leading, space and tab in middle and only LF EOL.
    std::vector<char> test2{ 
        '\t', ' ', ' ', 'S', 'u', 'b', ' ', '1', '\r', '\n', 
        '\t', ' ', ' ', 'L', 'F', '.', 'm', '\r', '\n', 
        '\t', '\r', '\n', 
        '\t', ' ', '\r', '\n', 
        '\t', 'H', '\t', 'i', '\r', '\n', 
        ' ', 'H', '\t', 'i', '\r', '\n', 
        'H', '\t', 'i', '\r', '\n', 
        'H', ' ', 'i', '\r', '\n', 
        '\r', '\n' 
    };
    input = compareDir + "/test2" + postfix + ".txt";
    writeBinaryFile(input, test2);

// test3.txt : A mix of space and tab leading, space and tab in middle and mix of CR LF and LF EOL.
    std::vector<char> test3{ 
        '\t', ' ', ' ', 'M', 'i', 'x', ' ', '1', '\r', '\n', 
        '\t', ' ', ' ', 'C', 'R', 'L', 'F', '.', 'm', '\r', '\n', 
        '\t', '\r', '\n', 
        '\t', ' ', '\r', '\n', 
        '\t', 'H', '\t', 'i', '\r', '\n', 
        ' ', 'H', '\t', 'i', '\r', '\n', 
        'H', '\t', 'i', '\r', '\n', 
        'H', ' ', 'i', '\r', '\n', 
        '\r', '\n' 
    };
    input = compareDir + "/test3" + postfix + ".txt";
    writeBinaryFile(input, test3);

// test4.txt : A mix of space and tab leading, space and tab in middle and malformed EOL.
    std::vector<char> test4{ 
        '\t', ' ', ' ', 'S', 'u', 'b', ' ', '1', '\r', '\n', 
        '\t', ' ', ' ', 'C', 'F', 'C', 'R', '.', 'm', '\r', '\n', 
        '\t', '\r', '\n', 
        '\t', ' ', '\r', '\n', 
        '\t', 'H', '\t', 'i', '\r', '\n', 
        ' ', 'H', '\t', 'i', '\r', '\n', 
        'H', '\t', 'i', '\r', '\n', 
        'H', ' ', 'i', '\r', '\n', 
        '\r', '\n' 
    };
    input = compareDir + "/test4" + postfix + ".txt";
    writeBinaryFile(input, test4);

    return 0;
}


/**
 * @section test leading space and trailing unix generation.
 *
 * tfc -s -u -i testdata/input/testX.txt -o testdata/output/testXsu.txt
 */
int spaceUnixTests(void)
{
    const std::string & postfix{"su"};
    std::string input{};

// test1.txt : A mix of space and tab leading, space and tab in middle and CR LF EOL.
    std::vector<char> test1{ 
        ' ', ' ', ' ', ' ', ' ', ' ', 'S', 'u', 'b', ' ', '1', '\n', 
        ' ', ' ', ' ', ' ', ' ', ' ', 'C', 'R', 'L', 'F', '.', 'm', '\n', 
        ' ', ' ', ' ', ' ', '\n', 
        ' ', ' ', ' ', ' ', ' ', '\n', 
        ' ', ' ', ' ', ' ', 'H', '\t', 'i', '\n', 
        ' ', 'H', '\t', 'i', '\n', 
        'H', '\t', 'i', '\n', 
        'H', ' ', 'i', '\n', 
        '\n' 
    };
    input = compareDir + "/test1" + postfix + ".txt";
    writeBinaryFile(input, test1);

// test2.txt : A mix of space and tab leading, space and tab in middle and only LF EOL.
    std::vector<char> test2{ 
        ' ', ' ', ' ', ' ', ' ', ' ', 'S', 'u', 'b', ' ', '1', '\n', 
        ' ', ' ', ' ', ' ', ' ', ' ', 'L', 'F', '.', 'm', '\n', 
        ' ', ' ', ' ', ' ', '\n', 
        ' ', ' ', ' ', ' ', ' ', '\n', 
        ' ', ' ', ' ', ' ', 'H', '\t', 'i', '\n', 
        ' ', 'H', '\t', 'i', '\n', 
        'H', '\t', 'i', '\n', 
        'H', ' ', 'i', '\n', 
        '\n' 
    };
    input = compareDir + "/test2" + postfix + ".txt";
    writeBinaryFile(input, test2);

// test3.txt : A mix of space and tab leading, space and tab in middle and mix of CR LF and LF EOL.
    std::vector<char> test3{ 
        ' ', ' ', ' ', ' ', ' ', ' ', 'M', 'i', 'x', ' ', '1', '\n', 
        ' ', ' ', ' ', ' ', ' ', ' ', 'C', 'R', 'L', 'F', '.', 'm', '\n', 
        ' ', ' ', ' ', ' ', '\n', 
        ' ', ' ', ' ', ' ', ' ', '\n', 
        ' ', ' ', ' ', ' ', 'H', '\t', 'i', '\n', 
        ' ', 'H', '\t', 'i', '\n', 
        'H', '\t', 'i', '\n', 
        'H', ' ', 'i', '\n', 
        '\n' 
    };
    input = compareDir + "/test3" + postfix + ".txt";
    writeBinaryFile(input, test3);

// test4.txt : A mix of space and tab leading, space and tab in middle and malformed EOL.
    std::vector<char> test4{ 
        ' ', ' ', ' ', ' ', ' ', ' ', 'S', 'u', 'b', ' ', '1', '\n', 
        ' ', ' ', ' ', ' ', ' ', ' ', 'L', 'F', 'C', 'R', '.', 'm', '\n', 
        ' ', ' ', ' ', ' ', '\n', 
        ' ', ' ', ' ', ' ', ' ', '\n', 
        ' ', ' ', ' ', ' ', 'H', '\t', 'i', '\n', 
        ' ', 'H', '\t', 'i', '\n', 
        'H', '\t', 'i', '\n', 
        'H', ' ', 'i', '\n', 
        '\n' 
    };
    input = compareDir + "/test4" + postfix + ".txt";
    writeBinaryFile(input, test4);

    return 0;
}


/**
 * @section test leading tab and trailing unix generation.
 *
 * tfc -t -u -i testdata/input/testX.txt -o testdata/output/testXtu.txt
 */
int tabUnixTests(void)
{
    const std::string & postfix{"tu"};
    std::string input{};

// test1.txt : A mix of space and tab leading, space and tab in middle and CR LF EOL.
    std::vector<char> test1{ 
        '\t', ' ', ' ', 'S', 'u', 'b', ' ', '1', '\n', 
        '\t', ' ', ' ', 'C', 'R', 'L', 'F', '.', 'm', '\n', 
        '\t', '\n', 
        '\t', ' ', '\n', 
        '\t', 'H', '\t', 'i', '\n', 
        ' ', 'H', '\t', 'i', '\n', 
        'H', '\t', 'i', '\n', 
        'H', ' ', 'i', '\n', 
        '\n' 
    };
    input = compareDir + "/test1" + postfix + ".txt";
    writeBinaryFile(input, test1);

// test2.txt : A mix of space and tab leading, space and tab in middle and only LF EOL.
    std::vector<char> test2{ 
        '\t', ' ', ' ', 'S', 'u', 'b', ' ', '1', '\n', 
        '\t', ' ', ' ', 'L', 'F', '.', 'm', '\n', 
        '\t', '\n', 
        '\t', ' ', '\n', 
        '\t', 'H', '\t', 'i', '\n', 
        ' ', 'H', '\t', 'i', '\n', 
        'H', '\t', 'i', '\n', 
        'H', ' ', 'i', '\n', 
        '\n' 
    };
    input = compareDir + "/test2" + postfix + ".txt";
    writeBinaryFile(input, test2);

// test3.txt : A mix of space and tab leading, space and tab in middle and mix of CR LF and LF EOL.
    std::vector<char> test3{ 
        '\t', ' ', ' ', 'M', 'i', 'x', ' ', '1', '\n', 
        '\t', ' ', ' ', 'C', 'R', 'L', 'F', '.', 'm', '\n', 
        '\t', '\n', 
        '\t', ' ', '\n', 
        '\t', 'H', '\t', 'i', '\n', 
        ' ', 'H', '\t', 'i', '\n', 
        'H', '\t', 'i', '\n', 
        'H', ' ', 'i', '\n', 
        '\n' 
    };
    input = compareDir + "/test3" + postfix + ".txt";
    writeBinaryFile(input, test3);

// test4.txt : A mix of space and tab leading, space and tab in middle and malformed EOL.
    std::vector<char> test4{ 
        '\t', ' ', ' ', 'S', 'u', 'b', ' ', '1', '\n', 
        '\t', ' ', ' ', 'L', 'F', 'C', 'R', '.', 'm', '\n', 
        '\t', '\n', 
        '\t', ' ', '\n', 
        '\t', 'H', '\t', 'i', '\n', 
        ' ', 'H', '\t', 'i', '\n', 
        'H', '\t', 'i', '\n', 
        'H', ' ', 'i', '\n', 
        '\n' 
    };
    input = compareDir + "/test4" + postfix + ".txt";
    writeBinaryFile(input, test4);

    return 0;
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
    deleteDirectory(rootDir);
    createDirectory(rootDir);
    createDirectory(inputDir);
    createDirectory(outputDir);
    createDirectory(compareDir);

    summaryTests(argc, argv);
//    genTestScript("runTests.sh", argv[0]);

    spaceTests();
    tabTests();
    dosTests();
    unixTests();
    spaceDosTests();
    tabDosTests();
    spaceUnixTests();
    tabUnixTests();

    return 0;
}
