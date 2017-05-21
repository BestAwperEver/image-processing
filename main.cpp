#include <CImg.h>
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <map>
#include <list>
#include "Complex.h"
#include "matrix.h"
#include "AVLTree.h"
#include <stack>
//#include <boost/algorithm/string/split.hpp>

// Use the library namespace to ease the declarations afterward.
using namespace cimg_library;
using namespace std;

const unsigned char
	white[] = { 255, 255, 255 },
	black[] = { 0, 0, 0 },
	gray[] = { 127, 127, 127 },
	red[] = { 255, 0, 0 },
	green[] = { 0, 255, 0 };

void draw_cont() {
	CImg<unsigned char> image(100, 100);

	CImgDisplay disp(image);
	disp.resize(500, 500);

	cout << "enter first point" << endl;
	string s;
	cin >> s;
	Complex z0(s);
	Complex bias(0, 0);

	for (int x = 0; x < image.width(); ++x)
		for (int y = 0; y < image.height(); ++y)
			image.draw_point(x, y, white);

	image.draw_point(z0.re(), z0.im(), black);

	image.display(disp);

	do {
		cout << "enter next translation vector" << endl;
		cin >> s;
		Complex z(s);
		Complex b(s);
		bias += z;
		z = z0 + bias;
		Complex zp = z - b;
		/*image.draw_point(z.re(), z.im(), black);*/
		image.draw_line(zp.re(), zp.im(), z.re(), z.im(), black);
		image.display(disp);
	} while (!bias.isNull());

	cout << "Done." << endl;

	while (!disp.is_closed()) {
		disp.wait();
	}
}

void main1() {
	ComplexVector v = { "1", "i", "2-2i", "-3+i" };
	ComplexVector w = { "1", "-i", "2+2i", "-3-i" };
	ComplexVector z = { "1", "i", "2", "-i", "1", "-2i", "-1", "-i", "-2", "i", "-1", "2i" };
	cout
		<< "v:\n" << v << endl
		<< "w:\n" << w << endl
		<< "ndp:\n" << ndp(v, w) << endl
		<< "\ntau_max:\n" << tau_max(v, w) << endl
		<< "\nhaar #2:\n" << v.conv(1) << endl << endl
		<< "acf:\n" << z.acf() << endl;
	system("pause");
}

double g_gen(double x, double y, double s) {
	const double pi = 3.14159263;
	return exp(-(x*x + y*y) / 2 / s / s) / sqrt(2 * pi) / s;
}

int arg_ind(double x, double y) {
	const double pi = 3.14159263;
	double a = atan(y / x);
	if (a <= 0) a += 2 * pi;
	if (y < 0 && x < 0) a += pi;
	else if (x < 0) a -= pi;
	a = a / pi * 4;
	return a + 0.5;
}

void prim() {
	srand(time(0));
	cout << "Enter the number of vertices:" << endl;
	int N;
	cin >> N;
	matrix<int> ad(N);
	for (int i = 0; i < N; ++i) {
		for (int j = i; j < N; ++j) {
			if (i == j) ad(i, j) = 20;
			else {
				ad(i, j) = rand() % 15 + 5;
				ad(j, i) = ad(i, j);
			}
		}
	}

	const int w = 300;

	CImg<unsigned char> image(w, w), image_2(w, w);

	vector<pair<int, int>> coord(N);

	const double pi = 3.14159265;

	for (int i = 0; i < N; ++i) {
		image.draw_point(coord[i].first, coord[i].second, red);
	}
	
	vector<bool> vert(N, false);
	vert[0] = true;

	//while (disp.is_closed() == false)
	//	disp.wait();

	//int min = 20, min_edge;
	//for (int i = 0; i < N; ++i) {
	//	if (ad(0, i) < min) {
	//		min = ad(0, i);
	//		min_edge = i;
	//	}
	//}

	vector<pair<int, int>> edges(N - 1, {0,0});
	int k = 0;
	//edges[k++] = { 0, min_edge };

	while (find(vert.begin(), vert.end(), false) != vert.end()) {
		int min = 20;
		pair<int, int> min_edge;
		for (int i = 0; i < N; ++i) {
			if (vert[i]) {
				for (int j = 0; j < N; ++j) {
					if (!vert[j] && ad(i, j) < min) {
						min = ad(i, j);
						min_edge = { i, j };
					}
				}
			}
		}
		edges[k++] = min_edge;
		vert[min_edge.second] = true;
	}

	for (int i = 0; i < N; ++i) {
		coord[i] = { 0.4 * w * cos(2 * pi / N*i) + w / 2, 0.4 * w * sin(2 * pi / N*i) + w / 2 };
		for (int j = 0; j < i; ++j) {
			image.draw_line(coord[i].first, coord[i].second,
				coord[j].first, coord[j].second,
				black);
			char buf[3] = { 0 };
			image.draw_text((coord[i].first + coord[j].first) / 2 + 5,
				(coord[i].second + coord[j].second) / 2 + 5,
				itoa(ad(i, j), buf, 10), black, white, 1, 20);
		}
	}
	for each (auto edge in edges)
	{
		int i = edge.first, j = edge.second;
		image_2.draw_line(	coord[edge.first].first, coord[edge.first].second,
							coord[edge.second].first, coord[edge.second].second,
							black);
		char buf[3] = { 0 };
		image_2.draw_text(	(coord[i].first + coord[j].first) / 2 + 5,
							(coord[i].second + coord[j].second) / 2 + 5,
							itoa(ad(i, j), buf, 10), black, white, 1, 20);
	}

	CImgDisplay disp_2(image_2);
	CImgDisplay disp(image);
	//disp.resize(500, 500);

	system("pause");
}

