/*
 * Cocos3D-X 1.0.0
 * Author: Bill Hollings
 * Copyright (c) 2010-2014 The Brenwill Workshop Ltd. All rights reserved.
 * http://www.brenwill.com
 *
 * Copyright (c) 2014-2015 Jason Wong
 * http://www.cocos3dx.org/
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * http://en.wikipedia.org/wiki/MIT_License
 */
#include "CC3ParticleEmitter.h"
#include "CC3ParticleNavigator.h"
#include "CC3Particle.h"
#include "../cc3Helper/Logging.h"
#include "../Nodes/CC3UtilityMeshNodes.h"

NS_COCOS3D_BEGIN

CC3ParticleEmitter::CC3ParticleEmitter()
{
	_particles = NULL;
	_particleNavigator = NULL;
}

CC3ParticleEmitter::~CC3ParticleEmitter()
{
	CC_SAFE_RELEASE( _particles ); 
	CC_SAFE_RELEASE( _particleNavigator );
}

CC3ParticleNavigator* CC3ParticleEmitter::getParticleNavigator()
{
	return _particleNavigator; 
}

void CC3ParticleEmitter::setParticleNavigator( CC3ParticleNavigator* aNavigator )
{
	if (aNavigator == _particleNavigator) 
		return;

	if ( _particleNavigator )
		_particleNavigator->setEmitter( NULL );
	
	CC_SAFE_RELEASE( _particleNavigator );
	_particleNavigator = aNavigator;
	CC_SAFE_RETAIN( aNavigator );
	
	if ( _particleNavigator ) 
		_particleNavigator->setEmitter( this );
}

GLfloat CC3ParticleEmitter::getEmissionDuration()
{
	return _emissionDuration;
}

void CC3ParticleEmitter::setEmissionDuration( GLfloat emiationDuration )
{
	_emissionDuration = emiationDuration;
}

GLfloat CC3ParticleEmitter::getElapsedTime()
{
	return _elapsedTime;
}

bool CC3ParticleEmitter::shouldRemoveOnFinish()
{
	return _shouldRemoveOnFinish;
}

void CC3ParticleEmitter::setShouldRemoveOnFinish( bool removeOnFinish )
{
	_shouldRemoveOnFinish = removeOnFinish;
}

GLuint CC3ParticleEmitter::getMaximumParticleCapacity()
{
	return _maximumParticleCapacity;
}

void CC3ParticleEmitter::setMaximumParticleCapacity( GLuint particleCapacity )
{
	_maximumParticleCapacity = particleCapacity;
}

GLuint CC3ParticleEmitter::getCurrentParticleCapacity()
{
	return _currentParticleCapacity;
}

GLuint CC3ParticleEmitter::getParticleCapacityExpansionIncrement()
{
	return _particleCapacityExpansionIncrement;
}

void CC3ParticleEmitter::setParticleCapacityExpansionIncrement( GLuint increment )
{
	_particleCapacityExpansionIncrement = increment;
}

bool CC3ParticleEmitter::shouldUpdateParticlesBeforeTransform()
{
	return _shouldUpdateParticlesBeforeTransform;
}

void CC3ParticleEmitter::setShouldUpdateParticlesBeforeTransform( bool shouldUpdate )
{
	_shouldUpdateParticlesBeforeTransform = shouldUpdate;
}

bool CC3ParticleEmitter::shouldUpdateParticlesAfterTransform()
{
	return _shouldUpdateParticlesAfterTransform;
}

void CC3ParticleEmitter::setShouldUpdateParticlesAfterTransform( bool shouldUpdate )
{
	_shouldUpdateParticlesAfterTransform = shouldUpdate;
}

bool CC3ParticleEmitter::isEmitting()
{
	return _isEmitting;
}

CCArray* CC3ParticleEmitter::getParticles()
{
	return _particles;
}

GLuint CC3ParticleEmitter::getParticleCount()
{
	return _particleCount;
}

bool CC3ParticleEmitter::isFull()
{
	return (_particleCount == _maximumParticleCapacity); 
}

GLfloat CC3ParticleEmitter::getEmissionInterval()
{
	return _emissionInterval; 
}

