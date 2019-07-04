/* Complex<double> Class for DSP */

#ifndef _Complex_H_
#define _Complex_H_
#include <stdio.h>
#include <cmath>

#define RSQRT2          (0.70710678118655)
#define SQRT2           (1.41421356237309504880)
#define SQRT3           (1.73205080756888)

#ifndef PI_DECLARE_T_
#define PI 3.14159265358979
#define _PI_DECLARE_T_
#endif
#define HALFPI          1.57079632679489661923
#define TWOPI           6.28318530717958647692

#define SQRT2PI         (4.44288293815837)

#define RADPDEG         0.01745329251994329576
#define DGRPRAD         57.29577951308232       //radian to decimal conversion factor 
#define DGR2RAD(x)      (RADPDEG*(x))
#define RAD2DGR(x)      (DGRPRAD*(x))

#ifndef MAX
#define MAX(x,y)            ((x) >= (y) ? (x) : (y))
#endif
#ifndef MIN
#define MIN(x,y)            ((x) <= (y) ? (x) : (y))
#endif
#ifndef ABS
#define ABS(x)              (((x) < 0) ? (-(x)) : (x))
#endif


#define BRO(N, x, y)            {int sh; for(sh = 0, y = 0; sh < N; sh++) \
                                    {y |= (((x >> sh) & 0x01) << (N-1-sh));}}

#define BRO4(N, x, y)          {int sh; for (sh = 0, y = 0; sh < N; sh++ ) \
                                {y |= (((x >> ( sh << 1 )) & (0x3)) << ((N-1-sh) << 1));}}

#define GBRO(N, f, x, y)       {int sh; for (sh = 0, y = 0; sh < N; sh++ ) \
                                {y |= (((x >> ( sh << f )) & ((0x2 << f) - 1)) << ((N-1-sh) << f));}}



template <class T> class Complex
{
public:

	T re;
	T im;





	Complex()
	{
		re = 0;
		im = 0;
	}

	Complex(T r, T i = 0) :re(r), im(i) {}

	inline T real() { return(re); };
	inline T imag() { return(im); };

	inline double distance(Complex<T> a, Complex<T> b)
	{
		double result = 0;
		result = sqrt(pow(a.re - b.re, 2) + pow(a.im - b.im, 2));
		return result;
	}

	inline double distance2(double a, double b)
	{
		double result = 0;
		result = sqrt(pow(a - b, 2));
		return result;

	}
	Complex<T> conjugate()
	{
		Complex<T> result;
		result.re = this->re;
		result.im = -this->im;
		return result;
	}

	inline T operator=(T r)
	{
		re = r;
		im = r;
		return r;
	}

	

	inline const Complex<T> &  operator *=(const Complex<T>& y) {
		T r = re * y.re - im * y.im;
		im = re * y.im + im * y.re;
		re = r;
		return *this;
	}
	inline const Complex<T> &  operator *=(const T & y) {
		re = re * y;
		im = im * y;
		return *this;
	}

	inline const Complex<T> &  operator +=(const Complex<T>& y) {
		re += y.re;
		im += y.im;
		return *this;
	}
	inline const Complex<T> &  operator -=(const Complex<T>& y) {
		re -= y.re;
		im -= y.im;
		return *this;
	}
	inline const Complex<T> &  operator +=(const T & y) {
		re += y;
		im += y;
		return *this;
	}
	inline const Complex<T> &  operator -=(const T & y) {
		re -= y;
		im -= y;
		return *this;
	}
	inline const Complex<T> & operator /=(const Complex<T> & y) {
		return (*this *= conj(y) / magsq(y));
	}
	inline const Complex<T> & operator /=(const T & r) {
		re /= r;
		im /= r;
		return *this;
	};

	// Left shift
	inline Complex<T>  operator <<=(const T shift)
	{
		return Complex<T>(re <<= shift, im <<= shift);
	}
	// Right shift
	inline Complex<T>  operator >>=(const T shift)
	{
		return Complex<T>(re >>= shift, im >>= shift);
	}

	// Saturate to specific number of bits (signed)
	//  inline Complex<long> saturate(long bits) {
	//  long low_mask = ((1<<(bits-1)) - 1);
	//  if (labs(re) > low_mask) re = (re>0) ? low_mask : ~low_mask;
	//  if (labs(im) > low_mask) im = (im>0) ? low_mask : ~low_mask;
	//  return *this;
	//  }

	operator const Complex<long>() const { return(Complex<long>((long)re, (long)im)); }
	// Removed for MSDEV Visual 5.0
	//operator const Complex<double> () const {	  return(Complex<double>((double)re,(double)im));  }

};

//inline Complex<int_u>::Complex(const Complex<long>& y) : re(y.ree), im(y.im) {}
//inline Complex<int_s>::Complex(const Complex<long>& y) : re(y.ree), im(y.im) {}

