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

double calculateAngle(const std::vector<double>& a, const std::vector<double>& b) {
    // Скалярное произведение векторов
    double dotProduct = a[0] * b[0] + a[1] * b[1] + a[2] * b[2];

    // Длины векторов
    double lengthA = std::sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
    double lengthB = std::sqrt(b[0] * b[0] + b[1] * b[1] + b[2] * b[2]);

    // Защита от деления на ноль
    if (lengthA == 0 || lengthB == 0) {
        return 0.0;  // Векторы нулевой длины
    }

    // Косинус угла
    double cosTheta = dotProduct / (lengthA * lengthB);

    // Угол в радианах
    return std::abs(std::acos(cosTheta));
}

double calculateLength(const std::vector<double>& a)
{
    return std::sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
}

std::vector<double> normalizee(const std::vector<double>& vec) {
    double length = std::sqrt(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
    if (length > 0) {
        return { vec[0] / length, vec[1] / length, vec[2] / length };
    }
    return vec; // Если длина нулевая, возвращаем нулевой вектор
}


std::vector<double> interpolateMy(const std::vector<double>& p,
                     const std::vector<double>& V1, 
                     const std::vector<double>& V2,
                     const std::vector<double>& V3,
                     const std::vector<double>& normalV1,
                     const std::vector<double>& normalV2, 
                     const std::vector<double>& normalV3)
{
    /*
    // Вектор от V1 к p
    std::vector<double> V1P = {p[0] - V1[0], p[1] - V1[1], p[2] - V1[2]};
    std::vector<double> V12 = {V2[0] - V1[0], V2[1] - V1[1], V2[2] - V1[2]};
    std::vector<double> V13 = {V3[0] - V1[0], V3[1] - V1[1], V3[2] - V1[2]};
    std::vector<double> V23 = {V2[0] - V3[0], V2[1] - V3[1], V2[2] - V3[2]};
    std::vector<double> V3P = {p[0] - V3[0], p[1] - V3[1], p[2] - V3[2]};
    
    // Длины
    double lenV12 = calculateLength(V12);
    double lenV13 = calculateLength(V13);
    double lenV23 = calculateLength(V23);
    
    // Вычисляем угол между V12 и V1P
    double angle12 = calculateAngle(V12, V1P);
    double l0 = std::cos(angle12) * calculateLength(V1P);
    double u = l0 / lenV12;

    // Интерполяция нормали между normalV1 и normalV2
    std::vector<double> normalV1V2 = {
        u * normalV1[0] + (1 - u) * normalV2[0],
        u * normalV1[1] + (1 - u) * normalV2[1],
        u * normalV1[2] + (1 - u) * normalV2[2]
    };

    // Вычисляем угол между V13 и V1P
    double angle13 = calculateAngle(V23, V3P);
    double l1 = std::cos(angle13) * calculateLength(V3P);
    double v = l1 / lenV23;

    // Интерполяция нормали между normalV2 и normalV3
    std::vector<double> normalV2V3 = {
        v * normalV2[0] + (1 - v) * normalV3[0],
        v * normalV2[1] + (1 - v) * normalV3[1],
        v * normalV2[2] + (1 - v) * normalV3[2]
    };

    // Финальная интерполяция между normalV1V2 и normalV2V3
    std::vector<double> interpolatedNormal = {
        (normalV1V2[0] + normalV2V3[0]) / 2,
        (normalV1V2[1] + normalV2V3[1]) / 2,
        (normalV1V2[2] + normalV2V3[2]) / 2
    };

    // Возвращаем нормализованную интерполированную нормаль
    return normalizee(interpolatedNormal);
    */
   return { ((normalV1[0] + normalV2[0]) / 2 + (normalV1[0] + normalV3[0]) / 2)/2, 
   ((normalV1[1] + normalV2[1]) / 2 + (normalV1[1] + normalV3[1]) / 2)/2 ,
   ((normalV1[2] + normalV2[2]) / 2 + (normalV1[2] + normalV3[2]) / 2)/2 };
}

double interpolateMyI(const std::vector<double>& p,
                      const std::vector<double>& V1, 
                      const std::vector<double>& V2,
                      const std::vector<double>& V3,
                      const double iV1,
                      const double iV2, 
                      const double iV3)
{
    // Вектор от V1 к p
    std::vector<double> V1P = {p[0] - V1[0], p[1] - V1[1], p[2] - V1[2]};
    std::vector<double> V12 = {V2[0] - V1[0], V2[1] - V1[1], V2[2] - V1[2]};
    std::vector<double> V13 = {V3[0] - V1[0], V3[1] - V1[1], V3[2] - V1[2]};
    std::vector<double> V23 = {V2[0] - V3[0], V2[1] - V3[1], V2[2] - V3[2]};
    std::vector<double> V3P = {p[0] - V3[0], p[1] - V3[1], p[2] - V3[2]};
    
    // Длины
    double lenV12 = calculateLength(V12);
    double lenV13 = calculateLength(V13);
    double lenV23 = calculateLength(V23);
    
    // Вычисляем угол между V12 и V1P
    double angle12 = calculateAngle(V12, V1P);
    double l0 = std::cos(angle12) * calculateLength(V1P);
    double u =  l0 / lenV12; // Защита от деления на ноль

    // Интерполяция интенсивности между iV1 и iV2
    double iV1V2 = u * iV1 + (1 - u) * iV2;

    // Вычисляем угол между V23 и V3P
    double angle13 = calculateAngle(V23, V3P);
    double l1 = std::cos(angle13) * calculateLength(V3P);
    double v = l1 / lenV23; // Защита от деления на ноль

    // Интерполяция интенсивности между iV2 и iV3
    double iV2V3 = v * iV2 + (1 - v) * iV3;

    // Финальная интерполяция между iV1V2 и iV2V3
    double interpolatedIntensity = (iV1V2 + iV2V3) / 2;

    // Возвращаем интерполированную интенсивность
    return interpolatedIntensity;
}

std::vector<double> addThreeVectorss(const std::vector<double>& v1, const std::vector<double>& v2, const std::vector<double>& v3) {
    if (v1.size() != v2.size() || v1.size() != v3.size()) {
        throw std::invalid_argument("Все векторы должны быть одинакового размера");
    }

    std::vector<double> result(v1.size());
    for (size_t i = 0; i < v1.size(); ++i) {
        result[i] = v1[i] + v2[i] + v3[i];
    }
    /*
    double length = std::sqrt(result[0] * result[0] + result[1] * result[1] + result[2] * result[2]);
    if (length != 0) {
        result[0] /= length;
        result[1] /= length;
        result[2] /= length;
    }*/

    return result;
}

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

std::vector<double> barycentricCoords(const std::vector<double>& p, 
                                       const std::vector<double>& v0, 
                                       const std::vector<double>& v1, 
                                       const std::vector<double>& v2) {
    // Векторы от v0 к другим вершинам
    std::vector<double> v0_v1 = { v1[0] - v0[0], v1[1] - v0[1], v1[2] - v0[2] };
    std::vector<double> v0_v2 = { v2[0] - v0[0], v2[1] - v0[1], v2[2] - v0[2] };
    std::vector<double> v0_p = { p[0] - v0[0], p[1] - v0[1], p[2] - v0[2] };

    // Определитель
    double d00 = v0_v1[0] * v0_v1[0] + v0_v1[1] * v0_v1[1] + v0_v1[2] * v0_v1[2];
    double d01 = v0_v1[0] * v0_v2[0] + v0_v1[1] * v0_v2[1] + v0_v1[2] * v0_v2[2];
    double d11 = v0_v2[0] * v0_v2[0] + v0_v2[1] * v0_v2[1] + v0_v2[2] * v0_v2[2];
    
    double d20 = v0_p[0] * v0_v1[0] + v0_p[1] * v0_v1[1] + v0_p[2] * v0_v1[2];
    double d21 = v0_p[0] * v0_v2[0] + v0_p[1] * v0_v2[1] + v0_p[2] * v0_v2[2];

    // Вычисляем барицентрические координаты
    double denom = d00 * d11 - d01 * d01;
    double v = (d11 * d20 - d01 * d21) / denom;
    double w = (d00 * d21 - d01 * d20) / denom;
    double u = 1.0 - v - w; // u + v + w = 1

    return { u, v, w};
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
    //std::cout << "I: " << intersectionPoint[0] << " " << intersectionPoint[1] << " " << intersectionPoint[2] << "\n";
    *tt = t;

    return true; // Пересечение найдено
}


PolygonDrawAdapter::PolygonDrawAdapter(std::shared_ptr<QtDrawer> drawer) : _drawer(drawer)
{}

std::vector<double> alignNormalToReference(const std::vector<double>& target, const std::vector<double>& reference) {
    double dotProduct = target[0] * reference[0] + target[1] * reference[1] + target[2] * reference[2];
    // Если нормаль противоположна по направлению, разворачиваем её
    if (dotProduct < 0) {
        return { -target[0], -target[1], -target[2] };
    }
    return target;
}

bool CheckShadow(std::vector<double> &lightDir, std::vector<double> &intersectionPoint, const std::shared_ptr<Scene>& scene)
{
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
                if (RayIntersectsTriangle(shadowRayOrigin, shadowRayDir, *shadowPolygon, 0.001, &shadowT, shadowIntersectionPoint))
                {
                    isInShadow = true;
                    break;
                }
            }
        }
        if (isInShadow)
            break;
    }

    return isInShadow;
}



