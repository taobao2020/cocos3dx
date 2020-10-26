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
#include "CC3RenderSurfaces.h"
#include "../OpenGL/CC3OpenGL.h"
#include "../Utility/CC3CC2Extensions.h"
#include "../Scenes/CC3Scene.h"
#include "../Utility/CC3Backgrounder.h"
#include "../Platforms/CC3OSExtensions.h"
#include "../cc3Helper/Logging.h"

NS_COCOS3D_BEGIN

CC3GLRenderbuffer::CC3GLRenderbuffer()
{
	_rbID = 0;
}

CC3GLRenderbuffer::~CC3GLRenderbuffer()
{
	deleteGLRenderbuffer();
}

GLuint CC3GLRenderbuffer::getRenderbufferID()
{
	ensureGLRenderbuffer();
	return _rbID;
}

void CC3GLRenderbuffer::ensureGLRenderbuffer()
{
	if (_isManagingGL && !_rbID) 
		_rbID = CC3OpenGL::sharedGL()->generateRenderbuffer();
}

void CC3GLRenderbuffer::deleteGLRenderbuffer()
{
	if (_isManagingGL && _rbID) 
		CC3OpenGL::sharedGL()->deleteRenderbuffer( _rbID );
	_rbID = 0;
}

/** If the renderbuffer has been created, set its debug label as well. */
void CC3GLRenderbuffer::setName( const std::string& name )
{
	super::setName( name );
	if (!name.empty() && _rbID) 
		CC3OpenGL::sharedGL()->setRenderBufferDebugLabel( name.c_str(), _rbID );
}

CC3IntSize CC3GLRenderbuffer::getSize()
{
	return _size; 
}

void CC3GLRenderbuffer::setSize( const CC3IntSize& size )
{
	if ( CC3IntSizesAreEqual(size, _size) ) 
		return;

	_size = size;

	if (isManagingGL()) 
	{
		CC3OpenGL::sharedGL()->allocateStorageForRenderbuffer( getRenderbufferID(), _size, _format, _samples );
	}
}

void CC3GLRenderbuffer::replacePixels( const CC3Viewport& rect, ccColor4B* colorArray )
{
	CCAssert(false, "CC3GLRenderbuffer does not support replaing pixel content.");
}

void CC3GLRenderbuffer::bind()
{
	CC3OpenGL::sharedGL()->bindRenderbuffer( getRenderbufferID() ); 
}

void CC3GLRenderbuffer::bindToFramebuffer( CC3GLFramebuffer* framebuffer, GLenum attachment )
{
	CC3OpenGL::sharedGL()->bindRenderbuffer( getRenderbufferID(), framebuffer->getFramebufferID(), attachment );
}

void CC3GLRenderbuffer::unbindFromFramebuffer( CC3GLFramebuffer* framebuffer, GLenum attachment )
{
	CC3OpenGL::sharedGL()->bindRenderbuffer( 0, framebuffer->getFramebufferID(), attachment );
}

void CC3GLRenderbuffer::deriveNameFromFramebuffer( CC3GLFramebuffer* framebuffer, GLenum attachment )
{
	if( m_sName.empty() ) 
		setName( CC3FramebufferAttachmentName(framebuffer, attachment) );
}

void CC3GLRenderbuffer::initWithTag( GLuint tag, const std::string& name )
{
	super::initWithTag( tag, name );
	_rbID = 0;
	_size = CC3IntSizeMake(0, 0);
	_format = GL_ZERO;
	_samples = 1;
	_isManagingGL = true;
}

CC3GLRenderbuffer* CC3GLRenderbuffer::renderbuffer()
{
	CC3GLRenderbuffer* renderBuf = new CC3GLRenderbuffer;
	renderBuf->autorelease();
	
	return renderBuf;
}

void CC3GLRenderbuffer::initWithPixelFormat( GLenum format )
{
	return initWithPixelFormatAndSampls( format, 1 );
}

CC3GLRenderbuffer* CC3GLRenderbuffer::renderbufferWithPixelFormat( GLenum format )
{
	CC3GLRenderbuffer* renderBuf = new CC3GLRenderbuffer;
	renderBuf->initWithPixelFormat( format );
	renderBuf->autorelease();
	
	return renderBuf;
}

void CC3GLRenderbuffer::initWithPixelFormatAndSampls( GLenum format, GLuint samples )
{
	init();
	_format = format;
	_samples = samples;
}

CC3GLRenderbuffer* CC3GLRenderbuffer::renderbufferWithPixelFormatAndSamples( GLenum format, GLuint samples )
{
	CC3GLRenderbuffer* renderBuf = new CC3GLRenderbuffer;
	renderBuf->initWithPixelFormatAndSampls( format, samples );
	renderBuf->autorelease();
	
	return renderBuf;
}

void CC3GLRenderbuffer::initWithPixelFormatAndRenderBuffer( GLenum format, GLuint rbID )
{
	return initWithPixelFormat( format, 1, rbID );
}

CC3GLRenderbuffer* CC3GLRenderbuffer::renderbufferWithPixelFormatAndRenderBuffer( GLenum format, GLuint rbID )
{
	CC3GLRenderbuffer* renderBuf = new CC3GLRenderbuffer;
	renderBuf->initWithPixelFormatAndRenderBuffer( format, rbID );
	renderBuf->autorelease();

	return renderBuf;
}

void CC3GLRenderbuffer::initWithPixelFormat( GLenum format, GLuint samples, GLuint rbID )
{
	initWithPixelFormatAndSampls( format, samples );
	_rbID = rbID;
	_isManagingGL = false;
}

CC3GLRenderbuffer* CC3GLRenderbuffer::renderbufferWithPixelFormat( GLenum format, GLuint samples, GLuint rbID )
{
	CC3GLRenderbuffer* renderBuf = new CC3GLRenderbuffer;
	renderBuf->initWithPixelFormat( format, samples, rbID );
	renderBuf->autorelease();

	return renderBuf;
}

GLuint CC3GLRenderbuffer::getPixelSamples()
{
	return _samples;
}

GLuint CC3GLRenderbuffer::getPixelFormat()
{
	return _format;
}

bool CC3GLRenderbuffer::isManagingGL()
{
	return _isManagingGL;
}

CC3TextureFramebufferAttachment::CC3TextureFramebufferAttachment()
{
	_texObj = NULL;
}

CC3TextureFramebufferAttachment::~CC3TextureFramebufferAttachment()
{
	CC_SAFE_RELEASE( _texObj );
}

CC3Texture* CC3TextureFramebufferAttachment::getTexture()
{
	return (CC3Texture*)_texObj;
}

void CC3TextureFramebufferAttachment::setTexture( CC3Texture* texture )
{
	if (texture == getTexture())
		return;

	if ( texture )
	{
		texture->setHorizontalWrappingFunction( GL_CLAMP_TO_EDGE );
		texture->setVerticalWrappingFunction( GL_CLAMP_TO_EDGE );
	}
	
	setTexObj( texture );
}

bool CC3TextureFramebufferAttachment::shouldUseStrongReferenceToTexture()
{
	return _shouldUseStrongReferenceToTexture; 
}

void CC3TextureFramebufferAttachment::setShouldUseStrongReferenceToTexture( bool shouldUseStrongRef )
{
	if (shouldUseStrongRef == _shouldUseStrongReferenceToTexture) 
		return;
	_shouldUseStrongReferenceToTexture = shouldUseStrongRef;
	setTexObj( getTexture() );		// Update the reference type of the texture
}

/**
 * Sets the _texObj instance variable from the specified texture.
 *
 * If the value of the shouldUseStrongReferenceToTexture property is YES, the texture
 * is held directly in the strongly referenced _texObj iVar. If the value of the
 * shouldUseStrongReferenceToTexture property is NO, the texture is first wrapped in
 * a weak reference, which is then assigned to the strongly referenced _texObj iVar.
 */