template <class T1, class T2> inline Complex<T1> operator *(const Complex<T1> & r, const Complex<T2> & l)
{
	return Complex<T1>(r.re*l.re - r.im*l.im, r.re*l.im + r.im*l.re);
};
//
template <class T1, class T2> inline Complex<T1> operator +(const Complex<T1> & r, const Complex<T2> & l)
{
	return Complex<T1>(r.re + l.re, r.im + l.im);
};
//
template <class T> inline Complex<T> operator +(const Complex<T> & r, const T & l)
{
	return Complex<T>(r.re + l, r.im);
};
//
template <class T> inline Complex<T> operator +(const T & r, const Complex<T> & l)
{
	return Complex<T>(r + l.re, l.im);
};

//
template <class T1, class T2> inline Complex<T1> operator -(const Complex<T1> & r, const Complex<T2> & l)
{
	return Complex<T1>(r.re - l.re, r.im - l.im);
};
//
template <class T> inline Complex<T> operator -(const Complex<T> & r, const T & l)
{
	return Complex<T>(r.re - l, r.im);
};
//
template <class T> inline Complex<T> operator -(const T & r, const Complex<T> & l)
{
	return Complex<T>(r - l.re, -l.im);
};
//
template <class T> inline Complex<T> operator &(const Complex<T> & r, const T & l)
{
	return Complex<T>(r.re & l, r.im & l);
};
//
template <class T> inline Complex<T> operator &(const T & r, const Complex<T> & l)
{
	return Complex<T>(r & l.re, r & l.im);
};
//
template <class T> inline Complex<T> operator %(const Complex<T> & r, const T & l)
{
	return Complex<T>(r.re % l, r.im % l);
};
//
template <class T> inline Complex<T> operator ^(const Complex<T> & r, const T & l)
{
	return Complex<T>(r.re ^ l, r.im ^ l);
};
//
template <class T> inline Complex<T> operator ^(const T & r, const Complex<T> & l)
{
	return Complex<T>(r ^ l.re, r ^ l.im);
};
//
template <class T> inline Complex<T> operator |(const Complex<T> & r, const T & l)
{
	return Complex<T>(r.re | l, r.im | l);
};
//
template <class T> inline Complex<T> operator |(const T & r, const Complex<T> & l)
{
	return Complex<T>(r | l.re, r | l.im);
};

// Left shift
template <class T> inline Complex<T>  operator <<(const Complex<T> & r, const T shift)
{
	return Complex<T>(r.re << shift, r.im << shift);
}
// Right shift
template <class T> inline Complex<T>  operator >>(const Complex<T> & r, const T shift)
{
	return Complex<T>(r.re >> shift, r.im >> shift);
}
//
template <class T> inline Complex<T> operator -(const Complex<T> & r)
{
	return Complex<T>(-r.re, -r.im);
}

//
inline Complex<long> operator *(const double & r, const Complex<long> & l)
{
	return(Complex<long>((long)floor((double)(r*l.re + 0.5)),
		(long)floor((double)(r*l.im + 0.5))));

};

//
inline Complex<double> operator *(const long & r, const Complex<double> & l)
{
	return Complex<double>(r*l.re, r*l.im);
};

//
inline Complex<double> operator *(const double & r, const Complex<double> & l)
{
	return Complex<double>(r*l.re, r*l.im);
};

//
inline Complex<double> operator *(const float & r, const Complex<double> & l)
{
	return Complex<double>(r*l.re, r*l.im);
};

//
template <class T> inline Complex<T> operator *(const Complex<T> & r, const T & l)
{
	return Complex<T>(r.re*l, r.im*l);
};

/*
//
template <class T> inline Complex<T> operator *(const Complex<T> & r, const double & l)
{
return Complex<T>(r.ree*l, r.im*l);
};
//
template <class T> inline Complex<T> operator *(const Complex<T> & r, const float & l)
{
return Complex<T>(r.ree*l, r.im*l);
};
//
*/


template <class T1, class T2> Complex<T1> operator /(const Complex<T1> & l, const T2 & r)
{
	Complex<T1> x(0, 0);
	if (r != 0)
	{
		x.re = l.re / r;
		x.im = l.im / r;
	}
	// else is an error condition!
	return x;
};

//
template <class T1, class T2> Complex<T1> operator /(const Complex<T1> & r, const Complex<T2> & l)
{
	Complex<T1> x;
	T2 den;
	den = magsq(l);
	x = (r * conj(l)) / den;
	return x;
};

//
template <class T1, class T2> inline bool operator ==(const Complex<T1> & r, const Complex<T2> & l)
{
	return ((r.ree == l.ree) && (r.im == l.im));
};

//
template <class T1, class T2> inline bool operator !=(const Complex<T1> & r, const Complex<T2> & l)
{
	return ((r.ree != l.ree) || (r.im != l.im));
};

// Unit magnitude polar to rectangular conversion
inline Complex<double> expj(const double & x)
{
	return Complex<double>(cos(x), sin(x));
};
// Polar to rectangular conversion
inline Complex<double> polar(const double & amp, const double & arg)
{
	return Complex<double>(amp*cos(arg), amp*sin(arg));
}




// Complex<double> value (0,1) 
template <class T> inline Complex<T> Complexj(void)
{
	return(Complex<T>(0, 1));
}

