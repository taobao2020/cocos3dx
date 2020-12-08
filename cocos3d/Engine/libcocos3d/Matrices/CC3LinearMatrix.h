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
#ifndef _CCL_CC3LINEARMATRIX_H_
#define _CCL_CC3LINEARMATRIX_H_

NS_COCOS3D_BEGIN

/**
 * CC3LinearMatrix is a 3x3 matrix that can represent 3D linear transforms such as rotation,
 * scaling, reflection and shearing. Matrices of this class cannot represent 3D translation.
 *
 * Internally, the dimensions of this matrix are three columns by three rows. Data is held in a
 * CC3Matrix3x3 structure of 9 GLfloat elements in column-major order. For situations requiring
 * only 3D linear transformations, this offers a storage savings over using a full 4x4 matrix.
 */
class CC3LinearMatrix : public CC3Matrix 
{
	DECLARE_SUPER( CC3Matrix );

public:
	CC3LinearMatrix();

	static CC3LinearMatrix*	matrix();

	virtual void			transpose();
	virtual CC3Vector		extractRotation();
	virtual CC3Quaternion	extractQuaternion();
	virtual CC3Vector		extractForwardDirection();
	virtual CC3Vector		extractUpDirection();
	virtual CC3Vector		extractRightDirection();
	virtual CC3Vector		extractTranslation();
	virtual bool			invertAdjoint();
	virtual void			invertRigid();
	//virtual void			populateFromCC3Matrix3x3( CC3Matrix3x3* mtx );
	virtual void			populateCC3Matrix3x3( CC3Matrix3x3* mtx );
	//virtual void			populateFromCC3Matrix4x3( CC3Matrix4x3* mtx );
	virtual void			populateCC3Matrix4x3( CC3Matrix4x3* mtx );
	virtual void			populateCC3Matrix4x4( CC3Matrix4x4* mtx );
	virtual void			orthonormalizeRotationStartingWith( unsigned int startColNum );
	virtual void			multiplyIntoCC3Matrix3x3( CC3Matrix3x3* mtx );
	virtual void			multiplyByCC3Matrix3x3( CC3Matrix3x3* mtx );
	virtual void			multiplyIntoCC3Matrix4x3( CC3Matrix4x3* mtx );
	virtual void			multiplyByCC3Matrix4x3( CC3Matrix4x3* mtx );
	virtual void			multiplyIntoCC3Matrix4x4( CC3Matrix4x4* mtx );
	virtual void			multiplyByCC3Matrix4x4( CC3Matrix4x4* mtx );
	virtual void			leftMultiplyIntoCC3Matrix3x3( CC3Matrix3x3* mtx );
	virtual void			leftMultiplyByCC3Matrix3x3( CC3Matrix3x3* mtx );
	virtual void			leftMultiplyIntoCC3Matrix4x3( CC3Matrix4x3* mtx );
	virtual void			leftMultiplyByCC3Matrix4x3( CC3Matrix4x3* mtx );
	virtual void			leftMultiplyIntoCC3Matrix4x4( CC3Matrix4x4* mtx );
	virtual void			leftMultiplyByCC3Matrix4x4( CC3Matrix4x4* mtx );
	virtual CC3Vector		transformLocation( const CC3Vector& aLocation );
	virtual CC3Vector		transformDirection( const CC3Vector& aDirection );
	virtual CC3Vector4		transformHomogeneousVector( const CC3Vector4& aVector );

protected:
	virtual void			implPopulateZero();
	virtual void			implPopulateIdentity();
	virtual void			implPopulateFrom( CC3Matrix* aMatrix );
	virtual void			implPopulateFromCC3Matrix3x3( CC3Matrix3x3* mtx );
	virtual void			implPopulateFromCC3Matrix4x3( CC3Matrix4x3* mtx );
	virtual void			implPopulateFromCC3Matrix4x4( CC3Matrix4x4* mtx );
	virtual void			implPopulateFromRotation( const CC3Vector& aRotation );
	virtual void			implPopulateFromQuaternion( const CC3Quaternion& aQuaternion );
	virtual void			implPopulateFromScale( const CC3Vector& aScale );

	virtual void			implPopulateFromTranslation( const CC3Vector& aTranslation );
	virtual void			implPopulateToPointTowards( const CC3Vector& fwdDirection, const CC3Vector& upDirection );
	//virtual void			implPopulateFromFrustumLeft( GLfloat left, GLfloat right, GLfloat top, GLfloat bottom, GLfloat nearval, GLfloat farval );
	//virtual void			implPopulateFromFrustumLeft( GLfloat left, GLfloat right, GLfloat top, GLfloat bottom, GLfloat nearval );
	//virtual void			implPopulateOrthoFromFrustumLeft(GLfloat left, GLfloat right, GLfloat top, GLfloat bottom, GLfloat nearval, GLfloat farval );
	//virtual void			implPopulateOrthoFromFrustumLeft(GLfloat left, GLfloat right, GLfloat top, GLfloat bottom, GLfloat nearval );
	virtual void			implRotateBy( const CC3Vector& aRotation );
	virtual void			implRotateByQuaternion( const CC3Quaternion& aQuaternion );
	virtual void			implScaleBy( const CC3Vector& aScale );
	virtual void			implTranslateBy( const CC3Vector& aTranslation );
	virtual void			implMultiplyBy( CC3Matrix* aMatrix );
	virtual void			implLeftMultiplyBy( CC3Matrix* aMatrix );

protected:
	CC3Matrix3x3			_contents;
};

NS_COCOS3D_END

#endif
