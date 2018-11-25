#include "QtGuiApplication1.h"
#include <QtWidgets/QApplication>
#include "QtWebEngineWidgets/QtWebEngineWidgets"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	WebShowEx2 x;
	x.Init();
	x.show();

	return a.exec();
}
