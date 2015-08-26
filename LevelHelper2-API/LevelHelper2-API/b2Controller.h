/*
* Copyright (c) 2006-2007 Erin Catto http://www.gphysics.com
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

#ifndef B2_CONTROLLER_H
#define B2_CONTROLLER_H


#include "Box2d/Box2D.h"

/// Base class for controllers. Controllers are a convience for encapsulating common
/// per-step functionality.
class b2Controller
{
public:
	virtual ~b2Controller(){
        m_world = NULL;
    }

    b2Controller(b2World* world):
        m_world(world)
    {
    }
	
    /// Controllers override this to apply functionality
    virtual void Apply(b2Body* body) = 0;

	/// Controllers override this to provide debug drawing.
	virtual void Draw(b2Draw *debugDraw) {B2_NOT_USED(debugDraw);};

    void Step(float dt){
        if(m_world){
            for (b2Body* b = m_world->GetBodyList(); b; b = b->GetNext()){
                Apply(b);
            }
        }
    }
    
	/// Get the parent world of this body.
	b2World* GetWorld(){return m_world;}
	const b2World* GetWorld() const{return m_world;}

protected:
	friend class b2World;
    friend class b2Body;
	b2World* m_world;
};

#endif

#endif//LH_USE_BOX2D