void CC3ParticleEmitter::setEmissionInterval( GLfloat anInterval )
{
	_emissionInterval = MAX(anInterval, 0.0f);		// Force it to non-negative.
}

GLfloat CC3ParticleEmitter::getEmissionRate()
{
	// Handle special cases first
	if (_emissionInterval <= 0.0f) 
		return kCC3ParticleInfiniteEmissionRate;
	if (_emissionInterval == kCC3ParticleInfiniteInterval) 
		return 0.0f;

	return 1.0f / _emissionInterval;
}

void CC3ParticleEmitter::setEmissionRate( GLfloat aRatePerSecond )
{
	// Handle special cases first
	if (aRatePerSecond <= 0.0f) 
		_emissionInterval = kCC3ParticleInfiniteInterval;
	if (aRatePerSecond == kCC3ParticleInfiniteEmissionRate) 
		_emissionInterval = 0.0f;
	_emissionInterval = 1.0f / aRatePerSecond;
}

void CC3ParticleEmitter::initWithTag( GLuint aTag, const std::string& aName )
{
	super::initWithTag( aTag, aName );
	_particles = CCArray::create();		// retained
	_particles->retain();
	_particleNavigator = NULL;
	_maximumParticleCapacity = kCC3ParticlesNoMax;
	_particleCapacityExpansionIncrement = 100;
	_particleCount = 0;
	_emissionDuration = kCC3ParticleInfiniteInterval;
	_emissionInterval = kCC3ParticleInfiniteInterval;
	_elapsedTime = 0.0f;
	_timeSinceEmission = 0.0f;
	_shouldRemoveOnFinish = false;
	_isEmitting = false;
	_wasStarted = false;
	_shouldUpdateParticlesBeforeTransform = true;
	_shouldUpdateParticlesAfterTransform = false;
}

/** Particles are not copied. */
void CC3ParticleEmitter::populateFrom( CC3ParticleEmitter* another )
{
	super::populateFrom( another ); 

	setMesh( NULL );														// Emitters can't share meshes
	setVertexContentTypes( another->getVertexContentTypes() );					// Use setter to establish a new mesh
	setParticleNavigator( (CC3ParticleNavigator*)another->getParticleNavigator()->copy()->autorelease() );	// Use setter to retain & link back here
	
	_maximumParticleCapacity = another->getMaximumParticleCapacity();
	_particleCapacityExpansionIncrement = another->getParticleCapacityExpansionIncrement();
	_emissionInterval = another->getEmissionInterval();
	_emissionDuration = another->getEmissionDuration();
	_shouldRemoveOnFinish = another->shouldRemoveOnFinish();
	_shouldUpdateParticlesBeforeTransform = another->shouldUpdateParticlesBeforeTransform();
	_shouldUpdateParticlesAfterTransform = another->shouldUpdateParticlesAfterTransform();
}

CCObject* CC3ParticleEmitter::copyWithZone( CCZone* zone )
{
	CC3ParticleEmitter* pVal = new CC3ParticleEmitter;
	pVal->init();
	pVal->populateFrom( this );
	pVal->addCopiesOfChildrenFrom( this );

	return pVal;
}

std::string CC3ParticleEmitter::fullDescription() 
{
	return "CC3ParticleEmitter";
	/*return [NSString stringWithFormat: @"%@ is %@emitting %u of %@ particles every %@ ms for %.1f of %@ seconds, and will expand by %u at %u particles",
			[self class], (_isEmitting ? @"" : @"not "), _particleCount,
			(_maximumParticleCapacity == kCC3ParticlesNoMax ? @"endless" : [NSString stringWithFormat: @"%u", _maximumParticleCapacity]),
			(_emissionInterval == kCC3ParticleInfiniteInterval ? @"endless" : [NSString stringWithFormat: @"%.1f", _emissionInterval * 1000.0f]),
			_elapsedTime,
			(_emissionDuration == kCC3ParticleInfiniteInterval ? @"endless" : [NSString stringWithFormat: @"%.1f", _emissionDuration]),
			_particleCapacityExpansionIncrement, (self.currentParticleCapacity + 1)];*/
}