//inline char bright(const CImg<unsigned char>& image, int x, int y) {
//	//if (x >= 0 && x < image.width() && y >= 0 && y < image.height()) {
//	//	return image(x, y, 1);
//	//}
//	if (x < 0) {
//		x = image.width() + x;
//	}
//	if (y < 0) {
//		y = image.height() + y;
//	}
//	if (x >= image.width()) {
//		x = x - image.width();
//	}
//	if (y >= image.height()) {
//		y = y - image.height();
//	}
//	return image(x, y, 0);
//}

void kanny() {
	std::cout << "Enter filename" << std::endl;
	char filename_jpg[128];
	std::cin >> filename_jpg;

	const matrix<double> Gx = { {1, 0, -1}, {2, 0, -2}, {1, 0, -1} };
	const matrix<double> Gy = { { 1, 2, 1 }, { 0, 0, 0 }, { -1, -2, -1 } };

	const double sigma = 1;
	const double shift = 2 * sigma;
	const int N = 2 * shift + 1;
	matrix<double> gauss(N);
	double sum = 0;
	for (int i = -shift; i <= shift; ++i) {
		for (int j = -shift; j <= shift; ++j) {
			gauss(shift + i, shift + j) = g_gen(i, j, sigma);
			sum += gauss(shift + i, shift + j);
		}
	}
	gauss = gauss / sum;

	cout << gauss << endl;

	//char filename_jpg[] = "habr.jpg";

	CImg<unsigned char> image(filename_jpg);// , image_orig(filename_jpg);
	CImgDisplay disp(image);// , disp_orig(image_orig);

	//for (int x = 0; x < image.width(); ++x) {
	//	for (int y = 0; y < image.height(); ++y) {
	//		double sumr = 0, sumb = 0, sumg = 0;;
	//		for (int i = -shift; i <= shift; ++i) {
	//			for (int j = -shift; j <= shift; ++j) {
	//				if (x + i >= 0 && x + i < image.width() && y + j >= 0 && y + j < image.height()) {
	//					sumr += image_orig(x, y, 0) * gauss(shift + i, shift + j);
	//					sumg += image_orig(x, y, 1) * gauss(shift + i, shift + j);
	//					sumb += image_orig(x, y, 2) * gauss(shift + i, shift + j);
	//				} else {
	//					sumr += 127 * gauss(shift + i, shift + j);
	//					sumg += 127 * gauss(shift + i, shift + j);
	//					sumb += 127 * gauss(shift + i, shift + j);
	//				}
	//			}
	//		}
	//		unsigned char color[3] = { sumr, sumg, sumb };
	//		image_new.draw_point(x, y, color);
	//	}
	//}

	cout << "Grayscaling... ";

	for (int x = 0; x < image.width(); ++x)
		for (int y = 0; y < image.height(); ++y) {
			unsigned char c = 0.299*image(x, y, 0) + 0.587*image(x, y, 1) + 0.114*image(x, y, 2);
			unsigned char color[3] = { c, c, c };
			image.draw_point(x, y, color);
		}

	cout << "Done." << endl;

	system("pause");
	image.display(disp);

	CImg<unsigned char> image_grayscaled(image);

	cout << "Gaussian blur... ";

	for (int x = 0; x < image.width(); ++x) {
		for (int y = 0; y < image.height(); ++y) {
			double sum = 0;
			for (int i = -shift; i <= shift; ++i) {
				for (int j = -shift; j <= shift; ++j) {
					//if (x + i >= 0 && x + i < image.width() && y + j >= 0 && y + j < image.height())
					//	sum += image(x, y, 1) * gauss(shift + i, shift + j);
					//else
					//	sum += 127 * gauss(shift + i, shift + j);
					int x_t = x + i, y_t = y + j;
					if (x_t < 0) {
						x_t = image.width() + x_t;
					} else if (x_t >= image.width()) {
						x_t = x_t - image.width();
					}
					if (y_t < 0) {
						y_t = image.height() + y_t;
					} else if (y_t >= image.height()) {
						y_t = y_t - image.height();
					}
					sum += image_grayscaled(x_t, y_t, 0) * gauss(shift + i, shift + j);
				}
			}
			//unsigned char oldr = image(x, y, 1), oldb = image(x, y, 2);
			unsigned char c = sum;
			unsigned char color[3] = { c, c, c };
			image.draw_point(x, y, color);
		}
	}

	cout << "Done." << endl;

	system("pause");
	image.display(disp);

	matrix<double>	grad(image.width(), image.height());
	matrix<int>		angle(image.width(), image.height());

	cout << "Calculating gradient... ";

	for (int x = 0; x < image.width(); ++x) {
		for (int y = 0; y < image.height(); ++y) {
			double gx = 0, gy = 0;
			for (int i = -1; i <= 1; ++i) {
				for (int j = -1; j <= 1; ++j) {
					int x_t = x + i, y_t = y + j;
					if (x_t < 0) {
						x_t = image.width() + x_t;
					}
					else if (x_t >= image.width()) {
						x_t = x_t - image.width();
					}
					if (y_t < 0) {
						y_t = image.height() + y_t;
					}
					else if (y_t >= image.height()) {
						y_t = y_t - image.height();
					}
					gx += image(x_t, y_t, 0) * Gx(1 + i, 1 + j);
					gy += image(x_t, y_t, 0) * Gy(1 + i, 1 + j);
				}
			}
			grad(x, y) = sqrt(gx * gx + gy * gy);
			angle(x, y) = arg_ind(gx, gy) % 8;
		}
	}

	double grad_max = grad.max();

	for (int x = 0; x < image.width(); ++x) {
		for (int y = 0; y < image.height(); ++y) {
			char c = grad(x, y) / grad_max * 255;
			char color[3] = { c, c, c };
			image.draw_point(x, y, color);
		}
	}

	cout << "Done." << endl;

	system("pause");
	image.display(disp);

	vector<pair<int, int>> dir = { {1, 0},{1, 1},{0,1},{-1,1},{-1,0},{-1,-1},{0,-1},{1,-1} };

	cout << "Non-maximum suppression... ";

	for (int x = 0; x < image.width(); ++x) {
		for (int y = 0; y < image.height(); ++y) {
			int a = angle(x, y);
			int i = dir[angle(x, y)].first,
				j = dir[angle(x, y)].second;
			int x_t = x + i, y_t = y + j;
			if (x_t < 0) {
				x_t = image.width() + x_t;
			}
			else if (x_t >= image.width()) {
				x_t = x_t - image.width();
			}
			if (y_t < 0) {
				y_t = image.height() + y_t;
			}
			else if (y_t >= image.height()) {
				y_t = y_t - image.height();
			}
			i = -i;
			j = -j;
			int x_t2 = x + i, y_t2 = y + j;
			if (x_t2 < 0) {
				x_t2 = image.width() + x_t2;
			}
			else if (x_t2 >= image.width()) {
				x_t2 = x_t2 - image.width();
			}
			if (y_t2 < 0) {
				y_t2 = image.height() + y_t2;
			}
			else if (y_t2 >= image.height()) {
				y_t2 = y_t2 - image.height();
			}
			if (image(x, y, 0) < image(x_t, y_t, 0) && image(x, y, 0) < image(x_t2, y_t2, 0)) {
				image.draw_point(x, y, black);
			}
		}
	}

	cout << "Done." << endl;

	system("pause");
	image.display(disp);

	cout << "Double thresholding... ";

	double low = .2, high = .25;// , percent = .01;

	for (int x = 0; x < image.width(); ++x) {
		for (int y = 0; y < image.height(); ++y) {
			//const int tens = image.width()*image.height() / 10;
			//if ((x*y + 1) % tens == 0) {
			//	cout << x*y/tens << "%.";
			//}
			int local_max = 255;
			//for (int i = -percent*image.width(); i <= percent*image.width(); ++i) {
			//	for (int j = -percent*image.height(); j <= percent*image.height(); ++j) {
			//		int x_t = x + i, y_t = y + j;
			//		if (x_t < 0) {
			//			x_t = image.width() + x_t;
			//		}
			//		else if (x_t >= image.width()) {
			//			x_t = x_t - image.width();
			//		}
			//		if (y_t < 0) {
			//			y_t = image.height() + y_t;
			//		}
			//		else if (y_t >= image.height()) {
			//			y_t = y_t - image.height();
			//		}
			//		if (image(x_t, y_t, 0) > local_max) {
			//			local_max = image(x_t, y_t, 0);
			//		}
			//	}
			//}
			if (image(x, y, 0) < low * local_max) {
				image.draw_point(x, y, black);
			} else if (image(x, y, 0) > high * local_max) {
				image.draw_point(x, y, white);
			} else {
				image.draw_point(x, y, gray);
			}
		}
	}

	cout << "Done." << endl;

	system("pause");
	image.display(disp);

	cout << "Linking... ";

	for (int x = 0; x < image.width(); ++x) {
		for (int y = 0; y < image.height(); ++y) {
			if (image(x, y, 0) == 127) {
				bool found = false;
				for (int i = -2; i <= 2; ++i) 
					for (int j = -2; j <= 2; ++j) {
						//int i = dir[k].first,
						//	j = dir[k].second;
						int x_t = x + i, y_t = y + j;
						if (x_t < 0) {
							x_t = image.width() + x_t;
						}
						else if (x_t >= image.width()) {
							x_t = x_t - image.width();
						}
						if (y_t < 0) {
							y_t = image.height() + y_t;
						}
						else if (y_t >= image.height()) {
							y_t = y_t - image.height();
						}
						if (image(x_t, y_t, 0) == 255) {
							image.draw_point(x, y, white);
							found = true;
							break;
						}
					}
				if (!found) {
					image.draw_point(x, y, black);
				}
			}
		}
	}

	cout << "Done." << endl;

	system("pause");
	image.display(disp);

	cout << "Finished." << endl;
	system("pause");

}

