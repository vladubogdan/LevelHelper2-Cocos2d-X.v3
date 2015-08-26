/*
* Copyright (c) 2006-2009 Erin Catto http://www.gphysics.com
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*/

#include "LHb2BuoyancyController.h"

#if LH_USE_BOX2D


void LH_b2BuoyancyController::Apply(b2Body* body)
{
    if(!body->IsAwake() || body->GetType() != b2_dynamicBody){
        return;
    }
    
    b2Fixture* f = body->GetFixtureList();
    while (f) {
        ApplyToFixture(f);
        f = f->GetNext();
    }
}

float32 LH_b2BuoyancyController::ComputeSubmergedArea(b2Shape* shape,
                                                   const b2Vec2& normal,
                                                   float32 offset,
                                                   const b2Transform& xf,
                                                   b2Vec2* c,
                                                   float32 density) const
{
    if(shape->GetType() == b2Shape::e_edge)
    {
        //Note that v0 is independant of any details of the specific edge
        //We are relying on v0 being consistent between multiple edges of the same body
        b2Vec2 v0 = offset * normal;
        //b2Vec2 v0 = xf.position + (offset - b2Dot(normal, xf.position)) * normal;
        
        b2Vec2 v1 = b2Mul(xf, ((b2EdgeShape*)shape)->m_vertex1);
        b2Vec2 v2 = b2Mul(xf, ((b2EdgeShape*)shape)->m_vertex2);
        
        float32 d1 = b2Dot(normal, v1) - offset;
        float32 d2 = b2Dot(normal, v2) - offset;
        
        if(d1>0)
        {
            if(d2>0)
            {
                return 0;
            }
            else
            {
                v1 = -d2 / (d1 - d2) * v1 + d1 / (d1 - d2) * v2;
            }
        }
        else
        {
            if(d2>0)
            {
                v2 = -d2 / (d1 - d2) * v1 + d1 / (d1 - d2) * v2;
            }
            else
            {
                //Nothing
            }
        }
        
        // v0,v1,v2 represents a fully submerged triangle
        float32 k_inv3 = 1.0f / 3.0f;
        
        // Area weighted centroid
        *c = k_inv3 * (v0 + v1 + v2);
        
        b2Vec2 e1 = v1 - v0;
        b2Vec2 e2 = v2 - v0;
        
        return 0.5f * b2Cross(e1, e2);
    }
    else if(shape->GetType() == b2Shape::e_polygon)
    {
        //Transform plane into shape co-ordinates
        b2Vec2 normalL = b2MulT(xf.q,normal);
        float32 offsetL = offset - b2Dot(normal,xf.p);
        
        float32 depths[b2_maxPolygonVertices];
        int32 diveCount = 0;
        int32 intoIndex = -1;
        int32 outoIndex = -1;
        
        bool lastSubmerged = false;
        int32 i;
        for(i=0;i<((b2PolygonShape*)shape)->m_count;i++){
            depths[i] = b2Dot(normalL,((b2PolygonShape*)shape)->m_vertices[i]) - offsetL;
            
            
            bool isSubmerged = depths[i]<-FLT_EPSILON;
            if(i>0){
                if(isSubmerged){
                    if(!lastSubmerged){
                        intoIndex = i-1;
                        diveCount++;
                    }
                }else{
                    if(lastSubmerged){
                        outoIndex = i-1;
                        diveCount++;
                    }
                }
            }
            lastSubmerged = isSubmerged;
        }
        switch(diveCount){
            case 0:
                if(lastSubmerged){
                    //Completely submerged
                    b2MassData md;
                    ((b2PolygonShape*)shape)->ComputeMass(&md, density);
                    *c = b2Mul(xf,md.center);
                    return md.mass/density;
                }else{
                    //Completely dry
                    return 0;
                }
                break;
            case 1:
                if(intoIndex==-1){
                    intoIndex = ((b2PolygonShape*)shape)->m_count-1;
                }else{
                    outoIndex = ((b2PolygonShape*)shape)->m_count-1;
                }
                break;
        }
        int32 intoIndex2 = (intoIndex+1)%((b2PolygonShape*)shape)->m_count;
        int32 outoIndex2 = (outoIndex+1)%((b2PolygonShape*)shape)->m_count;
        
        float32 intoLambda = (0 - depths[intoIndex]) / (depths[intoIndex2] - depths[intoIndex]);
        float32 outoLambda = (0 - depths[outoIndex]) / (depths[outoIndex2] - depths[outoIndex]);
        
        b2Vec2 intoVec(	((b2PolygonShape*)shape)->m_vertices[intoIndex].x*(1-intoLambda)+((b2PolygonShape*)shape)->m_vertices[intoIndex2].x*intoLambda,
                       ((b2PolygonShape*)shape)->m_vertices[intoIndex].y*(1-intoLambda)+((b2PolygonShape*)shape)->m_vertices[intoIndex2].y*intoLambda);
        b2Vec2 outoVec(	((b2PolygonShape*)shape)->m_vertices[outoIndex].x*(1-outoLambda)+((b2PolygonShape*)shape)->m_vertices[outoIndex2].x*outoLambda,
                       ((b2PolygonShape*)shape)->m_vertices[outoIndex].y*(1-outoLambda)+((b2PolygonShape*)shape)->m_vertices[outoIndex2].y*outoLambda);
        
        //Initialize accumulator
        float32 area = 0;
        b2Vec2 center(0,0);
        b2Vec2 p2 = ((b2PolygonShape*)shape)->m_vertices[intoIndex2];
        b2Vec2 p3;
        
        float32 k_inv3 = 1.0f / 3.0f;
        
        //An awkward loop from intoIndex2+1 to outIndex2
        i = intoIndex2;
        while(i!=outoIndex2){
            i=(i+1)%((b2PolygonShape*)shape)->m_count;
            if(i==outoIndex2)
                p3 = outoVec;
            else
                p3 = ((b2PolygonShape*)shape)->m_vertices[i];
            //Add the triangle formed by intoVec,p2,p3
            {
                b2Vec2 e1 = p2 - intoVec;
                b2Vec2 e2 = p3 - intoVec;
                
                float32 D = b2Cross(e1, e2);
                
                float32 triangleArea = 0.5f * D;
                
                area += triangleArea;
                
                // Area weighted centroid
                center += triangleArea * k_inv3 * (intoVec + p2 + p3);
                
            }
            //
            p2=p3;
        }
        
        //Normalize and transform centroid
        center *= 1.0f/area;
        
        *c = b2Mul(xf,center);
        
        return area;
    }
    else if(shape->GetType() == b2Shape::e_circle)
    {
        b2Vec2 p = b2Mul(xf,((b2CircleShape*)shape)->m_p);
        float32 l = -(b2Dot(normal,p) - offset);
        if(l<-((b2CircleShape*)shape)->m_radius+FLT_EPSILON){
            //Completely dry
            return 0;
        }
        if(l > ((b2CircleShape*)shape)->m_radius){
            //Completely wet
            *c = p;
            return b2_pi*((b2CircleShape*)shape)->m_radius*((b2CircleShape*)shape)->m_radius;
        }
        
        //Magic
        float32 r2 = ((b2CircleShape*)shape)->m_radius*((b2CircleShape*)shape)->m_radius;
        float32 l2 = l*l;
        //TODO: write b2Sqrt to handle fixed point case.
        float32 area = r2 * (asin(l/((b2CircleShape*)shape)->m_radius) + b2_pi/2.0f)+ l * b2Sqrt(r2 - l2);
        float32 com = -2.0f/3.0f*pow(r2-l2,1.5f)/area;
        
        c->x = p.x + normal.x * com;
        c->y = p.y + normal.y * com;
        
        return area;
    }
    
    return 0;
}


