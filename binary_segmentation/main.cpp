#include "ImageSegmentationUI.h"
#include <QtWidgets/QApplication>


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	ImageSegmentationUI w;
	w.show();
	return a.exec();
}
