#include <iostream>

extern "C" {
	#include "libavcodec/avcodec.h"
	#include "libavformat/avformat.h"
}

int main(int argc, char *argv[])
{
    std::cout << "hello world" << std::endl;
    std::cout << av_version_info() << std::endl;
    std::cout << "hello world" << std::endl;

    return 0;
}