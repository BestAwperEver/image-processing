#pragma once
#ifndef COMPLEX_H
#define COMPLEX_H
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
using namespace std;

class Complex {
	double r, i;
public:
	Complex(string);
	Complex(double Real = 0, double Imaginary = 0) : r(Real), i(Imaginary) { }
	~Complex(void) { };
public:
	double re() const;
	double im() const;
	double norm2() const;
	double norm() const;
	Complex conj() const;
	Complex dot(Complex) const;
	bool isNull() const { return abs(r) < 1e-8 && abs(i) < 1e-8; }
public:
	Complex operator += (Complex);
	Complex operator *= (Complex);
	Complex operator /= (Complex);
	Complex operator -= (Complex);
	bool operator != (Complex);
	bool operator == (Complex);
};

class ComplexVector : public std::vector<Complex> {
public:
	ComplexVector(int size, Complex def_el = 0);
	ComplexVector(initializer_list<string> l): vector<Complex>(l.size(), 0) {
		int i = 0;
		for (auto it = l.begin(), e = l.end(); it != e; ++it) {
			(*this)[i] = *it;
			++i;
		}
	}
	ComplexVector(initializer_list<Complex> l) : vector<Complex>(l) {}
	Complex operator * (const ComplexVector& v);
	double norm2() const;
	double norm() const;
	ComplexVector acf() const;
	Complex conv(int wave_index) const;
	ComplexVector ecv(int new_size);
};

ostream& operator << (ostream&, Complex);
ostream& operator << (ostream&, const ComplexVector&);

Complex operator + (Complex z1, Complex z2);
Complex operator - (Complex z1, Complex z2);
Complex operator * (Complex z1, Complex z2);
Complex operator / (Complex z1, Complex z2);

Complex ndp(ComplexVector G, ComplexVector N);
Complex tau_max(ComplexVector G, ComplexVector N);

#endif

