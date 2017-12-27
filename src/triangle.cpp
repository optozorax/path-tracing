#include <pt/shape/triangle.h>

namespace pt
{

//-----------------------------------------------------------------------------
double area(double a, double b, double c) {
	double p = (a + b + c)/2.0f;
	return sqrt(p*(p-a)*(p-b)*(p-c));
}
//-----------------------------------------------------------------------------
Triangle::Triangle(Vector a, Vector b, Vector c, Material* material) :
	a(a), b(b), c(c),
	Shape(material) {
	// Equation of plane, every x in plane when: (n, x) + d = 0
	normal = (cross(b-a, c-b)).normalize();
	d = -dot(normal, c);
	ab = (b-a).getLength();
	bc = (c-b).getLength();
	ac = (c-a).getLength();
	S = area(ab, bc, ac);				
}

//-----------------------------------------------------------------------------
bool Triangle::intersect(const Ray& ray, 
						 Intersection& inter, 
						 double tMin, 
						 double tMax) const {
	if (dot(ray.dir, normal)) {
		double t = (-d - dot(ray.pos, normal))/dot(ray.dir, normal);

		if (t > tMin && t < tMax) {
			// Position when ray intersect plane
			Vector x = ray.pos + ray.dir * t;

			// Point in triangle <=> area of triangle = sum of inner triangles
			double xa = (a-x).getLength();
			double xb = (b-x).getLength();
			double xc = (c-x).getLength();

			double S1 = area(ab, xa, xb);
			double S2 = area(bc, xb, xc);
			double S3 = area(ac, xa, xc);

			if (fabs((S1 + S2 + S3)/S) < 1.0001f) {
				inter.t = t;
				if (dot(ray.dir, normal) < 0)
					inter.normal = normal;
				else
					inter.normal = -normal;
				inter.pos = x;
				return true;
			} else
				return false;
		} else
			// Ray is not in ranges
			return false;
	} else {
		// Ray is parallel plane
		return false;
	}
}

};