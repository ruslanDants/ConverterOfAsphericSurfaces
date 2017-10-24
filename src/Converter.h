#ifndef CONVERTER_H
#define CONVERTER_H

#define BOOST_UBLAS_NDEBUG 1
#include <boost\numeric\ublas\matrix.hpp>
#include <boost\numeric\ublas\vector.hpp>
#include "AsphericSurface.h"
#include "inverse.h"

namespace bnu = boost::numeric::ublas;
class Converter
{
public:
	enum Method { MHK = 0, Analytical };
private:
	// Метод пересчета
	Method mthd;
	// Количество коэффициентов для пересчета
	int countCoeffs;
	// Количество точек для пересчета
	int countPoints;
	//// Среднеквадратичное отклонение
	//double stdev;
	// Подготовка вектора и матрицы для пересчета по методу наим. квадрата
	void FirstToSecondMHK(AsphericSurface *source, bnu::vector<double>& B, bnu::matrix<double>& A);
	void SecondToFirstMHK(AsphericSurface *source, bnu::vector<double>& B, bnu::matrix<double>& A);

	// Подготовка вектора и матрицы для пересчета по аналитическому методу
	void FirstToSecondAnalytical(AsphericSurface *source, AsphericSurface *convert);
	void SecondToFirstAnalytical(AsphericSurface *source, AsphericSurface *convert);
public:
	// Конструктор
	Converter(Method mtd = MHK, int kCoef = 2, int kPoint = 100);
	// Установить настройки пересчета
	// kCoef - количество коэффициентов для пересчета
	// kPoint - количество точек для пересчета
	void Settings(int kCoef = 2, int kPoint = 100);
	// Установить метод пересчета
	void SetMethod(Method mtd = MHK);
	// Выполнить пересчет
	bool Run(AsphericSurface *source, AsphericSurface *convert);
	// Вычисление среднеквадратичного отклонения
	double CalcStdev(AsphericSurface *source, AsphericSurface *convert, int Points);
	~Converter();
};

#endif // CONVERTER_H
