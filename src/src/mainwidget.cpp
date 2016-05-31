#include <mainwidget.h>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <fit.h>
#include <qcustomplot.h>
#include <QFile>
#include <QDateTime>
#include <QDebug>

MainWidget::MainWidget(QWidget *parent) 
	: QWidget(parent)
	, m_pVBoxLayout(new QVBoxLayout(this))
	, m_pRealTimePlot(new QCustomPlot(this))
	, m_pDataTimer(new QTimer(this))
	, m_pStatusLabel(new QLabel(this))
	, m_pDataReader(new QFile("data/data.txt"))
{
	if (!m_pDataReader->open(QIODevice::ReadOnly))
	{
		qWarning("File couldn't open!");
		std::exit(0);
	}

	m_pVBoxLayout->addWidget(m_pRealTimePlot);
	m_pVBoxLayout->addWidget(m_pStatusLabel);
	m_pStatusLabel->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);

	m_pRealTimePlot->addGraph(); // blue line
	m_pRealTimePlot->graph(0)->setPen(QPen(Qt::blue));
	m_pRealTimePlot->graph(0)->setBrush(QBrush(QColor(240, 255, 200)));
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
	m_pDataTimer->start(0); // Interval 0 means to refresh as fast as possible
}


void MainWidget::realtimeDataSlot()
{
	// calculate two new data points:
	double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
	static double lastPointKey = 0;
	if (key-lastPointKey > 0.01) // at most add point every 10 ms
	{
		for (int i=0;i<5;i++)
			QString(m_pDataReader->readLine());
		double value0 = QString(m_pDataReader->readLine()).toDouble();
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
	}
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
	m_pDataReader->close();
}