void CC3TextureFramebufferAttachment::setTexObj( CC3Texture* texture )
{
	CCObject* newTexObj = texture;
	if (newTexObj == _texObj) 
		return;
	
	CC_SAFE_RELEASE( _texObj );
	_texObj = newTexObj;
	CC_SAFE_RETAIN( newTexObj );
}

CC3IntSize CC3TextureFramebufferAttachment::getSize()
{
	return getTexture()->getSize(); 
}

void CC3TextureFramebufferAttachment::setSize( const CC3IntSize& size )
{ 
	getTexture()->resizeTo( size ); 
}

GLenum CC3TextureFramebufferAttachment::getPixelFormat()
{
	return getTexture()->getPixelFormat(); 
}

void CC3TextureFramebufferAttachment::bindToFramebuffer( CC3GLFramebuffer* framebuffer, GLenum attachment )
{
	CC3OpenGL::sharedGL()->bindTexture2D( getTexture()->getTextureID(), _face, _mipmapLevel, framebuffer->getFramebufferID(), attachment );
}

void CC3TextureFramebufferAttachment::unbindFromFramebuffer( CC3GLFramebuffer* framebuffer, GLenum attachment )
{
	CC3OpenGL::sharedGL()->bindTexture2D( 0, _face, _mipmapLevel, framebuffer->getFramebufferID(), attachment );
}

/** Only update the texture if it has not already been given a name, and if the framebuffer does have a name. */
void CC3TextureFramebufferAttachment::deriveNameFromFramebuffer( CC3GLFramebuffer* framebuffer, GLenum attachment )
{
	CC3Texture* tex = getTexture();
	if ( tex->getName().empty() ) 
		tex->setName( CC3FramebufferAttachmentName(framebuffer, attachment).c_str() );
}

void CC3TextureFramebufferAttachment::replacePixels( const CC3Viewport& rect, ccColor4B* colorArray )
{
	getTexture()->replacePixels( rect, _face, colorArray );
}

bool CC3TextureFramebufferAttachment::init()
{
	initWithTexture( NULL ); 
	return true;
}

CC3TextureFramebufferAttachment* CC3TextureFramebufferAttachment::attachment()
{
	CC3TextureFramebufferAttachment* pAtta = new CC3TextureFramebufferAttachment;
	pAtta->init();
	pAtta->autorelease();

	return pAtta;
}

void CC3TextureFramebufferAttachment::initWithTexture( CC3Texture* texture )
{
	initWithTexture( texture, texture->getInitialAttachmentFace() );
}

CC3TextureFramebufferAttachment* CC3TextureFramebufferAttachment::attachmentWithTexture( CC3Texture* texture )
{
	CC3TextureFramebufferAttachment* pAtta = new CC3TextureFramebufferAttachment;
	pAtta->initWithTexture( texture );
	pAtta->autorelease();

	return pAtta;
}

void CC3TextureFramebufferAttachment::initWithTexture( CC3Texture* texture, GLenum face )
{
	initWithTexture( texture, face, 0 );
}

CC3TextureFramebufferAttachment* CC3TextureFramebufferAttachment::attachmentWithTexture( CC3Texture* texture, GLenum face )
{
	CC3TextureFramebufferAttachment* pAtta = new CC3TextureFramebufferAttachment;
	pAtta->initWithTexture( texture, face );
	pAtta->autorelease();

	return pAtta;
}

void CC3TextureFramebufferAttachment::initWithTexture( CC3Texture* texture, GLenum face, GLint mipmapLevel )
{
	super::init();
	_face = face;
	_mipmapLevel = mipmapLevel;
	_shouldUseStrongReferenceToTexture = true;
	setTexture( texture );
}

CC3TextureFramebufferAttachment* CC3TextureFramebufferAttachment::attachmentWithTexture( CC3Texture* texture, GLenum face, GLint mipmapLevel )
{
	CC3TextureFramebufferAttachment* pAtta = new CC3TextureFramebufferAttachment;
	pAtta->initWithTexture( texture, face, mipmapLevel );
	pAtta->autorelease();

	return pAtta;
}

void CC3TextureFramebufferAttachment::setMipmapLevel( GLint miplevel )
{
	_mipmapLevel = miplevel;
}

GLint CC3TextureFramebufferAttachment::getMipmapLevel()
{
	return _mipmapLevel;
}

void CC3TextureFramebufferAttachment::setFace( GLenum face )
{
	_face = face;
}

GLenum CC3TextureFramebufferAttachment::getFace()
{
	return _face;
}

CC3SurfaceSection::CC3SurfaceSection()
{
	_baseSurface = NULL;
}

CC3SurfaceSection::~CC3SurfaceSection()
{
	CC_SAFE_RELEASE( _baseSurface );
}

CC3RenderSurface* CC3SurfaceSection::getBaseSurface()
{
	return _baseSurface; 
}

void CC3SurfaceSection::setBaseSurface( CC3RenderSurface* baseSurface )
{
	if ( baseSurface == _baseSurface ) 
		return;
	
	CC_SAFE_RELEASE( _baseSurface );
	_baseSurface = baseSurface;
	CC_SAFE_RETAIN( baseSurface );
	
	if ( CC3IntSizeIsZero(_size) && _baseSurface ) 
		setSize( _baseSurface->getSize() );
	
	checkCoverage();
}

CC3IntSize CC3SurfaceSection::getSize()
{
	return _size; 
}

void CC3SurfaceSection::setSize( const CC3IntSize& size )
{
	_size = size;
	checkCoverage();
}

CC3IntPoint CC3SurfaceSection::getOrigin()
{
	return _origin; 
}

void CC3SurfaceSection::setOrigin( const CC3IntPoint& origin )
{
	_origin = origin;
	checkCoverage();
}

/** Checks if this surface section covers the entire base surface. */
void CC3SurfaceSection::checkCoverage()
{
	_isFullCoverage = (CC3IntPointIsZero(getOrigin()) &&
					   CC3IntSizesAreEqual(getSize(), _baseSurface->getSize()));
}

bool CC3SurfaceSection::isFullCoverage()
{
	return _isFullCoverage && _baseSurface->isFullCoverage(); 
}

CC3Viewport CC3SurfaceSection::getViewport()
{
	return CC3ViewportFromOriginAndSize(getOrigin(), getSize()); 
}

bool CC3SurfaceSection::isOnScreen()
{
	return _baseSurface->isOnScreen(); 
}

void CC3SurfaceSection::setIsOnScreen( bool isOnScreen )
{

}

CC3RenderSurfaceAttachment* CC3SurfaceSection::getColorAttachment()
{
	return _baseSurface->getColorAttachment(); 
}

void CC3SurfaceSection::setColorAttachment( CC3RenderSurfaceAttachment* colorAttachment )
{

}

CC3RenderSurfaceAttachment* CC3SurfaceSection::getDepthAttachment()
{
	return _baseSurface->getDepthAttachment(); 
}

void CC3SurfaceSection::setDepthAttachment( CC3RenderSurfaceAttachment* depthAttachment )
{
}

CC3RenderSurfaceAttachment* CC3SurfaceSection::getStencilAttachment()
{
	return _baseSurface->getStencilAttachment(); 
}

void CC3SurfaceSection::setStencilAttachment( CC3RenderSurfaceAttachment* stencilAttachment )
{

}


void CC3SurfaceSection::clearColorContent()
{
	openScissors();
	_baseSurface->clearColorContent();
	closeScissors();
}

void CC3SurfaceSection::clearDepthContent()
{
	openScissors();
	_baseSurface->clearDepthContent();
	closeScissors();
}

void CC3SurfaceSection::clearStencilContent()
{
	openScissors();
	_baseSurface->clearStencilContent();
	closeScissors();
}

void CC3SurfaceSection::clearColorAndDepthContent()
{
	openScissors();
	_baseSurface->clearColorAndDepthContent();
	closeScissors();
}

