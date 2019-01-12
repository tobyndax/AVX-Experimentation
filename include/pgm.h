/* Author Jens Edhammer
*/
#pragma once
#include <inttypes.h>
#include <string>
#include <vector>
#include <fstream>

struct ImageInfo{
    uint64_t width;
    uint64_t height;
    uint64_t maxVal; 
};

class SimpleImage
{
private:
    std::vector<uint8_t> data;
    ImageInfo imgInfo;
public:
    SimpleImage(const uint64_t width, const uint64_t height);
    SimpleImage(const ImageInfo imgInfo);
    ~SimpleImage();
    std::vector<uint8_t>& getData(); //Reference to the vector with the data.
    const std::vector<uint8_t>& getData() const; //Reference to the vector with the data.
    void setImageInfo(const ImageInfo);
    ImageInfo getImageInfo() const;
};

class PGMReader
{
private:
    bool checkASCIIHeader(const std::string& line);
    void removeComment(std::string& line);
    ImageInfo resolveHeader(std::istream& inputStream);
    std::vector<uint64_t> splitIntoValues(const std::string &line);

public:
    PGMReader();
    ~PGMReader();

    SimpleImage* read(const std::string filePath);
};

class PGMWriter
{
private:
    void writeHeader(const ImageInfo info, std::ofstream& outputStream);
    void writeImageData(const SimpleImage& img, std::ofstream& outputStream);
public:
    PGMWriter();
    ~PGMWriter();

    void write(const std::string filePath, const SimpleImage& data);
};