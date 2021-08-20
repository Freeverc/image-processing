#include "ImageSegmentor.h"

Mat theImage;
Mat theResult;
int openflag = 0;
QString showName = "OutputImage\\BasicSegment.jpg";;

ImageSegmentor::ImageSegmentor()
{
}

ImageSegmentor::~ImageSegmentor()
{
}

Mat Banlance(Mat src_img) {
	Mat img_eh;
	double thresh = 128;
	equalizeHist(src_img, img_eh);
	return img_eh;
};

Mat BasicSegment(Mat src_img, double thresh) {
	Mat img_seg;
	threshold(src_img, img_seg, thresh, 255, THRESH_BINARY);
	return img_seg;
};

Mat OtsuSegment(Mat src_img) {
	Mat img_seg;
	double thresh = 250;
	threshold(src_img, img_seg, thresh, 255, CV_THRESH_OTSU);
	return img_seg;
};

Mat MaxEntropySegment(Mat src)
{
	int tbHist[256] = { 0 };
	int index = 0;
	double Property = 0.0;
	double maxEntropy = -1.0;
	double frontEntropy = 0.0;
	double backEntropy = 0.0;
	//����������������
	int TotalPixel = 0;
	int nCol = src.cols * src.channels();
	for (int i = 0; i < src.rows; i++)
	{
		uchar* pData = src.ptr<uchar>(i);
		for (int j = 0; j < nCol; ++j)
		{
			++TotalPixel;
			tbHist[pData[j]] += 1;
		}
	}

	for (int i = 0; i < 256; i++)
	{
		//���㱳��������
		double backTotal = 0;
		for (int j = 0; j < i; j++)
		{
			backTotal += tbHist[j];
		}

		//������
		for (int j = 0; j < i; j++)
		{
			if (tbHist[j] != 0)
			{
				Property = tbHist[j] / backTotal;
				backEntropy += -Property * logf((float)Property);
			}
		}
		//ǰ����
		for (int k = i; k < 256; k++)
		{
			if (tbHist[k] != 0)
			{
				Property = tbHist[k] / (TotalPixel - backTotal);
				frontEntropy += -Property * logf((float)Property);
			}
		}

		if (frontEntropy + backEntropy > maxEntropy)    //�õ������
		{
			maxEntropy = frontEntropy + backEntropy;
			index = i;
		}
		//��ձ��μ�����ֵ
		frontEntropy = 0.0;
		backEntropy = 0.0;
	}
	Mat dst;
	//cout << "index:" << index;
	cv::threshold(src, dst, index, 255, 0);             //������ֵ�ָ�
	return dst.clone();
}

Mat KmeansSegment(Mat src_img) {
	int nCluster = 2;
	Mat samples(src_img.cols*src_img.rows, 1, CV_32FC3);

	Mat labels(src_img.cols*src_img.rows, 1, CV_32SC1);
	uchar* p;
	int i, j, k = 0;
	for (i = 0; i < src_img.rows; i++)
	{
		p = src_img.ptr<uchar>(i);
		for (j = 0; j < src_img.cols; j++)
		{
			samples.at<Vec3f>(k, 0)[0] = float(p[j]);
			samples.at<Vec3f>(k, 0)[1] = float(p[j + 1]);
			samples.at<Vec3f>(k, 0)[2] = float(p[j + 2]);
			k++;
		}
	}
	Mat centers(nCluster, 1, samples.type());
	kmeans(samples, nCluster, labels,
		TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 10, 1.0),
		3, KMEANS_PP_CENTERS, centers);
	Mat dst_img(src_img.rows, src_img.cols, CV_8UC1);

	for (i = 0, k = 0; i < dst_img.rows; i++)
	{
		p = dst_img.ptr<uchar>(i);
		for (j = 0; j < dst_img.cols; j++, k++)
		{
			int tt = labels.at<int>(k, 0);
			p[j] = 255 - tt * 255;
		}
	}
	return dst_img;
}