/**
 * Invoked during node updates.
 * Emits new particles, updates existing particles, and expires aging particles.
 */
void CC3ParticleEmitter::processUpdateBeforeTransform( CC3NodeUpdatingVisitor* visitor )
{
	super::processUpdateBeforeTransform( visitor );
	
	GLfloat dt = visitor->getDeltaTime();
	// LogTrace(@"Updating after %.1f ms: %@", (dt * 1000.0f), self);
	
	// If configured to update particles before the node is transformed, do so here.
	// For each particle, invoke the updateBeforeTransform: method. 
	// Particles can also be removed during the update process.
	if (_shouldUpdateParticlesBeforeTransform)
		updateParticlesBeforeTransform( visitor );
	
	// If emitting and it's time to quit emitting, do so.
	// Otherwise check if it's time to emit particles.
	checkDuration( dt );
	checkEmission( dt );
}

/**
 * For each particle, invoke the updateBeforeTransform: method.
 * If the particle has expired, remove it from the particles array.
 */
void CC3ParticleEmitter::updateParticlesBeforeTransform( CC3NodeUpdatingVisitor* visitor )
{
	GLuint i = 0;
	while (i < _particleCount) 
	{
		CC3Particle* p = (CC3Particle*)_particles->objectAtIndex( i );
		
		p->updateBeforeTransform( visitor );
		
		if (p->isAlive())
		{
			i++;			// Move on to next particle
		} 
		else 
		{
			// Remove the particle from active use and don't increment iterator.
			// CCLOG_TRACE("[ptc]Expiring %s", p->fullDescription().c_str() );
			finalizeAndRemoveParticle( p, i );
		}
	}
}

/** Template method that checks if its time to quit emitting. */
void CC3ParticleEmitter::checkDuration( GLfloat dt )
{
	if (_isEmitting && (_emissionDuration != kCC3ParticleInfiniteInterval)) 
	{
		_elapsedTime += dt;
		if (_elapsedTime >= _emissionDuration) 
			pause();
	}
}

/**
 * Template method that checks if its time to emit a particle,
 * and if so, invokes the emitParticle method to emit the particle.
 */
void CC3ParticleEmitter::checkEmission( GLfloat dt )
{
	if ( !_isEmitting ) 
		return;

	_timeSinceEmission += dt;
	while ( !isFull() && (_timeSinceEmission >= _emissionInterval) ) 
	{
		_timeSinceEmission -= _emissionInterval;
		emitParticle();
	}
}

GLuint CC3ParticleEmitter::emitParticles( GLuint count )
{
	GLuint emitCount = 0;
	for (GLuint i = 0; i < count; i++) 
	{
		if ( emitParticle() ) 
			emitCount++;
	}

	return emitCount;
}

CC3Particle* CC3ParticleEmitter::emitParticle()
{
	CC3Particle* particle = acquireParticle();
	return emitParticle( particle ) ? particle : NULL;
}

/** Template method to create a new particle, or reuse an existing expired particle. */
CC3Particle* CC3ParticleEmitter::acquireParticle()
{
	if (_particleCount < _particles->count()) 
	{
//		CCLOG_TRACE("[ptc]reusing particle at %d", _particleCount);
		return (CC3Particle*)_particles->objectAtIndex( _particleCount );
	}
	else 
	{
//		CCLOG_TRACE("[ptc]creating new particle at %d", _particleCount);
		return makeParticle();
	}
}

bool CC3ParticleEmitter::emitParticle( CC3Particle* aParticle )
{
	if ( !aParticle || isFull() ) 
		return false;		// Can't add particles if there's no space

	/*CC3Assert([aParticle conformsToProtocol: self.requiredParticleProtocol],
			  @"%@ does not conform to the %@ protocol. All particles emitted by %@ must conform to that protocol.", aParticle,
			  [NSString stringWithUTF8String: protocol_getName(self.requiredParticleProtocol)], self);
	CC3Assert(!_particleNavigator || [aParticle conformsToProtocol: _particleNavigator.requiredParticleProtocol],
			  @"%@ does not conform to the %@ protocol. All particles configured by %@ must conform to that protocol.", aParticle,
			  [NSString stringWithUTF8String: protocol_getName(_particleNavigator.requiredParticleProtocol)], _particleNavigator);*/
	
	// Ensure that we have capacity for this particle, and add the particle to the living
	// particles, which also attaches the emitter to the particle.
	if ( !ensureParticleCapacityFor( aParticle ) ) 
		return false;

	addNewParticle( aParticle );

	aParticle->setIsAlive( true );
	initializeParticle( aParticle );

	if ( aParticle->isAlive() )
		_particleNavigator->initializeParticle( aParticle );

	if ( aParticle->isAlive() ) 
		aParticle->initializeParticle();
	
	// If particle not aborted during initialization, accept it.
	if ( aParticle->isAlive() ) 
		acceptParticle( aParticle );

	return aParticle->isAlive();
}

