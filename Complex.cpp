#include "Complex.h"
#include <boost/lexical_cast.hpp>

Complex::Complex(string s) :r(0), i(0) {
	int ind, beg;
	if (s[0] == '-') {
		beg = 1;
		for (ind = beg; ind < s.length() && s[ind] >= '0' && s[ind] <= '9'; ++ind);
		if (ind < s.length() && s[ind] == 'i') {
			if (beg == ind) i = -1; else
				i = (-1)*boost::lexical_cast<double, string>(s.substr(beg, ind - beg));
			return;
		}
		else
			r = (-1)*boost::lexical_cast<double, string>(s.substr(beg, ind - beg));
	}
	else {
		beg = 0;
		for (ind = beg; ind < s.length() && s[ind] >= '0' && s[ind] <= '9'; ++ind);
		if (ind < s.length() && s[ind] == 'i') {
			if (beg == ind) i = 1; else
				i = boost::lexical_cast<double, string>(s.substr(beg, ind - beg));
			return;
		}
		else
			r = boost::lexical_cast<double, string>(s.substr(beg, ind - beg));
	}
	beg = ind++;
	if (beg >= s.length() - 1) return;
	if (s[beg] == '-') {
		if (s[beg + 1] == 'i') i = -1; else
			i = (-1)*boost::lexical_cast<double, string>(s.substr(beg + 1, s.length() - beg - 2));
	}
	else {
		if (s[beg + 1] == 'i') i = 1; else
			i = boost::lexical_cast<double, string>(s.substr(beg + 1, s.length() - beg - 2));
	}
}

double Complex::re() const {
	return r;
}

double Complex::im() const {
	return i;
}

double Complex::norm2() const {
	return r*r + i*i;
}

double Complex::norm() const {
	return sqrt(norm2());
}

ostream& operator << (ostream& str, Complex z) {
	//if (abs(z.im()) != 1)
	//	if (z.im()>0)
	//		str << z.re() << "+" << z.im() << 'i';
	//	else
	//		str << z.re() << z.im() << 'i';
	//else
	//	if (z.im()>0)
	//		str << z.re() << "+" << 'i';
	//	else
	//		str << z.re() << "-i";
	if (z.isNull()) {
		str << 0;
		return str;
	}
	if (z.re() != 0) {
		str << z.re();
	}
	if (z.im() == 0) {
		return str;
	}
	if (z.re() == 0 || z.im() < 0) {
		str << z.im() << "i";;
		return str;
	}
	str << "+" << z.im() << "i";
	return str;
}
ostream& operator << (ostream& str, const ComplexVector& Z) {
	for each (Complex z in Z)
	{
		str << z << endl;
	}
	return str;
}

/*
1.03
а) Перегрузка арифметических операций += -= *= /= для комплексных чисел
z1 * z2 = (x1x2 - y1y2) + i(x1y2+x2y1)
z1/z2 = ( (x1x2+y1y2) + i(x1y2 - x2y1) ) / (x2^2+y2^2)
*/

Complex Complex::operator += (Complex z) {
	r += z.r;
	i += z.i;
	return *this;
}
Complex Complex::operator -= (Complex z) {
	r -= z.r;
	i -= z.i;
	return *this;
}
Complex Complex::operator *= (Complex z) {
	double ro = r;
	r = ro*z.r - i*z.i;
	i = ro*z.i + i*z.r;
	return *this;
}
Complex Complex::operator /= (Complex z) {
	r = r*z.r + i*z.i;
	r /= z.i*z.i + z.r*z.r;
	i = r*z.i - i*z.r;
	i /= z.i*z.i + z.r*z.r;
	return *this;
}

Complex operator + (Complex z1, Complex z2) {
	return Complex(z1 += z2);
}
Complex operator - (Complex z1, Complex z2) {
	return Complex(z1 -= z2);
}
Complex operator * (Complex z1, Complex z2) {
	return (z1 *= z2);
}
Complex operator / (Complex z1, Complex z2) {
	return (z1 /= z2);
}

Complex Complex::conj() const {
	return Complex(r, -i);
}
Complex Complex::dot(Complex z) const {
	return conj() * z;
}

bool Complex::operator == (Complex z) {
	return (r == z.r && i == z.i);
}
bool Complex::operator != (Complex z) {
	return !(*this == z);
}

ComplexVector::ComplexVector(int size, Complex defel): vector<Complex>(size, defel) {}

Complex ComplexVector::operator * (const ComplexVector& v) {
	if (v.size() != size()) {
		return 0;
	}
	Complex sum = 0;
	for (int i = 0; i < size(); ++i) {
		sum += (*this)[i].dot( v[i] );
	}
	return sum;
}
double ComplexVector::norm2() const {
	double norm2 = 0;
	for (int i = 0; i < size(); ++i) {
		norm2 += (*this)[i].norm2();
	}
	return norm2;
}
double ComplexVector::norm() const {
	return sqrt(norm2());
}
ComplexVector ComplexVector::acf() const {
	ComplexVector acf(size());
	for (int k = 0; k < size(); ++k) {
		for (int i = 0; i < size(); ++i) {
			acf[k] += (*this)[i].dot( (*this)[(i + k) % size()] );
		}
	}
	return acf;
}
Complex ComplexVector::conv(int wave_ind) const {
	int k = -1, step = size() / (wave_ind + 1);
	Complex c;
	for (int i = 0; i < size(); ++i) {
		k += (i % step == 0);
		c = k % 2 ? c - (*this)[i] : c + (*this)[i];
	}
	return c;
}

Complex tau_max(ComplexVector G, ComplexVector N) {
	if (G.size() != N.size())
		return 0;
	Complex max = 0;
	Complex tau = 0;
	for (int k = 0; k < G.size(); ++k) {
		for (int i = 0; i < G.size(); ++i) {
			tau += G[i].dot(N[(i + k) % G.size()]);
		}
		if (tau.norm2() > max.norm2()) {
			max = tau;
		}
	}
	return max / G.norm() / N.norm();
}
Complex ndp(ComplexVector G, ComplexVector N) {
	return (G * N) / G.norm() / N.norm();
}

ComplexVector ComplexVector::ecv(int new_size) {
	ComplexVector new_v = ComplexVector(new_size, 0);

	for (int i = 0; i < new_size; i++)
	{
		double index = double(i * size()) / new_size;
		int j = (int)index;
		double k = index - j;
		new_v[i] = (*this)[j] * (1 - k) + (*this)[(j + 1) % size()] * k;
	}

	return new_v;
}