Mat RegionGrowSegment(Mat srcImage, Point pt, int ch1Thres, int ch1LowerBind = 0, int ch1UpperBind = 255)
{
	Point pToGrowing;                       //��������λ��
	int pGrowValue = 0;                             //��������Ҷ�ֵ
	Scalar pSrcValue = 0;                               //�������Ҷ�ֵ
	Scalar pCurValue = 0;                               //��ǰ������Ҷ�ֵ
	Mat growImage = Mat::zeros(srcImage.size(), CV_8UC1);   //����һ���հ��������Ϊ��ɫ
	//��������˳������
	int DIR[8][2] = { {-1,-1}, {0,-1}, {1,-1}, {1,0}, {1,1}, {0,1}, {-1,1}, {-1,0} };
	vector<Point> growPtVector;                     //������ջ
	growPtVector.push_back(pt);                         //��������ѹ��ջ��
	growImage.at<uchar>(pt.y, pt.x) = 255;              //���������
	pSrcValue = srcImage.at<uchar>(pt.y, pt.x);         //��¼������ĻҶ�ֵ

	while (!growPtVector.empty())                       //����ջ��Ϊ��������
	{
		pt = growPtVector.back();                       //ȡ��һ��������
		growPtVector.pop_back();

		//�ֱ�԰˸������ϵĵ��������
		for (int i = 0; i < 9; ++i)
		{
			pToGrowing.x = pt.x + DIR[i][0];
			pToGrowing.y = pt.y + DIR[i][1];
			//����Ƿ��Ǳ�Ե��
			if (pToGrowing.x < 0 || pToGrowing.y < 0 ||
				pToGrowing.x >(srcImage.cols - 1) || (pToGrowing.y > srcImage.rows - 1))
				continue;

			pGrowValue = growImage.at<uchar>(pToGrowing.y, pToGrowing.x);       //��ǰ��������ĻҶ�ֵ
			pSrcValue = srcImage.at<uchar>(pt.y, pt.x);
			if (pGrowValue == 0)                    //�����ǵ㻹û�б�����
			{
				pCurValue = srcImage.at<uchar>(pToGrowing.y, pToGrowing.x);
				if (pCurValue[0] <= ch1UpperBind && pCurValue[0] >= ch1LowerBind)
				{
					if (abs(pSrcValue[0] - pCurValue[0]) < ch1Thres)                   //����ֵ��Χ��������
					{
						growImage.at<uchar>(pToGrowing.y, pToGrowing.x) = 255;      //���Ϊ��ɫ
						growPtVector.push_back(pToGrowing);                 //����һ��������ѹ��ջ��
					}
				}
			}
		}
	}
	return growImage.clone();
}

