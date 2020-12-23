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
#include "os.h"
#include <gpu.h>
#ifdef LINUX
#include <dlfcn.h>
#else
#error OpenGL 3.X extensions not implemented for this host.
#endif

namespace gl {

      PFNGLCREATEPROGRAMPROC CreateProgram;
      PFNGLLINKPROGRAMPROC LinkProgram;
      PFNGLDELETEPROGRAMPROC DeleteProgram;
      PFNGLISPROGRAMPROC IsProgram;
      PFNGLUSEPROGRAMPROC UseProgram;
      PFNGLGETPROGRAMIVPROC GetProgramIV;
      PFNGLGETPROGRAMINFOLOGPROC GetProgramInfoLog;

      PFNGLCOMPILESHADERPROC CompileShader;
      PFNGLCREATESHADERPROC CreateShader;
      PFNGLSHADERSOURCEPROC ShaderSource;
      PFNGLGETSHADERSOURCEPROC GetShaderSource;
      PFNGLATTACHSHADERPROC AttachShader;
      PFNGLGETATTACHEDSHADERSPROC GetAttachedShader;
      PFNGLDETACHSHADERPROC DetachShader;
      PFNGLDELETESHADERPROC DeleteShader;
      PFNGLISSHADERPROC IsShader;
      PFNGLGETSHADERIVPROC GetShaderIV;
      PFNGLGETSHADERINFOLOGPROC GetShaderInfoLog;

      PFNGLBINDBUFFERPROC BindBuffer;
      PFNGLGENBUFFERSPROC GenBuffers;
      PFNGLBUFFERDATAPROC BufferData;
      PFNGLISBUFFERPROC IsBuffer;
      PFNGLDELETEBUFFERSPROC DeleteBuffers;

      PFNGLGETACTIVEATTRIBPROC GetActiveAttrib;
      PFNGLGETATTRIBLOCATIONPROC GetAttribLocation;

      PFNGLGETACTIVEUNIFORMPROC GetActiveUniform;
      PFNGLGETUNIFORMLOCATIONPROC GetUniformLocation;
      PFNGLGETUNIFORMFVPROC GetUniformFV;
      PFNGLGETUNIFORMIVPROC GetUniformIV;
      PFNGLUNIFORM1FPROC Uniform1F;
      PFNGLUNIFORM2FPROC Uniform2F;
      PFNGLUNIFORM3FPROC Uniform3F;
      PFNGLUNIFORM4FPROC Uniform4F;
      PFNGLUNIFORM1IPROC Uniform1I;
      PFNGLUNIFORM2IPROC Uniform2I;
      PFNGLUNIFORM3IPROC Uniform3I;
      PFNGLUNIFORM4IPROC Uniform4I;
      PFNGLUNIFORM1FVPROC Uniform1FV;
      PFNGLUNIFORM2FVPROC Uniform2FV;
      PFNGLUNIFORM3FVPROC Uniform3FV;
      PFNGLUNIFORM4FVPROC Uniform4FV;
      PFNGLUNIFORM1IVPROC Uniform1IV;
      PFNGLUNIFORM2IVPROC Uniform2IV;
      PFNGLUNIFORM3IVPROC Uniform3IV;
      PFNGLUNIFORM4IVPROC Uniform4IV;
      PFNGLUNIFORMMATRIX2FVPROC UniformMatrix2FV;
      PFNGLUNIFORMMATRIX3FVPROC UniformMatrix3FV;
      PFNGLUNIFORMMATRIX4FVPROC UniformMatrix4FV;

      PFNGLENABLEVERTEXATTRIBARRAYPROC EnableVertexAttribArray;
      PFNGLDISABLEVERTEXATTRIBARRAYPROC DisableVertexAttribArray;
      PFNGLGETVERTEXATTRIBDVPROC GetVertexAttribDV;
      PFNGLGETVERTEXATTRIBFVPROC GetVertexAttribV;
      PFNGLGETVERTEXATTRIBIVPROC GetVertexAttribIV;
      PFNGLGETVERTEXATTRIBPOINTERVPROC GetVertexAttribPointerV;
      PFNGLGENVERTEXARRAYSPROC GenVertexArrays;
      PFNGLBINDVERTEXARRAYPROC BindVertexArray;
      PFNGLISVERTEXARRAYPROC IsVertexArray;
      PFNGLDELETEVERTEXARRAYSPROC DeleteVertexArrays;
      PFNGLVERTEXATTRIBPOINTERPROC VertexAttribPointer;

