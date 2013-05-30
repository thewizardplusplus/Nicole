#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <iostream>

const float PI = 3.1415926f;

float MathRandom(float minimum, float maximum) {
	return (maximum - minimum) * std::rand() / RAND_MAX + minimum;
}

struct Vec {
	double x;
	double y;
	double z;

	Vec(double x = 0, double y = 0, double z = 0) :
		x(x),
		y(y),
		z(z)
	{}

	Vec operator+(const Vec& b) const {
		return Vec(x + b.x, y + b.y, z + b.z);
	}

	Vec operator-(const Vec& b) const {
		return Vec(x - b.x, y - b.y, z - b.z);
	}

	Vec operator*(double b) const {
		return Vec(x * b, y * b, z * b);
	}

	Vec mult(const Vec& b) const {
		return Vec(x * b.x, y * b.y, z * b.z);
	}

	Vec& norm(void) {
		return *this = *this * (1/sqrt(x*x+y*y+z*z));
	}

	double dot(const Vec &b) const {
		return x * b.x + y * b.y + z * b.z;
	}

	Vec operator%(Vec& b) {
		return Vec(y * b.z - z * b.y, z * b.x - x * b.z, x * b.y - y * b.x);
	}
};

struct Ray {
	Vec o;
	Vec d;

	Ray(Vec o, Vec d) :
		o(o),
		d(d)
	{}
};

struct Sphere {
	double rad;
	Vec p;
	Vec e;
	Vec c;

	Sphere(double rad, Vec p, Vec e, Vec c):
		rad(rad),
		p(p),
		e(e),
		c(c)
	{}

	double intersect(const Ray& r) const {
		Vec op = p - r.o;
		double eps = 1e-4;
		double b = op.dot(r.d);
		double det = b * b - op.dot(op) + rad * rad;
		if (det < 0) {
			return 0;
		} else {
			det = sqrt(det);
		}
		double t = b - det;
		if (t > eps) {
			return t;
		} else {
			t = b + det;
			if (t > eps) {
				return t;
			} else {
				return 0;
			}
		}
	}
};

Sphere spheres[] = {
	Sphere(1e5, Vec(1e5 + 1, 40.8, 81.6), Vec(), Vec(.75,.25,.25)),
	Sphere(1e5, Vec(-1e5+99,40.8,81.6), Vec(), Vec(.25,.25,.75)),
	Sphere(1e5, Vec(50,40.8, 1e5), Vec(), Vec(.75,.75,.75)),
	Sphere(1e5, Vec(50,40.8,-1e5+170), Vec(), Vec()),
	Sphere(1e5, Vec(50, 1e5, 81.6), Vec(), Vec(.75,.75,.75)),
	Sphere(1e5, Vec(50,-1e5+81.6,81.6), Vec(), Vec(.75,.75,.75)),
	Sphere(16.5,Vec(27,16.5,47), Vec(), Vec(1,1,1) * .999),
	Sphere(16.5,Vec(73,16.5,78), Vec(), Vec(1,1,1) * .999),
	Sphere(600, Vec(50,681.6-.27,81.6), Vec(12,12,12), Vec())
};

bool intersect(const Ray& r, double& t, int& id) {
	double n = sizeof(spheres) / sizeof(Sphere);
	double inf = 1e20;

	t = 1e20;
	for (int i = int(n); i--;) {
		double d = spheres[i].intersect(r);
		if(d && d < t) {
			t = d;
			id = i;
		}
	}
	return t < inf;
}

Vec radiance(const Ray &r, int depth) {
	float REFLECTIONS = 5;

	double t;
	int id = 0;
	if (!intersect(r, t, id)) {
		return Vec();
	}
	const Sphere& obj = spheres[id];
	Vec x = r.o + r.d * t;
	Vec n = (x - obj.p).norm();
	Vec nl = n.dot(r.d) < 0 ? n : n * -1;
	Vec f = obj.c;

	if (++depth > REFLECTIONS) {
		return obj.e;
	}
	double r1 = 2 * PI * MathRandom(0, 1);
	double r2 = MathRandom(0, 1);
	double r2s = sqrt(r2);
	Vec w = nl;
	Vec u = ((fabs(w.x) > .1 ? Vec(0, 1) : Vec(1)) % w).norm();
	Vec v = w % u;
	Vec d = (u * cos(r1) * r2s + v * sin(r1) * r2s + w * sqrt(1 - r2)).norm();
	return obj.e + f.mult(radiance(Ray(x, d), depth));
}