Mat NewMethodSegment(Mat src)
{
	int tbHist[256] = { 0 };
	int MEindex = 0;
	double Property = 0.0;
	double maxEntropy = -1.0;
	double frontEntropy = 0.0;
	double backEntropy = 0.0;
	int TotalPixel = 0;
	int nCol = src.cols * src.channels();

	int n, n1, n2;
	int m1, m2;
	int k;
	double sum, csum;
	int OTindex = 255;
	double inter_varance = 0; // ��䷽��϶������ģ���������ʼֵΪ0
	double max_inter_varance = 0; // ��䷽��϶������ģ���������ʼֵΪ0

	for (int i = 0; i < src.rows; i++) // ����ʹ�ò���
	{
		uchar* pData = src.ptr<uchar>(i);
		for (int j = 0; j < nCol; ++j)
		{
			++TotalPixel;
			tbHist[pData[j]] += 1;
		}
	}
	sum = 0.0;
	n = 0;
	for (k = 0; k <= 255; k++)
	{
		sum += (double)k * (double)tbHist[k];
		n += tbHist[k]; //nΪͼ���ܵĵ�������һ��������ۻ�����
	}
	csum = 0.0;
	max_inter_varance = -1.0;
	n1 = 0;
	for (int i = 0; i < 256; i++)
	{
		double backTotal = 0;
		for (int j = 0; j < i; j++)
		{
			backTotal += tbHist[j];
		}
		////////////////////////////////////////
		for (int j = 0; j < i; j++)
		{
			if (tbHist[j] != 0)
			{
				Property = tbHist[j] / backTotal;
				backEntropy += -Property * logf((float)Property);
			}
		}
		for (k = i; k < 256; k++)
		{
			if (tbHist[k] != 0)
			{
				Property = tbHist[k] / (TotalPixel - backTotal);
				frontEntropy += -Property * logf((float)Property);
			}
		}
		if (frontEntropy + backEntropy > maxEntropy)
		{
			maxEntropy = frontEntropy + backEntropy;
			MEindex = i;
		}
		frontEntropy = 0.0;
		backEntropy = 0.0;

		//////////////////////////
		n1 += tbHist[k]; //n1Ϊ�ڵ�ǰ��ֵ��ǰ��ͼ��ĵ���
		if (n1 == 0) { continue; } //û�зֳ�ǰ����
		n2 = n - n1; //n2Ϊ����ͼ��ĵ���
		//n2Ϊ0��ʾȫ�����Ǻ�ͼ����n1=0������ƣ�֮��ı���������ʹǰ���������ӣ����Դ�ʱ�����˳�ѭ��
		if (n2 == 0) { break; }
		csum += (double)k * tbHist[k]; //ǰ���ġ��Ҷȵ�ֵ*����������ܺ�
		m1 = csum / n1;
		m2 = (sum - csum) / n2;

		inter_varance = (double)n1 * (double)n2 * (m1 - m2) * (m1 - m2);
		//cout << sum << csum << "inter:" << inter_varance<< endl;
		if (inter_varance > max_inter_varance)
		{
			max_inter_varance = inter_varance;
			OTindex = i;
		}
	}
	Mat dst;
	// cout << "MEindex:" << MEindex << endl;
	// cout << "OTindex:" << OTindex << endl;
	int index = (MEindex + OTindex) / 2;
	// cout << "index:"<<index << endl;
	threshold(src, dst, index, 255, 0);
	return dst.clone();
}

void ImageSegmentor::exeSegment()
{
	if (openflag == 0)
		return;
	else
	{
		//imshow("ԭʼͼƬ", theImage);
		//�ȴ�2s�󴰿��Զ��ر�
		//waitKey(2000);
		Mat img_gray;
		cvtColor(theImage, img_gray, COLOR_BGR2GRAY); //תΪ�Ҷ�ͼ��
		//imshow("gray image", img_gray);
		//waitKey(2000);
		imwrite("OutputImage\\GrayImage.jpg", img_gray);
		int im_col = img_gray.cols;
		int im_row = img_gray.rows;
		//cout << "ͼ��߶�:" << im_row << "  ͼ����:" << im_col << endl;
		Mat img_eh;
		img_eh = Banlance(img_gray);//ֱ������
		//imshow("banlanced image", img_eh);
		//waitKey(2000);
		imwrite("OutputImage\\BanlancedImage.jpg", img_eh);
		double thresh = 128;
		Mat img_seg;
		img_seg = BasicSegment(img_eh, thresh);
		theResult = img_seg;
		imwrite("OutputImage\\BasicSegment.jpg", img_seg);
		img_seg = OtsuSegment(img_eh);
		imwrite("OutputImage\\OtsuSegment.jpg", img_seg);
		img_seg = MaxEntropySegment(img_eh);
		
		imwrite("OutputImage\\MaxEntropySegment.jpg", img_seg);
		img_seg = KmeansSegment(img_eh);
		imwrite("OutputImage\\KmeansSegment.jpg", img_seg);
		Point initial_point = Point(128, 128);
		int growthresh = 17;
		img_seg = RegionGrowSegment(img_eh, initial_point, growthresh);
		imwrite("OutputImage\\RegionGrowSegment.jpg", img_seg);
		img_seg = NewMethodSegment(img_eh);
		imwrite("OutputImage\\NewMethodSegment.jpg", img_seg);
	}
}