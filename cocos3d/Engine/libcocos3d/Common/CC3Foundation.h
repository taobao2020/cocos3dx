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
#ifndef _CCL_CC3FOUNDATION_H_
#define _CCL_CC3FOUNDATION_H_

NS_COCOS3D_BEGIN

#define COCOS3D_VERSION 0x010000

class Cocos3d
{
public:
	static void			startUp( bool isEditor = true );
	static void			shutdown();
	static bool			isEditor();
	static void			addInternalShaderPaths();
	static std::string	version();

private:
	static bool			sm_isEditor;
};

typedef struct 
{
	unsigned long location;
	unsigned long length;
} CCRange;

static inline CCRange CCRangeMake( unsigned long location, unsigned long length )
{
	CCRange range;
	range.location = location;
	range.length = length;

	return range;
}

static inline unsigned int CCMaxRange( const CCRange& range )
{
	return range.location + range.length;
}

static inline bool CCLocationInRange( unsigned long location, const CCRange& range )
{
	return location >= range.location && location < CCMaxRange( range );
}

static inline CCRange CCIntersectionRange( const CCRange& range, const CCRange& otherRange )
{
	unsigned long min, loc, max1= CCMaxRange( range ), max2 = CCMaxRange( otherRange );
	CCRange result;

	min = ( max1 < max2 ) ? max1 : max2;
	loc = ( range.location > otherRange.location ) ? range.location : otherRange.location;

	if ( min < loc )
	{
		result.location = result.length = 0;
	}
	else
	{
		result.location = loc;
		result.length = min - loc;
	}

	return result;
}

static inline CCRange CCUnionRange( const CCRange& range, const CCRange& otherRange )
{
	unsigned long max, loc, max1 = CCMaxRange( range ), max2 = CCMaxRange( otherRange );
	CCRange result;

	max = ( max1 > max2 ) ? max1 : max2;
	loc = ( range.location < otherRange.location ) ? range.location : otherRange.location;

	result.location = loc;
	result.length = max - result.location;

	return result;
}

#define ccr( a, b ) CCRangeMake( (a), (b) )

/** An integer 2D point. */
typedef struct 
{
	GLint x;			/**< The X-componenent of the point. */
	GLint y;			/**< The Y-componenent of the point. */
} CC3IntPoint;

/** A CC3IntPoint at the origin. */
static const CC3IntPoint kCC3IntPointZero = { 0, 0 };

static inline std::string stringFromCC3IntPoint( const CC3IntPoint& point )
{
	return CC3String::stringWithFormat( (char*)"(%d, %d)", point.x, point.y );
}

/** Returns a CC3IntPoint structure constructed from the vector components. */
static inline CC3IntPoint CC3IntPointMake(GLint x, GLint y) 
{
	CC3IntPoint v;
	v.x = x;
	v.y = y;
	return v;
}

/** Returns a CC3IntPoint structure constructed from the specified CCPoint. */
static inline CC3IntPoint CC3IntPointFromCGPoint( const CCPoint& pt ) 
{
	return CC3IntPointMake((GLint)pt.x, (GLint)pt.y); 
}

/** Returns whether the two points are equal by comparing their respective components. */
static inline bool CC3IntPointsAreEqual(CC3IntPoint p1, CC3IntPoint p2)
{
	return	(p1.x == p2.x) &&
		(p1.y == p2.y);
}

/** Returns whether the specified point is zero, as specified by kCC3IntPointZero. */
static inline bool CC3IntPointIsZero(CC3IntPoint p) { return CC3IntPointsAreEqual(p, kCC3IntPointZero); }

/** Returns a point representing the sum of the two specified points, by add their respective components. */
static inline CC3IntPoint CC3IntPointAdd(CC3IntPoint p1, CC3IntPoint p2)
{
	return	CC3IntPointMake(p1.x + p2.x, p1.y + p2.y);
}

/** An integer 2D size. */
typedef struct 
{
	GLsizei width;			/**< The width measurement. */
	GLsizei height;			/**< The height measurement. */
} CC3IntSize;

/** A CC3IntSize of zero extent. */
static const CC3IntSize kCC3IntSizeZero = { 0, 0 };

/** Returns a CC3IntSize structure constructed from the dimensional components. */
static inline CC3IntSize CC3IntSizeMake(GLint w, GLint h) 
{
	CC3IntSize sz;
	sz.width = w;
	sz.height = h;
	return sz;
}

/** Returns a CC3IntSize structure constructed from the specified CGSize. */
static inline CC3IntSize CC3IntSizeFromCGSize(const CCSize& sz) 
{
	return CC3IntSizeMake((GLint)sz.width, (GLint)sz.height); 
}

/** Returns a CGSize structure constructed from the specified CC3IntSize. */
static inline CCSize CGSizeFromCC3IntSize(CC3IntSize isz) 
{
	return CCSizeMake(isz.width, isz.height); 
}

/** Returns whether the two sizes are equal by comparing their respective components. */
static inline bool CC3IntSizesAreEqual(CC3IntSize s1, CC3IntSize s2) 
{
	return	(s1.width == s2.width) &&
		(s1.height == s2.height);
}

/** Returns whether the specified size is zero, as specified by kCC3IntSizeZero. */
static inline bool CC3IntSizeIsZero(CC3IntSize s) 
{
	return CC3IntSizesAreEqual(s, kCC3IntSizeZero); 
}

/** A struct representing an integer tessellation. */
typedef CC3IntPoint CC3Tessellation;

/** Returns a CC3Tessellation structure constructed from the specified components. */
static inline CC3Tessellation CC3TessellationMake(GLint x, GLint y) 
{
	return CC3IntPointMake(x, y); 
}


/** An integer 3D vector. */
typedef struct 
{
	GLint x;			/**< The X-componenent of the vector. */
	GLint y;			/**< The Y-componenent of the vector. */
	GLint z;			/**< The Z-componenent of the vector. */
} CC3IntVector;

static inline std::string stringFromCC3IntVector( const CC3IntVector& vec )
{
	return CC3String::stringWithFormat( (char*)"(%d, %d, %d)", vec.x, vec.y, vec.z );
}

/** Returns a CC3IntVector structure constructed from the vector components. */
static inline CC3IntVector CC3IntVectorMake(GLint x, GLint y, GLint z) 
{
	CC3IntVector v;
	v.x = x;
	v.y = y;
	v.z = z;
	return v;
}

/** Returns whether the two vectors are equal by comparing their respective components. */
static inline bool CC3IntVectorsAreEqual(CC3IntVector v1, CC3IntVector v2)
{
	return	(v1.x == v2.x) &&
		(v1.y == v2.y) &&
		(v1.z == v2.z);
}

static inline std::string stringFromCCPoint( const CCPoint& point )
{
	return CC3String::stringWithFormat( (char*)"(%.3f, %.3f)", point.x, point.y );
}

/**
 * Orthonormalizes the specified array of vectors, using a Gram-Schmidt process,
 * and returns the orthonormal results in the same array.
 *
 * The vectorCount argument indicates the number of vectors in the vectors array argument.
 *
 * Upon completion, each vector in the specfied array will be a unit vector that
 * is orthagonal to all of the other vectors in the array.
 *
 * The first vector in the array is used as the starting point for orthonormalization.
 * Since the Gram-Schmidt process is biased towards the starting vector, if this function
 * will be used repeatedly on the same set of vectors, it is recommended that the order
 * of the vectors in the array be changed on each call to this function, to ensure that
 * the starting bias be averaged across each of the vectors over the long term.
 */
void CC3VectorOrthonormalize(CC3Vector* vectors, GLuint vectorCount);

/**
 * Orthonormalizes the specified array of three vectors, using a Gram-Schmidt process,
 * and returns the orthonormal results in the same array.
 *
 * The number of vectors in the specified array must be exactly three.
 *
 * Upon completion, each vector in the specfied array will be a unit vector that
 * is orthagonal to all of the other vectors in the array.
 *
 * The first vector in the array is used as the starting point for orthonormalization.
 * Since the Gram-Schmidt process is biased towards the starting vector, if this function
 * will be used repeatedly on the same set of vectors, it is recommended that the order
 * of the vectors in the array be changed on each call to this function, to ensure that
 * the starting bias be averaged across each of the vectors over the long term.
 */
