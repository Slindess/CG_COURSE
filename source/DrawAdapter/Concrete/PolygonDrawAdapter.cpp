//
// Created by Михаил Слиняков on 20.10.2024.
//

#include "PolygonDrawAdapter.h"
#include "../../Light/Light.h"
#include "../../Drawer/QtDrawer.h"
#include "../../Objects/Concrete/PolygonObject.h"
#include "../../Components/Concrete/Polygon.h"
#include <cmath>
#include <limits>
#include <iostream>
#include <thread>
#include <mutex> // Не забудьте добавить этот заголовок
// Определяем мьютекс для синхронизации
std::mutex bufferMutex;

std::vector<double> Reflect(const std::vector<double>& rayDir, const std::vector<double>& normal) {
    double dotProduct = rayDir[0] * normal[0] + rayDir[1] * normal[1] + rayDir[2] * normal[2];
    return {
            rayDir[0] - 2 * dotProduct * normal[0],
            rayDir[1] - 2 * dotProduct * normal[1],
            rayDir[2] - 2 * dotProduct * normal[2]
    };
}

inline std::vector<double> cross(const std::vector<double>& a, const std::vector<double>& b) {
    return {
            a[1] * b[2] - a[2] * b[1],  // Cx
            a[2] * b[0] - a[0] * b[2],  // Cy
            a[0] * b[1] - a[1] * b[0]   // Cz
    };
}

