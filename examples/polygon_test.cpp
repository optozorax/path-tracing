#include <pt/pt.h>
#include <pt/object/scene.h>
#include <pt/object/sky.h>
#include <pt/shape/triangle.h>
#include <pt/shape/polygon.h>
#include <pt/shape/sphere.h>
#include <pt/material/scatter.h>
#include <pt/renderer.h>
#include <pt/pt2easybmp.h>

using namespace pt;

void initScene(Scene& scene) {
	auto& array = scene.array;
	Material_ptr sc = makeScatter(Color(0.5, 0.5, 0.9));

	{
		const double size = 500;
		const double depth = -1.5;
		vec3 a(-size, -size, depth);
		vec3 b(-size, size, depth);
		vec3 c(size, size, depth);
		vec3 d(size, -size, depth);
		scene.add(makeTriangle(a, b, c, makeScatter(pt::Color(0.6, 0.6, 0.6))));
		scene.add(makeTriangle(c, d, a, makeScatter(pt::Color(0.6, 0.6, 0.6))));
	}

	crd3 coords1;
	coords1.pos = vec3(0);
	coords1.i = vec3(1, 0, 0);
	coords1.j = vec3(0, 0, 1);
	coords1.k = vec3(0, 1, 0);

	{
		std::vector<vec2> mas;
		mas.push_back({0, 0});
		mas.push_back({0, 1});
		mas.push_back({1, 0});
		array.push_back(makePolygon(mas, coords1, sc));

		crd3 coords2;
		coords2.pos = vec3(0, 2, -0.5);
		coords2.i = vec3(1.0/sqrt(2.0), 0, -1.0/sqrt(2.0));
		coords2.j = vec3(1.0/sqrt(2.0), 0, 1.0/sqrt(2.0));
		coords2.k = cross(coords2.i, coords2.j);
		coords2.i.normalize();
		coords2.j.normalize();
		coords2.k.normalize();
		array.push_back(makePolygon(mas, coords2, sc));

		coords2.pos = vec3(1.5, 2, -0.5);
		coords2.i = vec3(1.0/sqrt(2.0), -1.0/sqrt(2.0), 0);
		coords2.j = vec3(1.0/sqrt(2.0), 0, 1.0/sqrt(2.0));
		coords2.k = cross(coords2.i, coords2.j);
		coords2.i.normalize();
		coords2.j.normalize();
		coords2.k.normalize();
		array.push_back(makePolygon(mas, coords2, sc));

		coords2.pos = vec3(-1.5, 2, -0.5);
		coords2.i = vec3(1.0/sqrt(2.0), -1.0/sqrt(2.0), 0);
		coords2.j = vec3(1.0/sqrt(2.0), 1.0/sqrt(2.0), 0);
		coords2.k = cross(coords2.i, coords2.j);
		coords2.i.normalize();
		coords2.j.normalize();
		coords2.k.normalize();
		array.push_back(makePolygon(mas, coords2, sc));
	}

	{
		crd3 coords2 = coords1;
		coords2.pos = vec3(1.5, 0, 0);
		std::vector<vec2> mas;
		mas.push_back({0, 0});
		mas.push_back({0, 1});
		mas.push_back({1, 1});
		mas.push_back({1, 0});
		array.push_back(makePolygon(mas, coords2, sc));
	}

	{
		crd3 coords2 = coords1;
		coords2.pos = vec3(-1.5, 0, 0);
		std::vector<vec2> mas;
		for (int i = 0; i < 359; i++)
			mas.push_back({0.5 + 0.5 * sin(i/180.0 * pi), 0.5 + 0.5 * cos(i/180.0 * pi)});
		array.push_back(makePolygon(mas, coords2, sc));
	}

	{
		crd3 coords2 = coords1;
		coords2.pos = vec3(-3, 0, 0);
		std::vector<vec2> mas;
		mas.push_back({0, 0});
		mas.push_back({0, 1});
		mas.push_back({1, 0});
		mas.push_back({1, 1});
		array.push_back(makePolygon(mas, coords2, sc));
	}

	{
		crd3 coords2 = coords1;
		coords2.pos = vec3(3, 0, 0);
		std::vector<vec2> mas;
		mas.push_back({31/290.0, 34/313.0});
		mas.push_back({290/290.0, 100/313.0});
		mas.push_back({0/290.0, 199/313.0});
		mas.push_back({165/290.0, 313/313.0});
		mas.push_back({271/290.0, 226/313.0});
		mas.push_back({177/290.0, 0/313.0});
		mas.push_back({167/290.0, 240/313.0});
		mas.push_back({55/290.0, 108/313.0});
		mas.push_back({194/290.0, 113/313.0});
		array.push_back(makePolygon(mas, coords2, sc));
	}

	scene.add(makeSky(Color(1, 1, 1), Color(0, 0.2, 0.2)));
}

int main() {
	Scene scene;
	initScene(scene);

	Image img(1000, 500);

	PerspectiveCamera cam(2, 45*pt::pi/180, 0, vec3(0, 7, 1), img.getWidth(), img.getHeight());
	cam.lookAt(vec3(0, 0, 0));

	RayTracing ren2(2, 4, true);
	ren2.setAmbientLight(Color(0.5, 0.5, 0.5, 1));
	ren2.luminaries.push_back(PointLight(vec3(0, 0, 5), pt::Color(0.5, 0.5, 0.5).sqrt()));
	ren2.luminaries.push_back(PointLight(vec3(5, 5, 3), pt::Color(0.5, 0.5, 0.5).sqrt()));
	ren2.luminaries.push_back(PointLight(vec3(-5, -3, 3), pt::Color(0.5, 0.5, 0.5).sqrt()));
	ren2.assign(&cam, &scene, &img);
	ren2.render();
	img.colorCorrection();
	saveAsPng(img, "polygon_test.png");

	return 0;
}