static inline void CC3VectorOrthonormalizeTriple(CC3Vector* triVector) 
{
	return CC3VectorOrthonormalize(triVector, 3); 
}


/** 
 * An integer 4D vector.
 *
 * This structure can be referenced as containing 4 individual XYZW axes components,
 * or containing a 3D XYZ vector, plus a W component.
*/
typedef struct 
{
	union {
		struct {
			GLint x;			/**< The X-componenent of the vector. */
			GLint y;			/**< The Y-componenent of the vector. */
			GLint z;			/**< The Z-componenent of the vector. */
		};
		
		struct {
			CC3IntVector v;		/**< The X, Y & Z components as a 3D vector. */
		};
	};
	GLint w;					/**< The homogeneous ratio factor. */
} CC3IntVector4;

static inline std::string stringFromCC3IntVector4( const CC3IntVector4& vec )
{
	return CC3String::stringWithFormat( (char*)"(%d, %d, %d, %d)", vec.x, vec.y, vec.z, vec.w );
}

/** Returns a CC3IntVector4 structure constructed from the vector components. */
static inline CC3IntVector4 CC3IntVector4Make(GLint x, GLint y, GLint z, GLint w)
{
	CC3IntVector4 v;
	v.x = x;
	v.y = y;
	v.z = z;
	v.w = w;
	return v;
}

/** Returns whether the two vectors are equal by comparing their respective components. */
static inline bool CC3IntVector4sAreEqual(CC3IntVector4 v1, CC3IntVector4 v2) 
{
	return (v1.w == v2.w) && CC3IntVectorsAreEqual(v1.v, v2.v);
}

//typedef CC3Vector4 CC3Quaternion;

/**
 * Defines a ray or line in 3D space, by specifying a starting location and direction.
 *
 * For a line, the startLocation can variously be interpreted as the location of any
 * point on the line.
 */
typedef struct 
{
	CC3Vector startLocation;	/**< The location where the ray starts. */
	CC3Vector direction;		/**< The direction in which the ray points. */
} CC3Ray;

/** Returns a string description of the specified CC3Ray struct. */
static inline std::string stringFromCC3Ray( const CC3Ray& aRay )
{
	return CC3String::stringWithFormat( (char*)"(Start: %s, Towards: %s)",
		aRay.startLocation.stringfy().c_str(), aRay.direction.stringfy().c_str() );
}

/** Returns a CC3Ray structure constructed from the start location and direction vectors. */
static inline CC3Ray CC3RayFromLocDir(CC3Vector aLocation, CC3Vector aDirection) 
{
	CC3Ray aRay;
	aRay.startLocation = aLocation;
	aRay.direction = aDirection;
	return aRay;
}

/** Returns a CC3Ray structure constructed from the start location and direction components. */
static inline CC3Ray CC3RayMake(GLfloat locX, GLfloat locY, GLfloat locZ,
								GLfloat dirX, GLfloat dirY, GLfloat dirZ) 
{
	return CC3RayFromLocDir(CC3VectorMake(locX, locY, locZ),
							CC3VectorMake(dirX, dirY, dirZ));
}

/** Returns whether the specified location lies on the specified ray. */
static inline bool CC3IsLocationOnRay(CC3Vector aLocation, CC3Ray aRay) 
{
	// Get a vector from the start of the ray to the location to be tested.
	// Project that vector onto the ray to find the projection of the location
	// onto the ray. If the projected location is the same as the initial
	// location, then the location is on the ray.
	CC3Vector locVect = aLocation.difference( aRay.startLocation );
	GLfloat proj = locVect.dot( aRay.direction );
	CC3Vector projVect = aRay.direction.scaleUniform( proj );
	CC3Vector projLoc = aRay.startLocation.add( projVect );
	return aLocation.equals( projLoc );
}

/** Returns a ccTex2F structure constructed from the vector components. */
static inline ccTex2F CC3TexCoordsMake(GLfloat u, GLfloat v) 
{
	ccTex2F tc;
	tc.u = u;
	tc.v = v;
	return tc;
}

/** Convenience alias macro to create ccTex2F with less keystrokes. */
#define cc3tc(U,V) CC3TexCoordsMake((U),(V))

/**
 * Defines a simple vertex, containing location and color.
 * Useful for painting solid colors that ignore lighting conditions.
 */
typedef struct 
{
	CC3Vector location;			/**< The 3D location of the vertex. */
	ccColor4F color;			/**< The color at the vertex. */
} CC3ColoredVertex;

/**
 * Defines a simple vertex, containing location, normal and color.
 * Useful for painting solid colors that interact with lighting conditions.
 */
typedef struct 
{
	CC3Vector location;			/**< The 3D location of the vertex. */
	CC3Vector normal;			/**< The 3D normal at the vertex. */
	ccColor4F color;			/**< The color at the vertex. */
} CC3LitColoredVertex;

/**
 * Defines a simple vertex, containing location, normal, and texture coordinate
 * data. Useful for interleaving vertex data for presentation to the GL engine.
 */
typedef struct 
{
	CC3Vector location;			/**< The 3D location of the vertex. */
	CC3Vector normal;			/**< The 3D normal at the vertex. */
	ccTex2F texCoord;			/**< The 2D coordinate of this vertex on the texture. */
} CC3TexturedVertex;


/**
 * Defines an axially-aligned-box or bounding-box (AABB), describing
 * a 3D volume by specifying the minimum and maximum 3D corners.
 */
typedef struct
{
	CC3Vector minimum;			/**< The minimum corner (bottom-left-rear). */
	CC3Vector maximum;			/**< The maximum corner (top-right-front). */
} CC3Box;

/** A CC3Box of zero origin and dimensions. */
static const CC3Box kCC3BoxZero = { CC3Vector::kCC3VectorZero, CC3Vector::kCC3VectorZero };

/** The null bounding box. It cannot be drawn, but is useful for marking an uninitialized bounding box. */
static const CC3Box kCC3BoxNull = { CC3Vector::kCC3VectorNull, CC3Vector::kCC3VectorNull };

/** Returns a CC3Box structure constructed from the min and max vertices. */
static inline CC3Box CC3BoxFromMinMax(CC3Vector minVtx, CC3Vector maxVtx) 
{
	CC3Box bb;
	bb.minimum = minVtx;
	bb.maximum = maxVtx;
	return bb;
}

/** Returns a CC3Box structure constructed from the min and max components. */
static inline CC3Box CC3BoxMake(GLfloat minX, GLfloat minY, GLfloat minZ,
												GLfloat maxX, GLfloat maxY, GLfloat maxZ) 
{
	return CC3BoxFromMinMax(CC3VectorMake(minX, minY, minZ),
									CC3VectorMake(maxX, maxY, maxZ));
}

/** Returns whether the two bounding boxes are equal by comparing their respective components. */
static inline bool CC3BoxesAreEqual(CC3Box bb1, CC3Box bb2) 
{
	return bb1.minimum.equals( bb2.minimum )
		&& bb1.maximum.equals( bb2.maximum );
}

/**
 * Returns whether the specified bounding box is equal to
 * the zero bounding box, specified by kCC3BoxZero.
 */
static inline bool CC3BoxIsZero(CC3Box bb) 
{
	return CC3BoxesAreEqual(bb, kCC3BoxZero);
}

/**
 * Returns whether the specified bounding box is equal to
 * the null bounding box, specified by kCC3BoxNull.
 */
static inline bool CC3BoxIsNull(CC3Box bb) 
{
	return CC3BoxesAreEqual(bb, kCC3BoxNull);
}

/** Returns the geometric center of the specified bounding box. */
static inline CC3Vector CC3BoxCenter(CC3Box bb) 
{
	return bb.minimum.average( bb.maximum );
}

/** Returns the size of the specified bounding box, between the minimum to maximum corners. */
static inline CC3Vector CC3BoxSize(CC3Box bb) 
{
	return bb.maximum.difference( bb.minimum );
}

