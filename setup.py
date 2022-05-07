from distutils.core import setup, Extension

module = Extension("spark", sources = ["src/spark.c"], 
    extra_link_args = ["-Wall", "-lGL", "-lGLU", "-lglut", "-lGLEW", "-lglfw", "-lX11", "-lXxf86vm", "-lXrandr", "-lpthread",
     "-lXi", "-ldl", "-lXinerama", "-lXcursor"])

setup(
    name="spark",
    version = "1.0",
    description = "An easy to use graphics library written in C with OpenGL",
    ext_modules = [module]
)         
