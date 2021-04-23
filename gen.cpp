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


/**
 * @section test script generation, currently not used.
 */

std::vector<std::tuple<std::string, std::string, std::string>> tests;

static void addTest(const std::string & test, const std::string & output, const std::string & expected)
{
    tests.emplace_back(test, output, expected);
}

static int genTestScript(const std::string & fileName, const char * program)
{
    namespace fs = std::filesystem;

    if (std::ofstream os{fileName, std::ios::out})
    {
        os << "#!/bin/sh\n";
        os << '\n';
        os << "# This file was generated as \"" << fileName << "\" using " << program << '\n';
        os << "#\n";
        os << '\n';
        for (auto [test, output, expected] : tests)
        {
            os << "echo Comparing " << output << " with " << expected << '\n';
            os << "tfc " << test;
            if (!output.empty())
                os << " -o " << output;
            os << '\n' << "diff " << output << ' ' << expected << '\n';
        }

        os.close();

        // Ensure output scripts are executable.
        fs::perms permissions{fs::perms::owner_all |
            fs::perms::group_read | fs::perms::group_exec | 
            fs::perms::others_read | fs::perms::others_exec};
        fs::permissions(fileName, permissions, fs::perm_options::add);

        return 0;
    }

    return 1;
}


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


static int writeBinaryFile(const std::string & fileName, const std::vector<char> & data)
{
    std::cout << "Generating file " << fileName << "\n";
    if (std::ofstream os{fileName, std::ios::binary})
    {
        for (auto & c : data)
            os.put(c);

        return 0;
    }

    return 1;
}

