#ifndef ASPHERICSURFACE_H
#define ASPHERICSURFACE_H

#include <vector>
#include <cmath>

class AsphericSurface
{
public:
	// Вектор коэффициентов
	std::vector<double> Coefficients;
protected:
	double r;		     // Радиус
	double e2;		     // Эксцентриситет
	double h;		     // Световая высота
	int IdFormula;		 // Номер формулы
public:
	// Конструктор по умолчанию
	AsphericSurface(double R = 50, double E2 = 0.5, double H = 40);
	// Конструктор копирования
	AsphericSurface(const AsphericSurface& rs);
	// установить настройки асферической поверхности
	void SetParametres(double R, double E2, double H);
	// Установка радиуса
	void SetRadius(double R);
	// Установка эксцентриситета
	void SetEccentricity(double E2);
	// Установка световой высоты
	void SetHeight(double H);
	// Возврат значения радиуса
	double Radius();
	// Возврат значения эксцентриситета
	double Eccentricity();
	// Возврат значения световой высоты
	double Height();
	// Функция вычисления значения по формуле
	virtual double Calc(double val) = 0;
	// Возврат стрелки прогиба
	virtual double Sag() = 0;
	// Оператор присваивания
	AsphericSurface& operator=(const AsphericSurface& rs);
	// Номер формулы
	int getIdFormula();
	virtual ~AsphericSurface();
};

// Уравнение 1 типа u = C1*z - C2*z^2 + C3*z^3 + C4*z^4 + ...
class Formula1 : public AsphericSurface
{
public:
	Formula1(double R = 50, double E2 = 1, double H = 40);
	double Calc(double z);
	double Sag();
	~Formula1();
};

// Уравнение 2 типа z = u*ro/(1 + sqrt(1 - u*ro^2*(1 - e2))) + C2*u^2 + C3*u^3 + ...
class Formula2 : public AsphericSurface
{
public:
	Formula2(double R = 50, double E2 = 1, double H = 40);
	double Calc(double u);
	double Sag();
	~Formula2();
};

// Получить асфреическую поверхность с выбранным уравнением
AsphericSurface* SetFormula(int id);

#endif // ASPHERICSURFACE_H
