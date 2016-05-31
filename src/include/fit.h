/*!
 * @file    fit.h
 * @author  Ömer Göktaş [kozmon@hotmail.com]
 * @date    17 February, 2016
 * @version 1.0
 * @brief   Cross-Platform Screen Regulator for Qt.
 *
 *          This file is a part of Cross-Platform Screen                      \n
 *          Regulator Class Fit.
 */

#ifndef FIT_H
#define FIT_H

#include <QWidget>

/*! \class Fit
 *  \brief Cross-Platform Screen Regulator Class for Qt.
 *
 *         Regulation occurs in two stage; First is calculation              \n
 *         of regulation ratios. Second is regulating objects                \n
 *         with regulation ratios. So, firstly we need to                    \n
 *         calculate regulation ratios with update() function                \n
 *         before use fitting functions. Hence, after construct              \n
 *         a QApplication we need to call update() function. (To do          \n
 *         this you may also use FITUPDATE() macro.)                         \n
 *         Also, you need to call update() function after all                \n
 *         screen resolution changes, such as screen orientation changes     \n
 *         on mobile platforms, because regulation ratios will change        \n
 *         after all screen resolution changes.                              \n
 *                                                                           \n
 *         If you want to use this class in your mobile application          \n
 *         projects, you need to revise your main screen size of             \n
 *         applications according to \a DEFAULT_WIDTH and                    \n
 *         \a DEFAULT_HEIGHT macro which are defined in fit.cpp.             \n
 *                                                                           \n
 *         It's possible to keep your object's real size (such as in cm)     \n
 *         on different resolution based platforms with this class. For      \n
 *         example if you want to set your widget size to 50x50 follow this. \n
 *         @code
 *         QWidget* widget = new QWidget(this);
 *         widget->resize(FITMIN(50),FITMIN(50));
 *         ...
 *         @endcode
 *         Thus, you will keep your widget's size in approximately same      \n
 *         size on different platforms (such as in cm).                      \n
 */
class Fit
{
	private:
		static float m_WidthRatio;
		static float m_HeightRatio;

	public:

		/*! \fn 	static void update()
		 *  \brief 	Update regulation ratios.
		 */
		static void update();

		/*! \fn 	static inline int fitHeight(QWidget* const widget)
		 *  \brief 	Fit the given \a widget's height.
		 *  \param 	widget to be fitted.
		 *  \return Fitted height of given widget.
		 */
		static inline int fitHeight(QWidget* const widget)
		{
			widget->resize(widget->width(),m_HeightRatio*(widget->height()));
			return widget->height();
		}

		/*! \fn 	static inline int fitHeight(const int height)
		 *  \brief 	Fit the given \a height value.
		 *  \param 	height to be fitted.
		 *  \return Fitted value of given variable.
		 */
		static inline int fitHeight(const int height)
		{
			return m_HeightRatio*height;
		}

		/*! \fn 	static inline int fitWidth(QWidget* const widget)
		 *  \brief 	Fit the given \a widget's width.
		 *  \param 	widget to be fitted.
		 *  \return Fitted width of given widget.
		 */
		static inline int fitWidth(QWidget* const widget)
		{
			widget->resize(m_WidthRatio*(widget->width()),widget->height());
			return widget->width();
		}

		/*! \fn 	static inline int fitWidth(const int width)
		 *  \brief 	Fit the given \a width value.
		 *  \param 	width to be fitted.
		 *  \return Fitted value of given variable.
		 */
		static inline int fitWidth(const int width)
		{
			return m_WidthRatio*width;
		}
};

/*! \def   FITH(x)
 *  \brief A macro that returns the same result with Fit::fitHeight.                             \n
 *         You can use it instead of all the overloaded versions of Fit::fitHeight function.
 */
#define FITH(x)             Fit::fitHeight(x)

/*! \def   FITW(x)
 *  \brief A macro that returns the same result with Fit::fitWidth.                              \n
 *         You can use it instead of all the overloaded versions of Fit::fitWidth function.
 */
#define FITW(x)              Fit::fitWidth(x)

/*! \def   FITMIN(x)
 *  \brief A macro that returns the smallest of Fit::fitWidth and Fit::fitHeight.                \n
 *         You can not use it with any \a widget parameter. Useful with only integer parameters.
 */
#define FITMIN(x)       qMin(FITH(x),FITW(x))

/*! \def   FITUPDATE(x)
 *  \brief A macro that the same with Fit::update.                                               \n
 *         Useful with only void parameters.
 */
#define FITUPDATE(x)           Fit::update(x)

#endif // FIT_H