int main(void){
	float WIDTH = 640;
	float HEIGHT = 480;
	float SAMPLES = 5;

	float* camera_origin = new float[3];
	camera_origin[0] = 50;
	camera_origin[1] = 52;
	camera_origin[2] = 295.6;
	float* camera_direction = new float[3];
	camera_direction[0] = 0;
	camera_direction[1] = 0 - 0.042612;
	camera_direction[2] = 0 - 1;
	// нормализуем camera_direction
	float length = sqrt(camera_direction[0] * camera_direction[0] +
		camera_direction[1] * camera_direction[1] + camera_direction[2] *
		camera_direction[2]);
	camera_direction[0] = camera_direction[0] / length;
	camera_direction[1] = camera_direction[1] / length;
	camera_direction[2] = camera_direction[2] / length;
	float* camera_x = new float[3];
	camera_x[0] = WIDTH * 0.5135 / HEIGHT;
	camera_x[1] = 0;
	camera_x[2] = 0;
	float* camera_y = new float[3];
	// векторное произведение camera_x на camera_direction
	camera_y[0] = camera_x[1] * camera_direction[2] - camera_x[2] *
		camera_direction[1];
	camera_y[1] = camera_x[2] * camera_direction[0] - camera_x[0] *
		camera_direction[2];
	camera_y[2] = camera_x[0] * camera_direction[1] - camera_x[1] *
		camera_direction[0];
	// нормализуем camera_y
	length = sqrt(camera_y[0] * camera_y[0] + camera_y[1] * camera_y[1] +
		camera_y[2] * camera_y[2]);
	camera_y[0] = camera_y[0] / length;
	camera_y[1] = camera_y[1] / length;
	camera_y[2] = camera_y[2] / length;
	// умножаем camera_y на коэффициент
	camera_y[0] = camera_y[0] * 0.5135;
	camera_y[1] = camera_y[1] * 0.5135;
	camera_y[2] = camera_y[2] * 0.5135;
	float* r = new float[3];
	r[0] = 0;
	r[1] = 0;
	r[2] = 0;

	float** image = new float*[(int)(WIDTH * HEIGHT)];
	float i = 0;
	while (true) {
		if (!(i < WIDTH * HEIGHT)) {
			break;
		}

		float* pixel = new float[3];
		pixel[0] = 0;
		pixel[1] = 0;
		pixel[2] = 0;
		image[(int)i] = pixel;

		i = i + 1;
	}

	float y = 0;
	while (true) {
		if (!(y < HEIGHT)) {
			break;
		}

		std::cout << "\rRendering (" << SAMPLES << " spp): " << 100 * y /
			(HEIGHT - 1) << "%..." << std::flush;
		float x = 0;
		while (true) {
			if (!(x < WIDTH)) {
				break;
			}

			float s = 0;
			while (true) {
				if (!(s < SAMPLES)) {
					break;
				}

				float* d = new float[3];
				d[0] = camera_x[0] * (x / WIDTH - 0.5) + camera_y[0] * (y /
					HEIGHT - 0.5) + camera_direction[0];
				d[1] = camera_x[1] * (x / WIDTH - 0.5) + camera_y[1] * (y /
					HEIGHT - 0.5) + camera_direction[1];
				d[2] = camera_x[2] * (x / WIDTH - 0.5) + camera_y[2] * (y /
					HEIGHT - 0.5) + camera_direction[2];
				float* ray_origin = new float[3];
				ray_origin[0] = camera_origin[0] + d[0] * 140;
				ray_origin[1] = camera_origin[1] + d[1] * 140;
				ray_origin[2] = camera_origin[2] + d[2] * 140;
				// нормализуем d
				length = sqrt(d[0] * d[0] + d[1] * d[1] + d[2] * d[2]);
				d[0] = d[0] / length;
				d[1] = d[1] / length;
				d[2] = d[2] / length;

				Vec result = radiance(Ray(Vec(ray_origin[0], ray_origin[1],
					ray_origin[1]), Vec(d[0], d[1], d[2])), 0);
				r[0] = r[0] + result.x / SAMPLES;
				r[1] = r[1] + result.y / SAMPLES;
				r[2] = r[2] + result.z / SAMPLES;

				s = s + 1;
			}

			int index = (HEIGHT - y - 1) * WIDTH + x;
			if (r[0] < 0) {
				r[0] = 0;
			}
			if (r[0] > 1) {
				r[0] = 1;
			}
			if (r[1] < 0) {
				r[1] = 0;
			}
			if (r[1] > 1) {
				r[1] = 1;
			}
			if (r[2] < 0) {
				r[2] = 0;
			}
			if (r[2] > 1) {
				r[2] = 1;
			}
			image[index][0] = image[index][0] + r[0];
			image[index][1] = image[index][1] + r[1];
			image[index][2] = image[index][2] + r[2];
			r[0] = 0;
			r[1] = 0;
			r[2] = 0;

			x = x + 1;
		}

		y = y + 1;
	}

	FILE* f = fopen("image.ppm", "w");
	fprintf(f, "P3\n%d %d\n%d\n", (int)WIDTH, (int)HEIGHT, 255);
	i = 0;
	while (true) {
		if (!(i < WIDTH * HEIGHT)) {
			break;
		}

		float* pixel = image[(int)i];
		if (pixel[0] < 0)
			pixel[0] = 0;
		if (pixel[0] > 1)
			pixel[0] = 1;
		int red = (int)(pow(pixel[0], 1 / 2.2) * 255 + 0.5);
		if (pixel[1] < 0)
			pixel[1] = 0;
		if (pixel[1] > 1)
			pixel[1] = 1;
		int green = (int)(pow(pixel[1], 1 / 2.2) * 255 + 0.5);
		if (pixel[2] < 0)
			pixel[2] = 0;
		if (pixel[2] > 1)
			pixel[2] = 1;
		int blue = (int)(pow(pixel[2], 1 / 2.2) * 255 + 0.5);
		fprintf(f,"%d %d %d ", red, green, blue);

		i = i + 1;
	}
}