Complex sum(ComplexVector v) {
	Complex sum1 = 0;
	for each (Complex c in v)
	{
		sum1 += c;
	}
	return sum1;
}

void main2() {
	ComplexVector test_vec = { "6", "3+6i", "3-6i", "6", "-4-3i", "4-5i", "-9+5i", "-9-5i", "4+5i", "-4+3i" };
	cout << sum(test_vec) << endl;

	const int n = 50;

	CImg<unsigned int> image(n, n);

	Complex c0 = Complex(10, 30);

	for each (Complex c in test_vec)
	{
		Complex c1 = c0 + c;
		image.draw_line(c0.re(), c0.im(), c1.re(), c1.im(), black);
		c0 += c;
	}

	CImgDisplay disp(image);
	disp.resize(200, 200);

	ComplexVector new_vec = test_vec.ecv(test_vec.size() * 1.8);

	cout << test_vec << endl << new_vec << endl;

	CImg<unsigned int> image2(n, n);

	Complex c02 = Complex(10, 30);

	for each (Complex c in new_vec)
	{
		Complex c1 = c02 + c;
		image2.draw_line(c02.re(), c02.im(), c1.re(), c1.im(), black);
		c0 += c;
	}

	CImgDisplay disp2(image2);
	disp2.resize(200, 200);

	system("pause");
}