/** Ensures space has been allocated for the specified particle. */
bool CC3ParticleEmitter::ensureParticleCapacityFor( CC3Particle* aParticle )
{
	if ( aParticle->getEmitter() == this ) 
		return true;			// Reusing a particle so we're good

	// If we are at current capacity, see if we can expand
	if ( _particleCount == _currentParticleCapacity ) 
	{
		GLuint origCap = _currentParticleCapacity;
		_currentParticleCapacity = MIN(_currentParticleCapacity + _particleCapacityExpansionIncrement,
									   _maximumParticleCapacity);
		return (_currentParticleCapacity > origCap);	// Return if current capacity actually was increased
	}

	return true;
}

/**
 * If the specified particle is not being reused, it is added to the particles collection,
 * at the end of the living particles, and in front of any expired particles.
 * This emitter is also attached to the particle.
 */
void CC3ParticleEmitter::addNewParticle( CC3Particle* aParticle )
{
	if ( aParticle->getEmitter() == this ) 
		return;			// Reusing a particle so we're good

	_particles->insertObject( aParticle, _particleCount );
	aParticle->setEmitter( this );
}

/** Template method to create a new particle using the particleClass property. */
CC3Particle* CC3ParticleEmitter::makeParticle()
{
	return NULL;
}

void CC3ParticleEmitter::initializeParticle( CC3Particle* aParticle )
{

}

/**
 * Template method that accepts the particle if initialization did not abort the particle.
 *
 * This implementation simply increments the particleCount property. Subclasses may override
 * to perform additional activity to accept the particle.
 */
void CC3ParticleEmitter::acceptParticle( CC3Particle* aParticle )
{
	_particleCount++; 
}

/** Update the particles after the transform, and then update the mesh node.  */
void CC3ParticleEmitter::processUpdateAfterTransform( CC3NodeUpdatingVisitor* visitor )
{
	// If configured to update particles after the node is transformed, do so here.
	// For each particle, invoke the updateBeforeTransform: method. 
	// Particles can also be removed during the update process.
	if ( _shouldUpdateParticlesAfterTransform )
		updateParticlesAfterTransform( visitor );
	
	// If emission has stopped and all the particles have been killed off and the
	// emitter should be removed when finished, remove the emitter from its parent.
	if ( isFinished() && shouldRemoveOnFinish() ) 
	{
		CC3_TRACE("[ptc]CC3ParticleEmitter is exhausted and is being removed");
		visitor->requestRemovalOf( this );
	}
	
	super::processUpdateAfterTransform( visitor );
}

/**
 * For each particle, invoke the updateAfterTransform: method.
 * If the particle has expired, remove it from the particles array.
 */
void CC3ParticleEmitter::updateParticlesAfterTransform( CC3NodeUpdatingVisitor* visitor )
{
	GLuint i = 0;
	while ( i < _particleCount ) 
	{
		CC3Particle* p = (CC3Particle*)_particles->objectAtIndex( i );
		p->updateAfterTransform( visitor );
		
		if (p->isAlive())
		{
			i++;			// Move on to next particle
		}
		else
		{
			// Remove the particle from active use and don't increment iterator.
			CC3_TRACE("[ptc]Expiring %s", p->fullDescription().c_str());
			finalizeAndRemoveParticle( p, i );
		}
	}
}

