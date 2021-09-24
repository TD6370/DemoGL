#pragma once


#include <glm/glm.hpp>

using glm::vec4;
using glm::vec3;
using glm::vec2;

//enum {INTERSECTS, FRONT, BEHIND } ClassifySphereType;
enum { FRONT, INTERSECTS, BEHIND };


bool CheckPointInTriangle(vec3 point, vec3 a, vec3 b, vec3 c);

bool CheckPointInRectangle(vec3& point, vec2& startPos, vec2& endPos);

bool CheckPointInRectangleBorder(vec3& point, vec2& startPos, vec2& endPos, float boderSize);

class Plane {
public:
    int Index;
    float a;
    float b;
    float c;
    vec3 V0;
    vec3 V1;
    vec3 V2;
    vec3 V0_W;
    vec3 V1_W;
    vec3 V2_W;
    float D;
    vec3 Normal;
    vec3 Point; 

    int ClassifyPointType;
    float Radius;
    vec3 R_Point;
    vec3 P_Point;
    vec3 E_Point;
    float DistToPlaneIntersection;
    float DistToSphereIntersection;
    vec3 NormalizedVelocity;
    vec3 Velocity;

    Plane();

    //Plane(vec3 p_a, vec3 p_b, vec3 p_c, glm::mat4& trans, int vers = 0);
    Plane(vec3 p_a, vec3 p_b, vec3 p_c, glm::mat4& trans, int index);

    Plane(const vec3& _p, const vec3& _n);

    ~Plane();

    vec3 GetTriangleNormal(vec3 a, vec3 b, vec3 c);

    float DistanceToPoint2(const vec3& point);

    float DistanceToPoint(const vec3& point);

    //vec3 ClosestPointOnLine(vec3 point);
    vec3 ClosestPointOnLine(vec3 point, vec3 p_a, vec3 p_b);

    int ClassifySphere(vec3& point, float radius, float& distance);

    bool EdgeSphereCollision(vec3& vCenter, float radius);

    vec3 GetPointWorld(vec3 pos, glm::mat4& trans);

    //========================================================
    
    //========================================================

    void SetInspectPoint(vec3 point, float radius);

    //double IntersectRayPlane(vec3 rOrigin, vec3 rVector);
    double IntersectRayPlane(vec3 rOrigin, vec3 rVector, vec3 pOrigin, vec3 pNormal);

    //double ClassifyPoint(float radius);
    double ClassifyPoint();

    //void GetClassifyPointToPlane(float radius);
    void CalculatePlaneIntersectionPoint();

    void CalculatePolygonIntersectionPoint();
   
};

