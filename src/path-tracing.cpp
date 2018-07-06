#include <pt/renderer/path-tracing.h>

namespace pt
{

//-----------------------------------------------------------------------------
PathRenderer::PathRenderer(int samples, 
						   int maxDepth,
						   double maxT) : samples(samples), maxDepth(maxDepth), maxT(maxT) {
}

//-----------------------------------------------------------------------------
Color PathRenderer::computePixel(int x, int y, const Camera& camera, const Object& scene) {
	Color clr(0, 0, 0, 0);
	for (int i = 0; i < samples; ++i) {
		double x1 = x + random();
		double y1 = y + random();
		Ray ray = camera.getRay(x1, y1, true);
		clr += computeColor(ray, scene);
	}
	clr /= samples;
	return clr;
}

//-----------------------------------------------------------------------------
Color PathRenderer::computeColor(Ray ray, const Object& scene) {
	Color resultColor = Color(1, 1, 1, 1);
	Intersection inter;
	Color clrAbsorbtion;
	Ray scattered;
	double diffusion;
	ScatterType returned;

	for (int i = 0; i < maxDepth; ++i) {
		if (scene.intersect(ray, inter, 0, maxT)) {
			// Получить рассевание луча объектом
			returned = scene.scatter(ray, inter, clrAbsorbtion, scattered, diffusion);

			// Если цвет непрозрачный, то отправить луч сквозь объект с некоторой вероятностью
			if (clrAbsorbtion.a != 1) {
				if (random() > clrAbsorbtion.a) {
					scattered.dir = ray.dir;
					clrAbsorbtion = Color(1, 1, 1, 1);
					diffusion = 0;
				}
			}

			// Посчитать результирующй цвет после данного отражения
			resultColor = clrAbsorbtion * resultColor;

			// Изменить направление в соответствии с рассеиванием
			scattered.dir += randomSphere() * diffusion;
			scattered.dir.normalize();

			// Сместить положение луча в некотором направлении
			scattered.pos += scattered.dir * 0.00001;

			ray = scattered;
			if (returned == SCATTER_END)
				break;
		} else {
			//if (i == 0)
			resultColor = Color(0, 0, 0, 0);
			break;
		}
	}

	return resultColor;
}

//-----------------------------------------------------------------------------
void PathRenderer::render(const Camera& camera, Image& img, const Object& scene) {
	onStartRender();
	for (int i = 0; i < img.getWidth(); ++i) {
		onEveryLine(double(i)/img.getWidth());
		for (int j = 0; j < img.getHeight(); ++j) {
			img(i, j) = computePixel(i, j, camera, scene);
		}
	}
	onEndRendering();
}

//-----------------------------------------------------------------------------
FitfulPathRenderer::FitfulPathRenderer(int maxDepth, double maxT) : PathRenderer(0, maxDepth, maxT) {}

//-----------------------------------------------------------------------------
void FitfulPathRenderer::init(Camera* camera, Image* img, Object* scene) {
	m_samples = 0;
	m_camera = camera;
	m_img = img;
	m_scene = scene;
}
		
//-----------------------------------------------------------------------------
Image FitfulPathRenderer::renderStep(int samples) {
	onStartRender();
	for (int i = 0; i < m_img->getWidth(); ++i) {
		onEveryLine(double(i)/m_img->getWidth());
		for (int j = 0; j < m_img->getHeight(); ++j) {
			for (int k = 0; k < samples; ++k) {
				double x = i + random();
				double y = j + random();
				Ray ray = m_camera->getRay(x, y, true);
				(*m_img)(i, j) += computeColor(ray, *m_scene);
			}
		}
	}
	onEndRendering();

	m_samples += samples;
	Image img2(m_img->getWidth(), m_img->getHeight());
	for (int i = 0; i < img2.getWidth(); ++i) {
		for (int j = 0; j < img2.getHeight(); ++j) {
			img2(i, j) = (*m_img)(i, j);
			img2(i, j) /= m_samples;
		}
	}
	
	return img2;
}

};