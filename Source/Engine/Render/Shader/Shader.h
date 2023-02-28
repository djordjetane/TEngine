#pragma once

namespace Render {
	class ShaderManager;
	class Renderer;
    class Mesh;

    enum class ShaderType : Uint;

    class Shader
    {
    public:
        Shader(String shaderName);

        //Shader(Path path);

        bool Compile();

        void ActivateShader();

        void DeactivateShader();

    private:
        Uint   m_ID = 0;
        String m_name;

        bool m_CheckCompileErrors(Uint shader, const char* type) const;

        friend ShaderManager;
        friend Renderer;
        friend Mesh;

    public:

        #ifdef GLAPI

        template<typename T>
        void SetValue(const StringView &name, T value) const
                requires(std::is_floating_point_v<T> || std::is_integral_v<T>)
        {
            std::is_floating_point_v<T> ? glUniform1f(glGetUniformLocation(m_ID, name.data()), value)
                                        : glUniform1i(glGetUniformLocation(m_ID, name.data()), value);
        }

        void SetValue(const StringView &name, float value) const
        {
            glUniform1f(glGetUniformLocation(m_ID, name.data()), value);
        }

        void SetValue(const StringView &name, int value) const
        {
            glUniform1i(glGetUniformLocation(m_ID, name.data()), value);
        }

        void SetValue(const StringView &name, bool value) const
        {
            glUniform1i(glGetUniformLocation(m_ID, name.data()), value);
        }

        void SetValue(const StringView &name, double value) const
        {
            glUniform1d(glGetUniformLocation(m_ID, name.data()), value);
        }

        void SetValue(const StringView &name, unsigned value) const
        {
            glUniform1ui(glGetUniformLocation(m_ID, name.data()), value);
        }
        
        template<typename T>
        void SetValue(const StringView &name, T val1, T val2) const
                requires(std::is_floating_point_v<T> || std ::is_integral_v<T>)
        {
            if constexpr (std::is_floating_point_v<T>)
                glUniform2f(glGetUniformLocation(m_ID, name.data()), val1, val2);
            else
                glUniform2i(glGetUniformLocation(m_ID, name.data()), val1, val2);
        }

        void SetValue(const StringView &name, double val1, double val2) const
        {
            glUniform2d(glGetUniformLocation(m_ID, name.data()), val1, val2);
        }

        void SetValue(const StringView &name, Uint val1, Uint val2) const
        {
            glUniform2ui(glGetUniformLocation(m_ID, name.data()), val1, val2);
        }


        template<typename T>
        void SetValue(const StringView &name, T val1, T val2, T val3, T val4) const
                requires(std::is_floating_point_v<T> || std::is_integral_v<T>)
        {
            if constexpr (std::is_same_v<T, float>)
                glUniform4f(glGetUniformLocation(m_ID, name.data()), val1, val2, val3, val4);
            if constexpr (std::is_same_v<T, double>)
                glUniform4d(glGetUniformLocation(m_ID, name.data()), val1, val2, val3, val4);
            if constexpr (std::is_same_v<T, int>)
                glUniform4i(glGetUniformLocation(m_ID, name.data()), val1, val2, val3, val4);
            else
                glUniform4ui(glGetUniformLocation(m_ID, name.data()), val1, val2, val3, val4);
        }

        template<typename T>
        void SetValue(const StringView &name, T val1, T val2, T val3) const
                requires(std::is_floating_point_v<T> || std::is_integral_v<T>)
        {
            std::is_floating_point_v<T> ? glUniform3f(glGetUniformLocation(m_ID, name.data()), val1, val2, val3)
                                        : glUniform3i(glGetUniformLocation(m_ID, name.data()), val1, val2, val3);
        }

        
        void SetValue(const StringView &name, double val1, double val2, double val3) const
        {
            glUniform3d(glGetUniformLocation(m_ID, name.data()), val1, val2, val3);
        }

        void SetValue(const StringView &name, int val1, int val2, int val3) const
        {
            glUniform3i(glGetUniformLocation(m_ID, name.data()), val1, val2, val3);
        }

        void SetValue(const StringView &name, Uint val1, Uint val2, Uint val3) const
        {
            glUniform3ui(glGetUniformLocation(m_ID, name.data()), val1, val2, val3);
        }

        template<typename T, int L>
        void SetValue(const StringView &name, const Vec<L, T>& val) const
                requires(std::is_floating_point_v<T> &&L >= 2 && L <= 4)
        {
            if constexpr (std::is_same_v<T, float>)
            {
                if constexpr (L == 2)
                    glUniform2fv(glGetUniformLocation(m_ID, name.data()), 1, &val[0]);
                else if constexpr (L == 3)
                    glUniform3fv(glGetUniformLocation(m_ID, name.data()), 1, &val[0]);
                else
                    glUniform4fv(glGetUniformLocation(m_ID, name.data()), 1, &val[0]);
            }
            else
            {
                if constexpr (L == 2)
                    glUniform2dv(glGetUniformLocation(m_ID, name.data()), 1, &val[0]);
                else if constexpr (L == 3)
                    glUniform3dv(glGetUniformLocation(m_ID, name.data()), 1, &val[0]);
                else
                    glUniform4dv(glGetUniformLocation(m_ID, name.data()), 1, &val[0]);
            }
        }

