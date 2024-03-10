from distutils.core import setup, Extension

def main():
    setup(name="spam",
          version="1.0.0",
          description="Python interface for the system C library function",
          author="anna anna",
          author_email="anna.anna@hermail.com",
          ext_modules=[Extension("spam", ["systemmodule.c"])])

if __name__ == "__main__":
    main()
