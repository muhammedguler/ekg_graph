#include <fit.h>
#include <QRect>
#include <QApplication>
#include <QDesktopWidget>

#define BASE_WIDTH        1366.0
#define BASE_HEIGHT        768.0
#define DEFAULT_WIDTH      600.0
#define DEFAULT_HEIGHT     400.0

float Fit::m_HeightRatio = 1.0;
float Fit::m_WidthRatio  = 1.0;

void Fit::update()
{
	const QRect cS = QApplication::desktop()->screenGeometry(QApplication::desktop()->primaryScreen());
	const float cSWidthRatio = cS.width()/BASE_WIDTH;
	const float cSHeightRatio = cS.height()/BASE_HEIGHT;

#   if defined(Q_OS_ANDROID) || defined(Q_OS_IOS) || defined(Q_OS_WINPHONE)
	m_WidthRatio = cSWidthRatio*(BASE_WIDTH/DEFAULT_WIDTH);
#   else
	m_WidthRatio = cSWidthRatio;
#   endif

#   if defined(Q_OS_ANDROID) || defined(Q_OS_IOS) || defined(Q_OS_WINPHONE)
	m_HeightRatio = cSHeightRatio*(BASE_HEIGHT/DEFAULT_HEIGHT);
#   else
	m_HeightRatio = cSHeightRatio;
#   endif
}