void recursive_alg(matrix<int>& M, CImg<unsigned char>& img, int x, int y, int index) {
	static int s = 0;
	++s;
	if (M(x, y) == 0 && img(x, y, 1) == 0) {
		M(x, y) = index;
		for (int i = -1; i <= 1; ++i) {
			for (int j = -1; j <= 1; ++j) {
				if (x + i >= 0 && x + i < img.width() && y + j >= 0 && y + j < img.height()) {
					recursive_alg(M, img, x + i, y + j, index);
				}
			}
		}
	}
	--s;
}

void recur() {

	std::cout << "Enter filename" << std::endl;
	char filename[128];
	std::cin >> filename;

	CImg<unsigned char> image(filename);
	matrix<int> M(image.width(), image.height(), 0);

	int index = 0;

	for (int x = 0; x < image.width(); ++x) {
		for (int y = 0; y < image.height(); ++y) {
			if (M(x, y) == 0 && image(x, y, 1) == 0) {
				recursive_alg(M, image, x, y, ++index);
			}
		}
	}

	cout << M << endl;

	CImg<unsigned char> image2(image.width(), image.height(), 1, 3);

	vector<unsigned char> colours(index*3);
	for (int i = 0; i < index; ++i) {
		colours[3 * i] = rand() % 256;
		colours[3 * i + 1] = rand() % 256;
		colours[3 * i + 2] = rand() % 256;
	}

	for (int x = 0; x < image.width(); ++x) {
		for (int y = 0; y < image.height(); ++y) {
			if (M(x, y) != 0) {
				unsigned char c[3] =
				{
					colours[3 * (M(x, y) - 1)],
					colours[3 * (M(x, y) - 1) + 1],
					colours[3 * (M(x, y) - 1) + 2]
				};
				image2.draw_point(x, y, c);
			} else {
				image2.draw_point(x, y, white);
			}
		}
	}

	CImgDisplay disp2(image2);
	disp2.resize(100, 100);
	CImgDisplay disp(image);
	disp.resize(100, 100);

	CImg<unsigned char> img(10, 10);

	//for (int i = 0; i < img.width(); ++i) {
	//	for (int j = 0; j < img.height(); ++j) {
	//		img.draw_point(i, j, green);
	//	}
	//}

	//CImgDisplay dsp(img);
	//dsp.resize(100, 100);

	//for (int i = 0; i < 3; ++i)
	//	cout << int(img(5, 5, 0, i)) << endl;

	::system("pause");
}