/** Returns whether the specified bounding box contains the specified location. */
static inline bool CC3BoxContainsLocation(CC3Box bb, CC3Vector aLoc)
{
	if (aLoc.x > bb.maximum.x) return false;
	if (aLoc.x < bb.minimum.x) return false;
	if (aLoc.y > bb.maximum.y) return false;
	if (aLoc.y < bb.minimum.y) return false;
	if (aLoc.z > bb.maximum.z) return false;
	if (aLoc.z < bb.minimum.z) return false;
	return true;
}

/**
 * Returns the smallest CC3Box that contains both the specified bounding box
 * and location. If the specified bounding box is null, returns a bounding box of zero
 * size at the specified location.
 */
CC3Box CC3BoxEngulfLocation(CC3Box bb, CC3Vector aLoc);

/**
 * Returns the smallest CC3Box that contains the two specified bounding boxes.
 * If either bounding box is the null bounding box, simply returns the other bounding box
 * (which may also be the null bounding box).
 */
static inline CC3Box CC3BoxUnion(CC3Box bb1, CC3Box bb2) 
{
	if(CC3BoxIsNull(bb1)) return bb2;
	if(CC3BoxIsNull(bb2)) return bb1;
	
	bb1 = CC3BoxEngulfLocation(bb1, bb2.minimum);
	bb1 = CC3BoxEngulfLocation(bb1, bb2.maximum);
	return bb1;
}

/**
 * Returns a bounding box that has the same dimensions as the specified bounding box, but with
 * each corner expanded outward by the specified amount of padding.
 *
 * The padding value is added to the maximum vector, and subtracted from the minimum vector.
 */
static inline CC3Box CC3BoxAddPadding(CC3Box bb, CC3Vector padding) 
{
	CC3Box bbPadded;
	bbPadded.maximum = bb.maximum.add( padding );
	bbPadded.minimum = bb.minimum.difference( padding );
	return bbPadded;
}

/**
 * Returns a box that has the same dimensions as the specified  box, but with
 * each corner expanded outward by the specified amount of padding.
 *
 * The padding value is added to all three components of the maximum vector, and subtracted
 * from all three components of the minimum vector.
 */
static inline CC3Box CC3BoxAddUniformPadding(CC3Box bb, GLfloat padding) 
{
	return (padding != 0.0f) ? CC3BoxAddPadding(bb, cc3v(padding, padding, padding)) : bb;
}

/** Returns a box constructed by translating the specified box by the specified translation offset. */
static inline CC3Box CC3BoxTranslate(CC3Box bb, CC3Vector offset)
{
	CC3Box bbXltd;
	bbXltd.maximum = bb.maximum.add( offset );
	bbXltd.minimum = bb.minimum.add( offset );
	return bbXltd;
}

/**
 * Returns a box constructed by translating the specified box by a translation that is a
 * fraction of the box size, as determined by the specified offsetScale. For example, a
 * value of -0.2 in the X-component of the offsetScale, will move the box 20% of its
 * dimension in the X-axis, in the direction of the negative X-axis.
 */
static inline CC3Box CC3BoxTranslateFractionally(CC3Box bb, CC3Vector offsetScale) 
{
	return CC3BoxTranslate( bb, CC3BoxSize(bb).scale(offsetScale) );
}

/**
 * Returns a bounding box of the same size as the specified bounding box, and whose center
 * lies at the origin of the coordinate system. This effectivly moves the bounding box so
 * that its center is at the origin.
 */
static inline CC3Box CC3BoxMoveCenterToOrigin(CC3Box bb)
{
	return CC3BoxTranslate( bb, CC3BoxCenter(bb).negate() );
}

/**
 * Returns a bounding box constructed by scaling the specified bounding box by the specified
 * scale value. Scaling can be different along each axis of the box.
 *
 * This has the effect of multiplying each component of each of the vectors representing the
 * minimum and maximum corners of the box by the corresponding component in the scale vector.
 */
static inline CC3Box CC3BoxScale(CC3Box bb, CC3Vector scale) 
{
	CC3Box bbScaled;
	bbScaled.maximum = bb.maximum.scale( scale );
	bbScaled.minimum = bb.minimum.scale( scale );
	return bbScaled;
}

/**
 * Returns a bounding box constructed by scaling the specified bounding box by the specified
 * scale value. The same scaling is applied to each axis of the box.
 *
 * This has the effect of multiplying each component of each of the vectors representing the
 * minimum and maximum corners of the box by the scale value.
 */
static inline CC3Box CC3BoxScaleUniform(CC3Box bb, GLfloat scale) 
{
	CC3Box bbScaled;
	bbScaled.maximum = bb.maximum.scaleUniform( scale );
	bbScaled.minimum = bb.minimum.scaleUniform( scale );
	return bbScaled;
}

/**
 * Returns the location that the specified ray intersects the specified bounding box,
 * or returns kCC3VectorNull if the ray does not intersect the bounding box, or the
 * bounding box is behind the ray.
 *
 * The result takes into consideration the startLocation of the ray, and will return
 * kCC3VectorNull if the bounding box is behind the startLocation, even if the line
 * projecting back through the startLocation in the negative direction of the ray
 * intersects the bounding box.
 *
 * The ray may start inside the bounding box, and the exit location of the ray will be returned.
 */
CC3Vector CC3RayIntersectionWithBox(CC3Ray aRay, CC3Box bb);

/**
 * Returns the location that the specified ray intersects the specified bounding box,
 * on the side of the bounding box that has the specified normal, but only if the
 * intersection distance is less than the specified previous intersection location.
 *
 * The distance measurement is specified in the W component of the returned 4D vector.
 *
 * If the ray does not intersect the specified side of the bounding box, if the side
 * is behind the ray, or if the intersection distance is larger than that for the
 * specified previous intersection location is returned.
 *
 * This method first creates the plane on which the side exists, finds the intersection
 * of the ray and that plane, determines whether the intersection location is actually
 * within the bounding box, and then tests whether the intersection distance is less
 * than for the specified previous intersection.
 */
CC3Vector4 CC3RayIntersectionWithBoxSide(CC3Ray aRay, CC3Box bb, CC3Vector sideNormal, CC3Vector4 prevHit);


/**
 * An angle such as a heading or inclination.
 * Can be measured in degrees or radians and may be positive or negative.
 */ 
typedef GLfloat CC3Angle;

/** Specifies a vector using angular coordinate axes. Angles are measured in degrees or radians. */
typedef struct 
{
	CC3Angle heading;				/**< The horizontal heading. */
	CC3Angle inclination;			/**< The inclination from horizontal. */
	GLfloat radius;					/**< The radial distance. */
} CC3AngularVector;

/** Returns an CC3AngularVector structure constructed from the vector components. */
static inline CC3AngularVector CC3AngularVectorMake(GLfloat heading, GLfloat inclination, GLfloat radius) 
{
	CC3AngularVector av;
	av.heading = heading;
	av.inclination = inclination;	
	av.radius = radius;
	return av;
}

/**
 * Returns an CC3AngularVector providing the heading, inclination & radius of the specified CC3Vector.
 * Heading is measured in degrees, in the X-Z plane, clockwise from the negative Z-axis.
 * Inclination is measured in degrees, with up being in the positive-Y direction.
 */
static inline CC3AngularVector CC3AngularVectorFromVector(CC3Vector aCoord) 
{
	CC3AngularVector av;
	av.radius = aCoord.length();
	av.inclination = av.radius ? CC3RadToDeg(asinf(aCoord.y / av.radius)) : 0.0f;
	av.heading = CC3RadToDeg(atan2f(aCoord.x, -aCoord.z));
	return av;
}

/**
 * Returns a CC3Vector from the specified CC3AngularVector.
 * Heading is measured in degrees, in the X-Z plane, clockwise from the negative Z-axis.
 * Inclination is measured in degrees, with up being in the positive-Y direction.
 */