void CC3SurfaceSection::openScissors()
{
	bool shouldClip = !isFullCoverage();
	CC3OpenGL* gl = CC3OpenGL::sharedGL();
	gl->enableScissorTest( shouldClip );
	if (shouldClip) 
		gl->setScissor( getViewport() );
}

void CC3SurfaceSection::closeScissors()
{
	CC3OpenGL::sharedGL()->enableScissorTest( false ); 
}

void CC3SurfaceSection::readColorContentFrom( const CC3Viewport& rect, ccColor4B* colorArray )
{
	_baseSurface->readColorContentFrom( transformRect( rect ), colorArray );
}

void CC3SurfaceSection::replaceColorPixels( const CC3Viewport& rect, ccColor4B* colorArray )
{
	_baseSurface->replaceColorPixels( transformRect( rect ), colorArray );
}

/** Offsets the specified rectangle by the value of origin property. */
CC3Viewport CC3SurfaceSection::transformRect( const CC3Viewport& rect )
{
	return CC3ViewportTranslate(rect, _origin); 
}

void CC3SurfaceSection::activate()
{
	_baseSurface->activate();	
	openScissors();
}

void CC3SurfaceSection::initWithTag( GLuint tag, const std::string& aName )
{
	super::initWithTag( tag, aName );
	_baseSurface = NULL;
	_origin = kCC3IntPointZero;
	_size = kCC3IntSizeZero;
	_isFullCoverage = false;
}

void CC3SurfaceSection::initOnSurface( CC3RenderSurface* baseSurface )
{
	super::init();
	setBaseSurface( baseSurface );
}

CC3SurfaceSection* CC3SurfaceSection::surfaceOnSurface( CC3RenderSurface* baseSurface )
{
	CC3SurfaceSection* pSurface = new CC3SurfaceSection;
	pSurface->initOnSurface( baseSurface );
	pSurface->autorelease();
	
	return pSurface;
}

CC3GLFramebuffer::CC3GLFramebuffer()
{
	_fbID = 0;
	_size = CC3IntSizeMake(0, 0);
	_isManagingGL = true;
	_shouldBindGLAttachments = true;
	_isOnScreen = false;
	_colorAttachment = NULL;
	_depthAttachment = NULL;
	_stencilAttachment = NULL;
	_glLabelWasSet = false;
}

CC3GLFramebuffer::~CC3GLFramebuffer()
{
	deleteGLFramebuffer();
	CC_SAFE_RELEASE( _colorAttachment );
	CC_SAFE_RELEASE( _depthAttachment );
	CC_SAFE_RELEASE( _stencilAttachment );
}

GLuint CC3GLFramebuffer::getFramebufferID()
{
	ensureGLFramebuffer();
	return _fbID;
}

void CC3GLFramebuffer::ensureGLFramebuffer()
{
	if (_isManagingGL && !_fbID) 
		_fbID = CC3OpenGL::sharedGL()->generateFramebuffer();
}

void CC3GLFramebuffer::deleteGLFramebuffer()
{
	if (_isManagingGL && _fbID) 
		CC3OpenGL::sharedGL()->deleteFramebuffer( _fbID );
	_fbID = 0;
}

bool CC3GLFramebuffer::shouldBindGLAttachments()
{
	return _shouldBindGLAttachments;
}

void CC3GLFramebuffer::setShouldBindGLAttachments( bool shouldBind )
{
	_shouldBindGLAttachments = shouldBind;
}

void CC3GLFramebuffer::setName( const std::string& name )
{
	super::setName( name );
	if (!name.empty() && _fbID) 
		CC3OpenGL::sharedGL()->setFrameBufferDebugLabel( name.c_str(), _fbID );

	if ( _colorAttachment )
		_colorAttachment->deriveNameFromFramebuffer( this, GL_COLOR_ATTACHMENT0 );
	if ( _depthAttachment )
		_depthAttachment->deriveNameFromFramebuffer( this, GL_DEPTH_ATTACHMENT );
	if ( _stencilAttachment )
		_stencilAttachment->deriveNameFromFramebuffer( this, GL_STENCIL_ATTACHMENT );
}

CC3IntSize CC3GLFramebuffer::getSize()
{
	return _size; 
}

void CC3GLFramebuffer::setSize( const CC3IntSize& size )
{
	if ( CC3IntSizesAreEqual(size, _size) ) 
		return;
	
	_size = size;

	// Set the size of each attachment. After changing the size, we rebind each attachment because
	// texture attachments require the texture to be the correct size at the time of binding, and
	// changing the size of the texture itself is not enough.
	if ( _colorAttachment )
	{
		_colorAttachment->setSize( size );
		bind( _colorAttachment, GL_COLOR_ATTACHMENT0 );
	}
	
	if ( _depthAttachment )
	{
		_depthAttachment->setSize( size );
		bind( _depthAttachment, GL_DEPTH_ATTACHMENT );
	}

	if ( _stencilAttachment )
	{
		_stencilAttachment->setSize( size );
		bind( _stencilAttachment, GL_STENCIL_ATTACHMENT );
	}
	
	validate();
}

bool CC3GLFramebuffer::isFullCoverage()
{
	return true; 
}

CC3Viewport CC3GLFramebuffer::getViewport()
{
	return CC3ViewportFromOriginAndSize(kCC3IntPointZero, getSize()); 
}

CC3FramebufferAttachment* CC3GLFramebuffer::getColorAttachment()
{
	return _colorAttachment; 
}

void CC3GLFramebuffer::setColorAttachment( CC3FramebufferAttachment* colorAttachment )
{
	if ( colorAttachment == _colorAttachment ) 
		return;
	
	if ( _colorAttachment )
	{
		unbind( _colorAttachment, GL_COLOR_ATTACHMENT0 );
		_colorAttachment->release();
	}

	_colorAttachment = colorAttachment;
	CC_SAFE_RETAIN(colorAttachment);
	alignSizeOfAttachment( _colorAttachment );		// After attaching, as may change size of attachments.
	bind( _colorAttachment, GL_COLOR_ATTACHMENT0 );
	
	validate();
}

CC3FramebufferAttachment* CC3GLFramebuffer::getDepthAttachment()
{
	return _depthAttachment; 
}

void CC3GLFramebuffer::setDepthAttachment( CC3FramebufferAttachment* depthAttachment )
{
	if ( depthAttachment == _depthAttachment ) 
		return;
	
	if ( _depthAttachment )
	{
		unbind( _depthAttachment, GL_DEPTH_ATTACHMENT );
		_depthAttachment->release();
	}

	_depthAttachment = depthAttachment;

	CC_SAFE_RETAIN( depthAttachment );
	alignSizeOfAttachment( _depthAttachment );		// After attaching, as may change size of attachments.
	bind( _depthAttachment, GL_DEPTH_ATTACHMENT );

	// Check for combined depth and stencil buffer
	if ( _depthAttachment && CC3DepthFormatIncludesStencil(_depthAttachment->getPixelFormat()) )
		setStencilAttachment( _depthAttachment );
	
	validate();
}

CC3FramebufferAttachment* CC3GLFramebuffer::getStencilAttachment()
{
	return _stencilAttachment; 
}

void CC3GLFramebuffer::setStencilAttachment( CC3FramebufferAttachment* stencilAttachment )
{
	if ( stencilAttachment == _stencilAttachment ) 
		return;
	
	if ( _stencilAttachment )
	{
		unbind( _stencilAttachment, GL_STENCIL_ATTACHMENT );
		_stencilAttachment->release();
	}

	_stencilAttachment = stencilAttachment;
	CC_SAFE_RETAIN ( stencilAttachment );
	alignSizeOfAttachment( _stencilAttachment );		// After attaching, as may change size of attachments.
	bind( _stencilAttachment, GL_STENCIL_ATTACHMENT );

	validate();
}

/**
 * Aligns the size of the specified attachment with this framebuffer. Does nothing if the
 * sizes are the same. If they sizes are different, then if this framebuffer has no size
 * yet, set it from the attachment, otherwise resize the attachment to match this surface.
 */
