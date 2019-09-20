#include "stdafx.h"
#include "RenderingEngineGL.h"

namespace ose::rendering
{
	RenderingEngineGL::RenderingEngineGL() : RenderingEngine()
	{
		// NOTE - If RenderingEngineGL is made multithreadable, may need to move this
		// TODO - Only load GLEW if used OpenGL functions are not available
		InitGlew();
	}

	RenderingEngineGL::~RenderingEngineGL() {}

	void RenderingEngineGL::UpdateOrthographicProjectionMatrix(const int fbwidth, const int fbheight)
	{
		DEBUG_LOG("updating othographic projection matrix");
		float aspect_ratio = (float)fbwidth/(float)fbheight;
		// setting glOrtho and glViewport in the following ways worked in testing
		projection_matrix_ = glm::ortho(-(float)fbwidth/2 * aspect_ratio, (float)fbwidth/2 * aspect_ratio, -(float)fbheight/2 * aspect_ratio, (float)fbheight/2 * aspect_ratio);
		glViewport(0, 0, fbwidth, fbheight);
	}

	void RenderingEngineGL::UpdatePerspectiveProjectionMatrix(const float fovyDeg, const int fbwidth, const int fbheight, const float znear, const float zfar)
	{
		DEBUG_LOG("updating perspective projection matrix");
		// TODO - test aspect ratio is correct for a variety of resolutions
		projection_matrix_ = glm::perspective(glm::radians(fovyDeg), (float)fbwidth/(float)fbheight, znear, zfar);
		glViewport(0, 0, fbwidth, fbheight);	// still required with shaders as far as I'm aware
	}

	// Engine::update method overriden
	// Called every game update to render all object in the pool
	void RenderingEngineGL::Update()
	{
		for(auto const & render_pass : render_pool_.GetRenderPasses())
		{
			for(auto const & shader_group : render_pass.shader_groups_)
			{
				for(auto const & render_object : shader_group.render_objects_)
				{
					for(auto const texture : render_object.textures_)
					{
						glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
						glCullFace(GL_BACK);
						glEnable(GL_CULL_FACE);
						glEnable(GL_TEXTURE_2D);
						glMatrixMode(GL_MODELVIEW);
						glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
						glLoadIdentity();
						///texture->Bind();
						glBindTexture(GL_TEXTURE_2D, texture->GetGlTexId());
						//DEBUG_LOG(render_object.gl_tex_id_);
						glBegin(GL_QUADS);
						glTexCoord2i(1, 0);   glVertex2f(-0.5f,   0.5f);
						glTexCoord2i(1, 1);   glVertex2f(-0.5f,  -0.5f);
						glTexCoord2i(0, 1);   glVertex2f(0.5f, -0.5f);
						glTexCoord2i(0, 0);   glVertex2f(0.5f, 0.5f);
						glEnd();
					}
				}
			}
		}
	}

	// load OpenGL functions using GLEW
	// return of 0 = success, return of -1 = error
	int RenderingEngineGL::InitGlew()
	{
		GLenum err = glewInit();
		if(GLEW_OK != err)
		{
			// GLEW Initialisation failed
			fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
			return -1;		// return with error
		}
		fprintf(stderr, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

		std::cerr << "Initialised GLEW" << std::endl;
		if(GLEW_VERSION_3_2)
		{
			std::cerr << "OpenGL 3.2 Supported" << std::endl;
		}
		else
		{
			std::cerr << "OpenGL 3.2 Not Supported" << std::endl;

			if(GLEW_ARB_instanced_arrays)
			{
				std::cerr << "Instanced Arrays Available" << std::endl;
			}
			else
			{
				std::cerr << "Instanced Arrays Not Supported" << std::endl;
			}
		}

		return 0;			// return with success
	}
}