matrix<double> autocorm(CImg<unsigned char>& image, int x, int y) {
	matrix<double> M(2, 2, 0);
	for (int i = -1; i <= 1; ++i) {
		for (int j = -1; j <= 1; ++j) {
			M(0, 0) = image(x + i - 1, y + j, 0) - 2 * image(x + i, y + j, 0) + image(x + i + 1, y + j, 0);
			M(0, 1) = (image(x + i - 1, y + j - 1, 0) + image(x + i + 1, y + j + 1, 0)
				- image(x + i - 1, y + j + 1, 0) - image(x + i + 1, y + j - 1, 0)) / 4.0;
			M(1, 0) = M(0, 1);
			M(1, 1) = image(x + i, y + j - 1, 0) - 2 * image(x + i, y + j, 0) + image(x + i, y + j + 1, 0);
		}
	}
	return M;
}

double O(matrix<double> R, int x, int y) {
	double max = 0;
	for (int i = -1; i <= 1; ++i) {
		for (int j = -1; j <= 1; ++j) {
			if (i == 0 && j == 0)
				continue;
			if (x + i >= 0 && x + i < R.width() && y + j >= 0 && y + j < R.height()) {
				if (R(x + i, y + j) > max)
					max = R(x + i, y + j);
			}
		}
	}
	return max;
}

void harris() {
	std::cout << "Enter filename" << std::endl;
	char filename[128];
	std::cin >> filename;

	CImg<unsigned char> image(filename);
	

	for (int x = 0; x < image.width(); ++x)
		for (int y = 0; y < image.height(); ++y) {
			unsigned char c = 0.299*image(x, y, 0) + 0.587*image(x, y, 1) + 0.114*image(x, y, 2);
			unsigned char color[3] = { c, c, c };
			image.draw_point(x, y, color);
		}

	CImgDisplay disp(image);
	disp.resize(100, 100);

	matrix<double> M(2, 2, 0);
	matrix<double> R(image.width(), image.height(), 0);
	double k = 0.05;

	for (int x = 2; x < image.width() - 2; ++x) {
		for (int y = 2; y < image.height() - 2; ++y) {
			M = autocorm(image, x, y);
			R(x, y) = M.determinant() - k*M.trace()*M.trace();
		}
	}

	cout << R << endl << endl;

	double max = R.max();

	for (int x = 2; x < image.width() - 2; ++x) {
		for (int y = 2; y < image.height() - 2; ++y) {
			if (R(x, y) < max*0.3) R(x, y) = 0;
		}
	}

	cout << R << endl << endl;

	for (int x = 2; x < image.width() - 2; ++x) {
		for (int y = 2; y < image.height() - 2; ++y) {
			if (!(R(x, y) > O(R, x, y))) {
				R(x, y) = 0;
			}
		}
	}

	cout << R << endl;

	system("pause");
}

std::pair<double, double> ent(char* s, int len) {
	int x = 0;
	for (int i = 0; i < len; ++i)
	{
		if (*(s+i) == 'x') ++x;
	}
	double count = len;
	if (x == 0 || x == len) return { 0, x/count };
	return { -log(x / count)*x / count - log((count - x) / count)*(count - x) / count, x/count };
}

