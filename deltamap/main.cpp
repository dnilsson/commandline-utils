#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "stb_image_write.h"
#include <string>
#include <vector>

struct DatePrice
{
    std::string date;
    double price;
};

std::vector<DatePrice> ReadDatePrices(const std::string& filename)
{
    std::vector<DatePrice> result;
    return result;
}


int main(int argc, char** argv)
{
    auto datePrices = ReadDatePrices("fund1.xls.html");


    unsigned char data[512 * 512 * 4];

    for (int i = 0; i < 512; i++)
        for (int j = 0; j < 512; j++)
        {
            unsigned char a = (unsigned char)(i / 2);
            unsigned char b = (unsigned char)(j / 2);
            data[i * 512 * 4 + j * 4 ] = a;         // R
            data[i * 512 * 4 + j * 4 + 1] = b;      // G
            data[i * 512 * 4 + j * 4 + 2] = 0;      // B
            data[i * 512 * 4 + j * 4 + 3] = 255;      // A
        }

    int result = stbi_write_png("image1.png", 512, 512, 4, data, 512 * 4);

    return 0;
}
