
from setuptools import setup, Extension

base = Extension("spark", sources = ["src/spark.c", "/usr/include/glad/glad.c", "src/stb.c"], 
    extra_link_args = ["-Wall", "-lGL", "-lGLU", "-lglut", "-lglfw", "-lX11", "-lXxf86vm", "-lXrandr", "-lpthread",
     "-lXi", "-ldl", "-lXinerama", "-lXcursor"])

with open("src/spark.c", "r+") as f:
    original = f.read()
    f.write(" ")

with open("src/spark.c", "w") as f:
    f.truncate(0)
    f.write(original)

setup(
    name="spark",
    version = "1.0",
    description = "An easy to use graphics library written in C with OpenGL",
    ext_modules = [base]
)   