bool zero_entropy(char* s, int len, int& min_index) {
	double min_ent = -1;
	min_index = 0;
	auto s0 = ent(s, len);
	if (s0.first == 0) return true;
	for (int i = 1; i < len; ++i) {
		auto s1 = ent(s + i, len - i);
		auto s2 = ent(s, i);
		double e = s1.first + (s0.second - s1.second) * (s2.first - s1.first) / (s2.second - s1.second);
		//double e = (i == 0) ? (ent(s + i, len - i) + ent(s, i)):(ent(s + i, len - i) + ent(s, i)) / 2;
		if (e < min_ent || min_ent == -1) {
			min_ent = e;
			min_index = i;
		}
	}
	return (min_index == 0);
}

void step(char* s, int len, /*int tree[128], int place_index,*/ int shift, AVLTree<int>& T) {
	int index;
	if (!zero_entropy(s, len, index)) {
		//tree[place_index] = index + shift;
		T.add_item(index + shift);
		step(s, index, /*tree, place_index * 2 + 1,*/ shift, T);
		step(s + index, len - index, /*tree, place_index * 2 + 2,*/ shift + index, T);
	}
}

void dec_tree() {
	AVLTree<int> Tree([](const int& p1, const int& p2) {
			if (p1 == p2) return 0;
			if (p1> p2) return 1;
			return -1;
		});
	cout << "Enter string that contains x-es and y-es" << endl;
	//const int N = 7;
	//int tree[(1 << N)] = { 0 };

	//char buf[] = "xxxyxyyxxyxx";
	char buf[128] = { 0 };
	cin >> buf;

	int len = strlen(buf);
	step(buf, len, 0, Tree);

	//for (int k = 0; k < N; ++k) {
	//	for (int i = 0; i < (1 << k); ++i) {
	//		cout << tree[i + (1 << k) - 1] << ' ';
	//	}
	//	cout << endl;
	//}

	cout << endl;

	Tree.add_xy(true);

	Tree.print();

	system("pause");
}

struct edge {
	int first;
	int second;
	int weight;
};

