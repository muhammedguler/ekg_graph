#include <mainwidget.h>
#include <QVBoxLayout>
#include <qcustomplot.h>
#include <QDateTime>
#include <mcp3208.h>

MainWidget::MainWidget(QWidget *parent)
	: QWidget(parent)
	, m_pVBoxLayout(new QVBoxLayout())
	, m_pRealTimePlot(new QCustomPlot())
	, m_pDataTimer(new QTimer(this))
	, m_pStatusLabel(new QLabel())
	, m_pMcp3208(new Mcp3208())
{
	m_pMcp3208->initMcp3208();

	m_pVBoxLayout->setMargin(0);
	setLayout(m_pVBoxLayout);
	m_pVBoxLayout->addWidget(m_pRealTimePlot);
	m_pVBoxLayout->addWidget(m_pStatusLabel);
	m_pStatusLabel->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
	QFont font = m_pStatusLabel->font();
	font.setPointSize(36);
	font.setBold(true);
	m_pStatusLabel->setFont(font);
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
	m_pDataTimer->start(0); // Interval 0 means to refresh as fast as possible
}

void MainWidget::realtimeDataSlot()
{
	// calculate two new data points:
	double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
	static double lastPointKey = 0;
	char c=0;
	static double timerr=0.0 , timerr_old=0.0, dtt=0.0,heart=0.0,heart_old=0,heart_rate=0 ,value,value2,value1=0;
		double value0 = m_pMcp3208->readMcp3208(2);
					value = value0-2100;
			value =  value*value;
		// add data to lines:
		m_pRealTimePlot->graph(0)->addData( key ,  value0 );
		// set data of dots:
	//	m_pRealTimePlot->graph(1)->clearData();
	//	m_pRealTimePlot->graph(1)->addData( key ,  value0 );
		// remove data of lines that's outside visible range:
		m_pRealTimePlot->graph(0)->removeDataBefore( key - 8 );
		// rescale value (vertical) axis to fit the current data:
		m_pRealTimePlot->graph(0)->rescaleValueAxis();
		m_pRealTimePlot->yAxis->setRange(500,4096);
		lastPointKey = key;
		/*		int size=200;
		for(int i=0 ;i<size;i++)
			samples [ i ] = m_pRealTimePlot->graph(0)->data()->values().at(i).value;*/

		 value2 = value1;
		 value1= value ;
		 
		if (value1 >= value &&  value1 >= value2 && value1 >=300000) 
		{
        	if(c == 0)
           		{ 
           		c=1;
           		timerr_old=timerr;
           		timerr = QDateTime::currentDateTime().toMSecsSinceEpoch();
      			dtt=timerr - timerr_old;
      			if (dtt >350){
      			heart_old=heart;
            	heart=( 60000.0/dtt );
            	heart_rate=(heart+heart_old)/2;
            	}}}
    if( value0 <= 2800)
      { 	if (c == 1)
      			{	c=0;		}
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
					QString("%1 FPS,          HEART RATE : %2")
					.arg(frameCount/(key-lastFpsKey), 0, 'f', 0)
					.arg( (heart_rate), 0, 'f', 0 ));
					/*(m_pRealTimePlot->graph(0)->data()->count()));*/
		lastFpsKey = key;
		frameCount = 0;
	}

}

MainWidget::~MainWidget()
{
}

