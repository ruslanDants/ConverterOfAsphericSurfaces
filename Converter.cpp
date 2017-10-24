#include "Converter.h"

Converter::Converter(Method mtd, int kCoef, int kPoint)
	:  mthd(mtd), countCoeffs(kCoef), countPoints(kPoint)
{
}
void Converter::Settings(int kCoef, int kPoint)
{
	countCoeffs = kCoef;
	countPoints = kPoint;
}
void Converter::SetMethod(Method mtd)
{
	mthd = mtd;
}

void Converter::FirstToSecondMHK(AsphericSurface *source, bnu::vector<double>& B, bnu::matrix<double>& A)
{
	bnu::vector<double> U(countPoints), Z(countPoints);
	double ro = 1 / source->Radius();
	double step = source->Sag() / (countPoints - 1);
	for (int i = 0; i < countPoints; i++)
	{
		Z[i] = step * i;
		U[i] = source->Calc(Z[i]);
		B[i] = Z[i] - U[i] * ro / (1 + sqrt(1 - U[i] * ro*ro*(1 - source->Eccentricity())));
		for (int k = 0; k < countCoeffs; k++)
		{
			A(i, k) = pow(U[i], k + 3);
		}
	}
}
void Converter::SecondToFirstMHK(AsphericSurface *source, bnu::vector<double>& B, bnu::matrix<double>& A)
{
	bnu::vector<double> Z(countPoints), U(countPoints);
	double C1 = 2 * source->Radius();
	double C2 = source->Eccentricity() - 1;
	double step = source->Height() / (countPoints - 1);
	for (int i = 0; i < countPoints; i++)
	{
		U[i] = step * i;
		Z[i] = source->Calc(U[i]);
		B[i] = U[i] - C1*Z[i] - (C2*Z[i]*Z[i]);
		for (int k = 0; k < countCoeffs; k++)
		{
			A(i, k) = pow(Z[i], k + 3);
		}
	}
}
void Converter::FirstToSecondAnalytical(AsphericSurface *source, AsphericSurface *convert)
{
	int k = source->Coefficients.size();
	double r = source->Radius();
	double e = source->Eccentricity();
	double a3 = 0, a4 = 0, a5 = 0;
	if (k >= 1)
	{
		a3 = source->Coefficients[0];
	}
	double A3 = -(a3 / (16 * pow(r, 4)));
	convert->Coefficients.resize(1);
	convert->Coefficients[0] = A3;
	if (k >= 2)
	{
		a4 = source->Coefficients[1];
	}
	double A4 = ((-5)*(1 - e)*a3 - 2 * r*a4) / (64 * pow(r, 6));
	convert->Coefficients.push_back(A4);
	if (k >= 3)
	{
		a5 = source->Coefficients[2];
	}
	double A5 = (6 * r*pow(a3, 2)) - (21 * pow(1 - e, 2)*a3) - (12 * (1 - e)*r*a4) - (4 * r*r*a5);
	A5 /= 256 * pow(r, 8);
	convert->Coefficients.push_back(A5);
}
void Converter::SecondToFirstAnalytical(AsphericSurface *source, AsphericSurface *convert)
{
	int k = source->Coefficients.size();
	double r = source->Radius();
	double e = source->Eccentricity();
	double A3 = 0, A4 = 0, A5 = 0;

	if (k >= 1)
	{
		A3 = source->Coefficients[0];
	}	
	double a3 = -(16 * pow(r, 4)*A3);
	convert->Coefficients.resize(1);
	convert->Coefficients[0] = a3;
	if (k >= 2)
	{
		A4 = source->Coefficients[1];
	}	
	double a4 = 40*(1-e)*pow(r, 3)*A3 - 32*pow(r, 5)*A4;
	convert->Coefficients.push_back(a4);
	if (k >= 3)
	{
		A5 = source->Coefficients[2];
	}
	double a5 = -(36 * (1 - e)*r*r*A3) + (384*pow(r, 7)*A3*A3) + (96*(1-e)*pow(r, 4)*A4) - (64*pow(r, 6)*A5);
	convert->Coefficients.push_back(a5);
}
double Converter::CalcStdev(AsphericSurface *source, AsphericSurface *convert, int Points)
{
	bnu::vector<double> Subs(Points);
	double step, val1, val2, val3;
		if (source->getIdFormula() == 0)
		{
			step = source->Sag() / (Points - 1);
		}
		else
		{
			step = source->Height() / (Points - 1);
		}
		double mean = 0;
		for (int i = 0; i < Points; i++)
		{
			val1 = i*step;
			val2 = source->Calc(val1);
			val3 = convert->Calc(val2);
			mean += (val1 - val3);
			Subs[i] = val1 - val3;
		}
		mean /= Points;
		double skvo = 0;
		for (int i = 0; i < Points; i++)
		{
			skvo += pow((Subs[i] - mean), 2);
		}
	//}
	skvo /= Points;
	return sqrt(skvo);
}
bool Converter::Run(AsphericSurface* source, AsphericSurface* convert)
{
	if (mthd == MHK)
	{
		bnu::vector<double> B(countPoints);
		bnu::matrix<double> A(countPoints, countCoeffs);
		if (source->getIdFormula() == 0 && convert->getIdFormula() == 1)
		{
			FirstToSecondMHK(source, B, A);
		}
		else if (source->getIdFormula() == 1 && convert->getIdFormula() == 0)
		{
			SecondToFirstMHK(source, B, A);
		}
		else 
			return false;
		bnu::vector<double> C;
		bnu::matrix<double> Ai;
		Ai = prod(trans(A), A);
		InvertMatrix(Ai, Ai);
		Ai = prod(Ai, trans(A));
		C = prod(Ai, B);
		convert->Coefficients.resize(countCoeffs);
		for (int unsigned i = 0; i < countCoeffs; i++)
		{
			convert->Coefficients[i] = C[i];
		}
	return true;
	}
	else if (mthd == Analytical)
	{
		if (source->getIdFormula() == 0 && convert->getIdFormula() == 1)
		{
			FirstToSecondAnalytical(source, convert);
		}
		else if (source->getIdFormula() == 1 && convert->getIdFormula() == 0)
		{
			SecondToFirstAnalytical(source, convert);
		}
		else
			return false;
		return true;
	}
	return false;
}
Converter::~Converter()
{
}
