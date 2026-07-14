// Vector3D.h
#pragma once

#include <cmath>

/**
 * Einfacher 3D-Vektor für Positionen, Richtungen und Abstände im Raum.
 *
 * Kann für Positionen, Richtungen und Abstände im Raum verwendet werden
 * und bietet einfache Rechenoperatoren.
 */
struct Vector3D
{
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;

    // addieren
    Vector3D operator+(const Vector3D &v) const
    {
        return {x + v.x, y + v.y, z + v.z};
    }
    // subtrahieren
    Vector3D operator-(const Vector3D &v) const
    {
        return {x - v.x, y - v.y, z - v.z};
    }
    // skalieren
    Vector3D operator*(float s) const
    {
        return {x * s, y * s, z * s};
    }
    // skalieren
    Vector3D operator/(float s) const
    {
        return {x / s, y / s, z / s};
    }
    // Länge des Vektors
    float length() const
    {
        return std::sqrt(x * x + y * y + z * z);
    }
    // Länge² des Vektors (für Vergleiche ohne Wurzel)
    float lengthSquared() const
    {
        return x * x + y * y + z * z;
    }
};

/**
 * Berechnet einen Zwischenpunkt zwischen start und end.
 *
 * t = 0.0f -> start
 * t = 1.0f -> end
 * t = 0.5f -> genau in der Mitte
 */
inline Vector3D interpolate(const Vector3D &start, const Vector3D &end, float t)
{
    if (t < 0.0f)
        t = 0.0f;
    if (t > 1.0f)
        t = 1.0f;
    return start + (end - start) * t;
}