void CC3GLFramebuffer::alignSizeOfAttachment( CC3FramebufferAttachment* attachment )
{
	if ( attachment )
	{
		CC3IntSize mySize = getSize();
		CC3IntSize attSize = attachment->getSize();

		if ( CC3IntSizesAreEqual(mySize, attSize) ) 
			return;

		if ( CC3IntSizeIsZero(mySize) )
			setSize( attSize );
		else
			attachment->setSize( mySize );
	}
}

/** 
 * If appropriate, binds the specified framebuffer attachment to this framebuffer in the GL engine,
 * and derives the name of the framebuffer attachment from the framebuffer and attachment names.
 * Don't bind if this framebuffer has no size. Binding will occur once the size is set.
 */
void CC3GLFramebuffer::bind( CC3FramebufferAttachment* fbAttachment, GLenum attachment )
{
	if ( fbAttachment )
	{
		if (shouldBindGLAttachments() && !CC3IntSizeIsZero(getSize()))
			fbAttachment->bindToFramebuffer( this, attachment );

		fbAttachment->deriveNameFromFramebuffer( this, attachment );
	}
}

/** If appropriate, unbinds the specified framebuffer attachment from this framebuffer in the GL engine.*/
void CC3GLFramebuffer::unbind( CC3FramebufferAttachment* fbAttachment, GLenum attachment )
{
	if ( fbAttachment )
	{
		if (shouldBindGLAttachments()) 
			fbAttachment->unbindFromFramebuffer( this, attachment );
	}
}

CC3Texture* CC3GLFramebuffer::getColorTexture()
{
	return ((CC3TextureFramebufferAttachment*)getColorAttachment())->getTexture();
}

void CC3GLFramebuffer::setColorTexture( CC3Texture* colorTexture )
{
	setColorAttachment( CC3TextureFramebufferAttachment::attachmentWithTexture( colorTexture ) );
}

CC3Texture* CC3GLFramebuffer::getDepthTexture()
{
	return ((CC3TextureFramebufferAttachment*)getDepthAttachment())->getTexture();
}

void CC3GLFramebuffer::setDepthTexture( CC3Texture* depthTexture )
{
	setDepthAttachment( CC3TextureFramebufferAttachment::attachmentWithTexture( depthTexture ) );
}

void CC3GLFramebuffer::validate()
{
	// Validate only if this framebuffer has a size, and at least one attachment
	if ( !(_colorAttachment || _depthAttachment || _stencilAttachment) ) 
		return;

	if (CC3IntSizeIsZero(getSize())) 
		return;
	
	CCAssert(CC3OpenGL::sharedGL()->checkFramebufferStatus( getFramebufferID() ) , "CC3GLFramebuffer is incomplete." );
	checkGLDebugLabel();
}

bool CC3GLFramebuffer::init()
{
	return super::init();
}

bool CC3GLFramebuffer::isOnScreen()
{
	return _isOnScreen;
}

void CC3GLFramebuffer::setIsOnScreen( bool bOnScreen )
{
	_isOnScreen = bOnScreen;
}

/** Sets the GL debug label for the framebuffer, if required. */
void CC3GLFramebuffer::checkGLDebugLabel()
{
	if (_fbID && !_glLabelWasSet) {
		CC3OpenGL::sharedGL()->setFrameBufferDebugLabel( getName().c_str(), _fbID );
		_glLabelWasSet = true;
	}
}

void CC3GLFramebuffer::clearColorContent()
{
	activate();

	CC3OpenGL* gl = CC3OpenGL::sharedGL();
	gl->setColorMask( ccc4(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE) );
	gl->clearBuffers( GL_COLOR_BUFFER_BIT );
}

void CC3GLFramebuffer::clearDepthContent()
{
	activate();
	
	CC3OpenGL* gl = CC3OpenGL::sharedGL();
	gl->setDepthMask( true );
	gl->clearBuffers( GL_DEPTH_BUFFER_BIT );
}

void CC3GLFramebuffer::clearStencilContent()
{
	activate();
	
	CC3OpenGL* gl = CC3OpenGL::sharedGL();
	gl->setStencilMask( ~0 );
	gl->clearBuffers( GL_STENCIL_BUFFER_BIT );
}

void CC3GLFramebuffer::clearColorAndDepthContent()
{
	activate();
	
	CC3OpenGL* gl = CC3OpenGL::sharedGL();
	gl->setDepthMask( true );
	gl->setColorMask( ccc4(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE) );
	gl->clearBuffers( (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) );
}

void CC3GLFramebuffer::readColorContentFrom( const CC3Viewport& rect, ccColor4B* colorArray )
{
	CC3OpenGL::sharedGL()->readPixelsIn( rect, getFramebufferID(), colorArray );
}

void CC3GLFramebuffer::replaceColorPixels( const CC3Viewport& rect, ccColor4B* colorArray )
{
	_colorAttachment->replacePixels( rect, colorArray );
}

void CC3GLFramebuffer::activate()
{
	CC3OpenGL::sharedGL()->bindFramebuffer( getFramebufferID() ); 
}

void CC3GLFramebuffer::initWithTag( GLuint tag, const std::string& name )
{
	super::initWithTag( tag, name );
	_fbID = 0;
	_size = CC3IntSizeMake(0, 0);
	_isManagingGL = true;
	_shouldBindGLAttachments = true;
	_isOnScreen = false;
	_colorAttachment = NULL;
	_depthAttachment = NULL;
	_stencilAttachment = NULL;
	_glLabelWasSet = false;
}

CC3GLFramebuffer* CC3GLFramebuffer::surface()
{
	CC3GLFramebuffer* pSurface = new CC3GLFramebuffer;
	pSurface->init();
	pSurface->autorelease();

	return pSurface;
}

void CC3GLFramebuffer::initAsColorTextureIsOpaque( bool isOpaque )
{
	return initAsColorTextureIsOpaque( isOpaque, GL_DEPTH_COMPONENT16 );
}

CC3GLFramebuffer* CC3GLFramebuffer::colorTextureSurfaceIsOpaque( bool isOpaque )
{
	CC3GLFramebuffer* pFrameBuffer = new CC3GLFramebuffer;
	pFrameBuffer->initAsColorTextureIsOpaque( isOpaque );
	pFrameBuffer->autorelease();

	return pFrameBuffer;
}

void CC3GLFramebuffer::initAsColorTextureIsOpaque( bool isOpaque, GLenum depthFormat )
{
	return initAsColorTextureIsOpaque( isOpaque, CC3GLRenderbuffer::renderbufferWithPixelFormat(depthFormat) );
}

CC3GLFramebuffer* CC3GLFramebuffer::colorTextureSurfaceIsOpaque( bool isOpaque, GLenum depthFormat )
{
	CC3GLFramebuffer* pFrameBuffer = new CC3GLFramebuffer;
	pFrameBuffer->initAsColorTextureIsOpaque( isOpaque, depthFormat );
	pFrameBuffer->autorelease();

	return pFrameBuffer;
}

void CC3GLFramebuffer::initAsColorTextureIsOpaque( bool isOpaque, CC3FramebufferAttachment* depthAttachment )
{
	return initAsColorTextureWithPixelFormat( (isOpaque ? GL_RGB : GL_RGBA), (isOpaque ? GL_UNSIGNED_SHORT_5_6_5 : GL_UNSIGNED_BYTE), depthAttachment );
}

CC3GLFramebuffer* CC3GLFramebuffer::colorTextureSurfaceIsOpaque( bool isOpaque, CC3FramebufferAttachment* depthAttachment )
{
	CC3GLFramebuffer* pFrameBuffer = new CC3GLFramebuffer;
	pFrameBuffer->initAsColorTextureIsOpaque( isOpaque, depthAttachment );
	pFrameBuffer->autorelease();

	return pFrameBuffer;
}

