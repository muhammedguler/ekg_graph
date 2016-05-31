#include <fit.h>
#include <mainwidget.h>
#include <QApplication>
#include <QDesktopWidget>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QApplication::setStyle("fusion");
	FITUPDATE();

	MainWidget w;
	w.show();
	w.resize(FITMIN(600),FITMIN(400));
	w.move(QApplication::desktop()->availableGeometry().center()-w.rect().center());
	w.setWindowTitle("Steteskop Plot");

	return a.exec();
}