std::vector<double> interpolateNormals(const std::vector<double>& normalV1, 
                                       const std::vector<double>& normalV2, 
                                       const std::vector<double>& normalV3, 
                                       const std::vector<double>& baryCoords) {
    // Интерполируем нормали с использованием барицентрических координат
    std::vector<double> interpolatedNormal = {
        normalV1[0] * baryCoords[0] + normalV2[0] * baryCoords[1] + normalV3[0] * baryCoords[2],
        normalV1[1] * baryCoords[0] + normalV2[1] * baryCoords[1] + normalV3[1] * baryCoords[2],
        normalV1[2] * baryCoords[0] + normalV2[2] * baryCoords[1] + normalV3[2] * baryCoords[2]
    };

    // Нормализуем результирующую интерполированную нормаль
    return interpolatedNormal;
}

void ProccessPixel(int x, int y, const std::shared_ptr<Scene>& scene, const std::shared_ptr<Camera>& camera, std::shared_ptr<QtDrawer> drawer, std::vector<std::vector<Color>> &buff)
{
    LightSource lightSource(10.0, -10.0, 10.0); // Источник света

    double specularExponent = 1.0;  // Определяет "резкость" бликов
    double specularStrength = 0.1;   // Влияние specular составляющей
    

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
            if (!polygon) continue;

            std::vector<double> intersectionPoint;
            if (sphereT > tmin) break;  // ОПАСНАЯ ТЕМА
            if (!RayIntersectsTriangle(rayOrigin, rayDir, *polygon, t0, &t, intersectionPoint)) continue;
            
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
            
            // Получаем нормали вершин полигона
            std::vector<double> normalV1 = polygon->GetVertex1Normal();
            std::vector<double> normalV2 = polygon->GetVertex2Normal();
            std::vector<double> normalV3 = polygon->GetVertex3Normal();

            
            // Вычисляем бариатрические координаты
            std::vector<double> baryCoords = barycentricCoords(intersectionPoint, 
                                                                { polygon->x1, polygon->y1, polygon->z1 }, 
                                                                { polygon->x2, polygon->y2, polygon->z2 }, 
                                                                { polygon->x3, polygon->y3, polygon->z3 });


            std::vector<double> interpolatedNormal = interpolateNormals(normalV1, normalV2, normalV3, baryCoords);
            //std::vector<double> interpolatedNormal = interpolateMy(intersectionPoint, v0, v1, v2, normalV1, normalV2, normalV3);
            std::vector<double> normal = interpolatedNormal;

            //std::vector<double> normal = normalizee(addThreeVectorss(normalV1, normalV2, normalV3));

            // Вычисляем нормаль полигона


            
            std::vector<double> normalUnique = {
                    edge1[1] * edge2[2] - edge1[2] * edge2[1],
                    edge1[2] * edge2[0] - edge1[0] * edge2[2],
                    edge1[0] * edge2[1] - edge1[1] * edge2[0]
            };

    
            // Нормализуем нормаль
            
            double normMagnitudeUnique = sqrt(normalUnique[0] * normalUnique[0] + normalUnique[1] * normalUnique[1] + normalUnique[2] * normalUnique[2]);
            if (normMagnitudeUnique > 0)
            {
                normalUnique[0] /= normMagnitudeUnique;
                normalUnique[1] /= normMagnitudeUnique;
                normalUnique[2] /= normMagnitudeUnique;
            }

            //std::vector<double> normal = normalUnique;
        
            if (polygon->color.b != 104) goto jmp;
            std::cout << v0[0] << " " << v0[1] << " " << v0[2] << "\n";
            std::cout << v1[0] << " " << v1[1] << " " << v1[2] << "\n";
            std::cout << v2[0] << " " << v2[1] << " " << v2[2] << "\n";
            std::cout << "normal1: ";
            for (const auto& value : normalV1) {
                std::cout << value << " ";
            }
            std::cout << "\n";

            std::cout << "normal2: ";
            for (const auto& value : normalV2) {
                std::cout << value << " ";
            }
            std::cout << "\n";

            std::cout << "normal3: ";
            for (const auto& value : normalV3) {
                std::cout << value << " ";
            }
            std::cout << "\n";

            std::cout << "normalUnique: ";
            for (const auto& value : normalUnique) {
                std::cout << value << " ";
            }
            std::cout << "\n";

            // Вывод элементов вектора normal
            std::cout << "normal: ";
            for (const auto& value : normal) {
                std::cout << value << " ";
            }
            std::cout << "\n-------\n"; 
            // Направление света
            jmp:
            std::vector<double> lightDir = lightSource.getDirection();
            double lightMagnitude = std::abs(sqrt(lightDir[0] * lightDir[0] + lightDir[1] * lightDir[1] + lightDir[2] * lightDir[2]));
            lightDir[0] /= lightMagnitude;
            lightDir[1] /= lightMagnitude;
            lightDir[2] /= lightMagnitude;

            
            // Рассчитываем интенсивность света (diffuse)
            double dotProduct = std::abs(normal[0] * lightDir[0] + normal[1] * lightDir[1] + normal[2] * lightDir[2]);
            double intensity = dotProduct;

            //intensity *= -1; 

            double intensityV0 = std::abs(normalV1[0] * lightDir[0] + normalV1[1] * lightDir[1] + normalV1[2] * lightDir[2]);
            double intensityV1 = std::abs(normalV2[0] * lightDir[0] + normalV2[1] * lightDir[1] + normalV2[2] * lightDir[2]);
            double intensityV2 = std::abs(normalV3[0] * lightDir[0] + normalV3[1] * lightDir[1] + normalV3[2] * lightDir[2]);
            //intensity = interpolateMyI(intersectionPoint, v0, v1, v2, intensityV0, intensityV1, intensityV2);
            //intensity = intensityV0 * baryCoords[0] + intensityV1 * baryCoords[1] + intensityV2 * baryCoords[2];
            //std::cout << dotProduct << " " << intensity << "\n";

            // Устанавливаем минимальный уровень освещенности
            double minIntensity = 0.1;
            double diffuseIntensity = std::max(intensity, minIntensity);
            
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

            
            if (CheckShadow(lightDir, intersectionPoint, scene))
            {
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

void ProcessRays(int startX, int endX, const std::shared_ptr<Scene>& scene, const std::shared_ptr<Camera>& camera, std::shared_ptr<QtDrawer> drawer, std::vector<std::vector<Color>> &buff)
{
    for (int x = startX; x < endX; ++x)
    {
        for (int y = 0; y < camera->height; ++y)
        {
            ProccessPixel(x, y, scene, camera, drawer, buff);
        }
    }
}


#include <chrono>
void PolygonDrawAdapter::Draw(std::shared_ptr<Scene> scene, std::shared_ptr<Camera> camera)
{
    auto start = std::chrono::high_resolution_clock::now();
    Color backgroundColor(-1, -1, -1);
    std::vector<std::vector<Color>> buff(camera->height, std::vector<Color>(camera->width, backgroundColor));  // БУФЕР КАДРА
    const int numThreads = 42;
    std::cout << "NUM of Threads: " << numThreads << "\n";
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