# define M_PI           3.14159265358979323846  /* pi */

#include "GeometryLib.h"

#include <glm/glm.hpp>

using glm::vec4;
using glm::vec3;
using glm::vec2;
using glm::dot;

    Plane::Plane() {}

    Plane::Plane(const vec3& _p, const vec3& _n)
    {
        Normal = glm::normalize(_n);
        a = Normal.x;
        b = Normal.y;
        c = Normal.z;
        D = -(_p.x * a + _p.y * b + _p.z * c);
    }

    vec3 andOp(const vec3& _v1, const vec3& _v2)
    {
        return vec3(_v2.y * _v1.z - _v2.z * _v1.y,
            _v2.z * _v1.x - _v2.x * _v1.z,
            _v2.x * _v1.y - _v2.y * _v1.x);
    }

    Plane::Plane(vec3 p_a, vec3 p_b, vec3 p_c, glm::mat4& trans, int vers) 
    {
        vers = 1;
        if (vers == 1)
        {
            V0 = p_a;
            V1 = p_b;
            V2 = p_c;
            V0_W = vec3(trans * vec4(p_a, 1));
            V1_W = vec3(trans * vec4(p_b, 1));
            V2_W = vec3(trans * vec4(p_c, 1));

            Point = V0_W;

            vec3 edge1(V0_W - V1_W);
            vec3 edge2(V2_W - V1_W);
            Normal = andOp(edge1, edge2);
            vec3 Normal_1 = cross(edge1, edge2);
            Normal = glm::normalize(Normal);
            a = Normal.x;
            b = Normal.y;
            c = Normal.z;
            D = -(V0_W.x * a + V0_W.y * b + V0_W.z * c);
            //----------------
        }
        else 
        {
            V0 = p_a;
            V1 = p_b;
            V2 = p_c;
            Point = vec3(trans * vec4(V0, 1));

            Normal = GetTriangleNormal(V0, V1, V2);// cross(edge1, edge2);
            // рассто€ние от начала координат до плоскости: 
            //D = -Normal*Point (от нормали и точки плоскости)
            D = glm::dot(Normal, Point); //P is a point in the plane, like a, b or c
        }
    }

    Plane::~Plane() {

    }

    vec3 Plane::GetTriangleNormal(vec3 a, vec3 b, vec3 c) {
        vec3 edge1 = b - a;
        vec3 edge2 = c - a;
        vec3 normal = cross(edge1, edge2);
        return normal;
    }

    vec3 Plane::GetPointWorld(vec3 pos, glm::mat4& trans) {
        vec3 point = vec3(trans * vec4(pos, 1));;
        return point;
    }

    
    

    float Plane::DistanceToPoint2(const vec3& point)
    {
        //float dist = (glm::dot(plane.Normal, point) - plane.Normal) / glm::dot(plane.Normal, plane.Normal);
        //vec3 distV = (glm::dot(Normal, point) - Normal) / glm::dot(Normal, Normal);
        //-------------
        //auto dist2 = glm::dot(a, point.x) + glm::dot(b, point.y) + glm::dot(c, point.z) + D;// = (рассто€ние от точки до плоскости)
        //distV = (a * point.x) + (b * point.y) + (c * point.z) + D;// = (рассто€ние от точки до плоскости)
        //-------------
        float dist = glm::dot(Normal, point) + D;
        //-------------

        return dist;
    }

    float Plane::DistanceToPoint(const vec3& point)
    {
        //float test = DistanceToPoint2(point);
       //float dist1 = glm::dot(Normal, point) + D;

        float dist = glm::dot(Normal, point - Point);
        return dist;
    }

    //Ќахождение точки на данном отрезке, ближайшей к данной точке
    /*vec3 Plane::ClosestPointOnLine(vec3 point)
    {
        
        vec3 vVector1 = point - a;
        vec3 vVector2 = glm::normalize(b - a);
        float d = glm::distance(a, b);
        float t = glm::dot(vVector2, vVector1);

        if (t <= 0) return a;
        if (t >= d) return b;

        vec3 vVector3 = vVector2 * t;
        vec3 vClosestPoint = a + vVector3;
        return vClosestPoint;
    }*/

    vec3 Plane::ClosestPointOnLine(vec3 p_a, vec3 p_b, vec3 point)
    {
        vec3 vVector1 = point - p_a;
        vec3 vVector2 = glm::normalize(p_b - p_a);
        float d = glm::distance(p_a, p_b);
        float t = glm::dot(vVector2, vVector1);

        if (t <= 0) return p_a;
        if (t >= d) return p_b;

        vec3 vVector3 = vVector2 * t;
        vec3 vClosestPoint = a + vVector3;
        return vClosestPoint;
    }

    //ќпределение положени€ сферы относительно плоскости
    int Plane::ClassifySphere(vec3& point,
        float radius, float& distance)
    {
        distance = (Normal.x * point.x + Normal.y * point.y +
            Normal.z * point.z + D);
        //return 0;
        if (fabs(distance) < radius) return INTERSECTS;
        else if (distance >= radius) return FRONT;
        return BEHIND;
    }

    //ѕроверка пересечени€ сферы с ребром полигона
    bool Plane::EdgeSphereCollision(vec3& point, float radius)
    {
        int vertexCount = 3;
        vec3 vPolygon[3] = {V0,V1,V2};
        vec3 vPoint;
        for (int i = 0; i < vertexCount; i++)
        {
            vPoint = ClosestPointOnLine(vPolygon[i],
                vPolygon[(i + 1) % vertexCount], point);
            float distance = glm::distance(vPoint, point);
            if (distance < radius) return true;
        }
        return false;
    }

    //-->>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


    void Plane::SetInspectPoint(vec3 point, float radius)
    {
        P_Point = vec3(-50000);
        E_Point = vec3(-50000);
        R_Point = point;
        Radius = radius;
        Velocity = vec3(R_Point.x, R_Point.y - Radius, R_Point.z);
        NormalizedVelocity = glm::normalize(Velocity);
    }

    //double Plane::IntersectRayPlane(vec3 rOrigin, vec3 rVector, vec3 pOrigin, vec3 pNormal) {
    //double Plane::IntersectRayPlane(vec3 rOrigin, vec3 point) {

    //    double d = -(dot(Normal, Point));
    //    double numer = dot(Normal, Point) + d;
    //    double denom = dot(Normal, point);

    //    if (denom == 0)  // normal is orthogonal to vector, cant intersect
    //        return (-1.0f);
    //    return -(numer / denom);
    //}
    double Plane::IntersectRayPlane(vec3 rOrigin, vec3 rVector, vec3 pOrigin, vec3 pNormal)
    {

        double d = -(dot(pNormal, pOrigin));
        double numer = dot(pNormal, rOrigin) + d;
        double denom = dot(pNormal, rVector);

        if (denom == 0)  // normal is orthogonal to vector, cant intersect
            return (-1.0f);
        return -(numer / denom);
    }


    //double classifyPoint(vec3 point, vec3 pO, vec3 pN, 
    //    float radius) {

    //    vec3 dir = pO - point;
    //    double distance = dot(dir, pN);

    //    if (distance < -0.001f)
    //        return FRONT;// PLANE_FRONT;
    //    else
    //        if (distance > 0.001f)
    //            return BEHIND;//  PLANE_BACKSIDE;

    //    if (fabs(distance) < radius) return INTERSECTS;
    //    return;// ON_PLANE;
    //}

    double Plane::ClassifyPoint() {

        float radius = Radius;

        vec3 dir = Point - R_Point;
        double distance = dot(dir, Normal);

        if (distance < -0.001f)
        //if (distance < -radius)
            return FRONT;// PLANE_FRONT;
        else if (distance > 0.001f)
            //if (distance > radius)
            return BEHIND;//  PLANE_BACKSIDE;

        //return 0;// ON_PLANE;
        return INTERSECTS;
    }
      

    void Plane::CalculatePlaneIntersectionPoint()
    {

        ClassifyPointType = ClassifyPoint();

        // find the plane intersection point
        if (ClassifyPointType == BEHIND) { // plane spans sphere

            // find plane intersection point by shooting a ray from the 
            // sphere intersection point along the planes normal.
            // shoot ray along the velocity vector
            DistToPlaneIntersection = IntersectRayPlane(R_Point, NormalizedVelocity, Point, Normal);

            // calculate plane intersection point
            P_Point.x = R_Point.x + DistToPlaneIntersection * Normal.x;
            P_Point.y = R_Point.y + DistToPlaneIntersection * Normal.y;
            P_Point.z = R_Point.z + DistToPlaneIntersection * Normal.z;
        }
    }

    //................


    bool CheckPointInTriangle(vec3 point, vec3 a, vec3 b, vec3 c) {

        double total_angles = 0.0f;

        // make the 3 vectors
        vec3 v1 = point - a;
        vec3 v2 = point - b;
        vec3 v3 = point - c;

        v1 = glm::normalize(v1);
        v2 = glm::normalize(v2);
        v3 = glm::normalize(v3);


        total_angles += acos(dot(v1, v2));
        total_angles += acos(dot(v2, v3));
        total_angles += acos(dot(v3, v1));

        // allow a small margin because of the limited precision of
        // floating point math.
        if (fabs(total_angles - 2 * M_PI) <= 0.005)
            return (true);

        return(false);
    }

    vec3 ClosestPointOnLine(vec3& a, vec3& b, vec3& p) {

        // a-b is the line, p the point in question
        vec3 c = p - a;
        vec3 V = b - a;
        double d = V.length();

        glm::normalize(V);
        double t = dot(V, c);

        // Check to see if СtТ is beyond the extents of the line segment
        if (t < 0.0f) return (a);
        if (t > d) return (b);

        // Return the point between СaТ and СbТ
        //set length of V to t. V is normalized so this is easy
        V.x = V.x * t;
        V.y = V.y * t;
        V.z = V.z * t;

        return (a + V);
    }


    vec3 ClosestPointOnTriangle(vec3  a, vec3  b, vec3  c, vec3  p) {

        vec3  Rab = ClosestPointOnLine(a, b, p);
        vec3  Rbc = ClosestPointOnLine(b, c, p);
        vec3  Rca = ClosestPointOnLine(c, a, p);

        double dAB = vec3(p - Rab).length();
        double dBC = vec3(p - Rbc).length();
        double dCA = vec3(p - Rca).length();

        double min = dAB;
        vec3 result = Rab;

        if (dBC < min) {
            min = dBC;
            result = Rbc;
        }

        if (dCA < min)
            result = Rca;

        return (result);
    }

    double IntersectRaySphere(vec3 rO, vec3 rV, vec3 sO, double sR) {
        vec3 Q = sO - rO;
        double c = Q.length();
        double v = dot(Q, rV);
        double d = sR * sR - (c * c - v * v);

        // If there was no intersection, return -1
        if (d < 0.0) return (-1.0f);

        // Return the distance to the [first] intersecting point
        return (v - sqrt(d));
    }

    
    //The whole process of finding the final sphere intersection pointand polygon intersection finally boils down to a few lines of code :
    // find polygon intersection point. By default we assume its equal to the 
    // plane intersection point. In that case we already know the distance to it.
    void Plane::CalculatePolygonIntersectionPoint()
    {
        vec3 polyIPoint = P_Point;
        DistToSphereIntersection = DistToPlaneIntersection;

        if (!CheckPointInTriangle(P_Point, V0_W, V1_W, V2_W)) { // если не в треугольнике// if not in triangle
            polyIPoint = ClosestPointOnTriangle(V0_W, V1_W, V2_W, P_Point);

            //distToSphereIntersection = IntersectRaySphere(polyIPoint, -NormalizedVelocity, E_Point, 1.0f);
            DistToSphereIntersection = IntersectRaySphere(polyIPoint, -NormalizedVelocity, R_Point, Radius);

            // we cannot know if the ray will actually hit the sphere so we need this check
            if (DistToSphereIntersection > 0) {
                // calculate true ellipsoid intersection point
                E_Point.x = polyIPoint.x + DistToSphereIntersection * -NormalizedVelocity.x;
                E_Point.y = polyIPoint.y + DistToSphereIntersection * -NormalizedVelocity.y;
                E_Point.z = polyIPoint.z + DistToSphereIntersection * -NormalizedVelocity.z;
            }
        }
        //--
    }

    //0000000000000000
    
    