void CC3GLFramebuffer::initAsColorTextureWithPixelFormat( GLenum pixelFormat, GLenum pixelType, CC3FramebufferAttachment* depthAttachment )
{
	init();
	setColorTexture( CC3Texture::textureWithPixelFormat( pixelFormat, pixelType ) );
	setDepthAttachment( depthAttachment );
}

CC3GLFramebuffer* CC3GLFramebuffer::colorTextureSurfaceWithPixelFormat( GLenum pixelFormat, GLenum pixelType, CC3FramebufferAttachment* depthAttachment )
{
	CC3GLFramebuffer* pFrameBuffer = new CC3GLFramebuffer;
	pFrameBuffer->initAsColorTextureWithPixelFormat( pixelFormat, pixelType, depthAttachment );
	pFrameBuffer->autorelease();

	return pFrameBuffer;
}

void CC3GLFramebuffer::initWithFramebufferID( GLuint fbID )
{
	init();
	_fbID = fbID;
	_isManagingGL = false;
}

CC3GLFramebuffer* CC3GLFramebuffer::surfaceWithFramebufferID( GLuint fbID )
{
	CC3GLFramebuffer* pFrameBuffer = new CC3GLFramebuffer;
	pFrameBuffer->initWithFramebufferID( fbID );
	pFrameBuffer->autorelease();

	return pFrameBuffer;
}

CC3EnvironmentMapTexture::CC3EnvironmentMapTexture()
{
	_renderSurface = NULL;
}

CC3EnvironmentMapTexture::~CC3EnvironmentMapTexture()
{
	CC_SAFE_RELEASE( _renderSurface );
}

/** Set name of internal framebuffer. */
void CC3EnvironmentMapTexture::setName( const std::string& name )
{
	super::setName( name );
	if ( _renderSurface )
		_renderSurface->setName( stringWithFormat( (char*)"%s surface", name.c_str() ) );
}

// Clamp to between zero and six
void CC3EnvironmentMapTexture::setNumberOfFacesPerSnapshot( GLfloat numberOfFacesPerSnapshot )
{
	_numberOfFacesPerSnapshot = CLAMP(numberOfFacesPerSnapshot, 0.0f, 6.0f);
}

GLfloat CC3EnvironmentMapTexture::getNumberOfFacesPerSnapshot()
{
	return _numberOfFacesPerSnapshot;
}

void CC3EnvironmentMapTexture::generateSnapshotOfScene( CC3Scene* scene, const CC3Vector& location )
{
	// Determine how many cube-map faces to render on this snapshot
	GLuint facesToGenerate = getFacesToGenerate();
	if ( !facesToGenerate ) 
		return;
	
	// Get the scene and the cube-map visitor, and set the render surface to that of this texture.
	CC3NodeDrawingVisitor* envMapVisitor = scene->getEnvMapDrawingVisitor();
	envMapVisitor->setRenderSurface( getRenderSurface() );

	// Locate the camera of the cube-map visitor at the specified location,
	CC3Camera* envMapCam = envMapVisitor->getCamera();
	envMapCam->setLocation( location );
	
	// If the scene has an active camera, match the near and far clip distances, in an attempt
	// to capture the same scene content that is being viewed by the active camera.
	CC3Camera* sceneCam = scene->getActiveCamera();
	if ( sceneCam )
	{
		envMapCam->setNearClippingDistance( sceneCam->getNearClippingDistance() );
		envMapCam->setFarClippingDistance( sceneCam->getFarClippingDistance() );
	}
	
	for ( GLuint faceIdx = 0; faceIdx < facesToGenerate; faceIdx++ ) 
	{
		moveToNextFace();

		// Bind the texture face to the framebuffer
		CC3TextureFramebufferAttachment* fbAtt = (CC3TextureFramebufferAttachment*)_renderSurface->getColorAttachment();
		fbAtt->setFace( _currentFace );
		fbAtt->bindToFramebuffer( _renderSurface, GL_COLOR_ATTACHMENT0 );
		
		// Point the camera towards the face
		envMapCam->setForwardDirection( getCameraDirection() );
		envMapCam->setReferenceUpDirection( getUpDirection() );

		// Draw the scene to the texture face
		scene->drawSceneContentForEnvironmentMapWithVisitor( envMapVisitor );

//		paintFace();		// Uncomment to identify the faces
	}
}

CC3GLFramebuffer* CC3EnvironmentMapTexture::getRenderSurface()
{
	return _renderSurface;
}

/** 
 * Paints the entire face a solid color. Each face will have a ditinct color,
 * as determined by the faceColor method.
 *
 * This can be useful during testing to diagnose which face is which.
 */
void CC3EnvironmentMapTexture::paintFace()
{
	CC3IntSize faceSize = _renderSurface->getSize();
	GLuint pixCnt = faceSize.width * faceSize.height;
	ccColor4B* canvas = new ccColor4B[pixCnt];
	ccColor4B faceColor = getFaceColor();

	for (GLuint pixIdx = 0; pixIdx < pixCnt; pixIdx++) 
		canvas[pixIdx] = faceColor;

	_renderSurface->replaceColorPixels( CC3ViewportFromOriginAndSize(kCC3IntPointZero, faceSize), canvas );

	CC_SAFE_DELETE_ARRAY( canvas );
}

/** Returns the color to paint the current face, using the diagnostic paintFace method. */
ccColor4B CC3EnvironmentMapTexture::getFaceColor()
{
	switch (_currentFace) 
	{
		case GL_TEXTURE_CUBE_MAP_POSITIVE_X:
			return CCC4BFromCCC4F(kCCC4FRed);
		case GL_TEXTURE_CUBE_MAP_NEGATIVE_X:
			return CCC4BFromCCC4F(kCCC4FCyan);
		case GL_TEXTURE_CUBE_MAP_POSITIVE_Y:
			return CCC4BFromCCC4F(kCCC4FGreen);
		case GL_TEXTURE_CUBE_MAP_NEGATIVE_Y:
			return CCC4BFromCCC4F(kCCC4FMagenta);
		case GL_TEXTURE_CUBE_MAP_POSITIVE_Z:
			return CCC4BFromCCC4F(kCCC4FBlue);
		case GL_TEXTURE_CUBE_MAP_NEGATIVE_Z:
			return CCC4BFromCCC4F(kCCC4FYellow);
		default:
			break;
	}
	//CC3Assert(NO, @"%@ encountered unknown cube-map face %@", self, NSStringFromGLEnum(_currentFace));
	return CCC4BFromCCC4F(kCCC4FWhite);
}

/**
 * Returns the number of faces to generate on this snapshot.
 *
 * Updates the face count by adding the number of faces to generate on this snapshot,
 * and returns the count as an integer. Subtract the number that will be generated on
 * this snapshot from the running count. This math means that even if the number of
 * faces per snapshot does not divide evenly into an integer number, over time the rate
 * will average out to the value of the numberOfFacesPerSnapshot property.
 */
GLuint CC3EnvironmentMapTexture::getFacesToGenerate()
{
	_faceCount += _numberOfFacesPerSnapshot;
	GLuint facesToGenerate = _faceCount;		// Convert to int (rounding down)
	_faceCount -= facesToGenerate;				// Reduce by number that will be done now
	return facesToGenerate;
}

/** 
 * Update the reference to the current face.
 *
 * GL face enums are guaranteed to be consecutive integers. 
 * Increment until we get to the end, then cycle back to the beginning.
 * If we're just starting out, start at the beginning.
 */
void CC3EnvironmentMapTexture::moveToNextFace()
{
	switch ( _currentFace ) 
	{
		case GL_TEXTURE_CUBE_MAP_POSITIVE_X:
		case GL_TEXTURE_CUBE_MAP_NEGATIVE_X:
		case GL_TEXTURE_CUBE_MAP_POSITIVE_Y:
		case GL_TEXTURE_CUBE_MAP_NEGATIVE_Y:
		case GL_TEXTURE_CUBE_MAP_POSITIVE_Z:
			_currentFace++;
			break;
		case GL_TEXTURE_CUBE_MAP_NEGATIVE_Z:
		case GL_ZERO:
		default:
			_currentFace = GL_TEXTURE_CUBE_MAP_POSITIVE_X;
			break;
	}
}

