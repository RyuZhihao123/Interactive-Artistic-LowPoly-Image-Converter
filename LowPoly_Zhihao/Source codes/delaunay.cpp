#include "delaunay.h"



template<class T>
T half(T x)
{}

template <>
float half(float x)
{
    return 0.5f * x;
}

template <>
double half(double x)
{
    return 0.5 * x;
}

template <>
float Vector2<float>::dist(const Vector2<float> &v) const
{
    return hypotf(x - v.x, y - v.y);
}

template <>
double Vector2<double>::dist(const Vector2<double> &v) const
{
    return hypot(x - v.x, y - v.y);
}

template<typename T>
std::ostream &operator << (std::ostream &str, Vector2<T> const &point)
{
    return str << "Point x: " << point.x << " y: " << point.y;
}

template<typename T>
bool operator == (const Vector2<T>& v1, const Vector2<T>& v2)
{
    return (v1.x == v2.x) && (v1.y == v2.y);
}



template <class T>
inline std::ostream &operator << (std::ostream &str, Edge<T> const &e)
{
    return str << "Edge " << e.p1 << ", " << e.p2;
}

template <class T>
inline bool operator == (const Edge<T> & e1, const Edge<T> & e2)
{
    return 	(e1.p1 == e2.p1 && e1.p2 == e2.p2) ||
            (e1.p1 == e2.p2 && e1.p2 == e2.p1);
}

template <class T>
inline std::ostream &operator << (std::ostream &str, const Triangle<T> & t)
{
    return str << "Triangle:" << std::endl << "\t" << t.p1 << std::endl << "\t" << t.p2 << std::endl << "\t" << t.p3 << std::endl << "\t" << t.e1 << std::endl << "\t" << t.e2 << std::endl << "\t" << t.e3 << std::endl;

}

template <class T>
inline bool operator == (const Triangle<T> &t1, const Triangle<T> &t2)
{
    return	(t1.p1 == t2.p1 || t1.p1 == t2.p2 || t1.p1 == t2.p3) &&
            (t1.p2 == t2.p1 || t1.p2 == t2.p2 || t1.p2 == t2.p3) &&
            (t1.p3 == t2.p1 || t1.p3 == t2.p2 || t1.p3 == t2.p3);
}


