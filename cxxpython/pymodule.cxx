#include <Python.h>
#include <optional>
#include <string_view>
#include <format>

static constexpr auto module_doc = "Python module to search string in a file";

std::optional<size_t> search_string_core(
    const char* filename, std::string_view pattern
);

static PyObject* search_string_py(PyObject* self, PyObject* args)
{
    char* pattern;
    char* filename;
    if (!PyArg_ParseTuple(args, "ss", &filename, &pattern)) {
        return nullptr;
    }
    
    const auto res = search_string_core(filename, { pattern });

    if (not res) {
        PyErr_SetString(PyExc_RuntimeError, "Search string function failed");
        return nullptr;
    }

    return PyLong_FromSize_t(*res);
}

static PyMethodDef search_string_methods[] = {
    { "search_string", search_string_py, METH_VARARGS, module_doc },
    { nullptr, nullptr, 0, nullptr },
};
static PyModuleDef search_string_mod = {
    .m_base    = PyModuleDef_HEAD_INIT,
    .m_name    = "search_string",
    .m_doc     = module_doc,
    .m_size    = -1,
    .m_methods = search_string_methods,
};

PyMODINIT_FUNC PyInit_search_string()
{
    return PyModule_Create(&search_string_mod);
}