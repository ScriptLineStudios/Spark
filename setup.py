from distutils.core import setup, Extension

base = Extension("spark", sources = ["src/spark.c", "/usr/include/glad/glad.c", "/usr/include/stb/stb.c"], 
    extra_link_args = ["-Wall", "-lGL", "-lGLU", "-lglut", "-lglfw", "-lX11", "-lXxf86vm", "-lXrandr", "-lpthread",
     "-lXi", "-ldl", "-lXinerama", "-lXcursor"])
     
setup(
    name="spark",
    version = "1.0",
    description = "An easy to use graphics library written in C with OpenGL",
    ext_modules = [base]
)         