void graph() {
	srand(time(0));
	cout << "Enter the number of vertices:" << endl;
	int N;
	cin >> N;
	matrix<int> ad(N);
	for (int i = 0; i < N; ++i) {
		for (int j = i; j < N; ++j) {
			if (i == j) ad(i, j) = 20;
			else {
				ad(i, j) = rand() % 15 + 5;
				ad(j, i) = ad(i, j);
			}
		}
	}

	const int w = 300;

	CImg<unsigned char> image(w, w, 1, 3), image_2(w, w, 1, 3);

	vector<pair<int, int>> coord(N);

	const double pi = 3.14159265;

	for (int i = 0; i < N; ++i) {
		coord[i] = { 0.4 * w * cos(2 * pi / N*i) + w / 2, 0.4 * w * sin(2 * pi / N*i) + w / 2 };
		for (int j = 0; j < i; ++j) {
			image.draw_line(coord[i].first, coord[i].second,
				coord[j].first, coord[j].second,
				black);
			char buf[3] = { 0 };
			image.draw_text((coord[i].first + coord[j].first) / 2 + 5,
				(coord[i].second + coord[j].second) / 2 + 5,
				itoa(ad(i, j), buf, 10), black, white, 1, 20);
		}
	}

	for (int i = 0; i < N; ++i) {
		//image.draw_point(coord[i].first, coord[i].second, red);
		image.draw_circle(coord[i].first, coord[i].second, 5, red);
	}

	vector<bool> vert(N, false);
	vert[0] = true;

	vector<edge> edges(N - 1, { 0,0,0 });
	int k = 0;

	while (find(vert.begin(), vert.end(), false) != vert.end()) {
		int min = 20;
		edge min_edge;
		for (int i = 0; i < N; ++i) {
			if (vert[i]) {
				for (int j = 0; j < N; ++j) {
					if (!vert[j] && ad(i, j) < min) {
						min = ad(i, j);
						min_edge = { i, j, min };
					}
				}
			}
		}
		edges[k++] = min_edge;
		vert[min_edge.second] = true;
	}

	std::sort(edges.begin(), edges.end(), [](edge e1, edge e2)->bool {
		if (e1.weight < e2.weight) return true;
		return false;
	});

	const int K = 2;

	edges.resize(edges.size() - K + 1);

	vert.clear();
	vert.resize(N, false);

	matrix<bool> ad2(N);

	for (int i = 0; i < N; ++i)
		for (int j = 0; j < N; ++j)
			ad2(i, j) = false;

	//edges.clear();
	//edges.resize(N - 2, { 0,0,0 });
	//edges[0] = { 3,5,6 };
	//edges[1] = { 1,4,7 };
	//edges[2] = { 4,5,8 };
	//edges[3] = { 0,4,9 };

	for each (auto edge in edges)
	{
		int i = edge.first, j = edge.second;
		ad2(i, j) = true;
		ad2(j, i) = true;
	}

	cout << ad2 << endl;

	int i0, i0_old;

	for (int i = 0; i < N; ++i) {
		int sum = 0;
		for (int j = 0; j < N; ++j) {
			sum += ad2(i, j);
		}
		if (sum == 1) {
			i0 = i;
			break;
		}
	}

	vert[i0] = true;
	bool founded = true;
	bool repeat = false;
	edge edge0;
	edge* p_repeat_edge;

	while (founded) {

		founded = false;
		repeat = false;

		for (int i = 0; i < edges.size(); ++i) {
			edge& edge = edges[i];
			if (edge.first == i0) {
				if (edge.weight == 25) {
					repeat = true;
					founded = false;
					p_repeat_edge = &edge;
					continue;
				} else {
					edge.weight = 25;
				}
				i0 = edge.second;
				edge0 = edge;
				vert[i0] = true;
				founded = true;
				break;
			}
		}

		if (!founded) for (int i = 0; i < edges.size(); ++i) {
			edge& edge = edges[i];
			if (edge.second == i0) {
				if (edge.weight == 25) {
					repeat = true;
					founded = false;
					p_repeat_edge = &edge;
					continue;
				}
				else {
					edge.weight = 25;
				}
				i0 = edge.first;
				edge0 = edge;
				vert[i0] = true;
				founded = true;
				break;
			}
		}

		if (!founded && repeat) {
			founded = true;
			edge0 = *p_repeat_edge;
			i0 = edge0.first == i0 ? edge0.second : edge0.first;
			p_repeat_edge->first = -1;
			p_repeat_edge->second = -1;
			p_repeat_edge->weight = 0;
		}
		
		if (!founded) break;

		int i = edge0.first, j = edge0.second;

		image_2.draw_line(coord[edge0.first].first, coord[edge0.first].second,
			coord[edge0.second].first, coord[edge0.second].second,
			black);

		char buf[3] = { 0 };

		image_2.draw_text((coord[i].first + coord[j].first) / 2 + 5,
			(coord[i].second + coord[j].second) / 2 + 5,
			itoa(ad(i, j), buf, 10), black, white, 1, 20);
		image_2.draw_circle(coord[i].first, coord[i].second, 5, red);
		image_2.draw_circle(coord[j].first, coord[j].second, 5, red);
	}

	//for each (auto edge0 in edges) {
	//	if (edge0.first != -1) {
	//		int i = edge0.first, j = edge0.second;

	//		image_2.draw_line(coord[edge0.first].first, coord[edge0.first].second,
	//			coord[edge0.second].first, coord[edge0.second].second,
	//			black);

	//		char buf[3] = { 0 };

	//		image_2.draw_text((coord[i].first + coord[j].first) / 2 + 5,
	//			(coord[i].second + coord[j].second) / 2 + 5,
	//			itoa(ad(i, j), buf, 10), black, white, 1, 20);
	//		image_2.draw_circle(coord[i].first, coord[i].second, 5, green);
	//		image_2.draw_circle(coord[j].first, coord[j].second, 5, green);
	//	}
	//}

	for (int i = 0; i < N; ++i) {
		if (vert[i] == false) {
			for each (auto edge0 in edges) {
				if (edge0.first == i || edge0.second == i) {
					int i = edge0.first, j = edge0.second;

					image_2.draw_line(coord[edge0.first].first, coord[edge0.first].second,
						coord[edge0.second].first, coord[edge0.second].second,
						black);

					char buf[3] = { 0 };

					image_2.draw_text((coord[i].first + coord[j].first) / 2 + 5,
						(coord[i].second + coord[j].second) / 2 + 5,
						itoa(ad(i, j), buf, 10), black, white, 1, 20);
					image_2.draw_circle(coord[i].first, coord[i].second, 5, green);
					image_2.draw_circle(coord[j].first, coord[j].second, 5, green);
				}
			}
			image_2.draw_circle(coord[i].first, coord[i].second, 5, green);
		}
	}

	CImgDisplay disp_2(image_2);
	CImgDisplay disp(image);

	::system("pause");
}

list<int> gamma(matrix<int> ad, int v, int treshold) {
	list<int> G;
	for (int i = 0; i < ad.width(); ++i) {
		if (ad(i, v) <= treshold) {
			G.push_back(i);
		}
	}
	return move(G);
}