static int writeTextFile(const std::string & fileName, const std::vector<std::string> & lines)
{
    std::cout << "Generating text file " << fileName << "\n";
    if (std::ofstream os{fileName, std::ios::out})
    {
        for (auto & line : lines)
            os << line << '\n';

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
 * @section test summary generation.
 *
 */
int summaryTests(int argc, char *argv[])
{
    std::string filename{};
    std::string input{};
    std::string output{};
    std::string expected{};

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
    expected = expectedDir + filename;
    
    writeBinaryFile(input, test1);
    writeSummaryFile(expected, input, "9 1 1 3 4 9 0 0");
    addTest("-x -i " + input, output, expected);

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
    expected = expectedDir + filename;

    writeBinaryFile(input, test2);
    writeSummaryFile(expected, input, "9 1 1 3 4 0 9 0");
    addTest("-x -i " + input, output, expected);

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
    expected = expectedDir + filename;

    writeBinaryFile(input, test3);
    writeSummaryFile(expected, input, "9 1 1 3 4 6 3 0");
    addTest("-x -i " + input, output, expected);

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
    expected = expectedDir + filename;

    writeBinaryFile(input, test4);
    writeSummaryFile(expected, input, "9 1 1 3 4 0 0 9");
    addTest("-x -i " + input, output, expected);

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
    input = expectedDir + "/test1" + postfix + ".txt";
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
    input = expectedDir + "/test2" + postfix + ".txt";
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
    input = expectedDir + "/test3" + postfix + ".txt";
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
    input = expectedDir + "/test4" + postfix + ".txt";
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
    input = expectedDir + "/test1" + postfix + ".txt";
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
    input = expectedDir + "/test2" + postfix + ".txt";
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
    input = expectedDir + "/test3" + postfix + ".txt";
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
    input = expectedDir + "/test4" + postfix + ".txt";
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
    input = expectedDir + "/test1" + postfix + ".txt";
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
    input = expectedDir + "/test2" + postfix + ".txt";
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
    input = expectedDir + "/test3" + postfix + ".txt";
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
    input = expectedDir + "/test4" + postfix + ".txt";
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
    input = expectedDir + "/test1" + postfix + ".txt";
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
    input = expectedDir + "/test2" + postfix + ".txt";
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
    input = expectedDir + "/test3" + postfix + ".txt";
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
    input = expectedDir + "/test4" + postfix + ".txt";
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
        ' ', ' ', ' ', ' ', ' ', '\r', '\n', 
        ' ', ' ', ' ', ' ', 'H', '\t', 'i', '\r', '\n', 
        ' ', 'H', '\t', 'i', '\r', '\n', 
        'H', '\t', 'i', '\r', '\n', 
        'H', ' ', 'i', '\r', '\n', 
        '\r', '\n' 
    };
    input = expectedDir + "/test1" + postfix + ".txt";
    writeBinaryFile(input, test1);

// test2.txt : A mix of space and tab leading, space and tab in middle and only LF EOL.
    std::vector<char> test2{ 
        ' ', ' ', ' ', ' ', ' ', ' ', 'S', 'u', 'b', ' ', '1', '\r', '\n', 
        ' ', ' ', ' ', ' ', ' ', ' ', 'L', 'F', '.', 'm', '\r', '\n', 
        ' ', ' ', ' ', ' ', '\r', '\n', 
        ' ', ' ', ' ', ' ', ' ', '\r', '\n', 
        ' ', ' ', ' ', ' ', 'H', '\t', 'i', '\r', '\n', 
        ' ', 'H', '\t', 'i', '\r', '\n', 
        'H', '\t', 'i', '\r', '\n', 
        'H', ' ', 'i', '\r', '\n', 
        '\r', '\n' 
    };
    input = expectedDir + "/test2" + postfix + ".txt";
    writeBinaryFile(input, test2);

// test3.txt : A mix of space and tab leading, space and tab in middle and mix of CR LF and LF EOL.
    std::vector<char> test3{ 
        ' ', ' ', ' ', ' ', ' ', ' ', 'M', 'i', 'x', ' ', '1', '\r', '\n', 
        ' ', ' ', ' ', ' ', ' ', ' ', 'C', 'R', 'L', 'F', '.', 'm', '\r', '\n', 
        ' ', ' ', ' ', ' ', '\r', '\n', 
        ' ', ' ', ' ', ' ', ' ', '\r', '\n', 
        ' ', ' ', ' ', ' ', 'H', '\t', 'i', '\r', '\n', 
        ' ', 'H', '\t', 'i', '\r', '\n', 
        'H', '\t', 'i', '\r', '\n', 
        'H', ' ', 'i', '\r', '\n', 
        '\r', '\n' 
    };
    input = expectedDir + "/test3" + postfix + ".txt";
    writeBinaryFile(input, test3);

// test4.txt : A mix of space and tab leading, space and tab in middle and malformed EOL.
    std::vector<char> test4{ 
        ' ', ' ', ' ', ' ', ' ', ' ', 'S', 'u', 'b', ' ', '1', '\r', '\n', 
        ' ', ' ', ' ', ' ', ' ', ' ', 'L', 'F', 'C', 'R', '.', 'm', '\r', '\n', 
        ' ', ' ', ' ', ' ', '\r', '\n', 
        ' ', ' ', ' ', ' ', ' ', '\r', '\n', 
        ' ', ' ', ' ', ' ', 'H', '\t', 'i', '\r', '\n', 
        ' ', 'H', '\t', 'i', '\r', '\n', 
        'H', '\t', 'i', '\r', '\n', 
        'H', ' ', 'i', '\r', '\n', 
        '\r', '\n' 
    };
    input = expectedDir + "/test4" + postfix + ".txt";
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
    input = expectedDir + "/test1" + postfix + ".txt";
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
    input = expectedDir + "/test2" + postfix + ".txt";
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
    input = expectedDir + "/test3" + postfix + ".txt";
    writeBinaryFile(input, test3);

// test4.txt : A mix of space and tab leading, space and tab in middle and malformed EOL.
    std::vector<char> test4{ 
        '\t', ' ', ' ', 'S', 'u', 'b', ' ', '1', '\r', '\n', 
        '\t', ' ', ' ', 'L', 'F', 'C', 'R', '.', 'm', '\r', '\n', 
        '\t', '\r', '\n', 
        '\t', ' ', '\r', '\n', 
        '\t', 'H', '\t', 'i', '\r', '\n', 
        ' ', 'H', '\t', 'i', '\r', '\n', 
        'H', '\t', 'i', '\r', '\n', 
        'H', ' ', 'i', '\r', '\n', 
        '\r', '\n' 
    };
    input = expectedDir + "/test4" + postfix + ".txt";
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
    input = expectedDir + "/test1" + postfix + ".txt";
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
    input = expectedDir + "/test2" + postfix + ".txt";
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
    input = expectedDir + "/test3" + postfix + ".txt";
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
    input = expectedDir + "/test4" + postfix + ".txt";
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
    input = expectedDir + "/test1" + postfix + ".txt";
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
    input = expectedDir + "/test2" + postfix + ".txt";
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
    input = expectedDir + "/test3" + postfix + ".txt";
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
    input = expectedDir + "/test4" + postfix + ".txt";
    writeBinaryFile(input, test4);

    return 0;
}



/**
 * @section test leading space to tab replacement.
 * 
 * tfc -t -Y -i testdata/input/testSpace.txt -o testdata/output/testSpaceY.txt
 */

int spaceToTabTests(void)
{
    std::string filename{};
    std::string input{};
    std::string expected{};
    std::string postfix{};


    std::vector<std::string> testSpace{ 
        "0", 
        " 1", 
        "  2", 
        "   3", 
        "    4", 
        "     5", 
        "      6", 
        "       7", 
        "        8", 
        "         9"
    };
    filename = "/testSpace";
    input = inputDir + filename + ".txt";
    writeTextFile(input, testSpace);

    std::vector<std::string> testSpace2{ 
        "0", 
        " 1", 
        "\t2", 
        "\t 3", 
        "\t\t4", 
        "\t\t 5", 
        "\t\t\t6", 
        "\t\t\t 7", 
        "\t\t\t\t8", 
        "\t\t\t\t 9"
    };
    postfix = "2";
    expected = expectedDir + filename + postfix + ".txt";
    writeTextFile(expected, testSpace2);

    std::vector<std::string> testSpace4{ 
        "0", 
        " 1", 
        "  2", 
        "   3", 
        "\t4", 
        "\t 5", 
        "\t  6", 
        "\t   7", 
        "\t\t8", 
        "\t\t 9"
    };
    postfix = "4";
    expected = expectedDir + filename + postfix + ".txt";
    writeTextFile(expected, testSpace4);

    std::vector<std::string> testSpace8{ 
        "0", 
        " 1", 
        "  2", 
        "   3", 
        "    4", 
        "     5", 
        "      6", 
        "       7", 
        "\t8", 
        "\t 9"
    };
    postfix = "8";
    expected = expectedDir + filename + postfix + ".txt";
    writeTextFile(expected, testSpace8);

    return 0;
}

/**
 * @section test leading tab to space replacement.
 * 
 * tfc -s -Y -i testdata/input/testSpace.txt -o testdata/output/testSpaceY.txt
 */

int tabToSpaceTests(void)
{
    std::string filename{};
    std::string input{};
    std::string expected{};
    std::string postfix{};


    std::vector<std::string> testTab{ 
        "\t0", 
        " \t1", 
        "  \t2", 
        "   \t3", 
        "    \t4", 
        "     \t5", 
        "      \t6", 
        "       \t7", 
        "        \t8", 
        "         \t9"
    };
    filename = "/testTab";
    input = inputDir + filename + ".txt";
    writeTextFile(input, testTab);

    std::vector<std::string> testTab2{ 
        "  0", 
        "  1", 
        "    2", 
        "    3", 
        "      4", 
        "      5", 
        "        6", 
        "        7", 
        "          8", 
        "          9"
    };
    postfix = "2";
    expected = expectedDir + filename + postfix + ".txt";
    writeTextFile(expected, testTab2);

    std::vector<std::string> testTab4{ 
        "    0", 
        "    1", 
        "    2", 
        "    3", 
        "        4", 
        "        5", 
        "        6", 
        "        7", 
        "            8", 
        "            9"
    };
    postfix = "4";
    expected = expectedDir + filename + postfix + ".txt";
    writeTextFile(expected, testTab4);

    std::vector<std::string> testTab8{ 
        "        0", 
        "        1", 
        "        2", 
        "        3", 
        "        4", 
        "        5", 
        "        6", 
        "        7", 
        "                8", 
        "                9"
    };
    postfix = "8";
    expected = expectedDir + filename + postfix + ".txt";
    writeTextFile(expected, testTab8);

    return 0;
}


/**
 * @section test source file replacement.
 * 
 * tfc -r testdata/input/testOptions.txt
 */

int optionsTests(void)
{
    std::string input{};

    std::vector<std::string> testOptions{ 
        "Line 0", 
        "Line 1", 
        "Line 2", 
        "Line 3", 
        "Line 4"
    };
    input = inputDir + "/testOptions.txt";
    writeTextFile(input, testOptions);

    return 0;
}


/**
 * Test environment set up.
 *
 * @param  argc - command line argument count.
 * @param  argv - command line argument vector.
 * @return error value or 0 if no errors.
 */
int init(int argc, char *argv[])
{
    std::cout << "\nCreating test environment.\n";

    deleteDirectory(rootDir);
    createDirectory(rootDir);
    createDirectory(inputDir);
    createDirectory(outputDir);
    createDirectory(expectedDir);

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
    spaceToTabTests();
    tabToSpaceTests();
    optionsTests();

    return 0;
}
