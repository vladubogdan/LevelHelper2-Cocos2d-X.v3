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

#include "LHConfig.h"
#if LH_USE_BOX2D

#ifndef B2_BUOYANCYCONTROLLER_H
#define B2_BUOYANCYCONTROLLER_H

#include "b2Controller.h"

/// Calculates buoyancy forces for fluids in the form of a half plane.
class LH_b2BuoyancyController : public b2Controller{
public:
    
    /// The outer surface normal
	b2Vec2 normal;
	/// The height of the fluid surface along the normal
	float32 offset;
	/// The fluid density
	float32 density;
	/// Fluid velocity, for drag calculations
	b2Vec2 velocity;
	/// Linear drag co-efficient
	float32 linearDrag;
	/// Linear drag co-efficient
	float32 angularDrag;
	/// If false, bodies are assumed to be uniformly dense, otherwise use the shapes densities
	bool useDensity; //False by default to prevent a gotcha
	/// If true, gravity is taken from the world instead of the gravity parameter.
	bool useWorldGravity;
	/// Gravity vector, if the world's gravity is not used
	b2Vec2 gravity;
    
    virtual ~LH_b2BuoyancyController(){}
    LH_b2BuoyancyController(b2World* world):
        b2Controller(world),
        normal(b2Vec2(0,-1)),
        offset(0),
        density(1),
        velocity(b2Vec2(0,0)),
        linearDrag(2),
        angularDrag(1),
        useDensity(false),
        gravity(b2Vec2(0,10))
    {
    }
    
	

    /// Controllers override this to apply functionality
    virtual void Apply(b2Body* body);
    bool ApplyToFixture(b2Fixture* f);
    virtual void Draw(b2Draw *debugDraw);
    
protected:

private:
    float32 ComputeSubmergedArea(b2Shape* shape,
                                 const b2Vec2& normal,
                                 float32 offset,
                                 const b2Transform& xf,
                                 b2Vec2* c,
                                 float32 density) const;

};


#endif

#endif//LH_USE_BOX2D