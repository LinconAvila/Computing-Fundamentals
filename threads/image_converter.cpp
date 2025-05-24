#include <opencv2/opencv.hpp>
#include <iostream>
#include <pthread.h>

struct ThreadInfo
{
    cv::Mat *inputImage;
    cv::Mat *outputGrayImage;
    int startRow;
    int endRow;
    int threadIndex;
};

void *convertSectionToGray(void *arg)
{
    ThreadInfo *info = (ThreadInfo *)arg;
    int imageWidth = info->inputImage->cols;

    std::cout << "Thread " << info->threadIndex
              << " starting: lines " << info->startRow
              << " up to " << info->endRow - 1 << "\n";

    for (int row = info->startRow; row < info->endRow; ++row)
    {
        cv::Vec3b *colorPixels = info->inputImage->ptr<cv::Vec3b>(row);
        uchar *grayPixels = info->outputGrayImage->ptr<uchar>(row);

        for (int col = 0; col < imageWidth; ++col)
        {
            cv::Vec3b bgr = colorPixels[col];
            grayPixels[col] = static_cast<uchar>(0.11 * bgr[0] + 0.59 * bgr[1] + 0.30 * bgr[2]);
        }
    }

    std::cout << "Thread " << info->threadIndex << " terminou.\n";
    pthread_exit(nullptr);
}

int main()
{
    std::string imagePath;
    std::cout << "Type image's path: ";
    std::getline(std::cin, imagePath);

    cv::Mat inputImage = cv::imread(imagePath);

    if (inputImage.empty())
    {
        std::cerr << "Error loading image: " << imagePath << "\n";
        return 1;
    }

    cv::Mat outputGrayImage(inputImage.rows, inputImage.cols, CV_8UC1);

    const int THREAD_COUNT = 4;
    pthread_t threads[THREAD_COUNT];
    ThreadInfo threadData[THREAD_COUNT];

    int totalRows = inputImage.rows;
    int baseRows = totalRows / THREAD_COUNT;
    int remainder = totalRows % THREAD_COUNT;

    for (int t = 0; t < THREAD_COUNT; ++t)
    {
        int start = (t < remainder) ? t * (baseRows + 1) : t * baseRows + remainder;
        int rowsForThread = baseRows + (t < remainder ? 1 : 0);

        threadData[t].inputImage = &inputImage;
        threadData[t].outputGrayImage = &outputGrayImage;
        threadData[t].startRow = start;
        threadData[t].endRow = start + rowsForThread;
        threadData[t].threadIndex = t;

        pthread_create(&threads[t], nullptr, convertSectionToGray, &threadData[t]);
        std::cout << "Thread " << t
                  << " -> linhas [" << threadData[t].startRow
                  << ", " << threadData[t].endRow - 1 << "]\n";
    }

    for (int t = 0; t < THREAD_COUNT; ++t)
    {
        pthread_join(threads[t], nullptr);
    }

    cv::imwrite("output_gray.jpg", outputGrayImage);

    cv::namedWindow("Original Color", cv::WINDOW_NORMAL);
    cv::namedWindow("Gray Scale", cv::WINDOW_NORMAL);
    cv::resizeWindow("Original Color", 800, 600);
    cv::resizeWindow("Gray Scale", 800, 600);
    cv::imshow("Original Color", inputImage);
    cv::imshow("Gray Scale", outputGrayImage);
    cv::waitKey(0);

    return 0;
}
