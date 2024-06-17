from distutils.core import setup, Extension

def main():
    setup(name="stringfinder",
          version="1.0.0.",
          description="Python module for string searching",
          ext_modules=[
              Extension("search_string",
                        [
                            "pymodule.cxx",
                            "search_string.cxx"
                        ],
                        extra_compile_args= ["-std=c++2a"]
                        )])
    
if __name__ == "__main__":
    main()