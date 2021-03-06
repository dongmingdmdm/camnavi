#include "main.h"

static Mat const frequency_filter = cv::imread (SRCDIR"/masks/filter.jpg", CV_LOAD_IMAGE_GRAYSCALE);

static void
process (Mat const &src, Mat &dst)
{
  fft_filter (src, frequency_filter, NULL, &dst);
}