inline double dot(const std::vector<double>& a, const std::vector<double>& b) {
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

inline bool RayIntersectsTriangle(
        const std::vector<double>& rayOrigin,
        const std::vector<double>& rayDir,
        const Polygon& polygon,
        double t0,
        double *tt,
        std::vector<double>& intersectionPoint)
{
    // Координаты вершин треугольника
    std::vector<double> v0 = { polygon.x1, polygon.y1, polygon.z1 };
    std::vector<double> v1 = { polygon.x2, polygon.y2, polygon.z2 };
    std::vector<double> v2 = { polygon.x3, polygon.y3, polygon.z3 };

    double a = v0[0] - v1[0];
    double b = v0[1] - v1[1];
    double c = v0[2] - v1[2];
    double d = v0[0] - v2[0];
    double e = v0[1] - v2[1];
    double f = v0[2] - v2[2];

    double g = (rayDir[0]);
    double h = (rayDir[1]);
    double i = (rayDir[2]);

    // Разность между началом луча и первой вершиной треугольника
    double j = v0[0] - rayOrigin[0];
    double k = v0[1] - rayOrigin[1];
    double l = v0[2] - rayOrigin[2];



    // Вычисляем определитель матрицы |A|
    double detA = a * (e * i - f * h) + b * (f * g - d * i) + c * (d * h - e * g);

    if (fabs(detA) < 1e-6)
    {
        return false;
    }

    // Вычисляем параметр t (время пересечения)
    double t = -(f * (a * k - j * b) + e * (j * c - a * l) + d * (b * l - k * c)) / detA;

    if (t <= t0 || t <= 0)
    {
        return false; // Луч направлен в обратную сторону
    }

    // Вычисляем параметр γ
    double gamma = (i * (a * k - j * b) + h * (j * c - a * l) + g * (b * l - k * c)) / detA;
    if (gamma < 0 || gamma > 1)
    {
        return false; // Точка пересечения находится вне треугольника
    }

    // Вычисляем параметр β
    double beta = (j * (e * i - f * h) + k * (f * g - d * i) + l * (d * h - e * g)) / detA;
    if (beta < 0 || beta + gamma > 1)
    {
        return false; // Точка пересечения вне треугольника
    }

    // Вычисляем точку пересечения
    intersectionPoint = {
            rayOrigin[0] + t * rayDir[0],
            rayOrigin[1] + t * rayDir[1],
            rayOrigin[2] + t * rayDir[2]
    };
    *tt = t;

    return true; // Пересечение найдено
}


PolygonDrawAdapter::PolygonDrawAdapter(std::shared_ptr<QtDrawer> drawer) : _drawer(drawer)
{}


void ProcessRays(int startX, int endX, const std::shared_ptr<Scene>& scene, const std::shared_ptr<Camera>& camera, std::shared_ptr<QtDrawer> drawer, std::vector<std::vector<Color>> &buff)
{
    LightSource lightSource(100.0, -50.0, 10.0); // Источник света

    // Задаём параметры для specular (зеркальных бликов)
    double specularExponent = 5.0;  // Определяет "резкость" бликов
    double specularStrength = 0.5;   // Влияние specular составляющей

    for (int x = startX; x < endX; ++x)
    {
        for (int y = 0; y < camera->height; ++y)
        {
            std::vector<double> rayOrigin = { camera->x_view, camera->y_view, camera->z_view };

            double physX = camera->x_screen + (x - camera->width / 2);
            double physY = camera->y_screen + (y - camera->height / 2);
            double physZ = camera->z_screen;
            std::vector<double> rayDir = { physX - camera->x_view, physY - camera->y_view, physZ - camera->z_view };
            double magnitude = sqrt(rayDir[0] * rayDir[0] + rayDir[1] * rayDir[1] + rayDir[2] * rayDir[2]);

            rayDir[0] /= magnitude;
            rayDir[1] /= magnitude;
            rayDir[2] /= magnitude;

            double t0 = (camera->z_view - rayOrigin[2]) / rayDir[2];
            double tmin = 10000.0;
            double t = 0.0;

            for (const auto& object : scene->objects)
            {

                
                double sphereT;
                auto objpolygon = std::dynamic_pointer_cast<PolygonObject>(object);
                auto sphere = objpolygon->Sphere();

                if (!sphere.Intersects(rayOrigin, rayDir, sphereT))
                {
                    continue; // Если нет пересечения со сферой, перейти к следующему объекту
                }

                
                for (const auto& component : object->GetComponents())
                {
                    auto polygon = std::dynamic_pointer_cast<Polygon>(component);
                    if (polygon)
                    {
                        std::vector<double> intersectionPoint;
                        if (sphereT > tmin) break;  // ОПАСНАЯ ТЕМА
                        if (RayIntersectsTriangle(rayOrigin, rayDir, *polygon, t0, &t, intersectionPoint))
                        {
                            if (t > tmin)
                                break;

                            tmin = t;
                            double imageX = physX;
                            double imageY = physY;

                            Color illuminatedColor = polygon->color;

                            std::vector<double> v0 = { polygon->x1, polygon->y1, polygon->z1 };
                            std::vector<double> v1 = { polygon->x2, polygon->y2, polygon->z2 };
                            std::vector<double> v2 = { polygon->x3, polygon->y3, polygon->z3 };

                            // Вычисляем векторы ребер
                            std::vector<double> edge1 = { v1[0] - v0[0], v1[1] - v0[1], v1[2] - v0[2] };
                            std::vector<double> edge2 = { v2[0] - v0[0], v2[1] - v0[1], v2[2] - v0[2] };

                            // Вычисляем нормаль полигона
                            std::vector<double> normal = {
                                    edge1[1] * edge2[2] - edge1[2] * edge2[1],
                                    edge1[2] * edge2[0] - edge1[0] * edge2[2],
                                    edge1[0] * edge2[1] - edge1[1] * edge2[0]
                            };

                            // Нормализуем нормаль
                            double normMagnitude = sqrt(normal[0] * normal[0] + normal[1] * normal[1] + normal[2] * normal[2]);
                            if (normMagnitude > 0)
                            {
                                normal[0] /= normMagnitude;
                                normal[1] /= normMagnitude;
                                normal[2] /= normMagnitude;
                            }

                            // Направление света
                            std::vector<double> lightDir = lightSource.getDirection();
                            double lightMagnitude = sqrt(lightDir[0] * lightDir[0] + lightDir[1] * lightDir[1] + lightDir[2] * lightDir[2]);
                            lightDir[0] /= lightMagnitude;
                            lightDir[1] /= lightMagnitude;
                            lightDir[2] /= lightMagnitude;

                            // Рассчитываем интенсивность света (diffuse)
                            double dotProduct = normal[0] * lightDir[0] + normal[1] * lightDir[1] + normal[2] * lightDir[2];
                            double intensity = std::max(0.0, dotProduct);

                            // Устанавливаем минимальный уровень освещенности
                            double minIntensity = 0.4;
                            double diffuseIntensity = std::max(intensity, minIntensity);
                            //double diffuseIntensity = intensity + minIntensity;
                            // Вычисляем отражённое направление (reflectDir)
                            std::vector<double> reflectDir = {
                                    2 * dotProduct * normal[0] - lightDir[0],
                                    2 * dotProduct * normal[1] - lightDir[1],
                                    2 * dotProduct * normal[2] - lightDir[2]
                            };

                            // Направление взгляда (направление на камеру)
                            std::vector<double> viewDir = { -rayDir[0], -rayDir[1], -rayDir[2] };

                            // Вычисляем specular интенсивность (блики)
                            double reflectDotView = std::max(0.0, reflectDir[0] * viewDir[0] + reflectDir[1] * viewDir[1] + reflectDir[2] * viewDir[2]);
                            double specularIntensity = pow(reflectDotView, specularExponent) * specularStrength;

                            std::vector<double> shadowRayOrigin = intersectionPoint;
                            std::vector<double> shadowRayDir = { lightDir[0], lightDir[1], lightDir[2] };

                            bool isInShadow = false;

                            // Проверяем пересечение с объектами сцены для shadow-ray

                            for (const auto& shadowObject : scene->objects) {
                                for (const auto& shadowComponent : shadowObject->GetComponents()) {
                                    auto shadowPolygon = std::dynamic_pointer_cast<Polygon>(shadowComponent);
                                    if (shadowPolygon) {
                                        double shadowT;
                                        std::vector<double> shadowIntersectionPoint;
                                        if (RayIntersectsTriangle(shadowRayOrigin, shadowRayDir, *shadowPolygon, 0.001, &shadowT, shadowIntersectionPoint)) {
                                            // Точка пересечения с другим объектом найдена - тень
                                            isInShadow = true;
                                            break;
                                        }
                                    }
                                }
                                if (isInShadow)
                                    break;
                            }

                            // Если точка в тени, уменьшаем освещенность (либо полностью обнуляем)
                            if (isInShadow) {
                                diffuseIntensity *= 0.5;  // Слабая освещённость из-за тени
                                specularIntensity = 0.0;  // Отсутствие бликов в тени
                            }

                            // Общая освещённость с учётом diffuse и specular
                            illuminatedColor.r = std::min(255.0, illuminatedColor.r * diffuseIntensity + 255 * specularIntensity);
                            illuminatedColor.g = std::min(255.0, illuminatedColor.g * diffuseIntensity + 255 * specularIntensity);
                            illuminatedColor.b = std::min(255.0, illuminatedColor.b * diffuseIntensity + 255 * specularIntensity);


                            // Заполняем буфер кадра
                            // Синхронизированный доступ к буферу
                            {
                                std::lock_guard<std::mutex> lock(bufferMutex);
                                //std::unique_lock<std::mutex> lock(bufferMutex);
                                if (imageX + camera->width / 2 >= 0 && imageX + camera->width / 2 < buff.size() &&
                                    imageY + camera->height / 2 >= 0 && imageY + camera->height / 2 < buff[0].size()) {
                                    buff[imageX + camera->width / 2][imageY + camera->height / 2] = illuminatedColor;
                                }
                                //lock.unlock();
                            }
                        }
                    }
                }
            }
        }
    }
}


#include <chrono>
void PolygonDrawAdapter::Draw(std::shared_ptr<Scene> scene, std::shared_ptr<Camera> camera)
{
    auto start = std::chrono::high_resolution_clock::now();
    Color backgroundColor(-1, -1, -1);
    std::vector<std::vector<Color>> buff(camera->height, std::vector<Color>(camera->width, backgroundColor));  // БУФЕР КАДРА

    const int numThreads = 8;
    std::vector<std::thread> threads;

    int widthPerThread = camera->width / numThreads;

    for (int i = 0; i < numThreads; ++i)
    {
        int startX = i * widthPerThread;
        int endX = (i == numThreads - 1) ? camera->width : startX + widthPerThread; // Обрабатываем оставшиеся пиксели в последнем потоке
        threads.emplace_back(ProcessRays, startX, endX, scene, camera, _drawer, std::ref(buff));
    }

    for (auto& thread : threads)
    {
        thread.join();
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    // Вывод времени рендеринга
    std::cout << "Rendering time: " << duration.count() << " seconds" << std::endl;

    for (int i = 0; i < camera->height; i++)
    {
        for (int j = 0; j < camera->width; j++)
        {
            if (!buff[i][j].isBG())
            {
                _drawer->setColor(buff[i][j]);
                _drawer->drawPoint(j - camera->width / 2, -1*i + camera->height / 2);
            }
        }
    }
    _drawer->setColor(0, 0, 255);
    _drawer->drawPoint(0, 0);
    _drawer->drawPoint(camera->width / 2 - 10, camera->height / 2 - 10);
}


PolygonDrawAdapter::~PolygonDrawAdapter(){}