static inline CC3Vector CC3VectorFromAngularVector(CC3AngularVector av) 
{
	CC3Vector unitDir;
	
	// First, incline up the Y-axis from the negative Z-axis.
	GLfloat radInclination = CC3DegToRad(av.inclination);
	unitDir.y = sinf(radInclination);
	GLfloat xzLen = cosf(radInclination);
	
	// Now rotate around the Y-axis to the heading. The length of the projection of the direction
	// vector into the X-Z plane is the length of the projection onto the negative Z-axis after
	// the initial inclination. Use this length as the basis for calculating the X & Z CC3Vectors.
	// The result is a unit direction vector projected into all three axes.
	GLfloat radHeading = CC3DegToRad(av.heading);
	unitDir.x = xzLen * sinf(radHeading);
	unitDir.z = -xzLen * cosf(radHeading);
	return unitDir.scaleUniform( av.radius );
}

/**
 * Returns the difference between two CC3AngularVectors, by subtracting the corresponding heading,
 * inclination & radial components. Note that this is NOT true vector arithmetic, which would
 * yield a completely different angular and radial results.
 */
static inline CC3AngularVector CC3AngularVectorDifference(CC3AngularVector minuend, CC3AngularVector subtrahend) 
{
	CC3AngularVector difference;
	difference.heading = CC3SemiCyclicAngle(minuend.heading - subtrahend.heading);
	difference.inclination = minuend.inclination - subtrahend.inclination;
	difference.radius = minuend.radius - subtrahend.radius;
	return difference;
}


/** Defines a triangular face of the mesh, comprised of three vertices, stored in winding order. */
typedef struct 
{
	CC3Vector vertices[3];	/**< The vertices of the face, stored in winding order. */
} CC3Face;

/** A CC3Face with all vertices set to zero. */
static const CC3Face kCC3FaceZero = { CC3Vector::kCC3VectorZero, CC3Vector::kCC3VectorZero, CC3Vector::kCC3VectorZero };

/** 
 * Returns a CC3Face structure constructed from the three specified vectors,
 * which should be supplied in winding order.
 */
static inline CC3Face CC3FaceMake(CC3Vector v0, CC3Vector v1, CC3Vector v2) 
{
	CC3Face face;
	face.vertices[0] = v0;
	face.vertices[1] = v1;
	face.vertices[2] = v2;
	return face;
}

/**
 * Returns a CC3Face structure that has the same vertices
 * as the specified face, but in the opposite winding order.
 */
static inline CC3Face CC3FaceInvert(CC3Face face)
{
	return CC3FaceMake(face.vertices[0], face.vertices[2], face.vertices[1]);
}

/**
 * Returns the location of the center of the specified face, calculated
 * as the mathematical average of the three vertices that define the face.
 */
static inline CC3Vector CC3FaceCenter(CC3Face face) 
{
	CC3Vector* vtx = face.vertices;
	return cc3v((vtx[0].x + vtx[1].x + vtx[2].x) * kCC3OneThird,
				(vtx[0].y + vtx[1].y + vtx[2].y) * kCC3OneThird,
				(vtx[0].z + vtx[1].z + vtx[2].z) * kCC3OneThird);
}

/**
 * Returns a normalized normal vector derived from the location and winding order
 * of the three vertices in the specified face.
 *
 * The direction of the normal vector is affected by the winding order of the
 * vertices in the face. The vertices should wind vertex[0] -> vertex[1] -> vertex[2].
 * The normal will point in the direction that has the three points winding in a
 * counter-clockwise direction, according to a right-handed coordinate system.
 * If the direction of the normal is important, be sure the winding order of
 * the points in the face is correct.
 */
static inline CC3Vector CC3FaceNormal(CC3Face face) 
{
	return face.vertices[1].difference(face.vertices[0]).cross( face.vertices[2].difference(face.vertices[0]) ).normalize();
}

/** Defines the barycentric weights of the three vertices of a triangle, in the same order as the vertices in a CC3Face. */
typedef struct 
{
	GLfloat weights[3];		/**< The barycentric weights of the three vertices of the face. */
} CC3BarycentricWeights;

/** Returns a CC3BarycentricWeights structure constructed from the three specified weights. */
static inline CC3BarycentricWeights CC3BarycentricWeightsMake(GLfloat b0, GLfloat b1, GLfloat b2) 
{
	CC3BarycentricWeights bcw;
	bcw.weights[0] = b0;
	bcw.weights[1] = b1;
	bcw.weights[2] = b2;
	return bcw;
}

/**
 * Returns whether the specified barycentric weights indicate a location inside a triangle.
 *
 * To be inside a triangle, none of the weights may be negative. If any of the weights are negative,
 * the specified barycentric weights represent a location outside a triangle.
 */
static inline bool CC3BarycentricWeightsAreInsideTriangle(CC3BarycentricWeights bcw) 
{
	return bcw.weights[0] >= 0.0f && bcw.weights[1] >= 0.0f && bcw.weights[2] >= 0.0f;
}

/**
 * Returns the barycentric weights for the specified location on the plane of the specified face.
 * The returned weights are specified in the same order as the vertices of the face.
 * 
 * The specified location should be on the plane of the specified face. 
 *
 * If the location is on the plane of the specified face, the three returned weights will add up to one.
 * If all three of the returned weights are positive, then the location is inside the triangle
 * defined by the face, otherwise, at least one of the returned weights will be negative.
 */
CC3BarycentricWeights CC3FaceBarycentricWeights(CC3Face face, CC3Vector aLocation);

/**
 * Returns the 3D cartesian location on the specified face that corresponds to the specified
 * barycentric coordinate weights.
 */
static inline CC3Vector CC3FaceLocationFromBarycentricWeights(CC3Face face, CC3BarycentricWeights bcw) 
{
	CC3Vector* c = face.vertices;
	GLfloat* b = bcw.weights;
	CC3Vector v;
	v.x = b[0] * c[0].x + b[1] * c[1].x + b[2] * c[2].x;
	v.y = b[0] * c[0].y + b[1] * c[1].y + b[2] * c[2].y;
	v.z = b[0] * c[0].z + b[1] * c[1].z + b[2] * c[2].z;
	return v;
}

/**
 * Defines a triangular face of the mesh, comprised of three vertex indices,
 * each a GLuint, stored in winding order.
 */
typedef struct 
{
	GLuint vertices[3];	/**< The indices of the vertices of the face, stored in winding order. */
} CC3FaceIndices;

/** A CC3FaceIndices with all vertices set to zero. */
static const CC3FaceIndices kCC3FaceIndicesZero = { {0, 0, 0} };

/** 
 * Returns a CC3FaceIndices structure constructed from the three
 * specified vertex indices, which should be supplied in winding order.
 */
static inline CC3FaceIndices CC3FaceIndicesMake(GLuint i0, GLuint i1, GLuint i2) 
{
	CC3FaceIndices fi;
	fi.vertices[0] = i0;
	fi.vertices[1] = i1;
	fi.vertices[2] = i2;
	return fi;
}

/** The coefficients of the equation for a plane in 3D space (ax + by + cz + d = 0). */
typedef struct 
{
	GLfloat a;				/**< The a coefficient in the planar equation. */
	GLfloat b;				/**< The b coefficient in the planar equation. */
	GLfloat c;				/**< The c coefficient in the planar equation. */
	GLfloat d;				/**< The d coefficient in the planar equation. */
} CC3Plane;

/** An undefined plane. */
static const CC3Plane kCC3PlaneZero = { 0, 0, 0, 0 };

/** Returns a string description of the specified CC3Plane struct in the form "(a, b, c, d)" */
static inline std::string stringFromCC3Plane( const CC3Plane& p )
{
	return CC3String::stringWithFormat( (char*)"(%.3f, %.3f, %.3f, %.3f)", p.a, p.b, p.c, p.d );
}


/** Returns a CC3Plane structure constructed from the specified coefficients. */
static inline CC3Plane CC3PlaneMake(GLfloat a, GLfloat b, GLfloat c, GLfloat d) 
{
	CC3Plane p;
	p.a = a;
	p.b = b;
	p.c = c;
	p.d = d;
	return p;
}

