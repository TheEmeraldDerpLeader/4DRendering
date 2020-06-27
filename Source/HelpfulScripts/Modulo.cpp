#include "Modulo.h"
int Modulo(int dividend, int divisor)
{
	if (divisor == 0 || dividend == 0)
		return 0;
	if (dividend >= 0)
	{
		if (divisor >= 0)
		{
			return dividend % divisor;
		}
		else
		{
			return (-divisor) + (dividend % divisor);
		}
	}
	else
	{
		if (divisor >= 0)
		{
			return divisor - ((-dividend) % divisor);
		}
		else
		{
			return -((-dividend) % (-divisor));
		}
	}
}
float Modulo(float dividend, float divisor)
{
	if (divisor == 0 || dividend == 0)
		return 0;
	if (dividend >= 0)
	{
		if (divisor >= 0)
		{
			return fmodf(dividend, divisor);
		}
		else
		{
			return (-divisor) + fmodf(dividend, divisor);
		}
	}
	else
	{
		if (divisor >= 0)
		{
			return divisor - fmodf((-dividend), divisor);
		}
		else
		{
			return -fmodf((-dividend), (-divisor));
		}
	}
}
double Modulo(double dividend, double divisor)
{
	if (divisor == 0 || dividend == 0)
		return 0;
	if (dividend >= 0)
	{
		if (divisor >= 0)
		{
			return fmod(dividend, divisor);
		}
		else
		{
			return (-divisor) + fmod(dividend, divisor);
		}
	}
	else
	{
		if (divisor >= 0)
		{
			return divisor - fmod((-dividend), divisor);
		}
		else
		{
			return -fmod((-dividend), (-divisor));
		}
	}
}