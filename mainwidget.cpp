#include <mainwidget.h>
#include <QVBoxLayout>
#include <qcustomplot.h>
#include <QDateTime>

#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdint.h>

void print(...){};
MainWidget::MainWidget(QWidget *parent)
	: QWidget(parent)
	, m_pVBoxLayout(new QVBoxLayout())
	, m_pRealTimePlot(new QCustomPlot())
	, m_pDataTimer(new QTimer(this))
	, m_pStatusLabel(new QLabel())
{


	m_pVBoxLayout->setMargin(0);
	setLayout(m_pVBoxLayout);
	m_pVBoxLayout->addWidget(m_pRealTimePlot);
	m_pVBoxLayout->addWidget(m_pFftPlot);
	m_pVBoxLayout->addWidget(m_pStatusLabel);
	m_pStatusLabel->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
	m_pRealTimePlot->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

	m_pRealTimePlot->addGraph(); // blue line
	m_pRealTimePlot->graph(0)->setPen(QPen(Qt::blue));
	//m_pRealTimePlot->graph(0)->setBrush(QBrush(QColor(240, 255, 200)));
	m_pRealTimePlot->graph(0)->setAntialiasedFill(false);

	m_pRealTimePlot->addGraph(); // blue dot
	m_pRealTimePlot->graph(1)->setPen(QPen(Qt::blue));
	m_pRealTimePlot->graph(1)->setLineStyle(QCPGraph::lsNone);
	m_pRealTimePlot->graph(1)->setScatterStyle(QCPScatterStyle::ssDisc);

	m_pRealTimePlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
	m_pRealTimePlot->xAxis->setDateTimeFormat("hh:mm:ss");
	m_pRealTimePlot->xAxis->setAutoTickStep(false);
	m_pRealTimePlot->xAxis->setTickStep(2);
	m_pRealTimePlot->axisRect()->setupFullAxesBox();

	// make left and bottom axes transfer their ranges to right and top axes:
	connect(m_pRealTimePlot->xAxis, SIGNAL(rangeChanged(QCPRange)), m_pRealTimePlot->xAxis2, SLOT(setRange(QCPRange)));
	connect(m_pRealTimePlot->yAxis, SIGNAL(rangeChanged(QCPRange)), m_pRealTimePlot->yAxis2, SLOT(setRange(QCPRange)));

	// setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
	connect(m_pDataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
	m_pDataTimer->start(0.00001); // Interval 0 means to refresh as fast as possible


	/**************************************************************************************************/

}


void MainWidget::realtimeDataSlot()
{
	// calculate two new data points:
	double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
	static double lastPointKey = 0;

QString yazi; int sayi;
QFile file("/sys/devices/12d10000.adc/iio:device0/in_voltage1_raw");
if (file.open(QIODevice::ReadOnly)) {
QTextStream stream(&file);
stream >> yazi >> sayi; }
		double value0 =sayi; 
		// add data to lines:
		m_pRealTimePlot->graph(0)->addData(key, value0);
		// set data of dots:
		m_pRealTimePlot->graph(1)->clearData();
		m_pRealTimePlot->graph(1)->addData(key, value0);
		// remove data of lines that's outside visible range:
		m_pRealTimePlot->graph(0)->removeDataBefore(key-8);
		// rescale value (vertical) axis to fit the current data:
		m_pRealTimePlot->graph(0)->rescaleValueAxis();
		lastPointKey = key;
	// make key axis range scroll with the data (at a constant range size of 8):
	m_pRealTimePlot->xAxis->setRange(key+0.25, 8, Qt::AlignRight);
	m_pRealTimePlot->replot();
	


	// calculate frames per second:
	static double lastFpsKey;
	static int frameCount;
	++frameCount;
	if (key-lastFpsKey > 2) // average fps over 2 seconds
	{
		m_pStatusLabel->setText(
					QString("%1 FPS, Total Data points: %2")
					.arg(frameCount/(key-lastFpsKey), 0, 'f', 0)
					.arg(m_pRealTimePlot->graph(0)->data()->count()));
		lastFpsKey = key;
		frameCount = 0;
	}

}

MainWidget::~MainWidget()
{
}