/** Returns the direction to point the camera in order to render the current cube-map face. */
CC3Vector CC3EnvironmentMapTexture::getCameraDirection()
{
	switch (_currentFace) 
	{
		case GL_TEXTURE_CUBE_MAP_POSITIVE_X:
			return kCC3VectorUnitXPositive;
		case GL_TEXTURE_CUBE_MAP_NEGATIVE_X:
			return kCC3VectorUnitXNegative;
		case GL_TEXTURE_CUBE_MAP_POSITIVE_Y:
			return kCC3VectorUnitYPositive;
		case GL_TEXTURE_CUBE_MAP_NEGATIVE_Y:
			return kCC3VectorUnitYNegative;
		case GL_TEXTURE_CUBE_MAP_POSITIVE_Z:
			return kCC3VectorUnitZPositive;
		case GL_TEXTURE_CUBE_MAP_NEGATIVE_Z:
			return kCC3VectorUnitZNegative;
		default:
			break;
	}
	//CC3Assert(NO, @"%@ encountered unknown cube-map face %@", self, NSStringFromGLEnum(_currentFace));
	return kCC3VectorNull;
}

/** Returns the direction to orient the top of the camera to render the current cube-map face. */
CC3Vector CC3EnvironmentMapTexture::getUpDirection()
{
	switch (_currentFace) 
	{
		case GL_TEXTURE_CUBE_MAP_POSITIVE_X:
			return kCC3VectorUnitYNegative;
		case GL_TEXTURE_CUBE_MAP_NEGATIVE_X:
			return kCC3VectorUnitYNegative;
		case GL_TEXTURE_CUBE_MAP_POSITIVE_Y:
			return kCC3VectorUnitZPositive;
		case GL_TEXTURE_CUBE_MAP_NEGATIVE_Y:
			return kCC3VectorUnitZNegative;
		case GL_TEXTURE_CUBE_MAP_POSITIVE_Z:
			return kCC3VectorUnitYNegative;
		case GL_TEXTURE_CUBE_MAP_NEGATIVE_Z:
			return kCC3VectorUnitYNegative;
		default:
			break;
	}
	//CC3Assert(NO, @"%@ encountered unknown cube-map face %@", self, NSStringFromGLEnum(_currentFace));
	return kCC3VectorNull;
}

void CC3EnvironmentMapTexture::initCubeWithSideLength( GLuint sideLength )
{
	return initCubeWithSideLength( sideLength, GL_DEPTH_COMPONENT16 );
}

CC3EnvironmentMapTexture* CC3EnvironmentMapTexture::textureCubeWithSideLength( GLuint sideLength )
{
	CC3EnvironmentMapTexture* pTex = new CC3EnvironmentMapTexture;
	pTex->initCubeWithSideLength( sideLength );
	pTex->autorelease();

	return pTex;
}

void CC3EnvironmentMapTexture::initCubeWithSideLength( GLuint sideLength, GLenum depthFormat )
{
	CC3GLRenderbuffer* depthBuff = CC3GLRenderbuffer::renderbufferWithPixelFormat( depthFormat );
	return initCubeWithSideLength( sideLength, depthBuff );
}

CC3EnvironmentMapTexture* CC3EnvironmentMapTexture::textureCubeWithSideLength( GLuint sideLength, GLenum depthFormat )
{
	CC3EnvironmentMapTexture* pTex = new CC3EnvironmentMapTexture;
	pTex->initCubeWithSideLength( sideLength, depthFormat );
	pTex->autorelease();

	return pTex;
}

void CC3EnvironmentMapTexture::initCubeWithSideLength( GLuint sideLength, CC3FramebufferAttachment* depthAttachment )
{
	return initCubeWithSideLength( sideLength, GL_RGBA, GL_UNSIGNED_BYTE, depthAttachment );
}

CC3EnvironmentMapTexture* CC3EnvironmentMapTexture::textureCubeWithSideLength( GLuint sideLength, CC3FramebufferAttachment* depthAttachment )
{
	CC3EnvironmentMapTexture* pTex = new CC3EnvironmentMapTexture;
	pTex->initCubeWithSideLength( sideLength, depthAttachment );
	pTex->autorelease();

	return pTex;
}

void CC3EnvironmentMapTexture::initCubeWithSideLength( GLuint sideLength, GLenum colorFormat, GLenum colorType, CC3FramebufferAttachment* depthAttachment )
{
	super::initCubeWithSideLength( sideLength, colorFormat, colorType );

	_faceCount = 0;
	_numberOfFacesPerSnapshot = 1;
	_currentFace = GL_ZERO;
	_renderSurface = new CC3GLFramebuffer;	// retained
	_renderSurface->init();

	// Create the texture attachment, based on this texture. Since this texture holds the rendering surface,
	// it must be attached to the surface attachment with a weak reference, to avoid a retain cycle.
	CC3TextureFramebufferAttachment* ta = CC3TextureFramebufferAttachment::attachmentWithTexture( this );
	ta->setShouldUseStrongReferenceToTexture( false );

	_renderSurface->setColorAttachment( ta ); 
	_renderSurface->setDepthAttachment( depthAttachment );
	_renderSurface->setSize( CC3IntSizeMake(sideLength, sideLength) );
	_renderSurface->validate();
}

CC3EnvironmentMapTexture* CC3EnvironmentMapTexture::textureCubeWithSideLength( GLuint sideLength, GLenum colorFormat, GLenum colorType, 
	CC3FramebufferAttachment* depthAttachment )
{
	CC3EnvironmentMapTexture* pTex = new CC3EnvironmentMapTexture;
	pTex->initCubeWithSideLength( sideLength, colorFormat, colorType, depthAttachment );
	pTex->autorelease();

	return pTex;
}

CC3SurfaceManager::CC3SurfaceManager()
{
	_resizeableSurfaces = NULL;
}

CC3SurfaceManager::~CC3SurfaceManager()
{
	CC_SAFE_RELEASE( _resizeableSurfaces );
}

void CC3SurfaceManager::addSurface( CC3RenderSurface* surface )
{
	if ( !surface || _resizeableSurfaces->containsObject( surface ) ) 
		return;
	
	_resizeableSurfaces->addObject( surface );
	alignSizeOfSurface( surface );
}

void CC3SurfaceManager::removeSurface( CC3RenderSurface* surface )
{
	if ( surface ) 
		_resizeableSurfaces->removeObject( surface );
}

CC3IntSize CC3SurfaceManager::getSize()
{
	return _size; 
}

void CC3SurfaceManager::setSize( const CC3IntSize& size )
{
	if ( CC3IntSizesAreEqual(size, _size) ) 
		return;
	
	_size = size;

	CCObject* pObj;
	CCARRAY_FOREACH (_resizeableSurfaces, pObj)
	{
		CC3RenderSurface* surface = (CC3RenderSurface*)pObj;
		surface->setSize( size );
	}
}

/**
 * Aligns the size of the specified surface with this instance. Does nothing if the sizes 
 * are the same. If they sizes are different, then if this instance has no size yet, set 
 * it from the surface, otherwise resize the surface to match this instance.
 */
void CC3SurfaceManager::alignSizeOfSurface( CC3RenderSurface* aSurface )
{
	CC3IntSize mySize = getSize();
	CC3IntSize surfSize = aSurface->getSize();
	
	if ( CC3IntSizesAreEqual(mySize, surfSize) ) 
		return;
	
	if ( CC3IntSizeIsZero(mySize) )
		setSize( surfSize );
	else
		aSurface->setSize( mySize );
}

bool CC3SurfaceManager::init()
{
	_resizeableSurfaces = CCArray::create();		// retained
	_resizeableSurfaces->retain();
	_size = CC3IntSizeMake(0, 0);

	return true;
}

