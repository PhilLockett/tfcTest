/**
 * @file    BinaryFile.h
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
 * Interface for the Statistics Implementation.
 */

#if !defined(_BINARYFILE_H__20210503_1033__INCLUDED_)
#define _BINARYFILE_H__20210503_1033__INCLUDED_

#include <vector>
#include <string>
#include <fstream>
#include <filesystem>


/**
 * @section Statistics Singleton.
 *
 * Implementation of the Statistics Singleton.
 */

template<typename T=char>
class BinaryFile
{
public:
    using Iterator = std::vector<T>::const_iterator;

    BinaryFile(const std::string & file) : fileName{file} {}
    virtual ~BinaryFile(void) {}

    BinaryFile(const BinaryFile & other) : fileName{other.fileName}, data{other.data} {}
    void operator=(const BinaryFile & other) { fileName = other.fileName; data = other.data; }

    friend std::ostream & operator<<(std::ostream &os, const BinaryFile &A) { A.display(os); return os; }

    void load(const std::vector<T> & other) { data = other; }
    bool equal(const BinaryFile & other) const { return std::equal(data.begin(), data.end(), other.data.begin()); }
    void clear(void) { data.clear(); }

    void setFileName(const std::string & file) { fileName = file; }
    std::string getFileName(void) const { return fileName; }
    bool exists(void) const { return std::filesystem::exists(fileName); }

    size_t size(void) { return data.size(); }
    Iterator begin(void) { return data.begin(); }
    Iterator end(void) { return data.end(); }

    int write(const std::vector<T> & other) { load(other); return write(); }
    int write(void) const;
    int read(int reserve = 100);

private:
    void display(std::ostream &os) const;

    std::filesystem::path fileName; 
    std::vector<T> data;
};

template<typename T>
int BinaryFile<T>::write(void) const
{
    if (std::ofstream os{fileName, std::ios::binary})
    {
        for (auto & c : data)
            os.put(c);

        return 0;
    }

    return 1;
}

template<typename T>
int BinaryFile<T>::read(int reserve)
{
    if (std::ifstream is{fileName, std::ios::binary})
    {
        data.reserve(reserve);
        T event;
        for (is.get(event); !is.eof(); is.get(event))
            data.push_back(event);

        return 0;
    }

    return 1;
}



/**
 * Send the current name-value pairs to the output stream.
 *
 * @param  os - Output stream.
 */
template<typename T>
void BinaryFile<T>::display(std::ostream &os) const
{
    for (const auto & x: data)
        os << x << "\n";
}


#endif // !defined(_BINARYFILE_H__20210503_1033__INCLUDED_)

