#include "ImageSegmentationUI.h"
#include <QPushButton>
#include <QPainter>

using namespace std;
using namespace cv;

ImageSegmentationUI::ImageSegmentationUI(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	setWindowTitle(tr("Image Segmentation"));
	resize(windowWidth, windowHeight);

	QMenuBar *mb = menuBar();
	QMenu *menu = new QMenu("Menu");
	QAction *action1 = new QAction("Exit", this);
	//QAction *action2 = new QAction("OSPF", this);
	//QAction *action3 = new QAction("K-means", this);
	//QAction *action4 = new QAction("Max Entropy", this);
	//QAction *action5 = new QAction("Region Grow", this);
	//QAction *action6 = new QAction("Novel Method", this);
	menu->addAction(action1);
	//menu->addAction(action2);
	//menu->addAction(action3);
	//menu->addAction(action4);
	//menu->addAction(action5);
	//menu->addAction(action6);
	mb->addMenu(menu);//往菜单栏中添加菜单

	//int imagePosition[6][2] = {{200,300},{600,300},{1000,300},{200,700},{600,700},{1000,700}};
	//imagePositionX[6] = { 200, 600,1000, 200,600,1000 };
	//imagePositionY[6] = {100, 100, 100,300,300,300};

/**********************************************************************/
	QPushButton *openBtn = new QPushButton();
	openBtn->setParent(this);
	openBtn->setText("Open a Image");
	openBtn->resize(windowWidth / 10, windowHeight / 30);
	openBtn->move(windowWidth / 100, windowHeight / 5 * 2 / 6);

	QPushButton *segBtn = new QPushButton();
	segBtn->setParent(this);
	segBtn->setText("Start Segmentation");
	segBtn->resize(windowWidth / 10, windowHeight / 30);
	segBtn->move(windowWidth / 100, windowHeight / 5 * 2 / 3);

	QPushButton *showBtn = new QPushButton();
	showBtn->setParent(this);
	showBtn->setText("Show Results");
	showBtn->resize(windowWidth / 10, windowHeight / 30);
	showBtn->move(windowWidth / 100, windowHeight / 5 * 2 / 2);

	QPushButton *exitBtn = new QPushButton();
	exitBtn->setParent(this);
	exitBtn->setText("Exit");
	exitBtn->resize(windowWidth / 10, windowHeight / 30);
	exitBtn->move(windowWidth / 100, windowHeight / 5 * 2 * 4/ 6);

	QFont font("Microsoft YaHei", 12, 75);
	labelTxtForInstruction = new QLabel(this);
	labelTxtForInstruction->setFont(font);
	labelTxtForInstruction->resize(windowWidth / 10, windowHeight / 10);
	labelTxtForInstruction->move(windowWidth / 100, windowHeight / 5 * 2 * 5 / 6);

	QPaintEvent *event;
	paintEvent(event);

	labelTxtForImage[0] = new QLabel();
	labelTxtForImage[0]->setParent(this);
	labelTxtForImage[0]->setText("The Sourse Image");
	labelTxtForImage[0]->setFont(font);
	labelTxtForImage[0]->resize(windowWidth / 10, windowHeight / 30);
	labelTxtForImage[0]->move(imagePosition[0][0] + 120, imagePosition[0][1] + 100);
	labelTxtForImage[0]->show();
	
	for (int i = 1; i < 6; i++)
	{
		labelTxtForImage[i] = new QLabel();
		labelTxtForImage[i]->setParent(this);
		labelTxtForImage[i]->setText("Segmented Image");
		labelTxtForImage[i]->setFont(font);
		labelTxtForImage[i]->resize(windowWidth / 10, windowHeight / 30);
		labelTxtForImage[i]->move(imagePosition[i][0] + 120, imagePosition[i][1] + 100);
		labelTxtForImage[i]->show();
	}

	ist = new ImageSegmentor();
	connect(openBtn, &QPushButton::clicked, this, &ImageSegmentationUI::openAndShowImage);
	connect(segBtn, &QPushButton::clicked, this, &ImageSegmentationUI::imageSegmenting);
	connect(showBtn, &QPushButton::clicked, this, &ImageSegmentationUI::showSegImage);
	connect(exitBtn, &QPushButton::clicked, this, &QApplication::quit);
	connect(action1, &QAction::triggered, this, &QApplication::quit);
}

void ImageSegmentationUI::openAndShowImage()
{
	QString filename = QFileDialog::getOpenFileName(this,
		tr("open image"), ".", tr("Image file(*.png *.jpg *.bmp)"));
	theImage = imread(filename.toLocal8Bit().data());
	theResult = imread(filename.toLocal8Bit().data());
	QImage theQimage;
	theQimage.load(filename);
	theQimage = theQimage.scaled(imageWidth, imageHeight);
	openflag++;
	labelImage[0] = new QLabel();
	labelImage[0]->setParent(this);
	labelImage[0]->setPixmap(QPixmap::fromImage(theQimage));
	labelImage[0]->resize(imageWidth, imageHeight);
	labelImage[0]->move(imagePosition[0][0], imagePosition[0][1]);
	labelImage[0]->show();
	labelTxtForInstruction->setText("Image Opened!");
	labelTxtForInstruction->show();
}
void ImageSegmentationUI::imageSegmenting()
{
	labelTxtForInstruction->close();
	labelTxtForInstruction->setText("Image \n Segmenting...");
	labelTxtForInstruction->show();
	ist->exeSegment();
	labelTxtForInstruction->close();
	labelTxtForInstruction->setText("Segmentation \n Finished!");
	labelTxtForInstruction->show();
}
void ImageSegmentationUI::showSegImage()
{
	QImage theQresult;
	QFont font("Microsoft YaHei", 12, 75);
	
	theQresult = theQresult.scaled(400, 300, Qt::KeepAspectRatio);
	for (int i = 0; i < 6; i++)
	{
		theQresult.load(QString::fromStdString(resultPaths[i]));
		theQresult = theQresult.scaled(imageWidth, imageHeight);
		labelImage[i] = new QLabel();
		labelImage[i]->setParent(this);
		labelImage[i]->setPixmap(QPixmap::fromImage(theQresult));
		labelImage[i]->resize(imageWidth, imageHeight);
		labelImage[i]->move(imagePosition[i][0], imagePosition[i][1]);
		labelImage[i]->show();

		labelTxtForImage[i]->close();
		labelTxtForImage[i] = new QLabel();
		labelTxtForImage[i]->setParent(this);
		labelTxtForImage[i]->setText(QString::fromStdString(resultNames[i]));
		labelTxtForImage[i]->setFont(font);
		labelTxtForImage[i]->resize(windowWidth / 10, windowHeight / 30);
		labelTxtForImage[i]->move(imagePosition[i][0] + 120, imagePosition[i][1] - 40);
		labelTxtForImage[i]->show();
	}

	labelTxtForInstruction->close();
	labelTxtForInstruction->setText("Results Showed!");
	labelTxtForInstruction->show();
}
void ImageSegmentationUI::paintEvent(QPaintEvent *event)
{
	QPainter painter;
	painter.begin(this);
	painter.setPen(QPen(Qt::black, 3, Qt::SolidLine, Qt::RoundCap));
	for (int i = 0; i < 6; i++)
		painter.drawRect(QRect(imagePosition[i][0]-10, imagePosition[i][1]-10, imageWidth + 20, imageHeight + 20));
}