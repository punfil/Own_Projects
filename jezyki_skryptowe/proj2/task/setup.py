from distutils.core import setup, Extension

def main():
    setup(name="simple_graphs",
          author="Wojciech Panfil",
          author_email="s184657@student.pg.edu.pl",
          ext_modules=[Extension("simple_graphs", ["simple_graphs.c"])])

if __name__ == "__main__":
    main()