CC3SurfaceManager* CC3SurfaceManager::surfaceManager()
{
	CC3SurfaceManager* pMgr = new CC3SurfaceManager;
	pMgr->init();
	pMgr->autorelease();

	return pMgr;
}

CC3SceneDrawingSurfaceManager::CC3SceneDrawingSurfaceManager()
{
	_viewSurface = NULL;
	_pickingSurface = NULL;
}

CC3SceneDrawingSurfaceManager::~CC3SceneDrawingSurfaceManager()
{
	CC_SAFE_RELEASE( _viewSurface );
	CC_SAFE_RELEASE( _pickingSurface );
}

CC3SceneDrawingSurfaceManager* CC3SceneDrawingSurfaceManager::surfaceManager()
{
	CC3SceneDrawingSurfaceManager* pMgr = new CC3SceneDrawingSurfaceManager;
	pMgr->init();
	pMgr->autorelease();

	return pMgr;
}

/** Lazily create a surface section on the renderingSurface from the CC3ViewSurfaceManager singleton. */
CC3RenderSurface* CC3SceneDrawingSurfaceManager::getViewSurface()
{
	if ( !_viewSurface ) 
		setViewSurface( CC3SurfaceSection::surfaceOnSurface( CC3ViewSurfaceManager::sharedViewSurfaceManager()->getRenderingSurface() ) );
	return _viewSurface;
}

void CC3SceneDrawingSurfaceManager::setViewSurface( CC3SurfaceSection* aSurface )
{
	if ( aSurface == _viewSurface )
		return;

	if ( _viewSurface )
		removeSurface( _viewSurface );

	CC_SAFE_RELEASE( _viewSurface );
	_viewSurface = aSurface;
	CC_SAFE_RETAIN( aSurface );

	addSurface( _viewSurface );
}

CC3IntPoint CC3SceneDrawingSurfaceManager::getViewSurfaceOrigin()
{
	return _viewSurface->getOrigin(); 
}

void CC3SceneDrawingSurfaceManager::setViewSurfaceOrigin( const CC3IntPoint& viewSurfaceOrigin )
{
	((CC3SurfaceSection*)getViewSurface())->setOrigin( viewSurfaceOrigin );
}

/**
 * Lazily create a surface, using the color format of the view's color surface,
 * and with a new non-multisampling and non-stencilling depth buffer.
 */
CC3RenderSurface* CC3SceneDrawingSurfaceManager::getPickingSurface()
{
	if ( !_pickingSurface ) 
	{
		CC3ViewSurfaceManager* viewSurfMgr = CC3ViewSurfaceManager::sharedViewSurfaceManager();
		GLenum viewColorFormat = viewSurfMgr->getColorFormat();
		GLenum viewDepthFormat = viewSurfMgr->getDepthFormat();
		
		CC3GLFramebuffer* pickSurf = CC3GLFramebuffer::surface();
		pickSurf->setName( "Picking surface" );
		
		pickSurf->setColorAttachment( CC3GLRenderbuffer::renderbufferWithPixelFormat(viewColorFormat) );
		
		// Don't need stencil for picking, but otherwise match the rendering depth format
		if (viewDepthFormat)
		{
			if ( CC3DepthFormatIncludesStencil(viewDepthFormat) ) 
				viewDepthFormat = GL_DEPTH_COMPONENT24;
			pickSurf->setDepthAttachment( CC3GLRenderbuffer::renderbufferWithPixelFormat(viewDepthFormat) );
		}
		
		setPickingSurface( pickSurf );
		
		/*LogInfo(@"Created picking surface of size %@ with color format %@ and depth format %@.",
		NSStringFromCC3IntSize(pickSurf.size),
		NSStringFromGLEnum(pickSurf.colorAttachment.pixelFormat),
		NSStringFromGLEnum(pickSurf.depthAttachment.pixelFormat));*/
	}
	return _pickingSurface;
}

void CC3SceneDrawingSurfaceManager::setPickingSurface( CC3RenderSurface* aSurface )
{
	if ( aSurface == _pickingSurface )
		return;

	if ( _pickingSurface )
		removeSurface( _pickingSurface );

	CC_SAFE_RELEASE( _pickingSurface );
	_pickingSurface = aSurface;
	CC_SAFE_RETAIN( aSurface );

	addSurface( _pickingSurface );
}

CC3ViewSurfaceManager::CC3ViewSurfaceManager()
{
	_viewSurface = NULL;
	_multisampleSurface = NULL;
}

CC3ViewSurfaceManager::~CC3ViewSurfaceManager()
{
	CC_SAFE_RELEASE( _viewSurface );
	CC_SAFE_RELEASE( _multisampleSurface );
}

CC3GLFramebuffer* CC3ViewSurfaceManager::getViewSurface()
{
	return _viewSurface; 
}

void CC3ViewSurfaceManager::setViewSurface( CC3GLFramebuffer* aSurface )
{
	if ( aSurface == _viewSurface )
		return;

	if ( _viewSurface )
		removeSurface( _viewSurface );

	CC_SAFE_RELEASE( _viewSurface );
	_viewSurface = aSurface;
	CC_SAFE_RETAIN( aSurface );

	addSurface( _viewSurface );
}

CC3GLFramebuffer* CC3ViewSurfaceManager::getMultisampleSurface()
{
	return _multisampleSurface; 
}

void CC3ViewSurfaceManager::setMultisampleSurface( CC3GLFramebuffer* aSurface )
{
	if ( aSurface == _multisampleSurface )
		return;

	if ( _multisampleSurface )
		removeSurface( _multisampleSurface );

	CC_SAFE_RELEASE( _multisampleSurface );
	_multisampleSurface = aSurface;
	CC_SAFE_RETAIN( aSurface );

	addSurface( _multisampleSurface );
}

CC3GLFramebuffer* CC3ViewSurfaceManager::getRenderingSurface()
{
	return _multisampleSurface ? _multisampleSurface : _viewSurface;
}

bool CC3ViewSurfaceManager::shouldUseDedicatedPickingSurface()
{
	return true; 
}

void CC3ViewSurfaceManager::setShouldUseDedicatedPickingSurface( bool _shouldUseDedicatedPickingSurface )
{

}

GLenum CC3ViewSurfaceManager::getColorFormat()
{
	return getRenderingSurface()->getColorAttachment()->getPixelFormat(); 
}

GLenum CC3ViewSurfaceManager::getDepthFormat()
{
	CC3RenderSurfaceAttachment* pDepthAttachment = getRenderingSurface()->getDepthAttachment();
	if ( pDepthAttachment )
		return pDepthAttachment->getPixelFormat();

	return 0;
}

GLenum CC3ViewSurfaceManager::getStencilFormat()
{
	return getRenderingSurface()->getStencilAttachment()->getPixelFormat();
}

GLenum CC3ViewSurfaceManager::getColorTexelFormat()
{
	return CC3TexelFormatFromRenderbufferColorFormat( getColorFormat() );
}

GLenum CC3ViewSurfaceManager::getColorTexelType()
{ 
	return CC3TexelTypeFromRenderbufferColorFormat( getColorFormat() ); 
}

GLenum CC3ViewSurfaceManager::getDepthTexelFormat()
{
	return CC3TexelFormatFromRenderbufferDepthFormat( getDepthFormat() );
}

GLenum CC3ViewSurfaceManager::getDepthTexelType()
{ 
	return CC3TexelTypeFromRenderbufferDepthFormat( getDepthFormat() ); 
}

GLuint CC3ViewSurfaceManager::getPixelSamples()
{
	return ((CC3GLRenderbuffer*)getRenderingSurface()->getColorAttachment())->getPixelSamples(); 
}

bool CC3ViewSurfaceManager::isMultisampling()
{
	return getPixelSamples() > 1; 
}