/** Returns the normal of the plane, which is (a, b, c) from the planar equation. */
static inline CC3Vector CC3PlaneNormal(CC3Plane p) 
{
	return *(CC3Vector*)&p;
}

/** Returns a CC3Plane that has the specified normal and intersects the specified location. */
static inline CC3Plane CC3PlaneFromNormalAndLocation(CC3Vector n, CC3Vector loc)
{
	GLfloat d = -loc.dot( n );
	return CC3PlaneMake(n.x, n.y, n.z, d);
}

/**
 * Returns a CC3Plane structure that contains the specified locations.
 * 
 * The direction of the normal of the returned plane is dependent on the winding order
 * of the three locations. Winding is done in the order the locations are specified
 * (v1 -> v2 -> v3), and the normal will point in the direction that has the three
 * locations winding in a counter-clockwise direction, according to a right-handed
 * coordinate system. If the direction of the normal is important, be sure to specify
 * the three points in the appropriate order.
 */
static inline CC3Plane CC3PlaneFromLocations(CC3Vector v1, CC3Vector v2, CC3Vector v3) 
{
	CC3Vector n = v2.difference(v1).cross( v3.difference(v1) ).normalize();
	return CC3PlaneFromNormalAndLocation(n, v1);
}

/**
 * Returns a CC3Plane structure that contains the points in the specified face.
 * 
 * The direction of the normal of the returned plane is dependent on the winding order
 * of the face, which is taken to be vertex[0] -> vertex[1] -> vertex[2], and the normal
 * will point in the direction that has the three points winding in a counter-clockwise
 * direction, according to a right-handed coordinate system. If the direction of the
 * normal is important, be sure the winding order of the points in the face is correct.
 */
static inline CC3Plane CC3FacePlane(CC3Face face) 
{
	return CC3PlaneFromLocations(face.vertices[0], face.vertices[1], face.vertices[2]);
}

/** Returns whether the two planes are equal by comparing their respective components. */
static inline bool CC3PlanesAreEqual(CC3Plane p1, CC3Plane p2) 
{
	return p1.a == p2.a &&
		   p1.b == p2.b &&
		   p1.c == p2.c &&
		   p1.d == p2.d;
}

/** Returns whether the specified plane is equal to the zero plane, specified by kCC3PlaneZero. */
static inline bool CC3PlaneIsZero(CC3Plane p) { return CC3PlanesAreEqual(p, kCC3PlaneZero); }


/**
 * Returns a plane that is the negative of the specified plane in all dimensions, including D.
 *
 * The returned plane represents a plane that is coincident with the specified plane,
 * but whose normal points in the opposite direction.
 */
static inline CC3Plane CC3PlaneNegate(CC3Plane p) 
{
	return CC3PlaneMake(-p.a, -p.b, -p.c, -p.d);
}

/** Returns a normalized copy of the specified CC3Plane so that the length of its normal (a, b, c) is 1.0 */
static inline CC3Plane CC3PlaneNormalize(CC3Plane p) 
{
	GLfloat ooNormLen = 1.0f / CC3PlaneNormal(p).length();
	return CC3PlaneMake(p.a * ooNormLen,
						p.b * ooNormLen,
						p.c * ooNormLen,
						p.d * ooNormLen);
}

/**
 * Returns the distance from the specified location to the specified plane.
 *
 * The distance is returned in terms of the length of the normal. If the normal
 * is of unit length, then the distance is in absolute units.
 */
static inline GLfloat CC3DistanceFromPlane(CC3Vector v, CC3Plane p) 
{
	return v.dot( CC3PlaneNormal(p) ) + p.d;
}

/**
 * Returns whether the specified vector is in front of the specified normalized plane.
 *
 * If the vector is a location, being "in front" means the location is on the side of
 * the plane from which the plane normal points.
 *
 * If the vector is a direction, being "in front" means that the direction points away
 * from the plane on the same side of the plane as the normal points away from the plane.
 */
static inline bool CC3VectorIsInFrontOfPlane(CC3Vector v, CC3Plane p) 
{
	return (CC3DistanceFromPlane(v, p) > 0.0f);
}

/**
 * Returns whether the specified 4D homogeneous vector is in front of the
 * specified normalized plane.
 *
 * If the vector is a location (w = 1), being "in front" means the location
 * is on the side of the plane from which the plane normal points.
 *
 * If the vector is a direction (w = 0), being "in front" means that the
 * direction points away from the plane on the same side of the plane as
 * the normal points away from the plane.
 */
static inline bool CC3Vector4IsInFrontOfPlane(CC3Vector4 v, CC3Plane plane) 
{
	CC3Vector4& vec4 = *(CC3Vector4*)&plane;
	return vec4.dot( v ) > 0.0f;
}

/**
 * Returns the location of the point where the specified ray intersects the specified plane.
 *
 * The returned result is a 4D vector, where the x, y & z components give the intersection location
 * in 3D space, and the w component gives the distance from the startLocation of the ray to the
 * intersection location, in multiples of the ray direction vector. If this value is negative, the
 * intersection point is in the direction opposite to the direction of the ray.
 *
 * If the ray is parallel to the plane, no intersection occurs, and the returned 4D vector
 * will be equal to kCC3Vector4Null.
 */
CC3Vector4 CC3RayIntersectionWithPlane(CC3Ray ray, CC3Plane plane);

/**
 * Returns the instersection point of the three specified planes, or returns kCC3VectorNull
 * if the planes do not intersect at a single point, which can occur if the planes are
 * parallel, or if one plane is parallel to the line of intersection of the other two planes.
 */
CC3Vector CC3TriplePlaneIntersection(CC3Plane p1, CC3Plane p2, CC3Plane p3);


/** Defines a sphere. */
typedef struct 
{
	CC3Vector center;			/**< The center of the sphere. */
	GLfloat radius;				/**< The radius of the sphere */
} CC3Sphere;

/** Returns a CC3Spere constructed from the specified center and radius. */
static inline CC3Sphere CC3SphereMake(CC3Vector center, GLfloat radius) 
{
	CC3Sphere s;
	s.center = center;
	s.radius = radius;
	return s;
}

/** Returns a CC3Spere that circumscribes the specified box. */
static inline CC3Sphere CC3SphereFromCircumscribingBox(CC3Box box) 
{
	CC3Vector center = CC3BoxCenter(box);
	GLfloat radius = box.maximum.distance( center );
	return CC3SphereMake(center, radius);
}

/** Returns whether the specified location lies within the specified sphere. */
static inline bool CC3IsLocationWithinSphere(CC3Vector aLocation, CC3Sphere aSphere) 
{
	// Compare the squares of the distances to avoid taking an expensive square root. 
	GLfloat radiusSquared = aSphere.radius * aSphere.radius;
	return aLocation.distanceSquared( aSphere.center ) <= radiusSquared;
}

/** Returns whether the specified spheres intersect. */
static inline bool CC3DoesSphereIntersectSphere(CC3Sphere sphereOne, CC3Sphere sphereTwo) 
{
	// Expand the first sphere to have a radius equal to the sume of the two radii,
	// and test whether the center of the other sphere is inside the expanded sphere.
	CC3Sphere bigSphere = CC3SphereMake(sphereOne.center, (sphereOne.radius + sphereTwo.radius));
	return CC3IsLocationWithinSphere(sphereTwo.center, bigSphere);
}

/** Returns the smallest CC3Sphere that contains the two specified spheres. */
CC3Sphere CC3SphereUnion(CC3Sphere s1, CC3Sphere s2);

/** Returns whether the specified ray intersects the specified sphere. */
bool CC3DoesRayIntersectSphere(CC3Ray aRay, CC3Sphere aSphere);

/**
 * Returns the location that the specified ray intersects the specified sphere, or returns
 * kCC3VectorNull if the ray does not intersect the sphere, or the sphere is behind the ray.
 *
 * The result takes into consideration the startLocation of the ray, and will return
 * kCC3VectorNull if the sphere is behind the startLocation, even if the line projecting
 * back through the startLocation in the negative direction of the ray intersects the sphere.
 *
 * The ray may start inside the sphere, in which case, the returned location represents
 * the exit location of the ray.
 */
