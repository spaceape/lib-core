#ifndef core_GL_gpu_h
#define core_GL_gpu_h
/**
    Copyright (c) 2018, wicked systems

    Redistribution and use in source and binary forms, with or without modification, are
    permitted provided that the following conditions are met:
    1. Redistributions of source code must retain the above copyright notice, this list of
    conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice, this list
    of conditions and the following disclaimer in the documentation and/or other materials
    provided with the distribution.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
    MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
    THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
    OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
    HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
    TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**/
#ifndef GL_GLEXT_PROTOTYPES
#define GL_GLEXT_PROTOTYPES
#endif
#include <GL/gl.h>
#include <GL/glext.h>

namespace gl {

    inline auto   ClearColor = glClearColor;
    inline auto   DrawArrays = glDrawArrays;

    extern PFNGLCREATEPROGRAMPROC CreateProgram;
    extern PFNGLLINKPROGRAMPROC LinkProgram;
    extern PFNGLDELETEPROGRAMPROC DeleteProgram;
    extern PFNGLISPROGRAMPROC IsProgram;
    extern PFNGLUSEPROGRAMPROC UseProgram;
    extern PFNGLGETPROGRAMIVPROC GetProgramIV;
    extern PFNGLGETPROGRAMINFOLOGPROC GetProgramInfoLog;

    extern PFNGLCOMPILESHADERPROC CompileShader;
    extern PFNGLCREATESHADERPROC CreateShader;
    extern PFNGLSHADERSOURCEPROC ShaderSource;
    extern PFNGLGETSHADERSOURCEPROC GetShaderSource;
    extern PFNGLATTACHSHADERPROC AttachShader;
    extern PFNGLGETATTACHEDSHADERSPROC GetAttachedShader;
    extern PFNGLDETACHSHADERPROC DetachShader;
    extern PFNGLDELETESHADERPROC DeleteShader;
    extern PFNGLISSHADERPROC IsShader;
    extern PFNGLGETSHADERIVPROC GetShaderIV;
    extern PFNGLGETSHADERINFOLOGPROC GetShaderInfoLog;

    extern PFNGLBINDBUFFERPROC BindBuffer;
    extern PFNGLGENBUFFERSPROC GenBuffers;
    extern PFNGLBUFFERDATAPROC BufferData;
    extern PFNGLISBUFFERPROC IsBuffer;
    extern PFNGLDELETEBUFFERSPROC DeleteBuffers;

    extern PFNGLGETACTIVEATTRIBPROC GetActiveAttrib;
    extern PFNGLGETATTRIBLOCATIONPROC GetAttribLocation;

    extern PFNGLGETACTIVEUNIFORMPROC GetActiveUniform;
    extern PFNGLGETUNIFORMLOCATIONPROC GetUniformLocation;
    extern PFNGLGETUNIFORMFVPROC GetUniformFV;
    extern PFNGLGETUNIFORMIVPROC GetUniformIV;
    extern PFNGLUNIFORM1FPROC Uniform1F;
    extern PFNGLUNIFORM2FPROC Uniform2F;
    extern PFNGLUNIFORM3FPROC Uniform3F;
    extern PFNGLUNIFORM4FPROC Uniform4F;
    extern PFNGLUNIFORM1IPROC Uniform1I;
    extern PFNGLUNIFORM2IPROC Uniform2I;
    extern PFNGLUNIFORM3IPROC Uniform3I;
    extern PFNGLUNIFORM4IPROC Uniform4I;
    extern PFNGLUNIFORM1FVPROC Uniform1FV;
    extern PFNGLUNIFORM2FVPROC Uniform2FV;
    extern PFNGLUNIFORM3FVPROC Uniform3FV;
    extern PFNGLUNIFORM4FVPROC Uniform4FV;
    extern PFNGLUNIFORM1IVPROC Uniform1IV;
    extern PFNGLUNIFORM2IVPROC Uniform2IV;
    extern PFNGLUNIFORM3IVPROC Uniform3IV;
    extern PFNGLUNIFORM4IVPROC Uniform4IV;
    extern PFNGLUNIFORMMATRIX2FVPROC UniformMatrix2FV;
    extern PFNGLUNIFORMMATRIX3FVPROC UniformMatrix3FV;
    extern PFNGLUNIFORMMATRIX4FVPROC UniformMatrix4FV;

    extern PFNGLENABLEVERTEXATTRIBARRAYPROC EnableVertexAttribArray;
    extern PFNGLDISABLEVERTEXATTRIBARRAYPROC DisableVertexAttribArray;
    extern PFNGLGETVERTEXATTRIBDVPROC GetVertexAttribDV;
    extern PFNGLGETVERTEXATTRIBFVPROC GetVertexAttribV;
    extern PFNGLGETVERTEXATTRIBIVPROC GetVertexAttribIV;
    extern PFNGLGETVERTEXATTRIBPOINTERVPROC GetVertexAttribPointerV;
    extern PFNGLGENVERTEXARRAYSPROC GenVertexArrays;
    extern PFNGLBINDVERTEXARRAYPROC BindVertexArray;
    extern PFNGLISVERTEXARRAYPROC IsVertexArray;
    extern PFNGLDELETEVERTEXARRAYSPROC DeleteVertexArrays;
    extern PFNGLVERTEXATTRIBPOINTERPROC VertexAttribPointer;

    extern PFNGLGENERATEMIPMAPPROC GenerateMipmap;

    inline auto   GenTextures = glGenTextures;
    inline auto   BindTexture = glBindTexture;
    inline auto   DeleteTextures = glDeleteTextures;
/*namespace gl*/ }
#endif