/** If transitioning to emitting from not, mark as such and reset timers. */
void CC3ParticleEmitter::setIsEmitting( bool shouldEmit )
{
	if ( !_isEmitting && shouldEmit ) 
	{
		_elapsedTime = 0.0;
		_timeSinceEmission = 0.0;
		_wasStarted = true;
	}

	_isEmitting = shouldEmit;
}

void CC3ParticleEmitter::play()
{
	setIsEmitting( true );
}

void CC3ParticleEmitter::pause()
{
	setIsEmitting( false );
}

void CC3ParticleEmitter::stop() 
{
	pause();						// Stop emitting particles...
	removeAllParticles();			// ...and kill those already emitted.
	_particles->removeAllObjects();
}

bool CC3ParticleEmitter::isActive()
{
	return isEmitting() || _particleCount > 0; 
}

// Check for wasStarted needed so it doesn't indicate finished before it starts.
// Otherwise, auto-remove would cause the emitter to be removed immediately.
bool CC3ParticleEmitter::isFinished()
{
	return _wasStarted && !isActive(); 
}

CC3Particle* CC3ParticleEmitter::getParticleAt( GLuint aParticleIndex )
{
	return (CC3Particle*)_particles->objectAtIndex( aParticleIndex );
}

CC3Particle* CC3ParticleEmitter::getParticleWithVertexAt( GLuint vtxIndex )
{
	CCAssert(false, "CC3ParticleEmitter subclass must implement the particleWithVertexAt: method!");
	return NULL;
}

CC3Particle* CC3ParticleEmitter::getParticleWithVertexIndexAt( GLuint index )
{
	CCAssert(false, "CC3ParticleEmitter subclass must implement the particleWithVertexIndexAt: method" );
	return NULL;
}

CC3Particle* CC3ParticleEmitter::getParticleWithFaceAt( GLuint faceIndex )
{
	return getParticleWithVertexIndexAt( getVertexIndexCountFromFaceCount(faceIndex) );
}

void CC3ParticleEmitter::removeParticle( CC3Particle* aParticle )
{
	GLuint pIdx = (GLuint)_particles->indexOfObject( aParticle );
	if ( pIdx < _particleCount )
	{
		aParticle->setIsAlive( false );
		finalizeAndRemoveParticle( aParticle, pIdx );
	}
}

/**
 * Finalizes and removes the specified particle. Finalization must happen first, because the
 * particle may be removed to create space, which can result in its deallocation if this emitter
 * is all that is holding onto it.
 */
void CC3ParticleEmitter::finalizeAndRemoveParticle( CC3Particle* aParticle, GLuint anIndex )
{
	aParticle->finalizeParticle();
	removeParticle( aParticle, anIndex );
}

/**
 * Removes the current particle from the active particles, but keep it cached for future use.
 *
 * This basic implementation simply decrements the particleCount. Subclasses will define behaviour
 * for removing the particle from the particles collection, and for moving the underlying vertex content.
 */
void CC3ParticleEmitter::removeParticle( CC3Particle* aParticle, GLuint anIndex )
{
	_particleCount--;
}

void CC3ParticleEmitter::removeAllParticles()
{
	GLuint pCnt = getParticleCount();
	for (GLuint pIdx = 0; pIdx < pCnt; pIdx++) 
	{
		CC3Particle* aParticle = getParticleAt( pIdx );
		aParticle->setIsAlive( false );
		aParticle->finalizeParticle();
	}
	_particleCount = 0;
}

/** Overridden to test if active as well. If not active, there is nothing to intersect. */
bool CC3ParticleEmitter::doesIntersectBoundingVolume( CC3BoundingVolume* otherBoundingVolume )
{
	return isActive() && super::doesIntersectBoundingVolume( otherBoundingVolume );
}

/** Overridden to set the wireframe to automatically update as parent changes. */
void CC3ParticleEmitter::setShouldDrawLocalContentWireframeBox( bool shouldDraw )
{
	super::setShouldDrawLocalContentWireframeBox( shouldDraw );
	CC3WireframeBoundingBoxNode* pBoxNode = getLocalContentWireframeBoxNode();
	if ( pBoxNode )
		pBoxNode->setShouldAlwaysMeasureParentBoundingBox( true );
}

NS_COCOS3D_END