void clique() {
	srand(time(0));
	const int treshold = 22;
	cout << "Enter the number of vertices:" << endl;
	int N;
	cin >> N;
	matrix<int> ad(N);
	for (int i = 0; i < N; ++i) {
		for (int j = i; j < N; ++j) {
			if (i == j) ad(i, j) = 35;
			else {
				ad(i, j) = rand() % 30 + 5;
				ad(j, i) = ad(i, j);
			}
		}
	}

	const int w = 300;

	CImg<unsigned char> image(w, w, 1, 3), image_2(w, w, 1, 3);

	vector<pair<int, int>> coord(N);

	const double pi = 3.14159265;

	for (int i = 0; i < N; ++i) {

		coord[i] = { 0.4 * w * cos(2 * pi / N*i) + w / 2, 0.4 * w * sin(2 * pi / N*i) + w / 2 };
		
		for (int j = 0; j < i; ++j) {
			if (ad(i, j) > treshold) {
				image.draw_line(coord[i].first, coord[i].second,
					coord[j].first, coord[j].second,
					black);
			}
		}
	}

	for (int i = 0; i < N; ++i) {
		//image.draw_point(coord[i].first, coord[i].second, red);
		image.draw_circle(coord[i].first, coord[i].second, 5, red);
		char buf[3] = { 0 };
		image.draw_text(coord[i].first + 5, coord[i].second + 5,
			itoa(i, buf, 10), black, white, 1, 20);
	}

	stack<list<int>> M_stack, K_stack, P_stack;
	stack<int> v_stack;

	list<int> M, K, P, G;
	int v;

	for (int i = 0; i < N; ++i) {
		K.push_back(i);
	}

	while (!(K.empty() && M.empty())) {
		if (!K.empty()) {

			v = K.front();

			M_stack.push(M);
			K_stack.push(K);
			P_stack.push(P);
			v_stack.push(v);
			M.push_back(v);

			G = gamma(ad, v, treshold);
			for (auto it = G.begin(), e = G.end(); it != e; ++it) {
				K.remove(*it);
				P.remove(*it);
			}

			K.remove(v);
		} else {
			if (P.empty()) {
				for each (v in M) {
					cout << v << ' ';
				}
				cout << endl;
			}

			v = v_stack.top();
			v_stack.pop();
			P = P_stack.top();
			P_stack.pop();
			K = K_stack.top();
			K_stack.pop();
			M = M_stack.top();
			M_stack.pop();

			K.remove(v);

			P.push_back(v);
		}
	}

	//CImgDisplay disp_2(image_2);
	CImgDisplay disp(image);

	::system("pause");
}

void create_enlgish_letters() {
	for (int i = 0; i < 26; ++i) {
		CImg<unsigned int> image(16, 16, 1, 3);
		for (int x = 0; x < 16; ++x)
			for (int y = 0; y < 16; ++y) {
				image.draw_point(x, y, white);
			}
		char let[] = { 'A' + i, 0 };
		//char c;
		//cin >> c;
		std::string letter(let);
		image.draw_text(0, -2, letter.c_str(), black, white, 1, 24);

		int b = 0, e = 0;
		bool symb = false;

		for (int x = 0; x < 16; ++x) {
			for (int y = 0; y < 16; ++y) {
				if (image(x, y, 1) < 230) {
					if (!symb) b = x;
					else e = x;
					symb = true;
					break;
				}
			}
		}

		int shift = 8 - (b + e) / 2;

		if (i == 22) shift = 0;

		CImg<unsigned int> image2(16, 16, 1, 3);

		for (int x = 0; x < 16; ++x)
			for (int y = 0; y < 16; ++y) {
				image2.draw_point(x, y, image(x - shift, y, 1) < 230 ? black : white);
			}

		std::string filename = ".\\eng\\" + letter + ".bmp";
		image2.save(filename.c_str());
	}
}

int main() {
	//setlocale(0, "Rus");
	srand(time(0));
	//draw_cont();
	//main1();
	//kanny();
	prim();
	//main2();
	//recur();
	//harris();
	//dec_tree();
	//graph();
	//clique();
	//create_enlgish_letters();

	//CImg<unsigned char> img(640, 400, 1, 3);  // Define a 640x400 color image with 8 bits per color component.
	//img.fill(0);                           // Set pixel values to 0 (color : black)
	//unsigned char purple[] = { 255,0,255 };        // Define a purple color
	//for (int i = 0; i < img.width(); ++i) {
	//	for (int j = 0; j < img.height(); ++j) {
	//		img.draw_point(i, j, green);
	//	}
	//}
	//img.draw_text(100, 100, "Hello World", purple); // Draw a purple "Hello world" at coordinates (100,100).
	//CImgDisplay disp(img);
	//system("pause");
}