bool LH_b2BuoyancyController::ApplyToFixture(b2Fixture* f)
{
    b2Body* body = f->GetBody();
    b2Vec2 areac(0,0);
    b2Vec2 massc(0,0);
    float32 area = 0;
    float32 mass = 0;
    
    b2Shape* shape = f->GetShape();
    b2Vec2 sc(0,0);
    
    
    float32 sarea = ComputeSubmergedArea(shape,
                                         normal,
                                         offset,
                                         body->GetTransform(),
                                         &sc,
                                         f->GetDensity());
    
    area += sarea;
    areac.x += sarea * sc.x;
    areac.y += sarea * sc.y;
    float shapeDensity = useDensity ? f->GetDensity() : 1;
    mass += sarea * shapeDensity;
    massc.x += sarea * sc.x * shapeDensity;
    massc.y += sarea * sc.y * shapeDensity;
    
    areac.x /= area;
    areac.y /= area;
    massc.x /= mass;
    massc.y /= mass;
    if(area < FLT_EPSILON) {
        return false;
    }
    
    //Buoyancy
    b2Vec2 buoyancyForce = -density*area*gravity;
    body->ApplyForce(buoyancyForce,massc, true);
    //Linear drag
    b2Vec2 dragForce = body->GetLinearVelocityFromWorldPoint(areac) - velocity;
    dragForce *= -linearDrag*area;
    body->ApplyForce(dragForce,areac, true);
    //Angular drag
    //TODO: Something that makes more physical sense?
    body->ApplyTorque(-body->GetInertia()/body->GetMass()*area*body->GetAngularVelocity()*angularDrag, true);
    
    return true;
}

void LH_b2BuoyancyController::Draw(b2Draw *debugDraw)
{
	float32 r = 1000;
	b2Vec2 p1 = offset * normal + b2Cross(normal, r);
	b2Vec2 p2 = offset * normal - b2Cross(normal, r);

	b2Color color(0,0,0.8f);

	debugDraw->DrawSegment(p1, p2, color);
}

#endif