CC3Vector CC3RayIntersectionWithSphere(CC3Ray aRay, CC3Sphere aSphere);

/**
 * Returns the coefficients of the quadratic equation that describes the points of
 * intersection between the specified ray and sphere.
 *
 * Given the equation for a sphere at the origin:  x*x + y*y + z*z = r*r, and the
 * equation for a ray in the same frame of reference: p = s + tv, where s is the
 * ray start, v is the ray direction, and p is a point on the ray, we can solve for
 * the intersection points of the ray and sphere. The result is a quadratic equation
 * in t: at*t + bt + c = 0, where: a = v*v, b = 2(s.v), and c = s*s - r*r.
 *
 * The a, b and c elements of the returned CC3Plane structure contain the a, b and c
 * coefficients of the quadratic equation, respectively. The d element of the returned
 * CC3Plane structure contains the discriminant of the quadratic equation (d = b*b - 4ac).
 *
 * The returned quadratic coefficients are not a plane.
 * The CC3Plane structure is simply used for convenience.
 *
 * Reference: Mathematics for 3D Game Programming and Computer Graphics, 3rd ed. book, by Eric Lengyel
 */
CC3Plane CC3RaySphereIntersectionEquation(CC3Ray aRay, CC3Sphere aSphere);


/**
 * The constant, linear, and quadratic coefficients of the attenuation function equation:
 * (a + b*r + c*r*r), where r is the radial distance between a the source (light or camera)
 * and the 3D location at which we want to calculate attenuation.
 */
typedef struct 
{
	GLfloat a;				/**< The constant coefficient in the attenuation function. */
	GLfloat b;				/**< The linear coefficient in the attenuation function. */
	GLfloat c;				/**< The quadratic coefficient in the attenuation function. */
} CC3AttenuationCoefficients;

/** Attenuation coefficients corresponding to no attenuation with distance (constant size). */
static const CC3AttenuationCoefficients kCC3AttenuationNone = {1.0, 0.0, 0.0};

/** Returns a CC3AttenuationCoefficients structure constructed from the specified coefficients. */
static inline CC3AttenuationCoefficients CC3AttenuationCoefficientsMake(GLfloat constant,
																		GLfloat linear,
																		GLfloat quadratic) 
{
	CC3AttenuationCoefficients coeffs;
	coeffs.a = constant;
	coeffs.b = linear;
	coeffs.c = quadratic;
	return coeffs;
}

/** Returns whether the two attenuation coefficents are equal by comparing their respective components. */
static inline bool CC3AttenuationCoefficientsAreEqual(CC3AttenuationCoefficients ac1,
													  CC3AttenuationCoefficients ac2) 
{
	return (ac1.a == ac2.a) && (ac1.b == ac2.b) && (ac1.c == ac2.c);
}

/**
 * If at least one of the coefficients in the specified attenuation is not zero, the specified
 * attenuation coefficients is returned unchanged. However, if all three coefficients of the
 * specified attenuation coefficients are zero, the value is illegal (and will cause divide-by-zero
 * errors, especially in shaders). If that is the case, this function returns kCC3AttenuationNone.
 */
static inline CC3AttenuationCoefficients CC3AttenuationCoefficientsLegalize(CC3AttenuationCoefficients ac) 
{
	return (ac.a != 0.0f || ac.b != 0.0f || ac.c != 0.0f) ? ac : kCC3AttenuationNone;
}

/** 
 * A rectangle defining a GL viewport.
 *
 * This can also be used for any rectangular group of pixels.
 */
typedef union 
{
	struct {
		GLint x;				/**< The X-position of the bottom-left corner of the viewport. */
		GLint y;				/**< The Y-position of the bottom-left corner of the viewport. */
		GLsizei w;				/**< The width of the viewport. */
		GLsizei h;				/**< The height of the viewport. */
	};
	struct {
		CC3IntPoint origin;		/**< The origin of the viewport. */
		CC3IntSize size;		/**< The size of the viewport. */
	};
} CC3Viewport;

/** An empty or undefined viewport. */
static const CC3Viewport kCC3ViewportZero = { {0, 0, 0, 0} };


/** Returns a CC3Viewport structure constructed from the specified components. */
static inline CC3Viewport CC3ViewportMake(GLint x, GLint y, GLint w, GLint h) 
{
	CC3Viewport vp;
	vp.x = x;
	vp.y = y;
	vp.w = w;
	vp.h = h;
	return vp;
}

/** Returns a CC3Viewport constructed from the specified origin and size. */
static inline CC3Viewport CC3ViewportFromOriginAndSize(CC3IntPoint origin, CC3IntSize size) 
{
	CC3Viewport vp;
	vp.origin = origin;
	vp.size = size;
	return vp;
}

/** Returns whether the two viewports are equal by comparing their respective components. */
static inline bool CC3ViewportsAreEqual(CC3Viewport vp1, CC3Viewport vp2) 
{
	return (vp1.x == vp2.x) && (vp1.y == vp2.y) && (vp1.w == vp2.w) && (vp1.h == vp2.h);
}

/** Returns whether the specified viewport is equal to the zero viewport, specified by kCC3ViewportZero. */
static inline bool CC3ViewportIsZero(CC3Viewport vp) 
{
	return CC3ViewportsAreEqual(vp, kCC3ViewportZero);
}

/**
 * Returns whether the specified point lies within the specified viewport.
 * A point is considered inside the viewport if its coordinates lie inside
 * the viewport or on the minimum X or minimum Y edge.
 */
static inline bool CC3ViewportContainsPoint(CC3Viewport vp, CCPoint point) 
{
	return (point.x >= vp.x) && (point.x < vp.x + vp.w) &&
	(point.y >= vp.y) && (point.y < vp.y + vp.h);
}

/** Returns a CC3Viewport constructed from the specified CGRect. */
static inline CC3Viewport CC3ViewportFromCGRect(CCRect rect) 
{
	return CC3ViewportFromOriginAndSize(CC3IntPointFromCGPoint(rect.origin), CC3IntSizeFromCGSize(rect.size));
}

/** Returns a CCRect constructed from the specified viewport. */
static inline CCRect CGRectFromCC3Viewport(CC3Viewport vp) 
{
	return CCRectMake(vp.x, vp.y, vp.w, vp.h);
}

/** 
 * Returns a CC3Viewport of the same size as the specified viewport, but whose origin is 
 * translated from the origin of the specified viewport by the by the specified amount. 
 */
static inline CC3Viewport CC3ViewportTranslate(CC3Viewport vp, CC3IntPoint offset) 
{
	return CC3ViewportFromOriginAndSize(CC3IntPointAdd(vp.origin, offset), vp.size);
}


/** Returns a GLfloat between 0 and 1 converted from the specified GLubyte value between 0 and 255. */
static inline GLfloat CCColorFloatFromByte(GLubyte colorValue) 
{
	return (GLfloat)colorValue * kCC3OneOver255;
}

/**
 * Returns a GLubyte between 0 and 255 converted from the specified GLfloat value.
 *
 * The specified float value is clamped to between 0 and 1 before conversion, so that
 * the Glubyte does not overflow or underflow, which would create unexpected colors.
 */
static inline GLubyte CCColorByteFromFloat(GLfloat colorValue) 
{
	return (GLubyte)(CLAMP(colorValue, 0.0f, 1.0f) * 255.0f);
}

/**
 * Returns the luminosity of the specified color as calculated using the ITU-R BT.709
 * conversion for high-definition television (HDTV):
 *
 *   luminosity = (0.2126 * red) + (0.7152 * green) + (0.0722 * blue),
 */
static inline GLfloat CC3LuminosityBT709(GLfloat red, GLfloat green, GLfloat blue) 
{
	return (0.2126f * red) + (0.7152f * green) + (0.0722f * blue);
}

/**
 * Returns the luminosity of the specified color as calculated using the ITU-R BT.601
 * conversion for standard-definition television (SDTV):
 *
 *   luminosity = (0.299 * red) + (0.587 * green) + (0.114 * blue),
 */