CC3IntSize CC3ViewSurfaceManager::getMultisamplingSize()
{
	CC3IntSize baseSize = getSize();
	switch (getPixelSamples()) 
	{
		case 2:
		case 4:
			return CC3IntSizeMake(baseSize.width * 2, baseSize.height * 2);
		case 6:
		case 8:
		case 9:
			return CC3IntSizeMake(baseSize.width * 3, baseSize.height * 3);
		case 16:
			return CC3IntSizeMake(baseSize.width * 4, baseSize.height * 4);
		default:
			return baseSize;
	}
}

void CC3ViewSurfaceManager::setSize( const CC3IntSize& size )
{
	if ( CC3IntSizesAreEqual(size, getSize()) ) 
		return;
	
	super::setSize( size );
	
	/*LogInfo(@"View surface size set to: %@, %@.", NSStringFromCC3IntSize(self.size),
			(self.isMultisampling ? [NSString stringWithFormat: @"multisampling from %@",
									 NSStringFromCC3IntSize(self.multisamplingSize)]
			 : @"with no multisampling"));*/
	
	// After validating each surface, ensure we leave the rendering surface active for Cocos2D
	CC3RenderSurface* pSurface = getRenderingSurface();
	if ( pSurface )
		pSurface->activate();
}

void CC3ViewSurfaceManager::resolveMultisampling()
{
	CC3OpenGL* gl = CC3OpenGL::sharedGL();
	
	// If it exists, resolve the multisample buffer into the screen buffer
	if (_multisampleSurface)
		gl->resolveMultisampleFramebuffer( _multisampleSurface->getFramebufferID(), _viewSurface->getFramebufferID() );
	
	// Discard used buffers by assembling an array of framebuffer attachments to discard.
	// If multisampling, discard multisampling color buffer.
	// If using depth buffer, discard it from the rendering buffer (either multisampling or screen)
	GLenum fbAtts[3];			// Make room for color, depth & stencil attachments
	GLuint fbAttCount = 0;
	CC3RenderSurface* rendSurf = getRenderingSurface();
	if (_multisampleSurface) 
		fbAtts[fbAttCount++] = GL_COLOR_ATTACHMENT0;
	if (rendSurf->getDepthAttachment()) 
		fbAtts[fbAttCount++] = GL_DEPTH_ATTACHMENT;
	if (rendSurf->getStencilAttachment()) 
		fbAtts[fbAttCount++] = GL_STENCIL_ATTACHMENT;

	gl->discard( fbAttCount, fbAtts, ((CC3GLFramebuffer*)rendSurf)->getFramebufferID() );
	
	((CC3GLRenderbuffer*)_viewSurface->getColorAttachment())->bind();
}

void CC3ViewSurfaceManager::initFromView( CCEGLView* view )
{
    super::init();
	setSize( CC3IntSizeFromCGSize(view->getFrameSize()) );

	GLenum colorFormat = GL_RGBA;
	GLenum depthFormat = GL_DEPTH_STENCIL/*GL_DEPTH24_STENCIL8*/;
	GLuint viewFramebufferID = 0/*view->getDefaultFrameBuffer()*/;
	GLuint msaaFramebufferID = 0/*view->getMsaaFrameBuffer()*/;
	bool isMultiSampling = (msaaFramebufferID > 0);
		
	CC3GLFramebuffer* vSurf = CC3GLFramebuffer::surfaceWithFramebufferID( viewFramebufferID );
	vSurf->setName( "Display surface" );
	vSurf->setShouldBindGLAttachments( false );			// Attachments are bound already
	vSurf->setIsOnScreen( !isMultiSampling );		// View surface is off-screen when multisampling
	vSurf->setColorAttachment( CC3GLRenderbuffer::renderbufferWithPixelFormat( colorFormat/*view->getColorRenderBuffer()*/ ) );
	setViewSurface( vSurf );
		
	if ( isMultiSampling ) 
	{
		CC3GLFramebuffer* msSurf = CC3GLFramebuffer::surfaceWithFramebufferID( msaaFramebufferID );
		msSurf->setName( "Multisampling surface" );
		vSurf->setShouldBindGLAttachments( false );			// Attachments are bound already
		vSurf->setIsOnScreen( isMultiSampling );			// View surface is off-screen when multisampling
		msSurf->setColorAttachment( CC3GLRenderbuffer::renderbufferWithPixelFormat( colorFormat/*view->getMsaaColorBuffer()*/ ) );
		setMultisampleSurface( msSurf );
	}

	if (depthFormat)
		getRenderingSurface()->setDepthAttachment( CC3GLRenderbuffer::renderbufferWithPixelFormat( depthFormat/*view->getDepthBuffer()*/ ) );
}

static CC3ViewSurfaceManager* _sharedViewSurfaceManager = NULL;

CC3ViewSurfaceManager* CC3ViewSurfaceManager::sharedViewSurfaceManager()
{
	if ( !_sharedViewSurfaceManager ) 
	{
		CCEGLView* view = CCDirector::sharedDirector()->getOpenGLView();
		CCAssert(view, "OpenGL view not available. Be sure to install the CCGLView in"
				  " the CCDirector before attempting to create the surface manager.");
		_sharedViewSurfaceManager = new CC3ViewSurfaceManager;
		_sharedViewSurfaceManager->initFromView( view );
	}
	return _sharedViewSurfaceManager;
}

GLenum CC3TexelFormatFromRenderbufferColorFormat( GLenum rbFormat )
{
	switch ( rbFormat )
	{
		case GL_RGB565:
		case GL_RGB8:
			return GL_RGB;
		case GL_RGBA4:
		case GL_RGB5_A1:
		case GL_RGBA8:
		default:
			return GL_RGBA;
	}
}

GLenum CC3TexelTypeFromRenderbufferColorFormat( GLenum rbFormat )
{
	switch (rbFormat)
	{
		case GL_RGB565:
			return GL_UNSIGNED_SHORT_5_6_5;
		case GL_RGBA4:
			return GL_UNSIGNED_SHORT_4_4_4_4;
		case GL_RGB5_A1:
			return GL_UNSIGNED_SHORT_5_5_5_1;
		case GL_RGB8:
		case GL_RGBA8:
		default:
			return GL_UNSIGNED_BYTE;
	}
}

GLenum CC3TexelFormatFromRenderbufferDepthFormat( GLenum rbFormat )
{
	switch (rbFormat) 
	{
		case GL_DEPTH24_STENCIL8:
			return GL_DEPTH_STENCIL;
		case GL_DEPTH_COMPONENT16:
		case GL_DEPTH_COMPONENT24:
		case GL_DEPTH_COMPONENT32:
		default:
			return GL_DEPTH_COMPONENT;
	}
}

GLenum CC3TexelTypeFromRenderbufferDepthFormat( GLenum rbFormat) 
{
	switch (rbFormat) 
	{
		case GL_DEPTH24_STENCIL8:
			return GL_UNSIGNED_INT_24_8;
		case GL_DEPTH_COMPONENT24:
		case GL_DEPTH_COMPONENT32:
			return GL_UNSIGNED_INT;
		case GL_DEPTH_COMPONENT16:
		default:
			return GL_UNSIGNED_SHORT;
	}
}

std::string CC3FramebufferAttachmentName( CC3GLFramebuffer* framebuffer, GLenum attachment ) 
{
	std::string fbName = framebuffer->getName();
	if ( fbName.empty() ) 
		return "";
	
	std::string attachmentName = "";
	switch (attachment) {
		case GL_COLOR_ATTACHMENT0:
			attachmentName = "color";
			break;
		case GL_DEPTH_ATTACHMENT:
			attachmentName = "depth";
			break;
		case GL_STENCIL_ATTACHMENT:
			attachmentName = "stencil";
			break;
			
		default:
			attachmentName = "unknown";
			break;
	}

	return stringWithFormat( (char*)"%s-%s", fbName.c_str(), attachmentName.c_str() );
}

NS_COCOS3D_END
