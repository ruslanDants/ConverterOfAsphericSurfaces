#include "AsphericSurface.h"

AsphericSurface::AsphericSurface(double R, double E2, double H)
	: r(R), e2(E2), h(H), Coefficients(1, 0)
{
}

AsphericSurface::AsphericSurface(const AsphericSurface& rs)
	: r(rs.r), e2(rs.e2), h(rs.h), Coefficients(rs.Coefficients)
{
}

void AsphericSurface::SetParametres(double R, double E2, double H)
{
	r = R;
	e2 = E2;
	h = H;
}

void AsphericSurface::SetRadius(double R)
{
	r = R;
}

void AsphericSurface::SetEccentricity(double E2)
{
	e2 = E2;
}

void AsphericSurface::SetHeight(double H)
{
	h = H;
}

double AsphericSurface::Radius()
{
	return r;
}

double AsphericSurface::Eccentricity()
{
	return e2;
}

double AsphericSurface::Height()
{
	return h;
}

int AsphericSurface::getIdFormula()
{
	return IdFormula;
}

AsphericSurface& AsphericSurface::operator=(const AsphericSurface &rs)
{
	if (this == &rs)
		return *this;
	r = rs.r;
	e2 = rs.e2;
	h = rs.h;
	Coefficients = Coefficients;
	return *this;
}

AsphericSurface::~AsphericSurface()
{
}

Formula1::Formula1(double R, double E2, double H)
	: AsphericSurface(R, E2, H)
{
	IdFormula = 0;
}

double Formula1::Calc(double z)
{
	double C1 = 2 * r;
	double C2 = e2 - 1;
	double u = C1 * z + C2 * pow(z, 2);
	for (int i = 0; i < Coefficients.size(); i++)
	{
		u += Coefficients[i] * pow(z, i + 3);
	}
	return u;
}

double Formula1::Sag()
{
	double D = sqrt(abs(1 - (1-e2)*h*h/(r*r)));
	// стрелка прогиба второго порядка
	double sag = h*h/(r*(1+D));
	double f = h*h - Calc(sag);
	double f_, f__, signf, deltaZ;
	while (abs(f) > 0.00001)
	{
		// производная от функции 
		f_ = -2*r - 2*(e2-1)*sag;
		// 2ая производная от функции 
		f__= -2*(e2-1);
		for (int i = 0; i <Coefficients.size(); i++)
		{
			f_ -= (i + 3)*Coefficients[i]*pow(sag, i+2);
			f__ -= (i + 3)*(i + 2)*Coefficients[i]*pow(sag, i + 1);
		}
		signf = f_ < 0 ? -1 : ((f_ == 0) ? 0 : 1);
		deltaZ = -(2 * f / (f_ + signf*sqrt(abs(f_*f_ - 2 * f*f__))));
		sag += deltaZ;
		f = h*h - Calc(sag);
	}
	return sag;
}

Formula1::~Formula1()
{
}

Formula2::Formula2(double R, double E2, double H)
	: AsphericSurface(R, E2, H)
{
	IdFormula = 1;
}

double Formula2::Calc(double u)
{
	double ro = 1 / r;
	double z = u*ro / (1 + sqrt(1 - u*ro*ro*(1 - e2)));
	for (int i = 0; i < Coefficients.size(); i++)
	{
		z += Coefficients[i] * pow(u, i + 3);
	}
	return z;
}

double Formula2::Sag()
{
	return Calc(h);
}

Formula2::~Formula2()
{
}

AsphericSurface* SetFormula(int id)
{
	if (id == 0)
		return new Formula1;
	if (id == 1)
		return new Formula2;
	return 0;
}
