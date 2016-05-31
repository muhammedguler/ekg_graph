#include <mainwidget.h>
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QApplication::setStyle("fusion");

	MainWidget w;
	w.showFullScreen();
	w.setGeometry(0,0,800,600);

	return a.exec();
}
