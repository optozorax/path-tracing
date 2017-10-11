#ifndef PT_BASICS_H
#define PT_BASICS_H

#include <random>

namespace pt
{

	class Color;

	//-------------------------------------------------------------------------
	float random(void);
	Vector randomSphere(void);

	//-------------------------------------------------------------------------
	class Color
	{
	public:
		double r, g, b, a;
		Color(double r, double g, double b, double a = 1) : r(r), g(g), b(b), a(a) {}

		/** Делит цвет на некоторое число для взятия среднего арифметического среди нескольких цветов. Среднее арифметическое так же берется и для альфа канала. */
		Color& operator/=(double k);

		/** Умножает два цвета, только в пространстве r, g, b. Если один из цветов Transparent, то возвращает текущий цвет.

		ИЛИ. Считает, что оба цвета полупрозрачные и считает как если бы текущий накладывался поверх a. */
		Color& operator*(const Color& a);

		/** Получает квадратный корень из цвета. При этом квадратный корень из альфа-канала не берется. */
		Color& sqrt(void);
	};

	const Color transparent(0, 0, 0, 0);

};

#endif