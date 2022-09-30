from setuptools import setup, Extension
import os

base = Extension(
    "spark",
    sources=["src/spark.c", "src/glad.c", "src/stb.c"],
    extra_link_args=[
        "-Wall",
        "-lGL",
        "-lGLU",
        "-lglut",
        "-lglfw",
        "-lX11",
        "-lXxf86vm",
        "-lXrandr",
        "-lpthread",
        "-lXi",
        "-ldl",
        "-lXinerama",
        "-lXcursor",
    ],
)

def format():
    os.system("clang-format -i src/*")
    os.system("black .")

def build():
    setup(
        name="spark",
        version="1.0",
        description="An easy to use graphics library written in C with OpenGL",
        ext_modules=[base],
    )

if __name__ == "__main__":
    format()
    build()