static inline GLfloat CC3LuminosityBT601(GLfloat red, GLfloat green, GLfloat blue) 
{
	return (0.299f * red) + (0.587f * green) + (0.114f * blue);
}

/** Opaque Red */
static const ccColor4F kCCC4FRed = { 1.0, 0.0, 0.0, 1.0 };

/** Opaque Green */
static const ccColor4F kCCC4FGreen = { 0.0, 1.0, 0.0, 1.0 };

/** Opaque Blue */
static const ccColor4F kCCC4FBlue = { 0.0, 0.0, 1.0, 1.0 };

/** Opaque Cyan */
static const ccColor4F kCCC4FCyan = { 0.0, 1.0, 1.0, 1.0 };

/** Opaque Magenta */
static const ccColor4F kCCC4FMagenta = { 1.0, 0.0, 1.0, 1.0 };

/** Opaque Yellow */
static const ccColor4F kCCC4FYellow = { 1.0, 1.0, 0.0, 1.0 };

/** Opaque Orange */
static const ccColor4F kCCC4FOrange = { 1.0, 0.5, 0.0, 1.0 };

/** Opaque Light Gray */
static const ccColor4F kCCC4FLightGray = { 0.75, 0.75, 0.75, 1.0 };

/** Opaque Gray */
static const ccColor4F kCCC4FGray = { 0.5, 0.5, 0.5, 1.0 };

/** Opaque Dark Gray */
static const ccColor4F kCCC4FDarkGray = { 0.25, 0.25, 0.25, 1.0 };

/** Opaque White */
static const ccColor4F kCCC4FWhite = { 1.0, 1.0, 1.0, 1.0 };

/** Opaque Black */
static const ccColor4F kCCC4FBlack = { 0.0, 0.0, 0.0, 1.0 };

/** Transparent Black */
static const ccColor4F kCCC4FBlackTransparent = {0.0, 0.0, 0.0, 0.0};


#if COCOS2D_VERSION < 0x010100		// cocos2d 1.0 - exists in cocos2d versions above
/** Convenience alias macro to create ccColor4F with less keystrokes. */
#define ccc4f(R,G,B,A) CCC4FMake((R),(G),(B),(A))
#endif

/** Returns a ccColor4F structure constructed from the specified components */
static inline ccColor4F CCC4FMake(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) 
{
	ccColor4F color;
	color.r = red;
	color.g = green;
	color.b = blue;
	color.a = alpha;
	return color;
}

/** Returns a ccColor4F structure constructed from the specified ccColor4B */
static inline ccColor4F CCC4FFromCCC4B(ccColor4B byteColor) 
{
	return ccc4f(CCColorFloatFromByte(byteColor.r),
				 CCColorFloatFromByte(byteColor.g),
				 CCColorFloatFromByte(byteColor.b),
				 CCColorFloatFromByte(byteColor.a));
}

/** Returns a ccColor4F structure constructed from the specified ccColor3B and opacity. */
static inline ccColor4F CCC4FFromColorAndOpacity(ccColor3B byteColor, CCOpacity opacity) 
{
	return ccc4f(CCColorFloatFromByte(byteColor.r),
				 CCColorFloatFromByte(byteColor.g),
				 CCColorFloatFromByte(byteColor.b),
				 GLfloatFromCCOpacity(opacity));
}

/** Returns the intensity of the specified color, calculated as the arithmetic mean of the R, G & B components. */
static inline GLfloat CCC4FIntensity(ccColor4F color) 
{
	return (color.r + color.g + color.b) * kCC3OneThird;
}

/** Returns whether the two colors are equal by comparing their respective components. */
static inline bool CCC4FAreEqual(ccColor4F c1, ccColor4F c2) 
{
	return c1.r == c2.r && c1.g == c2.g && c1.b == c2.b && c1.a == c2.a;
}

/**
 * Returns the result of adding the two specified colors, by adding the corresponding components.
 * Each of the resulting color components is clamped to be between 0.0 and 1.0.
 * This can also be thought of as a translation of the first color by the second.
 */
static inline ccColor4F CCC4FAdd(ccColor4F rgba, ccColor4F translation) 
{
	return ccc4f(CLAMP(rgba.r + translation.r, 0.0f, 1.0f),
				 CLAMP(rgba.g + translation.g, 0.0f, 1.0f),
				 CLAMP(rgba.b + translation.b, 0.0f, 1.0f),
				 CLAMP(rgba.a + translation.a, 0.0f, 1.0f));
}

/**
 * Returns the difference between two colors, by subtracting the subtrahend from the minuend,
 * which is accomplished by subtracting each of the corresponding r,g, b, a components.
 * Each of the resulting color components is clamped to be between 0.0 and 1.0.
 */
static inline ccColor4F CCC4FDifference(ccColor4F minuend, ccColor4F subtrahend) 
{
	return ccc4f(CLAMP(minuend.r - subtrahend.r, 0.0f, 1.0f),
				 CLAMP(minuend.g - subtrahend.g, 0.0f, 1.0f),
				 CLAMP(minuend.b - subtrahend.b, 0.0f, 1.0f),
				 CLAMP(minuend.a - subtrahend.a, 0.0f, 1.0f));
}

/**
 * Returns a ccColor4F structure whose values are those of the specified original color,
 * where each color component has been translated by the specified offset.
 * Each of the resulting color components is clamped to be between 0.0 and 1.0.
 */
static inline ccColor4F CCC4FUniformTranslate(ccColor4F rgba, GLfloat offset) 
{
	return CCC4FAdd(rgba, ccc4f(offset, offset, offset, offset));
}

/**
 * Returns a ccColor4F structure whose values are those of the specified original color,
 * multiplied by the specified scaling factor.
 * Each of the resulting color components is clamped to be between 0.0 and 1.0.
 */
static inline ccColor4F CCC4FUniformScale(ccColor4F rgba, GLfloat scale) 
{
	return ccc4f(CLAMP(rgba.r * scale, 0.0f, 1.0f),
				 CLAMP(rgba.g * scale, 0.0f, 1.0f),
				 CLAMP(rgba.b * scale, 0.0f, 1.0f),
				 CLAMP(rgba.a * scale, 0.0f, 1.0f));
}

/**
 * Returns the result of modulating the specified colors, by multiplying the corresponding
 * components. Each of the resulting color components is clamped to be between 0.0 and 1.0.
 */
static inline ccColor4F CCC4FModulate(ccColor4F rgba, ccColor4F modulation) 
{
	return ccc4f(CLAMP(rgba.r * modulation.r, 0.0f, 1.0f),
				 CLAMP(rgba.g * modulation.g, 0.0f, 1.0f),
				 CLAMP(rgba.b * modulation.b, 0.0f, 1.0f),
				 CLAMP(rgba.a * modulation.a, 0.0f, 1.0f));
}

/**
 * Returns a ccColor4F structure whose values are a weighted average of the specified base color and
 * the blend color. The parameter blendWeight should be between zero and one. A value of zero will leave
 * the base color unchanged. A value of one will result in the blend being the same as the blend color.
 */
static inline ccColor4F CCC4FBlend(ccColor4F baseColor, ccColor4F blendColor, GLfloat blendWeight) 
{
	return ccc4f(CC3WeightedAverage(baseColor.r, blendColor.r, blendWeight),
				 CC3WeightedAverage(baseColor.g, blendColor.g, blendWeight),
				 CC3WeightedAverage(baseColor.b, blendColor.b, blendWeight),
				 CC3WeightedAverage(baseColor.a, blendColor.a, blendWeight));
}

/**
 * Returns a ccColor4F color whose R, G & B components are those of the specified color multiplied
 * by the alpha value of the specified color, clamping to the range between zero and one if needed.
 * The alpha value remains unchanged.
 * 
 * This function performs the same operation on the specified color that is known as pre-multiplied
 * alpha when applied to the texels of a texture.
 */
