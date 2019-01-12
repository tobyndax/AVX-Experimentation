/* Author Jens Edhammer
*/
#include "pgm.hpp"
#include <iostream>
#include <sstream>

SimpleImage::SimpleImage(
    const uint64_t aWidth,
    const uint64_t aHeight)
{
    imgInfo.width = aWidth;
    imgInfo.width = aHeight;
    data.resize(imgInfo.width * imgInfo.height);
}
SimpleImage::SimpleImage(
    const ImageInfo imgInfo)
{
    setImageInfo(imgInfo);
    data.resize(imgInfo.width * imgInfo.height);
}
SimpleImage::~SimpleImage()
{
}
const std::vector<uint8_t>& SimpleImage::getData() const{
    return data;
}
std::vector<uint8_t>& SimpleImage::getData()
{
    return data;
}
void SimpleImage::setImageInfo(const ImageInfo info)
{
    imgInfo = info;
}
ImageInfo SimpleImage::getImageInfo() const
{
    return imgInfo;
}

PGMReader::PGMReader(/* args */)
{
}

PGMReader::~PGMReader()
{
}

SimpleImage *PGMReader::read(std::string filePath)
{
    std::ifstream inputStream{filePath};
    if (!inputStream.good() || !inputStream.is_open())
    {
        throw std::invalid_argument("Input stream was not good");
    }
    //read first line, clean it and check for the magic number.
    ImageInfo imgInfo = resolveHeader(inputStream);
    if(imgInfo.maxVal > 255){
        throw std::domain_error("MaxValue larger than supported by this implementation (max 255)");
    }

    SimpleImage* img = new SimpleImage{imgInfo};
    std::vector<uint8_t>& results = img->getData();
    std::string line;
    size_t pos = 0;
    while (std::getline(inputStream, line))
    {
        removeComment(line);
        std::vector<uint64_t> partialRes = splitIntoValues(line);
        for(size_t t = 0; t < partialRes.size(); t++){
            results[pos] = partialRes[t];
            pos++; 
        }
    }

    return img;
}

ImageInfo PGMReader::resolveHeader(std::istream& inputStream){
    std::string line;
    std::getline(inputStream, line);
    removeComment(line);

    bool ascii = checkASCIIHeader(line);
    if (!ascii)
    {
        throw std::invalid_argument("Header did not match ASCII PGM header");
    }
    //Read lines until we get a non-empty line, this have to be the width and height
    ImageInfo imgInfo;
    while (std::getline(inputStream, line))
    {
        removeComment(line);
        std::vector<uint64_t> partialRes = splitIntoValues(line);
        if (partialRes.size() == 0)
        {
            continue;
        }
        else if (partialRes.size() != 2)
        {
            throw std::domain_error("Unexpected number of values when searching for width and height");
        }
        else
        {
            imgInfo.width = partialRes[0];
            imgInfo.height = partialRes[1];
            break;
        }
    }
    //Read lines until we get a line without a comment, with length one, this will be the maxval.
    while (std::getline(inputStream, line))
    {
        removeComment(line);
        std::vector<uint64_t> partialRes = splitIntoValues(line);
        if (partialRes.size() == 0)
        {
            continue;
        }
        else if (partialRes.size() != 1)
        {
            throw std::domain_error("Unexpected number of values when searching for width and height");
        }
        else
        {
            imgInfo.maxVal = partialRes[0];
            break;
        }
    }

    //header is now resolved fully. 
    return imgInfo;
}

bool PGMReader::checkASCIIHeader(const std::string &line)
{
    return (line == "P2");
}
void PGMReader::removeComment(std::string &line)
{
    size_t pos = line.find_first_of('#');
    if (pos != std::string::npos)
    {
        line = line.substr(0, pos);
    }
}

std::vector<uint64_t> PGMReader::splitIntoValues(const std::string &line)
{
    std::vector<uint64_t> result;
    std::istringstream iss(line);
    for (std::string s; iss >> s;)
        result.push_back(std::stoi(s));
    return result;
}

PGMWriter::PGMWriter(/* args */)
{
}

PGMWriter::~PGMWriter()
{
}
// void PGMWriter::writeImageData(const SimpleImage& img, std::ofstream& outputStream){
//     std::string tempOutput = ""; 
//     std::vector<uint8_t> data = img.getData();
//     int valCounter = 0;
//     for(size_t i = 0; i < data.size(); i++){
//         uint8_t val = data[i];
//         std::string valString = std::to_string(val);
//         if(valCounter > 10){
//             outputStream << tempOutput << std::endl;
//             tempOutput = "";
//             valCounter = 0;
//         }
//         tempOutput = tempOutput + valString + " ";
//         valCounter++;
//     }
//     tempOutput.pop_back(); // Remove extraneous space. 
//     outputStream << tempOutput << std::endl; // output last line of data
//     return;
// }
void PGMWriter::writeImageData(const SimpleImage& img, std::ofstream& outputStream){
    std::string tempOutput = ""; 
    std::vector<uint8_t> data = img.getData();
    int valCounter = 0;
    for(size_t i = 0; i < data.size(); i++){
        uint8_t val = data[i];
        std::string valString = std::to_string(val);
        if(valString.size() + tempOutput.size() > 70){
            outputStream << tempOutput << std::endl;
            tempOutput = "";
        }
        tempOutput = tempOutput + valString + " ";
    }
    tempOutput.pop_back(); // Remove extraneous space. 
    outputStream << tempOutput << std::endl; // output last line of data
    return;
}

void PGMWriter::writeHeader(const ImageInfo info, std::ofstream& outputStream){
    //Write magic number
    outputStream << "P2" << std::endl;
    outputStream << info.width << " " << info.height << std::endl;
    outputStream << info.maxVal << std::endl;
}

void PGMWriter::write(const std::string filePath, const SimpleImage& data){
    std::ofstream outputStream;

    outputStream.open(filePath, std::ios::out | std::ios::trunc); 
    if(!outputStream.is_open()){
        throw std::invalid_argument("File could not be opened");
    }

    writeHeader(data.getImageInfo(),outputStream);

    writeImageData(data,outputStream);
}