        template<typename T, int L>
        void SetValue(const StringView &name, const Vec<L, T> &val) const
                requires(std::is_integral_v<T> &&L >= 2 && L <= 4)
        {
            if constexpr (std::is_same_v<T, Uint>)
            {
                if constexpr (L == 2)
                    glUniform2uiv(glGetUniformLocation(m_ID, name.data()), 1, &val[0]);
                else if constexpr (L == 3)
                    glUniform3uiv(glGetUniformLocation(m_ID, name.data()), 1, &val[0]);
                else
                    glUniform4uiv(glGetUniformLocation(m_ID, name.data()), 1, &val[0]);
            }
            else
            {
                if constexpr (L == 2)
                    glUniform2iv(glGetUniformLocation(m_ID, name.data()), 1, &val[0]);
                else if constexpr (L == 3)
                    glUniform3iv(glGetUniformLocation(m_ID, name.data()), 1, &val[0]);
                else
                    glUniform4iv(glGetUniformLocation(m_ID, name.data()), 1, &val[0]);
            }
        }

        template<int N, int M>
        void SetValue(const StringView &name, const Mat<N, M, float> &val, bool transpose = false) const
                requires(N >= 2 && N <= 4 && M >= 2 && M <= 4)
        {
            if constexpr (N == 2)
            {
                if constexpr (M == 2)
                    glUniformMatrix2fv(glGetUniformLocation(m_ID, name.data()), 1, transpose, &val[0][0]);
                else if constexpr (M == 3)
                    glUniformMatrix2x3fv(glGetUniformLocation(m_ID, name.data()), 1, transpose, &val[0][0]);
                else
                    glUniformMatrix2x4fv(glGetUniformLocation(m_ID, name.data()), 1, transpose, &val[0][0]);
            }
            else if constexpr (N == 3)
            {

                if constexpr (M == 2)
                    glUniformMatrix3x2fv(glGetUniformLocation(m_ID, name.data()), 1, transpose, &val[0][0]);
                else if constexpr (M == 3)
                    glUniformMatrix3fv(glGetUniformLocation(m_ID, name.data()), 1, transpose, &val[0][0]);
                else
                    glUniformMatrix3x4fv(glGetUniformLocation(m_ID, name.data()), 1, transpose, &val[0][0]);
            }
            else // N == 4
            {
                if constexpr (M == 2)
                    glUniformMatrix4x2fv(glGetUniformLocation(m_ID, name.data()), 1, transpose, &val[0][0]);
                else if constexpr (M == 3)
                    glUniformMatrix4x3fv(glGetUniformLocation(m_ID, name.data()), 1, transpose, &val[0][0]);
                else
                    glUniformMatrix4fv(glGetUniformLocation(m_ID, name.data()), 1, transpose, &val[0][0]);
            }
        }

        template<int N, int M>
        void SetValue(const StringView &name, const Mat<N, M, double> &val, bool transpose = false) const
                requires(N >= 2 && N <= 4 && M >= 2 && M <= 4)
        {
            if constexpr (N == 2)
            {
                if constexpr (M == 2)
                    glUniformMatrix2dv(glGetUniformLocation(m_ID, name.data()), 1, transpose, &val[0][0]);
                else if constexpr (M == 3)
                    glUniformMatrix2x3dv(glGetUniformLocation(m_ID, name.data()), 1, transpose, &val[0][0]);
                else
                    glUniformMatrix2x4dv(glGetUniformLocation(m_ID, name.data()), 1, transpose, &val[0][0]);
            }
            else if constexpr (N == 3)
            {

                if constexpr (M == 2)
                    glUniformMatrix3x2dv(glGetUniformLocation(m_ID, name.data()), 1, transpose, &val[0][0]);
                else if constexpr (M == 3)
                    glUniformMatrix3dv(glGetUniformLocation(m_ID, name.data()), 1, transpose, &val[0][0]);
                else
                    glUniformMatrix3x4dv(glGetUniformLocation(m_ID, name.data()), 1, transpose, &val[0][0]);
            }
            else // N == 4
            {
                if constexpr (M == 2)
                    glUniformMatrix4x2dv(glGetUniformLocation(m_ID, name.data()), 1, transpose, &val[0][0]);
                else if constexpr (M == 3)
                    glUniformMatrix4x3dv(glGetUniformLocation(m_ID, name.data()), 1, transpose, &val[0][0]);
                else
                    glUniformMatrix4dv(glGetUniformLocation(m_ID, name.data()), 1, transpose, &val[0][0]);
            }
        }
        #endif
    };
} // namespace Render