      PFNGLGENERATEMIPMAPPROC GenerateMipmap;

/*namespace gl*/ }

__attribute__((constructor))
static bool  load_extensions(unsigned int) noexcept
{
    void* ld = dlopen(nullptr, RTLD_NOW);
    if(ld) {
        gl::CreateProgram =(PFNGLCREATEPROGRAMPROC)dlsym(ld, "glCreateProgram");
        gl::LinkProgram =(PFNGLLINKPROGRAMPROC)dlsym(ld, "glLinkProgram");
        gl::DeleteProgram =(PFNGLDELETEPROGRAMPROC)dlsym(ld, "glDeleteProgram");
        gl::IsProgram =(PFNGLISPROGRAMPROC)dlsym(ld, "glIsProgram");
        gl::UseProgram =(PFNGLUSEPROGRAMPROC)dlsym(ld, "glUseProgram");
        gl::GetProgramIV =(PFNGLGETPROGRAMIVPROC)dlsym(ld, "glGetProgramiv");
        gl::GetProgramInfoLog =(PFNGLGETPROGRAMINFOLOGPROC)dlsym(ld, "glGetProgramInfoLog");

        gl::CompileShader =(PFNGLCOMPILESHADERPROC)dlsym(ld, "glCompileShader");
        gl::CreateShader =(PFNGLCREATESHADERPROC)dlsym(ld, "glCreateShader");
        gl::ShaderSource =(PFNGLSHADERSOURCEPROC)dlsym(ld, "glShaderSource");
        gl::GetShaderSource =(PFNGLGETSHADERSOURCEPROC)dlsym(ld, "glGetShaderSource");
        gl::AttachShader =(PFNGLATTACHSHADERPROC)dlsym(ld, "glAttachShader");

        gl::GetAttachedShader =(PFNGLGETATTACHEDSHADERSPROC)dlsym(ld, "glGetAttachedShader");
        gl::DetachShader =(PFNGLDETACHSHADERPROC)dlsym(ld, "glDetachShader");
        gl::DeleteShader =(PFNGLDELETESHADERPROC)dlsym(ld, "glDeleteShader");
        gl::IsShader =(PFNGLISSHADERPROC)dlsym(ld, "glIsShader");
        gl::GetShaderIV =(PFNGLGETSHADERIVPROC)dlsym(ld, "glGetShaderiv");
        gl::GetShaderInfoLog =(PFNGLGETSHADERINFOLOGPROC)dlsym(ld, "glGetShaderInfoLog");

        gl::BindBuffer =(PFNGLBINDBUFFERPROC)dlsym(ld, "glBindBuffer");
        gl::GenBuffers =(PFNGLGENBUFFERSPROC)dlsym(ld, "glGenBuffers");
        gl::BufferData =(PFNGLBUFFERDATAPROC)dlsym(ld, "glBufferData");
        gl::IsBuffer =(PFNGLISBUFFERPROC)dlsym(ld, "glIsBuffer");
        gl::DeleteBuffers =(PFNGLDELETEBUFFERSPROC)dlsym(ld, "glDeleteBuffers");

        gl::GetActiveAttrib =(PFNGLGETACTIVEATTRIBPROC)dlsym(ld, "glGetActiveAttrib");
        gl::GetAttribLocation =(PFNGLGETATTRIBLOCATIONPROC)dlsym(ld, "glGetAttribLocation");

        gl::GetActiveUniform =(PFNGLGETACTIVEUNIFORMPROC)dlsym(ld, "glGetActiveUniform");
        gl::GetUniformLocation =(PFNGLGETUNIFORMLOCATIONPROC)dlsym(ld, "glGetUniformLocation");
        gl::GetUniformFV =(PFNGLGETUNIFORMFVPROC)dlsym(ld, "glGetUniformfv");
        gl::GetUniformIV =(PFNGLGETUNIFORMIVPROC)dlsym(ld, "glGetUniformiv");
        gl::Uniform1F =(PFNGLUNIFORM1FPROC)dlsym(ld, "glUniform1f");
        gl::Uniform2F =(PFNGLUNIFORM2FPROC)dlsym(ld, "glUniform2f");
        gl::Uniform3F =(PFNGLUNIFORM3FPROC)dlsym(ld, "glUniform3f");
        gl::Uniform4F =(PFNGLUNIFORM4FPROC)dlsym(ld, "glUniform4f");
        gl::Uniform1I =(PFNGLUNIFORM1IPROC)dlsym(ld, "glUniform1i");
        gl::Uniform2I =(PFNGLUNIFORM2IPROC)dlsym(ld, "glUniform2i");
        gl::Uniform3I =(PFNGLUNIFORM3IPROC)dlsym(ld, "glUniform3i");
        gl::Uniform4I =(PFNGLUNIFORM4IPROC)dlsym(ld, "glUniform4i");
        gl::Uniform1FV =(PFNGLUNIFORM1FVPROC)dlsym(ld, "glUniform1fv");
        gl::Uniform2FV =(PFNGLUNIFORM2FVPROC)dlsym(ld, "glUniform2fv");
        gl::Uniform3FV =(PFNGLUNIFORM3FVPROC)dlsym(ld, "glUniform3fv");
        gl::Uniform4FV =(PFNGLUNIFORM4FVPROC)dlsym(ld, "glUniform4fv");
        gl::Uniform1IV =(PFNGLUNIFORM1IVPROC)dlsym(ld, "glUniform1iv");
        gl::Uniform2IV =(PFNGLUNIFORM2IVPROC)dlsym(ld, "glUniform2iv");
        gl::Uniform3IV =(PFNGLUNIFORM3IVPROC)dlsym(ld, "glUniform3iv");
        gl::Uniform4IV =(PFNGLUNIFORM4IVPROC)dlsym(ld, "glUniform4iv");
        gl::UniformMatrix2FV =(PFNGLUNIFORMMATRIX2FVPROC)dlsym(ld, "glUniformMatrix2fv");
        gl::UniformMatrix3FV =(PFNGLUNIFORMMATRIX3FVPROC)dlsym(ld, "glUniformMatrix3fv");
        gl::UniformMatrix4FV =(PFNGLUNIFORMMATRIX4FVPROC)dlsym(ld, "glUniformMatrix4fv");

        gl::EnableVertexAttribArray =(PFNGLENABLEVERTEXATTRIBARRAYPROC)dlsym(ld, "glEnableVertexAttribArray");
        gl::DisableVertexAttribArray =(PFNGLDISABLEVERTEXATTRIBARRAYPROC)dlsym(ld, "glDisableVertexAttribArray");
        gl::GetVertexAttribDV =(PFNGLGETVERTEXATTRIBDVPROC)dlsym(ld, "glGetVertexAttribdv");
        gl::GetVertexAttribV =(PFNGLGETVERTEXATTRIBFVPROC)dlsym(ld, "glGetVertexAttribv");
        gl::GetVertexAttribIV =(PFNGLGETVERTEXATTRIBIVPROC)dlsym(ld, "glGetVertexAttribiv");
        gl::GetVertexAttribPointerV =(PFNGLGETVERTEXATTRIBPOINTERVPROC)dlsym(ld, "glGetVertexAttribPointerv");
        gl::GenVertexArrays =(PFNGLGENVERTEXARRAYSPROC)dlsym(ld, "glGenVertexArrays");
        gl::BindVertexArray =(PFNGLBINDVERTEXARRAYPROC)dlsym(ld, "glBindVertexArray");
        gl::IsVertexArray =(PFNGLISVERTEXARRAYPROC)dlsym(ld, "glIsVertexArray");
        gl::DeleteVertexArrays =(PFNGLDELETEVERTEXARRAYSPROC)dlsym(ld, "glDeleteVertexArrays");
        gl::VertexAttribPointer =(PFNGLVERTEXATTRIBPOINTERPROC)dlsym(ld, "glVertexAttribPointer");

        gl::GenerateMipmap =(PFNGLGENERATEMIPMAPPROC)dlsym(ld, "glGenerateMipmap");
        dlclose(ld);
    }
    return true;
}

__attribute__((destructor))
static void  free_extensions() noexcept {
}
