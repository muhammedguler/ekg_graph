#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>

class QCustomPlot;
class QTimer;
class QHBoxLayout;
class QVBoxLayout;
class QLabel;
class QFile;

class MainWidget : public QWidget
{
		Q_OBJECT
	private:
		QVBoxLayout* m_pVBoxLayout;
		QCustomPlot* m_pRealTimePlot;
		QTimer* m_pDataTimer;
		QLabel* m_pStatusLabel;
		QFile* m_pDataReader;

	public:
		explicit MainWidget(QWidget *parent = 0);
		~MainWidget();

	signals:

	public slots:
		void realtimeDataSlot();
};

#endif // MAINWIDGET_H