static inline ccColor4F CCC4FBlendAlpha(ccColor4F rgba) 
{
	return ccc4f(CLAMP(rgba.r * rgba.a, 0.0f, 1.0f),
				 CLAMP(rgba.g * rgba.a, 0.0f, 1.0f),
				 CLAMP(rgba.b * rgba.a, 0.0f, 1.0f),
				 rgba.a);
}

/**
 * Returns a random ccColor4F where each component value between the specified min inclusive and
 * the specified max exclusive. This can be useful when creating particle systems.
 */
static inline ccColor4F RandomCCC4FBetween(ccColor4F min, ccColor4F max) 
{
	return ccc4f(CC3RandomFloatBetween(min.r, max.r),
				 CC3RandomFloatBetween(min.g, max.g),
				 CC3RandomFloatBetween(min.b, max.b),
				 CC3RandomFloatBetween(min.a, max.a));
}

/** Returns whether the two colors are equal by comparing their respective components. */
static inline bool CCC4BAreEqual(ccColor4B c1, ccColor4B c2) 
{
	return c1.r == c2.r && c1.g == c2.g && c1.b == c2.b && c1.a == c2.a;
}

/** Returns a ccColor4B structure constructed from the specified ccColor4F */
static inline ccColor4B CCC4BFromCCC4F(ccColor4F floatColor) 
{
	return ccc4(CCColorByteFromFloat(floatColor.r),
				CCColorByteFromFloat(floatColor.g),
				CCColorByteFromFloat(floatColor.b),
				CCColorByteFromFloat(floatColor.a));
}

/** Returns a ccColor4B structure constructed from the specified ccColor3B and opacity. */
static inline ccColor4B CCC4BFromColorAndOpacity(ccColor3B byteColor, CCOpacity opacity) 
{
	return ccc4(byteColor.r, byteColor.g, byteColor.b, GLubyteFromCCOpacity(opacity));
}

/**
 * Returns a ccColor4B color whose R, G & B components are those of the specified color multiplied
 * by the alpha value of the specified color, clamping to the range between zero and 255 if needed.
 * The alpha value remains unchanged.
 * 
 * This function performs the same operation on the specified color that is known as pre-multiplied
 * alpha when applied to the texels of a texture.
 */
static inline ccColor4B CCC4BBlendAlpha(ccColor4B rgba) 
{
	GLfloat alpha = rgba.a * kCC3OneOver255;
	return ccc4((GLubyte)CLAMP(rgba.r * alpha, 0, 255),
				(GLubyte)CLAMP(rgba.g * alpha, 0, 255),
				(GLubyte)CLAMP(rgba.b * alpha, 0, 255),
				rgba.a);
}

/** Returns whether the two colors are equal by comparing their respective components. */
static inline bool CCC3BAreEqual(ccColor3B c1, ccColor3B c2) 
{
	return c1.r == c2.r && c1.g == c2.g && c1.b == c2.b;
}

/** Returns a ccColor3B structure constructed from the specified ccColor4F */
static inline ccColor3B CCC3BFromCCC4F(ccColor4F floatColor) 
{
	return ccc3(CCColorByteFromFloat(floatColor.r),
				CCColorByteFromFloat(floatColor.g),
				CCColorByteFromFloat(floatColor.b));
}

/** Returns a ccColor3B structure constructed from the specified ccColor4B */
static inline ccColor3B CCC3BFromCCC4B(ccColor4B color) { return *(ccColor3B*)&color; }

/**
 * Returns an ccColor3B structure whose values are a weighted average of the specified base color and
 * the blend color. The parameter blendWeight should be between zero and one. A value of zero will leave
 * the base color unchanged. A value of one will result in the blend being the same as the blend color.
 */
static inline ccColor3B CCC3BBlend(ccColor3B baseColor, ccColor3B blendColor, GLfloat blendWeight) 
{
	return ccc3((GLubyte)CC3WeightedAverage(baseColor.r, blendColor.r, blendWeight),
				(GLubyte)CC3WeightedAverage(baseColor.g, blendColor.g, blendWeight),
				(GLubyte)CC3WeightedAverage(baseColor.b, blendColor.b, blendWeight));
}

/** 
 * Returns whether the two boolean values are in equal state.
 *
 * This test will work if the two booleans each contain different non-zero content to indicate
 * logical YES, whereas a simple == comparison would give an erroneous result in that situation.
 */
static inline bool CC3BooleansAreEqual(bool b1, bool b2) { return (b1 && b2) || (!b1 && !b2); }


/** Returns whether the specified bit in the specified bitfield is set to one. */
static inline bool CC3IsBitSet(GLbitfield bits, GLuint bitIdx) 
{
	CCAssert(bitIdx < (8 * sizeof(bits)), "Bit index is too large for the bitfield.");
	GLbitfield marker = 1;
	marker <<= bitIdx;
	return (bits & marker) ? true : false;
}

/** Returns whether the specified bit in the specified bitfield is set to zero. */
static inline bool CC3IsBitClear(GLbitfield bits, GLuint bitIdx) { return !CC3IsBitSet(bits, bitIdx); }

/** 
 * Sets the specified bit in the specified bitfield is to one if the specified value is YES,
 * or to zero if the specified value is NO.
 */
static inline void CC3SetBit(GLbitfield* bits, GLuint bitIdx, bool val) 
{
	CCAssert(bitIdx < (8 * sizeof(bits)), "Bit index is too large for the bitfield.");
	GLbitfield marker = 1;
	marker <<= bitIdx;
	if (val)
		*bits |= marker;
	else
		*bits &= ~marker;
}

static inline std::string stringFromCCSize( const CCSize& size )
{
	return CC3String::stringWithFormat( (char*)"(%.2f, %.2f)", size.width, size.height );
}

/** Returns a string description of the specified ccColor4F in the form "(r, g, b, a)" */
static inline std::string stringFromCCC4F(ccColor4F rgba) 
{
	return CC3String::stringWithFormat( (char*)"(%.3f, %.3f, %.3f, %.3f)", rgba.r, rgba.g, rgba.b, rgba.a );
}

/**
 * Reverses the order of the rows in the specified data block.
 * The transformation is performed in-place.
 *
 * This function can be useful for vertically flipping the contents of a texture or image,
 * which is a common requirement when transferring content data between an OpenGL texture
 * and an iOS or OSX image. Or it may be used to reverse the order of the rows of any
 * row-major memory structure.
 *
 * The specified data block is assumed to be in row-major order, containing the specified
 * number of rows, and with the specified number of bytes in each row. The total number of
 * bytes in the data block must be at least (bytesPerRow * rowCount).
 */
void CC3FlipVertically(GLubyte* rowMajorData, GLuint rowCount, GLuint bytesPerRow);

/** Temporarily turn off compiler warnings for hidden variable shadowing. */
#define CC3_PUSH_NOSHADOW	_Pragma("GCC diagnostic push") _Pragma("GCC diagnostic ignored \"-Wshadow\"")

/** Resume compiler warnings for hidden variable shadowing. */
#define CC3_POP_NOSHADOW	_Pragma("GCC diagnostic pop")

/** Temporarily turn off compiler warnings of unimplemented selectors. */
#define CC3_PUSH_NOSELECTOR	_Pragma("GCC diagnostic push") _Pragma("GCC diagnostic ignored \"-Wselector\"")

/** Resume compiler warnings of unimplemented selectors. */
#define CC3_POP_NOSELECTOR	_Pragma("GCC diagnostic pop")

const static CCRect CCRectNull = CCRectMake( INFINITY, INFINITY, 0, 0 );
static inline CCRect CCRectUnion( const CCRect& a, const CCRect& b )
{
	if ( a.equals( CCRectNull ) )
		return b;

	if ( b.equals( CCRectNull ) )
		return a;

	float minX = MIN( a.getMinX(), b.getMaxX() );
	float minY = MIN( a.getMaxY(), b.getMinY() );
	float maxX = MAX( a.getMaxX(), b.getMaxX() );
	float maxY = MAX( a.getMaxY(), b.getMaxY() );

	return CCRectMake( minX, minY, maxX-minX, maxY-minY);
}

NS_COCOS3D_END

#endif