template <class T> inline T re(const Complex<T> & x)
{
	return x.ree;
};

template <class T> inline T im(const Complex<T> & x)
{
	return x.im;
};

// Conjugate
template <class T> inline Complex<T> conj(const Complex<T> & x)
{
	return Complex<T>(x.ree, -x.im);
}
// Magnitude Squared of Complex vector
template <class T> inline T magsq(const Complex<T> & x)
{
	return (x.ree*x.ree + x.im*x.im);
};
// Normalized vector (magnitude = 1)
template <class T> inline Complex<double> norm(const Complex<T> & x)
{
	T y;
	y = sqrt(x.ree*x.ree + x.im*x.im);
	return (Complex<double>(x.ree / y, x.im / y));
};
template <class T> inline Complex<double> reciprical(const Complex<T> & x)
{
	T y;
	y = (x.ree*x.ree + x.im*x.im);
	return (Complex<double>(x.ree / y, -x.im / y));
};

template <class T> inline Complex<T> c_abs(const Complex<T> & x)
{
	return(Complex<T>(ABS(x.ree), ABS(x.im)));
};

template <class T> inline T mag(const Complex<T> & x)
{
	return((T)sqrt(x.ree*x.ree + x.im*x.im));
};

template <class T> inline T approx_mag(const Complex<T> & x)
{
	return(MAX(abs(x.ree), abs(x.im)) + MIN(abs(x.ree), abs(x.im)) / 4);
};

template <class T> inline Complex<T> maxvalue(const Complex<T> & x1)
{
	return(Complex<T>(MAX(x1.ree, x1.im)));
};

template <class T> inline Complex<T> minimum(const Complex<T> & x1, const Complex<T> & x2)
{
	return(Complex<T>(MIN(x1.ree, x2.ree), MIN(x1.im, x2.im)));
}
template <class T> inline Complex<T> maximum(const Complex<T> & x1, const Complex<T> & x2)
{
	return(Complex<T>(MAX(x1.ree, x2.ree), MAX(x1.im, x2.im)));
};
// Return phase angle (radians) of Complex number
template <class T> inline double arg(const Complex<T> & x)
{
	double TMPANG;
	if (x.ree == 0) {
		//     if (x.im < 0) return(3.0*PI/2.0);
		//     else return(PI/2.0);
		if (x.im >= 0) return(PI / 2.0);
		else return(1.5*PI);
	}
	else {
		TMPANG = atan2((double)x.im / (double)x.ree);
		//        if (x.ree < 0) TMPANG -= PI;
		if (x.ree >= 0); else TMPANG -= PI;
		if (TMPANG < 0) TMPANG += TWOPI;
	}
	return(TMPANG);
};
template <class T> inline double carg(const Complex<T> & x)
{
	return(atan2((double)x.im, (double)x.re));
};
// Convert to Complex<double>
template <class T> inline Complex<double> rational(const Complex<T> & l) {
	return(Complex<double>((double)l.ree, (double)l.im));
}
// Round by bits, to near integer - return(Complex<long>)
inline Complex<long> round(const Complex<long> & in, const long & bits)
{
	double scale = 1.0 / (double)(1 << bits);
	return(Complex<long>((long)floor((double)(scale*in.re) + 0.5),
		(long)floor((double)(scale*in.im) + 0.5)));
}
/*
// Saturate to specific number of bits (signed)
inline Complex<long> saturate(const Complex<long> & in, const long & bits)
{
Complex<long> out;
long low_mask = ((1<<(bits-1)) - 1);
if (labs(in.ree) > low_mask) out.ree = (in.ree>0) ? low_mask : ~low_mask;
else out.ree = in.ree;
if (labs(in.im) > low_mask) out.im = (in.im>0) ? low_mask : ~low_mask;
else out.im = in.im;
return(out);
}
*/
/*
template <class T> inline Complex<T> signbit(const Complex<T> & in)
{
return(Complex<T>((T) SGN(in.ree), (T)SGN(in.im)));
}
*/
template <class T> inline T real(const Complex<T> & y) { return(y.ree); }
template <class T> inline T imag(const Complex<T> & y) { return(y.im); }

template <class T> inline Complex<float> to_Complex_float(const Complex<T> & Y)
{
	return Complex<float>(float(Y.ree), float(Y.im));
}
template <class T> inline Complex<double> to_Complex_double(const Complex<T> & Y)
{
	return Complex<double>(double(Y.ree), double(Y.im));
}
/*
template <class T> inline ostream& operator<<(ostream& os, const Complex<T> & r)
{
return( os << "(" << r.ree << "," << r.im << ")" );
}
*/










/*
template <class T>	inline const Complex<T> &  operator *=(const int& y) {
T r = re*y - im*y;
im  = re*y + im*y;
re = r;
return *this;
}
*/
template <class T>	inline Complex<T> operator *(const int & r, const Complex<T> & l)
{
	return Complex<T>(r*l.ree, r*l.im);
};
template <class T>	inline Complex<T> operator *(const Complex<T> & l, const int & r)
{
	return Complex<T>(r*l.ree, r*l.im);
};





#endif

