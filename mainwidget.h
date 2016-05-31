#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>

class QCustomPlot;
class QTimer;
class QHBoxLayout;
class QVBoxLayout;
class QLabel;
class QFile;
class Mcp3208;

class MainWidget : public QWidget
{
		Q_OBJECT
	private:
		QVBoxLayout* m_pVBoxLayout;
		QCustomPlot* m_pRealTimePlot;
		QTimer* m_pDataTimer;
		QLabel* m_pStatusLabel;
		Mcp3208* m_pMcp3208;

	public:
		explicit MainWidget(QWidget *parent = 0);
		~MainWidget();

	signals:

	public slots:
		void realtimeDataSlot();
};

#endif // MAINWIDGET_H
